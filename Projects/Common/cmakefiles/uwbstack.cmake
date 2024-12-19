# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

if(PROJECT_FP STREQUAL "hard")
  set(STACK_FP "hfp")
elseif(PROJECT_FP STREQUAL "softfp")
  set(STACK_FP "sfp")
  set(PROJECT_FP "soft")
elseif(PROJECT_FP STREQUAL "soft")
  set(STACK_FP "sfp")
endif()

set(QORVO_DISABLE_KCONFIG ON)
# Check if it is UCI or CLI build
if(USE_UCI)
  set(CONFIG_UCI
      ON
      CACHE BOOL "activate UCI" FORCE
  )
  set(CONFIG_UCI_CORE
      ON
      CACHE BOOL "activate UCI Core" FORCE
  )
  set(CONFIG_UCI_BACKENDS_DEPS
      ON
      CACHE BOOL "activate UCI backends deps" FORCE
  )
  set(CONFIG_UCI_BACKEND_CORE
      ON
      CACHE BOOL "activate UCI CORE backend" FORCE
  )
  set(CONFIG_UCI_BACKEND_CONFIG_MANAGER
      ON
      CACHE BOOL "activate UCI backend config manager" FORCE
  )
else()
  set(CONFIG_UCI
      OFF
      CACHE BOOL "disable UCI" FORCE
  )
  set(CONFIG_UCI_CORE
      OFF
      CACHE BOOL "disable UCI Core" FORCE
  )
  set(CONFIG_UCI_BACKENDS_DEPS
      OFF
      CACHE BOOL "disable UCI backends deps" FORCE
  )
  set(CONFIG_UCI_BACKEND_CORE
      OFF
      CACHE BOOL "disable UCI CORE backend" FORCE
  )
  set(CONFIG_UCI_BACKEND_CONFIG_MANAGER
      OFF
      CACHE BOOL "disable UCI backend config manager" FORCE
  )
endif()

if(CONFIG_LOG)
  add_definitions(-DCONFIG_LOG=1)
endif()

set(CMAKE_LIBRARY_ARCHITECTURE "arm-cortex-${PROJECT_ARCH}-${PROJECT_FP}_floating")
# Check if we need fira only
if(USE_UCI OR USE_PCTT)
  message("Using full uwbstack")
  set(UWBSTACK_LIBS_PATH
      "${CMAKE_CURRENT_LIST_DIR}/../../../${LIBS_PATH}/uwbstack_libs/delivery/full/Release"
  )
else()
  message("Using Fira only uwbstack")
  set(UWBSTACK_LIBS_PATH
      "${CMAKE_CURRENT_LIST_DIR}/../../../${LIBS_PATH}/uwbstack_libs/delivery/fira/Release"
  )
endif()

# Define the path where find_package should look for libs
set(CMAKE_PREFIX_PATH "${UWBSTACK_LIBS_PATH}")

# List packages, order is important for some
find_package(cbor QUIET)
find_package(log QUIET)
find_package(qtils QUIET)
find_package(linux-compat_itf QUIET)
find_package(linux-compat QUIET)
find_package(uwbmac_itf QUIET)
find_package(uwbmac_common QUIET)
find_package(uwbmac_helper QUIET)
find_package(uwbmac_msg QUIET)
find_package(uwbmac_msg_serialization QUIET)
find_package(uwbmac_embedded QUIET)
find_package(mac_itf QUIET)
find_package(quwbs_itf QUIET)
find_package(qobpack QUIET)
find_package(qblk QUIET)
find_package(embedded_mac QUIET)
find_package(mcps802154_fira_round_hopping_crypto QUIET)
find_package(mcps802154_fira_time_sync QUIET)
find_package(mcps802154_core QUIET)
find_package(mcps802154_fbs QUIET)
find_package(mcps802154_fbs_fira QUIET)
find_package(llhw QUIET)
find_package(l1_config_itf QUIET)
find_package(l1_config QUIET)
find_package(mac_config_itf QUIET)
find_package(mac_config QUIET)
find_package(config_manager_itf QUIET)
find_package(config_manager QUIET)

if(USE_PCTT)
  find_package(mcps802154_pctt QUIET)
endif()

if(USE_UCI)
  find_package(uci_core QUIET)
  find_package(uci_backend QUIET)
  find_package(uci_backend_coordinator QUIET)
endif()

add_library(uwbstack ALIAS embedded_mac)
