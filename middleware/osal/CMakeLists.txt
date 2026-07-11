# Copyright (c) 2025 RobotPilots-SZU
# SPDX-License-Identifier: Apache-2.0
#
# OSAL: OS Abstraction Layer. OS-agnostic core (log/time/mutex/memory) plus
# C++ raw-bus interfaces (UART/CAN/SPI). No OS headers here; platforms provide
# implementations and inject them at start-up.

set(OSAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(OSAL_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE OSAL_SRCS CONFIGURE_DEPENDS
    ${OSAL_SRC_DIR}/*.c
    ${OSAL_SRC_DIR}/*.cpp
)

add_library(middleware_osal STATIC ${OSAL_SRCS})
add_library(middleware::osal ALIAS middleware_osal)

target_include_directories(middleware_osal PUBLIC ${OSAL_INCLUDE_DIR})

set_target_properties(middleware_osal PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)