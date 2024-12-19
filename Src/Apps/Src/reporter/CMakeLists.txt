# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

add_library(reporter STATIC usb_reporter usb_reporter/usb_reporter.c)

target_include_directories(reporter PUBLIC ./ usb_reporter)

target_link_libraries(reporter PUBLIC app_common HAL SDK)
