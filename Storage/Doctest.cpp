#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <cstdlib>
#include <ctime>

extern "C" {
#include "Disk_Mng_Master.h"
#include "Disk_Mng_Master_API.h"
}
struct GlobalSetup {
    GlobalSetup() {
        srand(time(0));
    }
};
GlobalSetup globalSetup;

int generateRandomNumber() {
    int lower = 0;
    int upper = DISK_SIZE;
    return (rand() % (upper - lower + 1)) + lower;
}
int index, popValue,value;
StackNode_t* newStackNode;

//stack tests

//TEST_CASE("test_stack_initilaizeWithEmptyIndexes")
//{
//    CHECK(disk_mng_CB->diskFreeIndexesInArray->top == NULL);
//}
TEST_CASE("test_stack_push")
{
    disk_mng_initialize();
    for (size_t i = 0; i < DISK_SIZE; i++)
    {  
    index = generateRandomNumber();
    newStackNode = stackNode_create(index);
    stack_push(newStackNode);
    popValue = stack_pop();
    CHECK(popValue == index);
    }

}
TEST_CASE("test_stack_pop")
{
    disk_mng_initialize();
    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        index = generateRandomNumber();
        newStackNode = stackNode_create(index);
        stack_push(newStackNode);
        value = stack_pop();
        CHECK(value !=stack_top());
    }
}
TEST_CASE("test_stack_top")
{
    disk_mng_initialize();
    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        index = generateRandomNumber();
        newStackNode = stackNode_create(index);
        stack_push(newStackNode);
        value = stack_pop();
        CHECK(value == index);
    }
}
TEST_CASE("test_stack_is_empty")
{
    disk_mng_initialize();
    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        index = generateRandomNumber();
        newStackNode = stackNode_create(index);
        stack_push(newStackNode);
    }
    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        stack_pop();
    }
    CHECK(stack_is_empty() == TRUE);

}
