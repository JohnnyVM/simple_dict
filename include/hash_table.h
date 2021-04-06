#if !defined(HASH_TABLE_H)
#define HASH_TABLE_H

#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

#if !defined(__linux)
#define uintmax_t unsigned long long
#endif

#define DUMMY_KEY ULLONG_MAX

// todo explain this magic number
#define OCCUPACY 2 / (long double)3.0

struct hash_element {
	uintmax_t hash; // its use as sentinel value fot check if was used
	uintmax_t key;
	void* value;
};


struct hash_table {
	struct hash_element* slot;
	uintmax_t capacity;
	uintmax_t used;
};

//uint key implementation
uintmax_t hash_insert(struct hash_table* table, uintmax_t key, const void* const value);
uintmax_t hash_search(const struct hash_table* const table, const uintmax_t key);
bool hash_has_key(const struct hash_table *const, const uintmax_t);
uintmax_t hash_delete(struct hash_table* const table, const uintmax_t key);
inline uintmax_t hash_len(const struct hash_table* const);
void* hash_get(const struct hash_table* const, uintmax_t key, const void* const defaul);

// Char key implementation
uintmax_t char2key( const char* const );
uintmax_t dict_insert(struct hash_table* const, const char* const, const void* const);
uintmax_t dict_search(struct hash_table* const, const char* const);
bool dict_has_key(const struct hash_table *const, const char* const);
uintmax_t dict_delete(struct hash_table* const, const char* const);
#define dict_len hash_len
void* dict_get(const struct hash_table* const, const char* const, const void* const);

#define Dict_len hash_len

#define Dict_insert(table, key, value) \
	_Generic(key, \
		char*: dict_insert, \
		default: hash_insert \
		)(table, key, value)

#define Dict_search(table, key) \
	_Generic(key, \
		char*: dict_search, \
		default: hash_search \
		)(table, key)

#define Dict_delete(table, key) \
	_Generic(key, \
		char*: dict_delete, \
		default: hash_delete \
		)(table, key)

#define Dict_get(table, key, value) \
	_Generic(key, \
		char*: dict_get, \
		default: hash_get \
		)(table, key, value)

#define Dict_has_key(table, key) \
	_Generic(key, \
		char*: dict_has_key, \
		default: hash_has_key \
		)(table, key)

#endif // HASH_TABLE_H
