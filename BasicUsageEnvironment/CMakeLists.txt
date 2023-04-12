cmake_minimum_required(VERSION 3.0)

project(BasicUsageEnvironment VERSION 1.0.0 LANGUAGES CXX)

file(GLOB BasicUsageEnvironment_SRC
    "include/*.h"
    "*.cpp"
)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(BasicUsageEnvironment STATIC ${BasicUsageEnvironment_SRC})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)