# Copyright (c) 2025 RobotPilots-SZU
# SPDX-License-Identifier: Apache-2.0

set(GENERATED_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/generated)
set(INCLUDE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(nanopb CONFIG REQUIRED)

# Generated *.pb.c files placed manually into generated/.
file(GLOB GENERATED_SRCS CONFIGURE_DEPENDS ${GENERATED_DIR}/*.pb.c)

file(GLOB_RECURSE WRAPPER_SRCS CONFIGURE_DEPENDS
    ${SRC_DIR}/*.c
    ${SRC_DIR}/*.cpp
)

add_library(midware_protocol STATIC
    ${GENERATED_SRCS}
    ${WRAPPER_SRCS}
)
add_library(midware::protocol ALIAS midware_protocol)

target_include_directories(midware_protocol
    PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<BUILD_INTERFACE:${GENERATED_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(midware_protocol
    PUBLIC
        nanopb::nanopb
)

# nanopb runtime is C99; the wrapper is C++17.
set_target_properties(midware_protocol PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

install(TARGETS midware_protocol
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# message.hpp and generated protobuf headers are part of the middleware API.
install(DIRECTORY ${INCLUDE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY ${GENERATED_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.pb.h"
)
