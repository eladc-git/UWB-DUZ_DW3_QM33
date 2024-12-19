# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

# get module relative path
if(PROJECT_FP STREQUAL "hard")
  set(STACK_FP "hfp")
elseif(PROJECT_FP STREQUAL "softfp")
  set(STACK_FP "sfp")
elseif(PROJECT_FP STREQUAL "soft")
  set(STACK_FP "sfp")
endif()

message("Build driver from sources")
set(UWB_DRIVER_SOURCE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../${LIBS_PATH}/dwt_uwb_driver)

add_library(uwbdriver)

if(USE_DRV_DW3000 EQUAL 1)
  target_sources(uwbdriver PRIVATE ${UWB_DRIVER_SOURCE_PATH}/dw3000/dw3000_device.c)
endif()
if(USE_DRV_DW3720 EQUAL 1)
  target_sources(uwbdriver PRIVATE ${UWB_DRIVER_SOURCE_PATH}/dw3720/dw3720_device.c)
endif()

target_sources(
  uwbdriver
  PRIVATE ${UWB_DRIVER_SOURCE_PATH}/deca_interface.c ${UWB_DRIVER_SOURCE_PATH}/deca_compat.c
          ${UWB_DRIVER_SOURCE_PATH}/deca_rsl.c ${UWB_DRIVER_SOURCE_PATH}/lib/qmath/src/qmath.c
)
target_include_directories(
  uwbdriver PUBLIC ${UWB_DRIVER_SOURCE_PATH} ${UWB_DRIVER_SOURCE_PATH}/lib/qmath/include
)

if(USE_DRV_DW3000 EQUAL 1)
  target_include_directories(uwbdriver PRIVATE ${UWB_DRIVER_SOURCE_PATH}/dw3000)
endif()
if(USE_DRV_DW3720 EQUAL 1)
  target_include_directories(uwbdriver PRIVATE ${UWB_DRIVER_SOURCE_PATH}/dw3720)
endif()

target_link_libraries(uwbdriver PUBLIC qplatform_deca_compat)

target_compile_options(
  uwbdriver PRIVATE -Wall -Wextra -Wno-error=pointer-sign -Wno-error=unused-variable
                    -Wno-error=unused-function -Wno-error=maybe-uninitialized
)
