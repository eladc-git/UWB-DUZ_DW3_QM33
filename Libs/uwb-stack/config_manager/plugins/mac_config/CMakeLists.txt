# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-Qorvo-2

cmake_minimum_required(VERSION 3.13)

add_library(mac_config_itf INTERFACE)
target_include_directories(mac_config_itf INTERFACE include)

if(COMMAND qorvo_install_target)
  qorvo_install_target(mac_config_itf)
endif()

add_library(mac_config INTERFACE)
target_link_libraries(mac_config INTERFACE mac_config_itf)

if(COMMAND qorvo_install_target)
  qorvo_install_target(mac_config)
endif()
