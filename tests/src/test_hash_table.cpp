#include "CppUTest/TestHarness.h"

extern "C" {
	#include "hash_table.h"
}

TEST_GROUP(hash_table)
{
	struct hash_table table = {.capacity = 0};

  void setup()
  {
  }

  void teardown()
  {
  }
};

TEST(hash_table, create_insert_delete)
{
	long long unsigned key, key2;

	key = hash_insert(&table, 0LLU, NULL);
	CHECK(key != DUMMY_KEY);
	CHECK(table.used == 1);
	key2 = hash_delete(&table, 0LLU);
	CHECK(key2 == key);
	free(table.slot);
	table.capacity = 0;
}

TEST(hash_table, create_insert_update_delete)
{
	long long unsigned key, key2;

	key = hash_insert(&table, 0LLU, (const void*)1);
	CHECK(key != DUMMY_KEY);
	key2 = hash_insert(&table, 0LLU, (const void*)2);
	CHECK(key2 == key);
	CHECK(table.used == 1);
	key = hash_delete(&table, 0LLU);
	CHECK(table.used == 0);
	CHECK(key2 == key);
	CHECK(hash_get(&table, key, NULL) == NULL);
	CHECK(table.used == 0);
	free(table.slot);
	table.capacity = 0;
}

TEST(hash_table, create_insert_collision_search)
{
	long long unsigned key, key2, key3;

	key = hash_insert(&table, 0LLU, (const void*)1);
	CHECK(key != DUMMY_KEY);
	key2 = hash_insert(&table, table.capacity, (const void*)2);
	CHECK(table.used == 2);
	CHECK(key2 != DUMMY_KEY);
	key3 = hash_search(&table, 0LLU);
	CHECK(key3 == key);
	CHECK((long unsigned)hash_get(&table, key3, NULL) == 1);
	key3 = hash_search(&table, table.capacity);
	CHECK(key3 == key2);
	CHECK((long unsigned)hash_get(&table, table.capacity, NULL) == 2);
	free(table.slot);
	table.capacity = 0;
}

TEST(hash_table, stress)
{
	long long unsigned key;
	unsigned int tryes = 100;

	for(unsigned long i = 0; i < tryes; ++i ) {
		key = hash_insert(&table, i, (void*)(long long)i);
		CHECK(key != DUMMY_KEY);
	}
	CHECK(table.used == (unsigned)tryes);
	for(unsigned long i = 0; i < tryes; ++i ) {
		key = hash_search(&table, i);
		CHECK(key != DUMMY_KEY);
	}
	for(unsigned long i = 0; i < tryes; ++i ) {
		key = hash_delete(&table, i);
		CHECK(key != DUMMY_KEY);
	}
	CHECK(table.used == 0);
	for(unsigned long i = 0; i < tryes; ++i ) {
		key = hash_search(&table, i);
		CHECK(key == DUMMY_KEY);
	}
	free(table.slot);
	table.capacity = 0;
}

TEST(hash_table, char2ull)
{
	char a[] = "a";

	CHECK(int('a') == char2ull(a));
}
