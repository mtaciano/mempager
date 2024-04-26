cmake_minimum_required(VERSION 3.14...3.29)

project(
  mempager
  VERSION 0.1
  DESCRIPTION "Memory pager"
  LANGUAGES C)

add_executable(
  mempager
  src/mempager.c
  src/utils.c
  src/utils.h
  src/pager.c
  src/pager.h
  src/pagers/fifo.c
  src/pagers/fifo.h
  src/pagers/clock.c
  src/pagers/clock.h)

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
  target_link_libraries(mempager PUBLIC ${MATH_LIBRARY})
endif()

set(CMAKE_C_FLAGS_DEBUG
    "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -fsanitize=address,undefined"
)
