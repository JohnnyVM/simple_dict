#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

/** For each C test group */
TEST_GROUP_C_WRAPPER(dict_table)
{
    TEST_GROUP_C_SETUP_WRAPPER(dict_table); /** optional */
    TEST_GROUP_C_TEARDOWN_WRAPPER(dict_table); /** optional */
};

TEST_C_WRAPPER(dict_table, create_insert_delete_number);
TEST_C_WRAPPER(dict_table, create_insert_delete_char);
TEST_C_WRAPPER(dict_table, create_insert_update_delete);
TEST_C_WRAPPER(dict_table, create_insert_collision_search);
TEST_C_WRAPPER(dict_table, stress);
TEST_C_WRAPPER(dict_table, char2ull);

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
