# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

add_library(EventManager STATIC EventManager.c)

target_include_directories(EventManager PUBLIC ./)

target_link_libraries(EventManager PUBLIC app_common Board)
