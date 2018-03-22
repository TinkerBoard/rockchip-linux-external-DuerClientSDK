cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(APPRESOURCES LANGUAGES CXX)

include(../build/BuildDefaults.cmake)

if(EXISTS "${PROJECT_SOURCE_DIR}/${Platform}/")
    install(DIRECTORY
            "${PROJECT_SOURCE_DIR}/${Platform}/"
            DESTINATION
            "${CMAKE_INSTALL_PREFIX}/appresources")
else()
    install(DIRECTORY
            "${PROJECT_SOURCE_DIR}/Ubuntu/"
            DESTINATION
            "${CMAKE_INSTALL_PREFIX}/appresources")
endif()

