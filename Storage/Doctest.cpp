#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <cstdlib>
#include <ctime>

extern "C" {
#include "Disk_Mng_Master.h",
}

struct GlobalSetup {
	GlobalSetup() {
		srand(time(0));
	}
};
GlobalSetup globalSetup;

int generateRandomNumber(int upper) {
	int lower = 0;
	return (rand() % (upper - lower + 1)) + lower;
}


//initialize 

TEST_CASE("test_disk_mng_initialize_CB")
{
	disk_mng_initialize_CB();
	CHECK(disk_mng_CB != NULL);
}
