# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

set(SDK_VERSION NORDIC_SDK_17_1_0)
set(SDK_SRC
    ${SDK_VERSION}/components/boards/boards.c
    ${SDK_VERSION}/components/libraries/fifo/app_fifo.c
    ${SDK_VERSION}/components/libraries/uart/app_uart_fifo.c
    ${SDK_VERSION}/components/libraries/util/app_util_platform.c
    ${SDK_VERSION}/components/libraries/util/app_error.c
    ${SDK_VERSION}/components/libraries/util/app_error_weak.c
    ${SDK_VERSION}/components/libraries/atomic/nrf_atomic.c
    ${SDK_VERSION}/integration/nrfx/legacy/nrf_drv_spi.c
    ${SDK_VERSION}/integration/nrfx/legacy/nrf_drv_clock.c
    ${SDK_VERSION}/integration/nrfx/legacy/nrf_drv_power.c
    ${SDK_VERSION}/integration/nrfx/legacy/nrf_drv_uart.c
    ${SDK_VERSION}/integration/nrfx/legacy/nrf_drv_rng.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_rng.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_gpiote.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_rtc.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_spi.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_spim.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_timer.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_uart.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_uarte.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_power.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_wdt.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_clock.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_nvmc.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/prs/nrfx_prs.c
    ${SDK_VERSION}/modules/nrfx/hal/nrf_nvmc.c
    ${SDK_VERSION}/modules/nrfx/soc/nrfx_atomic.c
    ${SDK_VERSION}/components/libraries/queue/nrf_queue.c
    # Only for debugging
    ${SDK_VERSION}/components/libraries/strerror/nrf_strerror.c
)
set(SDK_RTT_SRC
    ${SDK_VERSION}/components/libraries/log/src/nrf_log_backend_rtt.c
    ${SDK_VERSION}/components/libraries/log/src/nrf_log_default_backends.c
    ${SDK_VERSION}/components/libraries/log/src/nrf_log_backend_serial.c
    ${SDK_VERSION}/components/libraries/log/src/nrf_log_frontend.c
    ${SDK_VERSION}/components/libraries/log/src/nrf_log_str_formatter.c
    ${SDK_VERSION}/external/segger_rtt/SEGGER_RTT.c
    ${SDK_VERSION}/external/segger_rtt/SEGGER_RTT_Syscalls_SES.c
    ${SDK_VERSION}/external/segger_rtt/SEGGER_RTT_printf.c
    ${SDK_VERSION}/components/libraries/memobj/nrf_memobj.c
    ${SDK_VERSION}/components/libraries/balloc/nrf_balloc.c
    ${SDK_VERSION}/components/libraries/ringbuf/nrf_ringbuf.c
    ${SDK_VERSION}/external/fprintf/nrf_fprintf.c
    ${SDK_VERSION}/external/fprintf/nrf_fprintf_format.c
)

set(SDK_INC
    ${SDK_VERSION}/integration/nrfx/legacy
    ${SDK_VERSION}/modules/nrfx
    ${SDK_VERSION}/modules/nrfx/hal
    ${SDK_VERSION}/modules/nrfx/drivers/include
    ${SDK_VERSION}/components/boards
    ${SDK_VERSION}/components/libraries/atomic
    ${SDK_VERSION}/components/libraries/delay
    ${SDK_VERSION}/components/libraries/fifo
    ${SDK_VERSION}/components/libraries/log
    ${SDK_VERSION}/components/libraries/log/src
    ${SDK_VERSION}/components/libraries/uart
    ${SDK_VERSION}/components/libraries/usbd
    ${SDK_VERSION}/components/libraries/timer
    ${SDK_VERSION}/components/libraries/usbd/class/cdc
    ${SDK_VERSION}/components/libraries/usbd/class/cdc/acm
    ${SDK_VERSION}/components/libraries/experimental_section_vars
    ${SDK_VERSION}/components/libraries/strerror
    ${SDK_VERSION}/modules/nrfx/mdk
    ${SDK_VERSION}/integration/nrfx
    ${SDK_VERSION}/components/toolchain/cmsis/include
    ${SDK_VERSION}/components/libraries/util
    ${SDK_VERSION}/components/drivers_nrf/nrf_soc_nosd
    ${SDK_VERSION}/external/utf_converter
    ${SDK_VERSION}/external/mbedtls/include
    ${SDK_VERSION}/components/libraries/fds
)

set(SDK_RTT_INC
    ${SDK_VERSION}/components/libraries/log/
    ${SDK_VERSION}/external/segger_rtt
    ${SDK_VERSION}/components/libraries/memobj
    ${SDK_VERSION}/components/libraries/balloc
    ${SDK_VERSION}/external/fprintf
    ${SDK_VERSION}/components/libraries/ringbuf
    ${SDK_VERSION}/components/libraries/memobj
)

set(SYSTICK_SRC ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_systick.c)
set(SYSTICK_INC ${SDK_VERSION}/modules/nrfx/drivers/include ${SDK_VERSION}/modules/nrfx)

set(SDK_USB_SRC
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_usbd.c
    ${SDK_VERSION}/modules/nrfx/drivers/src/nrfx_systick.c
    ${SDK_VERSION}/components/libraries/usbd/app_usbd.c
    ${SDK_VERSION}/components/libraries/usbd/app_usbd_core.c
    ${SDK_VERSION}/components/libraries/usbd/app_usbd_serial_num.c
    ${SDK_VERSION}/components/libraries/usbd/app_usbd_string_desc.c
    ${SDK_VERSION}/components/libraries/usbd/class/cdc/acm/app_usbd_cdc_acm.c
    ${SDK_VERSION}/components/libraries/uart/app_uart_fifo.c
)

set(SDK_CRYPTO_INC
    ${SDK_VERSION}/components/libraries/crypto
    ${SDK_VERSION}/components/libraries/crypto/backend/cc310
    ${SDK_VERSION}/components/libraries/crypto/backend/mbedtls
    ${SDK_VERSION}/components/libraries/crypto/backend/cifra
    ${SDK_VERSION}/components/libraries/crypto/backend/oberon
    ${SDK_VERSION}/components/libraries/crypto/backend/nrf_hw
    ${SDK_VERSION}/components/libraries/crypto/backend/optiga
    ${SDK_VERSION}/components/libraries/stack_info
)

set(SDK_CRYPTO_SRC
    ${SDK_VERSION}/external/mbedtls/library/aes.c
    ${SDK_VERSION}/external/mbedtls/library/arc4.c
    ${SDK_VERSION}/external/mbedtls/library/blowfish.c
    ${SDK_VERSION}/external/mbedtls/library/camellia.c
    ${SDK_VERSION}/external/mbedtls/library/ccm.c
    ${SDK_VERSION}/external/mbedtls/library/chacha20.c
    ${SDK_VERSION}/external/mbedtls/library/chachapoly.c
    ${SDK_VERSION}/external/mbedtls/library/cipher.c
    ${SDK_VERSION}/external/mbedtls/library/cipher_wrap.c
    ${SDK_VERSION}/external/mbedtls/library/cmac.c
    ${SDK_VERSION}/external/mbedtls/library/des.c
    ${SDK_VERSION}/external/mbedtls/library/gcm.c
    ${SDK_VERSION}/external/mbedtls/library/platform_util.c
    ${SDK_VERSION}/external/mbedtls/library/poly1305.c
    ${SDK_VERSION}/components/libraries/crypto/nrf_crypto_aes.c
    ${SDK_VERSION}/components/libraries/crypto/backend/mbedtls/mbedtls_backend_aes.c
    ${SDK_VERSION}/components/libraries/crypto/nrf_crypto_rng.c
    ${SDK_VERSION}/components/libraries/crypto/backend/nrf_hw/nrf_hw_backend_init.c
    ${SDK_VERSION}/components/libraries/crypto/backend/nrf_hw/nrf_hw_backend_rng.c
    ${SDK_VERSION}/components/libraries/crypto/nrf_crypto_init.c
    ${SDK_VERSION}/components/libraries/crypto/backend/mbedtls/mbedtls_backend_aes_aead.c
    ${SDK_VERSION}/components/libraries/crypto/nrf_crypto_aead.c
)

if(${MY_CPU} STREQUAL NRF52833_XXAA)
  set(SDK_SRC
      ${SDK_SRC} ${SDK_CRYPTO_SRC} ${SDK_USB_SRC} ${SDK_LIBS_SRC}
      ${SDK_VERSION}/modules/nrfx/mdk/gcc_startup_nrf52833.S
      ${SDK_VERSION}/modules/nrfx/mdk/system_nrf52833.c
  )
endif()

if(${MY_CPU} STREQUAL NRF52840_XXAA)
  set(SDK_SRC
      ${SDK_SRC} ${SDK_CRYPTO_SRC} ${SDK_USB_SRC}
      ${SDK_VERSION}/modules/nrfx/mdk/gcc_startup_nrf52840.S
      ${SDK_VERSION}/modules/nrfx/mdk/system_nrf52840.c
  )
endif()

add_library(sdk_systick ${SYSTICK_SRC})
target_include_directories(
  sdk_systick
  PUBLIC ${SYSTICK_INC}
  PRIVATE ${SDK_INC} ../../Src/Boards/Src/${MY_BOARD}/${MY_OS}
)
target_link_libraries(sdk_systick PUBLIC ProjectDefinition)

if(CONFIG_LOG)
  add_library(log_rtt ${SDK_RTT_SRC})

  target_include_directories(
    log_rtt
    PUBLIC ${SDK_RTT_INC}
    PRIVATE ${SDK_INC}
  )

  target_link_libraries(log_rtt PUBLIC ProjectDefinition Board)
endif(CONFIG_LOG)

set(SDK_INC ${SDK_INC} ${SDK_CRYPTO_INC} ${SDK_LIB_INC})

add_library(SDK ${SDK_SRC})

target_include_directories(
  SDK
  PUBLIC ${SDK_INC}
  PRIVATE ./${SDK_VERSION} ../../Src/Boards/Src/${MY_BOARD}/${MY_OS}
)

target_link_libraries(SDK ProjectDefinition Board)

if(CONFIG_LOG)
  target_link_libraries(SDK log_rtt)
endif()
