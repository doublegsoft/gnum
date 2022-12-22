
cmake_minimum_required(VERSION 3.6)

project(gnum VERSION 1.0.0 DESCRIPTION "g number framework")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-format-security -Wno-unused-parameter -Wno-ignored-qualifiers" )

find_library(METAL Metal)
find_library(FOUNDATION Foundation)
find_library(QUARTZCORE QuartzCore)

include_directories(include 3rd/metal-cmake/metal-cpp)

add_subdirectory(3rd/metal-cmake)

add_library(gnum STATIC
  src/gnum.cpp
)

target_link_libraries(gnum PRIVATE 
  "-framework Metal" 
  "-framework Foundation" 
  "-framework QuartzCore"
)


add_executable(test_gnum++
  test/test_gnum.cpp
)

target_link_libraries(test_gnum++ PRIVATE gnum)

add_executable(test_gnum
  test/test_gnum.c
)

target_link_libraries(test_gnum PRIVATE gnum)