# cMemoryAllocator

A custom memory allocator in C implementing standard dynamic memory management routines.

## Features

- Custom heap memory allocation (`my_malloc`)
- Memory deallocation (`my_free`)
- Contiguous array allocation (`my_calloc`)
- Dynamic reallocation (`my_realloc`)

## Project Structure

- `memalloc.h` - Public interface and block definitions
- `memalloc.c` - Core allocator logic
- `main.c` - Test driver / entry point
- `CMakeLists.txt` - CMake build configuration

## Building

Requires CMake and a C11 compliant compiler.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
