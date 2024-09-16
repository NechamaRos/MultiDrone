cmake_minimum_required(VERSION 3.8)
project(MasterCacheImages C CXX)

# Add source to this project's executable.
add_executable(MasterCacheImages "MasterCacheImages.c" "Doctest.cpp")

# Define paths for DLL and Header
set(DLL_DIR "${CMAKE_SOURCE_DIR}/../SYCL/build/DLL")  # Updated to use relative path
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/MasterCacheImages")  # Path to the directory containing your Headers

# Add path for Headers
include_directories(${HEADER_DIR})

# Add the directory where the .lib file is located
link_directories(${DLL_DIR})

# Link with the DLL library
target_link_libraries(MasterCacheImages PRIVATE "${DLL_DIR}/SYCL_DLL.lib")

# Set the runtime output directory
set_target_properties(MasterCacheImages PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE Debug)

# Copy the DLL to the output directory
add_custom_command(TARGET MasterCacheImages POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${DLL_DIR}/SYCL_DLL.dll"
    $<TARGET_FILE_DIR:MasterCacheImages>)



