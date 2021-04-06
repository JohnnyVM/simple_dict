#include "CppUTest/TestHarness_c.h"

#include "hash_table.h"

TEST_GROUP_C_SETUP(Dict_table)
{
}

TEST_GROUP_C_TEARDOWN(Dict_table)
{
}

TEST_C(Dict_table, create_insert_delete_number)
{
	struct hash_table table = {.capacity = 0};
	uintmax_t key, key2;

	key = Dict_insert(&table, 0LLU, NULL);
	CHECK_C(key != DUMMY_KEY);
	CHECK_C(table.used == 1);
	key2 = Dict_delete(&table, 0LLU);
	CHECK_C(key2 == key);
	free(table.slot);
	table.capacity = 0;
}

TEST_C(Dict_table, create_insert_delete_char)
{
	struct hash_table table = {.capacity = 0};
	uintmax_t key, key2;

	key = Dict_insert(&table, "casa", NULL);
	CHECK_C(key != DUMMY_KEY);
	CHECK_C(table.used == 1);
	CHECK_C(Dict_has_key(&table, "casa"));
	CHECK_C(!Dict_has_key(&table, "asa"));
	key2 = Dict_delete(&table, "casa");
	CHECK_C(key2 == key);
	free(table.slot);
	table.capacity = 0;
}

TEST_C(Dict_table, create_insert_update_delete)
{
	struct hash_table table = {.capacity = 0};
	uintmax_t key, key2;

	key = Dict_insert(&table, 0LLU, (const void*)1);
	CHECK_C(Dict_has_key(&table, 0));
	CHECK_C(key != DUMMY_KEY);
	key2 = Dict_insert(&table, 0LLU, (const void*)2);
	CHECK_C(key2 == key);
	CHECK_C(table.used == 1);
	key = Dict_delete(&table, 0LLU);
	CHECK_C(table.used == 0);
	CHECK_C(key2 == key);
	CHECK_C(Dict_get(&table, key, NULL) == NULL);
	CHECK_C(table.used == 0);
	free(table.slot);
	table.capacity = 0;
}

TEST_C(Dict_table, create_insert_collision_search)
{
	struct hash_table table = {.capacity = 0};
	uintmax_t key, key2, key3;

	key = Dict_insert(&table, 0LLU, (const void*)1);
	CHECK_C(Dict_has_key(&table, 0LLU));
	CHECK_C(key != DUMMY_KEY);
	key2 = Dict_insert(&table, table.capacity, (const void*)2);
	CHECK_C(Dict_has_key(&table, table.capacity));
	CHECK_C(table.used == 2);
	CHECK_C(key2 != DUMMY_KEY);
	key3 = Dict_search(&table, 0LLU);
	CHECK_C(key3 == key);
	CHECK_C((long unsigned)Dict_get(&table, 0LLU, NULL) == 1);
	key3 = Dict_search(&table, table.capacity);
	CHECK_C(key3 == key2);
	CHECK_C((long unsigned)Dict_get(&table, table.capacity, NULL) == 2);
	free(table.slot);
	table.capacity = 0;
}

TEST_C(Dict_table, stress)
{
	struct hash_table table = {.capacity = 0};
	uintmax_t key;
	unsigned int tryes = 100;

	for(unsigned long i = 0; i < tryes; ++i ) {
		key = Dict_insert(&table, i, (void*)(long long)i);
		CHECK_C(key != DUMMY_KEY);
	}
	CHECK_C(table.used == (unsigned)tryes);
	for(unsigned long i = 0; i < tryes; ++i ) {
		CHECK_C(i == (unsigned long)Dict_get(&table, i, NULL));
	}
	for(unsigned long i = 0; i < tryes; ++i ) {
		key = Dict_delete(&table, i);
		CHECK_C(key != DUMMY_KEY);
	}
	CHECK_C(table.used == 0);
	for(unsigned long i = 0; i < tryes; ++i ) {
		key = Dict_search(&table, i);
		CHECK_C(key == DUMMY_KEY);
	}
	free(table.slot);
	table.capacity = 0;
}

TEST_C(Dict_table, char2ull)
{
	char a[] = "a";

	CHECK_C('a' == char2key(a));
}
