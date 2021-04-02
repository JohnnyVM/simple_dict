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
unsigned long long hash_search(const struct hash_table *const table, const unsigned long long key) {

	unsigned long long j, i = 0;
	do {
		j = hash_method(table, key, i);
		if (table->slot[j].hash != DUMMY_KEY && table->slot[j].key == key) {
			return j;
		}
		++i;
	} while (table->slot[j].hash != DUMMY_KEY && i < table->capacity);

	return DUMMY_KEY;
}

/**	\brief hash table insert, override value if key already exists
 *
 *	\todo remove that goto...
 *	\param table hash table
 *	\param key key value
 *	\param i number of probes
 *	\return index of hash
 */
static unsigned long long _hash_insert(struct hash_table *const table, unsigned long long key, const void *value) {

	#if GCC_VERSION > 70000
	__label__ hash_element_position_found;
	#endif

	unsigned long long j, i = 0;

	j = hash_search(table, key);
	if (j != DUMMY_KEY) {
		goto hash_element_position_found;
	}

	do {
		j = hash_method(table, key, i);
		if (table->slot[j].hash == DUMMY_KEY || table->slot[j].key == DUMMY_KEY) {
			++table->used;
hash_element_position_found:
			table->slot[j].hash = hash_method(table, key, 0);
			table->slot[j].key = key;
			table->slot[j].value = (void *)value;
			return j;
		}
		++i;
	} while (i < table->capacity);
	assert(0);

	return DUMMY_KEY;
}

/**	\brief hash table insert, override value if key already exists
 *
 *	\param table hash table
 *	\param key key value
 *	\param i number of probes
 *	\return index of hash
 */
unsigned long long hash_insert(struct hash_table *table, unsigned long long key, const void *value) {
	struct hash_table aux;
	struct hash_element *el;
	unsigned long long j,i;

	if (!table->capacity || table->used / (long double)table->capacity > OCCUPACY) {
		aux.capacity = 2 * (table->capacity ? table->capacity : 1);
		aux.used = 0;
		aux.slot = malloc(aux.capacity * sizeof(struct hash_element));
		assert(aux.slot);

		//mark the new table as unused
		for (i = 0; i < aux.capacity; ++i) { aux.slot[i].hash = DUMMY_KEY; }

		for (el = table->slot, i = 0; i < table->capacity; ++i, el = table->slot + i) {
			if (el->hash != DUMMY_KEY && el->key != DUMMY_KEY) {
				_hash_insert(&aux, el->key, el->value);
			}
		}
		if(table->slot) { free(table->slot); }
		memcpy(table, &aux, sizeof(struct hash_table));
	}

	j = _hash_insert(table, key, value);

	return j;
}

/**	\brief hash table delete entry
 *
 *	\param table hash table
 *	\param key key value to delete
 *	\return index of hash
 */
unsigned long long hash_delete(struct hash_table *const table, const unsigned long long key)
{
	unsigned long long j = hash_search(table, key);

	if (j != DUMMY_KEY) {
		table->slot[j].key = DUMMY_KEY;
		--table->used;
	}

	return j;
}

/** \brief Return the number of items in the hash table
 *	\param t hash table
 * \return return the number of items
 */
inline unsigned long long hash_len(const struct hash_table *const t)
{
	return t->used;
}

/**	\brief Return the value for key if key is in the hash, else default.
 *	\param table hash_table
 *	\param key key table
 *	\param default default value if key not found
 *	\return if key exist return key, else default
 */
void* hash_get(const struct hash_table* const table , unsigned long long key, void* const defaul)
{
	unsigned long long j = hash_search(table, key);

	if (j != DUMMY_KEY) {
		return table->slot[j].value;
	}

	return defaul;
}
