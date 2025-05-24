cmake_minimum_required(VERSION 3.12)
include(../pico_sdk_import.cmake)

project(PicoTracer C CXX ASM)
set(CMAKE_C_STNDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(driver STATIC indicators.c cycle.c sw.c lineSensor.c TB6612FNG.c adchange.c)
target_link_libraries(driver pico_stdlib hardware_pwm)
target_link_libraries(driver pico_stdlib hardware_adc)
