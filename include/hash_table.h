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

// TODO explain this magic number
#define OCCUPACY 8 / (long double)10.0

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
uintmax_t hash_insert(struct hash_table*, uintmax_t key, const void*);
uintmax_t hash_search(const struct hash_table*, uintmax_t key);
bool hash_has_key(const struct hash_table*, uintmax_t);
uintmax_t hash_delete(struct hash_table* table, uintmax_t);
inline uintmax_t hash_len(const struct hash_table*);
void* hash_get(const struct hash_table*, uintmax_t key, const void*);

// Char key implementation
uintmax_t char2key( const char* );
uintmax_t hash_insert_char(struct hash_table*, const char*, const void*);
uintmax_t hash_search_char(struct hash_table*, const char*);
bool hash_has_key_char(const struct hash_table*, const char*);
uintmax_t hash_delete_char(struct hash_table*, const char*);
#define hash_len_char hash_len
void* hash_get_char(const struct hash_table*, const char*, const void*);

#define dict_len hash_len

#define dict_insert(table, key, value) \
	_Generic(key, \
		char*: hash_insert_char, \
		const char*: hash_insert_char, \
		default: hash_insert \
		)(table, key, value)

#define dict_search(table, key) \
	_Generic(key, \
		char*: hash_search_char, \
		const char*: hash_search_char, \
		default: hash_search \
		)(table, key)

#define dict_delete(table, key) \
	_Generic(key, \
		char*: hash_delete_char, \
		const char*: hash_delete_char, \
		default: hash_delete \
		)(table, key)

#define dict_get(table, key, value) \
	_Generic(key, \
		char*: hash_get_char, \
		const char*: hash_get_char, \
		default: hash_get \
		)(table, key, value)

#define dict_has_key(table, key) \
	_Generic(key, \
		char*: hash_has_key_char, \
		const char*: hash_has_key_char, \
		default: hash_has_key \
		)(table, key)

#endif // HASH_TABLE_H
