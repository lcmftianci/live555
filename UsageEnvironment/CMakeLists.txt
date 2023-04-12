cmake_minimum_required(VERSION 3.0)

project(UsageEnvironment)

file(GLOB UsageEnvironment_SRC "*.cpp" "include/*.h" "include/*.hh")

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(UsageEnvironment STATIC ${UsageEnvironment_SRC})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)