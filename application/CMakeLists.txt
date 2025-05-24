cmake_minimum_required(VERSION 3.12)
include(../pico_sdk_import.cmake)

project(PicoTracer C CXX ASM)
set(CMAKE_C_STNDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(application STATIC control.c dc_motor.c)
target_link_libraries(application pico_stdlib)
