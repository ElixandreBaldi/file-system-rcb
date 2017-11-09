cmake_minimum_required(VERSION 3.8)
project(rcb_file_system)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c messages.h format_utils.h)
add_executable(rcb_file_system ${SOURCE_FILES})