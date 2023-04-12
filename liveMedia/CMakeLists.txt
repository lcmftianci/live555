cmake_minimum_required(VERSION 3.0)

project(liveMedia)

file(GLOB liveMedia_SRC
    "*.cpp"
    "*.c" 
    "include/*.h"
    "include/*.hh"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/UsageEnvironment/include)
add_library(liveMedia STATIC ${liveMedia_SRC})

target_compile_definitions(liveMedia PRIVATE -D_CRT_SECURE_NO_WARNINGS)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)