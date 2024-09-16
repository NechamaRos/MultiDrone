#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cstdint>  
#include"doctest.h"
#include"func.h"
#include<iostream>

TEST_CASE("multiply_arrays ")
{
	//arrange
    const int size = 10; // Size of the arrays
    int array1[size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int array2[size] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int result[size] = {0};
    std::cout<<"befor";

    // Act
    multiply_arrays(array1, array2, result, size);
    std::cout<<"after";
	//assert
	CHECK(result[0]==0);
		
}
 
