cmake_minimum_required(VERSION 3.5)
project(communication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB COMMUNICATION_FILES "Communication/*.cpp")
file(GLOB VALIDATION_FILES "Validation/*.cpp")
file(GLOB DATATRANSFERRING_FILES "Data_transferring/*.cpp")

include_directories("Communication" "Validation" "Data_transferring")
set(SOURCE_FILES ${VALIDATION_FILES})
list(APPEND SOURCE_FILES ${COMMUNICATION_FILES} )
list(APPEND SOURCE_FILES ${DATATRANSFERRING_FILES} )
add_executable(communication ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(communication PRIVATE Threads::Threads)

target_include_directories(communication PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

