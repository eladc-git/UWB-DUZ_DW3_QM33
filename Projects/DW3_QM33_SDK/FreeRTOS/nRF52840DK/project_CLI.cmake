# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

include(project_common.cmake)

set(MY_TARGET ${MY_BOARD}-${MY_PROJECT}_CLI-${MY_OS})

# define a CLI build
set(USE_CLI 1)
set(USE_UCI 0)

# Non-FiRa applications
set(USE_LISTENER 1)

# MAC regions
set(USE_FIRA 1)
