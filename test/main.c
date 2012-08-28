#include <libchannel.h>

#include <assert.h>
#include <fcntl.h>
#include <stdio.h> // TODO: tmp

#include <sys/mman.h> // TODO: tmp

#include "tests.h"

struct test all_tests[] = {
	TEST(lifecycle)
};
int test_count = sizeof(all_tests) / sizeof(struct test);

int
main(int argc, char *argv[])
{
	return (run_all_tests(test_count, all_tests, argc, argv) != PASSED);
}

