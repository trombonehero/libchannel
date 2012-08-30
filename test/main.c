#include <libchannel.h>

#include <assert.h>
#include <fcntl.h>

#include "tests.h"

struct test all_tests[] = {
	TEST(lifecycle),
	TEST(message_data),
	TEST(message_complex),
};
int test_count = sizeof(all_tests) / sizeof(struct test);

int
main(int argc, char *argv[])
{
	return (run_all_tests(test_count, all_tests, argc, argv) != PASSED);
}

