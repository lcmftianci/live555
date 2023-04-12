cmake_minimum_required(VERSION 3.10)

project(RtspServer)

include_directories(${CMAKE_SOURCE_DIR}/liveMedia/include)
include_directories(${CMAKE_SOURCE_DIR}/BasicUsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/UsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/groupsock/include)

file(GLOB MEDIA_SERVER_SRCS "*.cpp" "*.hpp")
add_executable(RtspServer ${MEDIA_SERVER_SRCS})

target_link_libraries(RtspServer PRIVATE liveMedia BasicUsageEnvironment groupsock UsageEnvironment)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)