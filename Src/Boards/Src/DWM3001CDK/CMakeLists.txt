# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

add_library(
  Board ${CMAKE_CURRENT_LIST_DIR}/../Common_NRF/nrf52_board.c
        ${CMAKE_CURRENT_LIST_DIR}/${MY_OS}/DWM3001CDK.c
)

target_sources(
  Board PRIVATE ${CMAKE_CURRENT_LIST_DIR}/platform_l1_config.c
                ${CMAKE_CURRENT_LIST_DIR}/../Common/helper_platform_l1_config.c
)

target_include_directories(
  Board
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Common ${CMAKE_CURRENT_LIST_DIR}/../../Inc
         ${CMAKE_CURRENT_LIST_DIR}/../Common_NRF ${CMAKE_CURRENT_LIST_DIR}/../Common_OTP
         ${CMAKE_CURRENT_LIST_DIR}/../Common
)

target_link_libraries(Board app_common)
