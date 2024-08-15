cmake_minimum_required(VERSION 3.5)
project(communication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB COMMUNICATION_FILES "Communication/Communication/*.cpp")
file(GLOB VALIDATION_FILES "Communication/Validation/*.cpp")
file(GLOB DATA_COMPRESSION_FILES "Communication/Data_Compression/*.cpp")
file(GLOB DATA_TRANSFERRING_FILES "Communication/Data_Transferring/*.cpp")

include_directories("Communication/Communication" "Communication/Validation" "Communication/Data_Compression" "Communication/Data_Transferring")

set(SOURCE_FILES ${VALIDATION_FILES})
list(APPEND SOURCE_FILES ${COMMUNICATION_FILES} )
list(APPEND SOURCE_FILES ${DATA_TRANSFERRING_FILES} )
list(APPEND SOURCE_FILES ${DATA_COMPRESSION_FILES} )
add_executable(communication ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(communication PRIVATE Threads::Threads)

target_include_directories(communication PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

