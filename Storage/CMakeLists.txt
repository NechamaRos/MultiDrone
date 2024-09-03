cmake_minimum_required(VERSION "3.30.2")

project("Disk_MNG_Master")

add_executable("${PROJECT_NAME}" "Doctest.cpp" "Disk_Mng_Master.c" "Disk_Mng_Master.h" "Disk_Mng_Master_API.h" "doctest.h")

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)     #$ (CMAKE_INSTALL_PREFIX)/bin

install(FILES "Doctest.cpp" DESTINATION src)		#$ (CMAKE_INSTALL_PREFIX)/src