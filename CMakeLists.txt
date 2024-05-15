cmake_minimum_required(VERSION 3.14...3.29)

project(
  mempager
  VERSION 0.2
  DESCRIPTION "Memory Pager"
  LANGUAGES C)

add_executable(
  mempager
  src/mempager.c
  src/argparse.c
  src/argparse.h
  src/pager.c
  src/pager.h
  src/pagers/fifo.c
  src/pagers/fifo.h
  src/pagers/clock.c
  src/pagers/clock.h
  src/utils/mem.c
  src/utils/mem.h
  src/utils/rand.c
  src/utils/rand.h
  src/utils/misc.c
  src/utils/misc.h
  src/utils/types.h)

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
  target_link_libraries(mempager PUBLIC ${MATH_LIBRARY})
else()
  message(FATAL_ERROR "C Math library not found.")
endif()

set_property(TARGET mempager PROPERTY C_STANDARD 11)

set(CMAKE_C_FLAGS_DEBUG
    "${CMAKE_C_FLAGS_DEBUG} -g3 -Wall -Wextra -Wpedantic -Wdouble-promotion \
    -Wconversion -fsanitize=address,undefined -fdiagnostics-color=always")
