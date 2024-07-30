cmake_minimum_required(VERSION 3.5)

project(communication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB COMMUNICATION_FILES "communication/*.*")
file(GLOB VALIDATION_FILES "Validation/*.*")
include_directories("communication" "Validation")

set(SOURCE_FILES ${VALIDATION_FILES})
list(APPEND SOURCE_FILES ${COMMUNICATION_FILES} )
add_executable(communication ${SOURCE_FILES})

target_include_directories(communication PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(communication PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
