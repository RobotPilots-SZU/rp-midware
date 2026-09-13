# Copyright (c) 2025 RobotPilots-SZU
# SPDX-License-Identifier: Apache-2.0
#
# middleware protocol library (nanopb runtime + generated messages + C++ wrapper).
#
# The wrapper (src/) is C++ and depends on OSAL for logging. The nanopb
# runtime and the generated *.pb.c are pure C. Can be built stand-alone or
# reused by the Zephyr module glue via add_subdirectory().

set(NANOPB_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/nanopb)
set(GENERATED_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/generated)
set(INCLUDE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/src)

# nanopb runtime sources (from the submodule, do not edit).
set(NANOPB_RUNTIME_SRCS
    ${NANOPB_DIR}/pb_common.c
    ${NANOPB_DIR}/pb_encode.c
    ${NANOPB_DIR}/pb_decode.c
)

# Generated *.pb.c files placed manually into generated/.
# GLOB is used on purpose so newly copied files are picked up on re-configure.
file(GLOB GENERATED_SRCS CONFIGURE_DEPENDS ${GENERATED_DIR}/*.pb.c)

# Wrapper/implementation sources for the protocol API (C++).
file(GLOB_RECURSE WRAPPER_SRCS CONFIGURE_DEPENDS
    ${SRC_DIR}/*.c
    ${SRC_DIR}/*.cpp
)

add_library(middleware_protocol STATIC
    ${NANOPB_RUNTIME_SRCS}
    ${GENERATED_SRCS}
    ${WRAPPER_SRCS}
)
add_library(middleware::protocol ALIAS middleware_protocol)

target_include_directories(middleware_protocol
    PUBLIC
        ${INCLUDE_DIR}
        # ${NANOPB_DIR}
    PRIVATE
        ${GENERATED_DIR}
)


# nanopb build-time options. Adjust as needed; kept minimal and OS-free.
target_compile_definitions(middleware_protocol PUBLIC
    PB_FIELD_32BIT
)

# nanopb runtime is C99; the wrapper is C++17.
set_target_properties(middleware_protocol PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
