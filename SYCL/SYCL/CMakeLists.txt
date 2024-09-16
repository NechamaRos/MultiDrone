# if(DEFINED USM AND (NOT(USM EQUAL 0)))
#     message(STATUS "Using the USM variant.")
set(SOURCE_FILE SyclSearch.cpp)
set(TARGET_NAME SyclSearch)
# endif()

if(WIN32)
    set(WIN_FLAG "/EHsc")
endif()

set(COMPILE_FLAGS "-fsycl -Wall ${WIN_FLAG}")
set(LINK_FLAGS "-fsycl")

add_executable(${TARGET_NAME} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")
set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS}")
add_custom_target(cpu-gpu DEPENDS ${TARGET_NAME})

add_library(SYCL_DLL SHARED ${SOURCE_FILE})

# Enable export of symbols for DLL
set_target_properties(SYCL_DLL PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Apply compile and link flags
set_target_properties(SYCL_DLL PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")
set_target_properties(SYCL_DLL PROPERTIES LINK_FLAGS "${LINK_FLAGS}")

# Define export macro for DLL
target_compile_definitions(SYCL_DLL PRIVATE SYCL_DLL_EXPORTS)

# Link to SYCL libraries
target_link_libraries(SYCL_DLL PRIVATE sycl)

# Output directories for the DLL and LIB files
set_target_properties(SYCL_DLL PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/DLL
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/DLL  # Location for .lib file
)

# Custom target for the DLL
add_custom_target(cpu-gpuDLL DEPENDS SYCL_DLL)
