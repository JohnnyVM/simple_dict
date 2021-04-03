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

	if(table == NULL) { return DUMMY_KEY; }

	do {
		j = hash_method(table, key, i);
		if (table->slot[j].hash != DUMMY_KEY && table->slot[j].key == key) {
			return j;
		}
		++i;
	} while (table->slot[j].hash != DUMMY_KEY && i < table->capacity);

	return DUMMY_KEY;
}

/**	\brief helper common method search key
 *	\param table table to seaarch
 *	\param key key to search
 */
int hash_has_key(const struct hash_table *const table, const unsigned long long key) {
	if(hash_search(table, key) != DUMMY_KEY) {
		return 1;
	}

	return 0;
}

/**	\brief hash table insert, override value if key already exists
 *
 *	\todo remove that goto...
 *	\param table hash table
 *	\param key key value
 *	\param i number of probes
 *	\return index of hash
 */
static unsigned long long _hash_insert(struct hash_table *const table, unsigned long long key, const void* const value) {

	unsigned long long j, i = 0;

	if(table == NULL) { return DUMMY_KEY; }

	j = hash_search(table, key);
	if (j != DUMMY_KEY) {
		table->slot[j].hash = hash_method(table, key, 0);
		table->slot[j].key = key;
		table->slot[j].value = (void *)value;
		return j;
	}

	do {
		j = hash_method(table, key, i);
		if (table->slot[j].hash == DUMMY_KEY || table->slot[j].key == DUMMY_KEY) {
			++table->used;
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
unsigned long long hash_insert(struct hash_table *table, unsigned long long key, const void* const value) {
	struct hash_table aux;
	struct hash_element *el;
	unsigned long long j,i;

	if(table == NULL) { return DUMMY_KEY; }

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
	unsigned long long j;

	if(table == NULL) { return DUMMY_KEY; }

	j = hash_search(table, key);
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
	if(t == NULL) { return DUMMY_KEY; }

	return t->used;
}

/**	\brief return a array containing the keys, must be freed
 *	\param table hash table
 */

/**	\brief Return the value for key if key is in the hash, else default.
 *	\param table hash_table
 *	\param key key table
 *	\param default default value if key not found
 *	\return if key exist return key, else default
 */
void* hash_get(const struct hash_table* const table , unsigned long long key, const void* const defaul)
{
	unsigned long long j;

	if(table == NULL) { return (void*)defaul; }

	j = hash_search(table, key);
	if (j != DUMMY_KEY) {
		return table->slot[j].value;
	}

	return (void*)defaul;
}

/**	\brief get a char and transform in unsigned long long
 *
 *	if the string its longer than sizeof(unsigned long long) its truncated
 *	assumption, the end of the string its more probably to be different
 *	\param cad string to transform
 */
unsigned long long char2ull(const char* const cad)
{
	unsigned long long c, key = 0;
	size_t len;

	len = strlen(cad);

	for(unsigned long long i = 0; len - i > 0 && i < sizeof(unsigned long long); ++i) {
		c = (unsigned long long)cad[len - i - 1];
		c <<= 8 * i;
		key |= c;
	}

	return key;
}

/**	\helper method of insert for string */
unsigned long long dict_insert(struct hash_table* const dict, const char* const key, const void* const value)
{
	return hash_insert(dict, char2ull(key), value);
}

/**	\helper method of search for string */
unsigned long long dict_search(struct hash_table* const dict, const char* const key)
{
	return hash_search(dict, char2ull(key));
}

/**	\helper method of search for string */
int dict_has_key(const struct hash_table* const dict, const char* const key)
{
	return hash_has_key(dict, char2ull(key));
}

/**	\helper method of delete for string */
unsigned long long dict_delete(struct hash_table* const dict, const char* const key)
{
	return hash_delete(dict, char2ull(key));
}

/**	\helper method of get for string */
void* dict_get(const struct hash_table* const dict, const char* const key, const void* const defaul)
{
	return hash_get(dict, char2ull(key), defaul);
}
