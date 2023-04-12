cmake_minimum_required(VERSION 3.10)

project(RtspClient)

file(GLOB SOURCES "*.cpp" "*.hh" "*.h")

set(FFMPEG_INCLUDE_DIR "D:/commonlib/include")
set(FFMPEG_LIBRARY_DIR "D:/commonlib/lib")
set(OPENCV_INCLUDE_DIR "D:/commonlib/opencv/opencv3416/build/include")
set(OPENCV_LIBRARY_DIR "D:/commonlib/opencv/opencv3416/build/x64/vc15/lib")

include_directories(${CMAKE_SOURCE_DIR}/liveMedia/include)
include_directories(${CMAKE_SOURCE_DIR}/BasicUsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/groupsock/include)
include_directories(${CMAKE_SOURCE_DIR}/UsageEnvironment/include)
include_directories(${FFMPEG_INCLUDE_DIR})
include_directories(${OPENCV_INCLUDE_DIR})


add_executable(RtspClient ${SOURCES})

target_link_directories(RtspClient PRIVATE ${FFMPEG_LIBRARY_DIR})
target_link_directories(RtspClient PRIVATE ${OPENCV_LIBRARY_DIR})
target_link_libraries(RtspClient PRIVATE ws2_32)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(RtspClient PRIVATE opencv_world3416d)
endif()

target_link_libraries(RtspClient PRIVATE avcodec avutil avdevice swresample swscale avformat)
target_link_libraries(RtspClient PRIVATE liveMedia BasicUsageEnvironment groupsock UsageEnvironment)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)