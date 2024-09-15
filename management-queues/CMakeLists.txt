cmake_minimum_required(VERSION 3.5)
project(QueuesManagment)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find pthread library
find_package(Threads REQUIRED)

file(GLOB SOURCE_FILES "management-queues/*.cpp")

add_executable(QueuesManagment ${SOURCE_FILES})

# Include directories
target_include_directories(QueuesManagment PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link pthread library
target_link_libraries(QueuesManagment PRIVATE Threads::Threads)
