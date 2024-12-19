# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

add_library(fira STATIC common_fira.c fira_dw3000.c fira_app.c fira_fn.c)

target_include_directories(fira PUBLIC ./ ../../Inc ../common)

target_link_libraries(fira PUBLIC Config HAL UWB uwbstack reporter Crypto)
