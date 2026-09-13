# Copyright (c) 2025 RobotPilots-SZU
# SPDX-License-Identifier: Apache-2.0
#
# Stand-alone entry point for the OS-agnostic Breeze middleware.
#
# Build without any OS/RTOS:
#   cmake -S . -B build
#   cmake --build build
#
# The Zephyr build does NOT use this file; it goes through zephyr/CMakeLists.txt
# which reuses the same middleware/ libraries via add_subdirectory().

cmake_minimum_required(VERSION 3.20)

project(breeze_middleware
    VERSION 0.1.0
    LANGUAGES C CXX
)

option(MIDDLEWARE_BUILD_TESTS "Build stand-alone middleware tests" OFF)

add_subdirectory(midware)

if(MIDDLEWARE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
