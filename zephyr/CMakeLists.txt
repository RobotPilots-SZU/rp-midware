# if(NOT COMMAND zephyr_include_directories)
#     message(FATAL_ERROR
#         "breeze-middleware Zephyr port must be included by a Zephyr application")
# endif()

# set(BREEZE_MIDDLEWARE_DIR
#     ${CMAKE_CURRENT_LIST_DIR}/../..
# )

add_subdirectory(midware)

# zephyr_include_directories(
#     ${BREEZE_MIDDLEWARE_DIR}/posix_limit
# )

zephyr_compile_definitions(PLATFORM_ZEPHYR)
