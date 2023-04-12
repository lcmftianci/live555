cmake_minimum_required(VERSION 3.0)

project(groupsock)

file(GLOB groupdock_src
    "*.cpp"
    "include/*.h"
    "include/*.hh"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(groupsock STATIC ${groupdock_src})

target_compile_definitions(groupsock PRIVATE -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)