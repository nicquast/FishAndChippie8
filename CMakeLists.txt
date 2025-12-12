cmake_minimum_required(VERSION 4.0)
project(FishAndChippie8 C)

set(CMAKE_C_STANDARD 23)

find_package(SDL3 REQUIRED)

add_executable(FishAndChippie8
        src/main.c
        src/display/display.c
        src/cpu/cpu.c
        src/cpu/cpu.h
        src/memory/memory.c
        src/memory/memory.h
        src/system/system.c
        src/system/system.h
        src/keypad/keypad.c
        src/keypad/keypad.h)
target_link_libraries(FishAndChippie8 PRIVATE SDL3::SDL3 m)
