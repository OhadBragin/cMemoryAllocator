cmake_minimum_required(VERSION 4.3)
project(cMemoryAllocator C)

set(CMAKE_C_STANDARD 11)

add_executable(cMemoryAllocator main.c
        memalloc.c
        memalloc.h)
