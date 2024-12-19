# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

include(project_common.cmake)

set(MY_TARGET ${MY_BOARD}-${MY_PROJECT}_UCI-${MY_OS})

# define a UCI build
set(USE_CLI 0)
set(USE_UCI 1)

# Enable PCTT commands on UCI Backend
set(USE_PCTT 1)
# Enable FTM UCI
set(USE_FTM_UCI 1)
# enable uci config manager
set(USE_CONF_MANAGER_UCI 1)
# Enable UCI mac
set(USE_MAC_UCI 1)
# Enable UCI mac calibrations
set(USE_MAC_CALIB_UCI 1)

# Non-FiRa applications
set(USE_LISTENER 0)

# MAC regions
set(USE_FIRA 1)
