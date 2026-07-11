# Copyright (c) 2025 RobotPilots-SZU
# SPDX-License-Identifier: Apache-2.0
#
# Aggregates all OS-agnostic middleware libraries.

add_subdirectory(osal)
add_subdirectory(protocol)

# link module (add once it has sources)
# add_subdirectory(link)