# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

set(MY_BOARD nRF52840DK)
set(MY_OS FreeRTOS)
set(MY_PROJECT "DW3_QM33_SDK")
set(MY_HAL nrfx)
set(MY_BSP Nordic)
set(MY_CPU NRF52840_XXAA)
set(MY_LD_FILE nRF52840.ld)

set(CMAKE_CUSTOM_C_FLAGS
    "-Werror \
                          -DBOARD_CUSTOM \
                          -DUSB_ENABLE \
                          -DCONFIG_GPIO_AS_PINRESET \
                          -DNRF_CRYPTO_BACKEND_MBEDTLS_ENABLED=1 \
                          -DNRF_CRYPTO_ALLOCATOR=NRF_CRYPTO_ALLOCATOR_MALLOC \
                          -DMBEDTLS_CMAC_C"
)

add_definitions(-O3)

# Processor related defines
set(PROJECT_ARCH "m4")
set(PROJECT_FP "hard")
set(PROJECT_FPU "fpv4-sp-d16")

set(USE_CRYPTO 1)

set(USE_DRV_DW3000 1)
set(USE_DRV_DW3720 1)

set(CONFIG_QM33 1)

# Disable APPROTECT
set(CONFIG_SOC_PROTECT 0)

# enable RTT logs
set(CONFIG_LOG ON)

# 0=NONE 1=ERROR 2=WARNING 3=INFO 4=DEBUG
set(CONFIG_QLOG_LEVEL 3)
