# Copyright (c) 2025 RobotPilots-SZU
# SPDX-License-Identifier: Apache-2.0
#
# Stand-alone entry point for the OS-agnostic Breeze middleware.
#
# This is the build used by the Conan recipe. Zephyr applications consume the
# resulting package instead of adding this source tree as a Zephyr module.

cmake_minimum_required(VERSION 3.24)

set(RP_MIDWARE_VERSION_FILE "${CMAKE_CURRENT_LIST_DIR}/VERSION")

if(NOT EXISTS "${RP_MIDWARE_VERSION_FILE}")
    message(FATAL_ERROR "Missing version file: ${RP_MIDWARE_VERSION_FILE}")
endif()

file(READ "${RP_MIDWARE_VERSION_FILE}" RP_MIDWARE_VERSION)
string(STRIP "${RP_MIDWARE_VERSION}" RP_MIDWARE_VERSION)

if(RP_MIDWARE_VERSION STREQUAL "")
    message(FATAL_ERROR "Empty version file: ${RP_MIDWARE_VERSION_FILE}")
endif()

project(rp_midware
    VERSION "${RP_MIDWARE_VERSION}"
    LANGUAGES C CXX
)

include(GNUInstallDirs)

option(MIDDLEWARE_BUILD_TESTS "Build middleware tests" OFF)

add_subdirectory(midware)

if(MIDDLEWARE_BUILD_TESTS)
    enable_testing()
endif()
