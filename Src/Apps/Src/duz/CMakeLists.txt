# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

add_library(duz STATIC utils.c responder.c responder_fn.c task_responder.c investigator.c investigator_fn.c task_investigator.c)

target_include_directories(
  listener PRIVATE ${PROJECT_BASE}/Src/UWB ./ ../../Inc ../common ${MY_OS}/
)

target_link_libraries(duz PUBLIC Config EventManager HAL app_common)
