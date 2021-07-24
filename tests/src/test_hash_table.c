#include <string.h>

#include "CppUTest/TestHarness_c.h"

#include "hash_table.h"

TEST_GROUP_C_SETUP(dict_table)
{
}

TEST_GROUP_C_TEARDOWN(dict_table)
{
}

TEST_C(dict_table, create_insert_delete_number)
{
	struct hash_table* dict = Dict();
	uintmax_t key, key2;

	key = dict_insert(dict, 0LLU, NULL, 0);
	CHECK_C(key != DUMMY_KEY);
	CHECK_C(dict->used == 1);
	key2 = dict_delete(dict, 0LLU);
	CHECK_C(key2 == key);
	dict_free(dict);
}

TEST_C(dict_table, create_insert_delete_char)
{
	struct hash_table* table = Dict();
	uintmax_t key, key2;
	char arbitrary_string[] = "Casa";

	key = dict_insert(table, arbitrary_string, arbitrary_string, strlen(arbitrary_string) + 1);
	CHECK_C(key != DUMMY_KEY);
	CHECK_C(table->used == 1);
	CHECK_C(dict_has_key(table, arbitrary_string));
	CHECK_C(!dict_has_key(table, "asa"));
	CHECK_EQUAL_C_STRING(dict_get(table, arbitrary_string, NULL), arbitrary_string);
	CHECK_C(!dict_get(table, "tetera", NULL));
	key2 = dict_delete(table, arbitrary_string);
	CHECK_C(key2 == key);
	dict_free(table);
}

TEST_C(dict_table, create_insert_update_delete)
{
	struct hash_table *table = Dict();
	uintmax_t key, key2;
	int i = 1;

	key = dict_insert(table, 0LLU, &i, sizeof(int));
	CHECK_C(dict_has_key(table, 0));
	CHECK_C(key != DUMMY_KEY);
	i = 2;
	key2 = dict_insert(table, 0LLU, &i, sizeof(int));
	CHECK_C(key2 == key);
	CHECK_C(table->used == 1);
	key = dict_delete(table, 0LLU);
	CHECK_C(table->used == 0);
	CHECK_C(key2 == key);
	CHECK_C(dict_get(table, key, NULL) == NULL);
	CHECK_C(table->used == 0);
	dict_free(table);
}

TEST_C(dict_table, create_insert_collision_search)
{
	struct hash_table table = {.capacity = 0};
	uintmax_t key, key2, key3;
	int i = 1;

	key = dict_insert(&table, 0LLU, &i, sizeof(int));
	CHECK_C(dict_has_key(&table, 0LLU));
	CHECK_C(key != DUMMY_KEY);
	i = 2;
	key2 = dict_insert(&table, table.capacity, &i, sizeof(int));
	CHECK_C(dict_has_key(&table, table.capacity));
	CHECK_C(table.used == 2);
	CHECK_C(key2 != DUMMY_KEY);
	key3 = dict_search(&table, 0LLU);
	CHECK_C(key3 == key);
	CHECK_C(*(int*)dict_get(&table, 0LLU, NULL) == 1);
	key3 = dict_search(&table, table.capacity);
	CHECK_C(key3 == key2);
	CHECK_C(*(int*)dict_get(&table, table.capacity, NULL) == 2);
	dict_clear(&table);
}

TEST_C(dict_table, char2ull)
{
	char a[] = "a";

	CHECK_C('a' == char2key(a));
}
