# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

add_library(ProjectDefinition ProjectName.c ../isr_hs.c)

target_include_directories(ProjectDefinition PUBLIC ./)

include(uwb_stack_llhw.cmake)
