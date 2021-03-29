#if !defined(HASH_TABLE_H)
#define HASH_TABLE_H

#include <stdlib.h>
#include <limits.h>

#define dummy_key ULLONG_MAX

struct hash_element {
	unsigned long long hash;
	unsigned long long key;
	void* value;
};


struct hash_table {
	struct hash_element* slot;
	unsigned long long capacity;
	unsigned long long used;
};

unsigned long long char2ull( const char* const , const size_t );

unsigned long long _hash_insert(struct hash_table* const table, unsigned long long key, const void *value);
unsigned long long hash_insert(struct hash_table* table, unsigned long long key, const void *value);
unsigned long long hash_search(const struct hash_table* const table, const unsigned long long key);
unsigned long long hash_delete(struct hash_table* const table, const unsigned long long key);

#endif // HASH_TABLE_H