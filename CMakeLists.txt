cmake_minimum_required(VERSION 3.5)
project(DOOM_PROJECT)

file(GLOB_RECURSE sources src/*.c src/*.h)

add_executable(DOOM ${sources})

# target_compile_options(DOOM PUBLIC -Wall -std=c23 -D_POSIX_C_SOURCE=200809L)
target_compile_options(DOOM PUBLIC -std=c23 -D_POSIX_C_SOURCE=200809L)

if (MSVC)
  add_compile_options(/W0)
else()
  add_compile_options(-w)
  target_link_libraries(DOOM m)
endif()