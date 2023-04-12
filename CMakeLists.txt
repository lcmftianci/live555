cmake_minimum_required(VERSION 3.0)

project(live555 VERSION 2023.04.12 LANGUAGES C CXX)

add_subdirectory(BasicUsageEnvironment)
add_subdirectory(groupsock)
add_subdirectory(liveMedia)
add_subdirectory(UsageEnvironment)
add_subdirectory(RtspClient)
add_subdirectory(RtspServer)
add_subdirectory(CommonPlay)