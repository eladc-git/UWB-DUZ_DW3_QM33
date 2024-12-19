# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

add_library(Helpers STATIC cJSON.c deca_dbg.c)

target_include_directories(Helpers PUBLIC Inc)

target_link_libraries(Helpers PUBLIC OSAL)
