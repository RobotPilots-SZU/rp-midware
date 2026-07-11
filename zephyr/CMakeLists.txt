# Copyright (c) 2025 RobotPilots-SZU
# SPDX-License-Identifier: Apache-2.0
#
# Zephyr module glue. Reuses the OS-agnostic middleware libraries defined in
# middleware/ and links them into the Zephyr application.

if(CONFIG_MIDDLEWARE)

  # Reuse the stand-alone middleware build. The second argument gives the
  # generated build tree a stable name.
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../middleware middleware)

  if(CONFIG_MIDDLEWARE_OSAL)
    zephyr_link_libraries(middleware::osal)

    # Platform backend. This DOES include <zephyr/*.h>, so it is compiled by
    # the Zephyr build system (not part of the OS-agnostic osal library).
    zephyr_library_sources(${CMAKE_CURRENT_LIST_DIR}/port/osal_backend_zephyr.c)
  endif()

  if(CONFIG_MIDDLEWARE_PROTOCOL)
    zephyr_link_libraries(middleware::protocol)
  endif()

endif()