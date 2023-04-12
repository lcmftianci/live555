cmake_minimum_required(VERSION 3.10)

project(RtspClient)

include_directories(${CMAKE_SOURCE_DIR}/liveMedia/include)
include_directories(${CMAKE_SOURCE_DIR}/BasicUsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/UsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/groupsock/include)

file(GLOB RtspClient_SRCS "*.cpp" "*.hpp")
add_executable(RtspClient ${RtspClient_SRCS})

target_link_libraries(RtspClient PRIVATE liveMedia BasicUsageEnvironment groupsock UsageEnvironment)

target_compile_definitions(RtspClient PRIVATE _CRT_SECURE_NO_WARNINGS)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)