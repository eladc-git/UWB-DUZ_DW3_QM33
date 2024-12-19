# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

add_library(Config STATIC ${MY_BOARD}/config.c)

target_include_directories(Config PUBLIC Inc DefaultConfigs)

target_link_libraries(
  Config
  PUBLIC UWB
  Interface ProjectDefinition app_common qhal uwbstack
)
