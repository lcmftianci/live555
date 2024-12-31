cmake_minimum_required(VERSION 3.10)

project(WebCamRtspServer)

# add_definitions(-DUSE_LIVE555_BOOLEAN)
include_directories(${CMAKE_SOURCE_DIR}/liveMedia/include)
include_directories(${CMAKE_SOURCE_DIR}/BasicUsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/UsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/groupsock/include)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB MEDIA_SERVER_SRCS "*.cpp" "*.hpp")

add_executable(WebCamRtspServer ${MEDIA_SERVER_SRCS})

target_link_libraries(WebCamRtspServer PRIVATE liveMedia BasicUsageEnvironment groupsock UsageEnvironment)
target_link_libraries(WebCamRtspServer PRIVATE ${OpenCV_LIBS})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)