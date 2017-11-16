cmake_minimum_required(VERSION 3.8)
project(rcb_file_system)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES main.c messages.h format_utils.h data_structures.h reader.h generic_utils.h writer.h boot_utils.h rcb_utils.h)
add_executable(rcb_file_system ${SOURCE_FILES})
target_link_libraries(rcb_file_system m)