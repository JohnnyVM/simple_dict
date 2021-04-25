/**
 * \file hash_table.c
 * \brief Hash table and dict implementation
 */

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "hash_table.h"

#if !defined(hash_method)
#define hash_method hash_method_geometric
#endif

#define GCC_VERSION                                                            \
	(__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

/**
 * \brief most simple hashing method
 * \todo this algorithm its innadecuate for most uses
 * \param table hash table
 * \param key key value
 * \param i number of probes
 * \return index of hash
 */
static inline uintmax_t
hash_method_linear_division(const struct hash_table *const table,
				                    const uintmax_t key,
				                    const uintmax_t i) {
	return (key + i) % table->capacity;
}

/**	\brief geometric hash
 *
 *	j = ((5*j) + 1) mod (2**i: size)
 *	For any initial j in range(2**i), repeating that 2**i times generates each int in range(2**i) exactly once
 */
static inline uintmax_t
hash_method_geometric(const struct hash_table *const table,
				                    const uintmax_t key,
				                    const uintmax_t i) {

	uintmax_t j;

	j = key % table->capacity;
	return ((5 * (j + i)) + 1) % table->capacity;
}

/**
 * \brief hash table search
 *
 * \param table hash table
 * \param key key value
 * \param i number of probes
 * \return index of hash
 */
uintmax_t hash_search(const struct hash_table *const table, const uintmax_t key) {

	uintmax_t j, i = 0;

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

/**
 * \brief helper common method search key
 * \param table table to seaarch
 * \param key key to search
 */
bool hash_has_key(const struct hash_table *const table, const uintmax_t key) {

	if(hash_search(table, key) != DUMMY_KEY) {
		return true;
	}

	return false;
}

/**
 * \brief hash table insert, override value if key already exists
 *
 * \todo remove that goto...
 * \param table hash table
 * \param key key value
 * \param i number of probes
 * \return index of hash
 */
static uintmax_t _hash_insert(struct hash_table *const table, uintmax_t key, const void* const value) {

	uintmax_t j, i = 0;

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

/**
 * \brief hash table insert, override value if key already exists
 *
 * \param table hash table
 * \param key key value
 * \param i number of probes
 * \return index of hash
 */
uintmax_t hash_insert(struct hash_table *table, uintmax_t key, const void* value) {
	struct hash_table aux;
	struct hash_element *el;
	uintmax_t j,i;

	if(table == NULL) { return DUMMY_KEY; }

	if (!table->capacity || table->used / (long double)table->capacity > OCCUPACY) {
		aux.capacity = 2 * (table->capacity ? table->capacity : 4);
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

/**
 * \brief hash table delete entry
 *
 * \param table hash table
 * \param key key value to delete
 * \return index of hash
 */
uintmax_t hash_delete(struct hash_table* table, uintmax_t key)
{
	uintmax_t j;

	if(table == NULL) { return DUMMY_KEY; }

	j = hash_search(table, key);
	if (j != DUMMY_KEY) {
		table->slot[j].key = DUMMY_KEY;
		--table->used;
	}

	return j;
}

/**
 * \brief Return the number of items in the hash table
 * \param t hash table
 * \return return the number of items
 */
inline uintmax_t hash_len(const struct hash_table* t)
{
	if(t == NULL) { return 0; }

	return t->used;
}

/**
 * \brief return a array containing the keys, must be freed
 * \param table hash table
 */

/**
 * \brief Return the value for key if key is in the hash, else default.
 * \param table hash_table
 * \param key key table
 * \param default default value if key not found
 * \return if key exist return key, else default
 */
void* hash_get(const struct hash_table* table , uintmax_t key, const void* defaul)
{
	uintmax_t j;

	if(table == NULL) { return (void*)defaul; }

	j = hash_search(table, key);
	if (j != DUMMY_KEY) {
		return table->slot[j].value;
	}

	return (void*)defaul;
}

/**
 * \brief get a char and transform in uintmax_t
 *
 * if the string its longer than sizeof(uintmax_t) its truncated
 * assumption, the end of the string its more probably to be different
 * \param cad string to transform
 */
uintmax_t char2key(const char* cad)
{
	uintmax_t c, key = 0;
	size_t len;

	len = strlen(cad);

	for(uintmax_t i = 0; len - i > 0 && i < sizeof(uintmax_t); ++i) {
		c = (uintmax_t)cad[len - i - 1];
		c <<= 8 * i;
		key |= c;
	}

	return key;
}

/** \brief helper method of insert for string */
uintmax_t hash_insert_char(struct hash_table* dict, const char* key, const void* value)
{
	return hash_insert(dict, char2key(key), value);
}

/** \brief helper method of search for string */
uintmax_t hash_search_char(struct hash_table* dict, const char* key)
{
	return hash_search(dict, char2key(key));
}

/** \brief helper method of has key for string */
bool hash_has_key_char(const struct hash_table* dict, const char* key)
{
	return hash_has_key(dict, char2key(key));
}

/** \brief helper method of delete for string */
uintmax_t hash_delete_char(struct hash_table* dict, const char* key)
{
	return hash_delete(dict, char2key(key));
}

/** \brief helper method of get for string */
void* hash_get_char(const struct hash_table* dict, const char* key, const void* defaul)
{
	return hash_get(dict, char2key(key), defaul);
}
