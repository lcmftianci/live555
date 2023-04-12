cmake_minimum_required(VERSION 3.10)

project(CommonPlay)

include_directories(${CMAKE_SOURCE_DIR}/liveMedia/include)
include_directories(${CMAKE_SOURCE_DIR}/BasicUsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/UsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/groupsock/include)

file(GLOB CommonPlay_SRCS "*.cpp" "*.hpp")
add_executable(CommonPlay ${CommonPlay_SRCS})

target_link_libraries(CommonPlay PRIVATE liveMedia BasicUsageEnvironment groupsock UsageEnvironment)

target_compile_definitions(CommonPlay PRIVATE _CRT_SECURE_NO_WARNINGS)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)