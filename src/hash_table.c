/**	\file hash_table.c
 *	\brief Hash table and dict implementation
 *
 *	In this functions memory must be initialized to 0 to work
 */

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#if !defined(hash_method)
#define hash_method hash_method_linear_division
#endif

#define GCC_VERSION                                                            \
  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if GCC_VERSION < 70000
#define __label__ [[maybe_unused]]int
#endif

/**	\brief most simple hashing method
 *	\todo this algorithm its innadecuate for most uses
 *	\param table hash table
 *	\param key key value
 *	\param i number of probes
 *	\return index of hash
 */
static inline unsigned long long
hash_method_linear_division(const struct hash_table *const table,
                            const unsigned long long key,
                            const unsigned long long i) {
  return (key + i) % table->capacity;
}

/**	\brief hash table search
 *
 *	\param table hash table
 *	\param key key value
 *	\param i number of probes
 *	\return index of hash
 */
unsigned long long hash_search(const struct hash_table *const table,
                               const unsigned long long key) {

  unsigned long long j, i = 0;
  do {
    j = hash_method(table, key, i);
    if (table->slot[j].key == key) {
      return j;
    }
    ++i;
  } while (table->slot + j != NULL && i < table->capacity);

  return dummy_key;
}

/**	\brief hash table insert
 *
 *	this function not handle expanding keys, but for fixed quantityes of
 *elements its fine \param table hash table \param key key value \param i number
 *of probes \return index of hash
 */
unsigned long long _hash_insert(struct hash_table *const table,
                                unsigned long long key, const void *value) {
  __label__ hash_element_position_finded;
  unsigned long long j, i = 0;

  j = hash_search(table, key);
  if (j != dummy_key) {
    goto hash_element_position_finded;
  }

  do {
    j = hash_method(table, key, i);
    if (table->slot + j == NULL || table->slot[j].key == dummy_key) {
    hash_element_position_finded:
      table->slot[j].key = key;
      table->slot[j].value = (void *)value;
      ++table->used;
      return j;
    }
    ++i;
  } while (i < table->capacity);
  assert(0);
  return dummy_key;
}

/**	\brief hash table insert, overridevalue if key already exists
 *
 *	\param table hash table
 *	\param key key value
 *	\param i number of probes
 *	\return index of hash
 */
unsigned long long hash_insert(struct hash_table *table, unsigned long long key,
                               const void *value) {
  struct hash_table aux;
  struct hash_element *el;
  unsigned long long j;

  if (table->used / (float)table->capacity > 2 / 3.0) {
    aux.capacity = 2 * table->capacity;
    aux.used = 0;
    aux.slot = (struct hash_element *)calloc(aux.capacity,
                                             sizeof(struct hash_element));
    for (unsigned long long i = 0; i < table->capacity;
         el = table->slot + i, ++i) {
      if (el != NULL && el->key != dummy_key) {
        j = _hash_insert(&aux, el->key, el->value);
        assert(j != dummy_key);
      }
    }
    free(table->slot);
    memcpy(table, &aux, sizeof(struct hash_table));
  }

  j = _hash_insert(table, key, value);
  assert(j != dummy_key);

  return j;
}

/**	\brief hash table delete
 *
 *	\param table hash table
 *	\param key key value
 *	\param i number of probes
 *	\return index of hash
 */
unsigned long long hash_delete(struct hash_table *const table,
                               const unsigned long long key) {
  unsigned long long j = hash_search(table, key);
  if (j != dummy_key) {
    table->slot[j].key = dummy_key;
  }
  return j;
}
