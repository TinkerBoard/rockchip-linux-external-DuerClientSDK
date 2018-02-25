cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Set project information
project(DCSDeviceApp VERSION 1.0.3 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LOG_FILENAME__='\"$(notdir $<)\"' -D__SAMPLEAPP_VERSION__='\"${__SAMPLEAPP_VERSION__}\"' -D__DCSSDK_VERSION__='\"${__DCSSDK_VERSION__}\"' -D__DUER_LINK_VERSION__='\"${__DUER_LINK_VERSION__}\"'")

include_directories(
    sdk/include
    sdk/include/duer_link
    sdk/include/duer_link/network_observer
    sdk/include/duer_link/device_ctrl_observer
    third/include)

link_directories(
    sdk/lib)

add_definitions(-DLinux)

add_subdirectory("DCSApp")
add_subdirectory("Framework")
add_subdirectory("appresources")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/sdk/lib DESTINATION ${CMAKE_INSTALL_PREFIX})
#install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_INSTALL_PREFIX}/../data/)
