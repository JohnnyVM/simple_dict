#if !defined(HASH_TABLE_H)
#define HASH_TABLE_H

#include <stdlib.h>
#include <limits.h>

#define DUMMY_KEY ULLONG_MAX

// todo explain this magic number
#define OCCUPACY 2 / (long double)3.0

struct hash_element {
	unsigned long long hash; // its use as sentinel value fot check if was used
	unsigned long long key;
	void* value;
};


struct hash_table {
	struct hash_element* slot;
	unsigned long long capacity;
	unsigned long long used;
};

unsigned long long hash_insert(struct hash_table* table, unsigned long long key, const void* const value);
unsigned long long hash_search(const struct hash_table* const table, const unsigned long long key);
int hash_has_key(const struct hash_table *const, const unsigned long long);
unsigned long long hash_delete(struct hash_table* const table, const unsigned long long key);
inline unsigned long long hash_len(const struct hash_table* const);
void* hash_get(const struct hash_table* const, unsigned long long key, const void* const defaul);

unsigned long long char2ull( const char* const );
unsigned long long dict_insert(struct hash_table* const, const char* const, const void* const);
unsigned long long dict_search(struct hash_table* const, const char* const);
int dict_has_key(const struct hash_table *const, const char* const);
unsigned long long dict_delete(struct hash_table* const, const char* const);
#define dict_len hash_len
void* dict_get(const struct hash_table* const, const char* const, const void* const);

#endif // HASH_TABLE_H
