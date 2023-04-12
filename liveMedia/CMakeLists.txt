cmake_minimum_required(VERSION 3.0)

project(liveMedia)

file(GLOB liveMedia_SRC
    "*.cpp"
    "include/*.h"
    "include/*.hh"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(liveMedia STATIC ${liveMedia_SRC})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)