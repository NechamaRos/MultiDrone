cmake_minimum_required(VERSION "3.0")

project(Disk_Mng_Master)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB C "./Disk_Mng_Master.c")
file(GLOB SYCL "./Disk_Mng_Master_Sycl.cpp")
file(GLOB DOCTEST "./Doctest.cpp")
set(SOURCE_FILES "doctest.h")
list(APPEND  SOURCE_FILES ${C} ${SYCL} ${DOCTEST})



add_executable(Disk_Mng_Master ${SOURCE_FILES})

target_include_directories(Disk_Mng_Master PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Disk_Mng_Master PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

