cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

set(BOARD damiao_mc02)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(middleware)

zephyr_include_directories(include)
# zephyr_library()

target_sources(app PRIVATE ./main.cpp)