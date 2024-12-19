/**
 * @file      custom_board.h
 *
 * @brief     Pin mapping description corresponding to nRF52840DK module
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "pca10056.h"
#include "nrf_spim.h"


/* UART symbolic constants. */
#define UART_0_TX_PIN TX_PIN_NUMBER
#define UART_0_RX_PIN RX_PIN_NUMBER

#define LED_ERROR     BSP_LED_0

#ifdef HAL_DW3000
#define DW3000_RST_Pin ARDUINO_7_PIN
#define DW3000_IRQ_Pin ARDUINO_8_PIN
#define DW3000_WUP_Pin ARDUINO_9_PIN
/* SPI defs. */
#define DW3000_RTS_PIN_NUM UART_PIN_DISCONNECTED
#define DW3000_CTS_PIN_NUM UART_PIN_DISCONNECTED
#define DW3000_CS_Pin      ARDUINO_10_PIN
/* DWM3000 shield SPIM1 sck connected to UWB chip. */
#define DW3000_CLK_Pin ARDUINO_13_PIN
/* DWM3000 shield SPIM1 mosi connected to UWB chip. */
#define DW3000_MOSI_Pin ARDUINO_11_PIN
/* DWM3000 shield SPIM1 miso connected to UWB chip. */
#define DW3000_MISO_Pin         ARDUINO_12_PIN
#define DW3000_SPI_IRQ_PRIORITY APP_IRQ_PRIORITY_LOW
#define DW3000_WUP_Pin          ARDUINO_9_PIN

#define DW3000_MIN_SPI_FREQ     (NRF_SPIM_FREQ_4M)
#define DW3000_MAX_SPI_FREQ     (NRF_SPIM_FREQ_32M)
#define DW3000_SPI_IDX          (3)
#endif /* HAL_DW3000 */

#ifdef __cplusplus
}
#endif
