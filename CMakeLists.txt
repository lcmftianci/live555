cmake_minimum_required(VERSION 3.0)

project(live555 VERSION 2023.04.12 LANGUAGES C CXX)


option(ENABLE_WEBCAM "Enable WebCam RTSP Server" ON)
option(ENABLE_PLAYER "Enable RTSP Player" OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/UsageEnvironment/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/liveMedia/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/BasicUsageEnvironment/include)


add_subdirectory(BasicUsageEnvironment)
add_subdirectory(groupsock)
add_subdirectory(liveMedia)
add_subdirectory(UsageEnvironment)
add_subdirectory(RtspClient)
add_subdirectory(RtspServer)

# 如果需要支持播放功能，需要下载ffmpeg和opencv,下方是例子
if(ENABLE_PLAYER)
    add_subdirectory(RtspPlayer)
endif()
# 下方是支持webcam的rtspserver例子，如果需要支持需要下载opencv
if(ENABLE_WEBCAM)
    find_package(OpenCV REQUIRED)
    message(STATUS "OpenCV include dir: ${OpenCV_INCLUDE_DIRS}")
    add_subdirectory(WebCamRtspServer)
endif()