/**
 * @file      custom_board.h
 *
 * @brief     Pin mapping description for DWM3001CDK
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

#include "nrf_gpio.h"
#include "nrf_spim.h"

/* LEDs definitions for PCA10056. */
#define LEDS_NUMBER 4

/* If AOA chip is populated on the PCB without second antenna, this shall be set to (1). */
#define AOA_CHIP_ON_NON_AOA_PCB (0)

#define LED_1                   NRF_GPIO_PIN_MAP(0, 4)  /* D9 on the schematics. */
#define LED_2                   NRF_GPIO_PIN_MAP(0, 5)  /* D10 on the schematics. */
#define LED_3                   NRF_GPIO_PIN_MAP(0, 22) /* D11 on the schematics. */
#define LED_4                   NRF_GPIO_PIN_MAP(0, 14) /* D12 on the schematics. */
#define LED_START               LED_1
#define LED_STOP                LED_4

#define LEDS_ACTIVE_STATE       0

#define LEDS_LIST                  \
    {                              \
        LED_1, LED_2, LED_3, LED_4 \
    }

#define LEDS_INV_MASK        LEDS_MASK

#define BSP_LED_0            4
#define BSP_LED_1            5
#define BSP_LED_2            22
#define BSP_LED_3            14

#define BUTTONS_NUMBER       1

#define BUTTON_1             2

#define BUTTON_PULL          NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST \
    {                \
        BUTTON_1     \
    }

#define BSP_BUTTON_0   BUTTON_1

#define RX_PIN_NUMBER  15
#define TX_PIN_NUMBER  19
#define CTS_PIN_NUMBER (-1)
#define RTS_PIN_NUMBER (-1)
#define HWFC           false


#define LED_ERROR      BSP_LED_0
/* UART symbolic constants. */
#define UART_0_TX_PIN TX_PIN_NUMBER
#define UART_0_RX_PIN RX_PIN_NUMBER

#ifdef HAL_DW3000
/* Arduino board mappings. */
#define ARDUINO_13_PIN NRF_GPIO_PIN_MAP(0, 3)  /* Used as DW3000_CLK_Pin. */
#define ARDUINO_12_PIN NRF_GPIO_PIN_MAP(0, 29) /* Used as DW3000_MISO_Pin. */
#define ARDUINO_11_PIN NRF_GPIO_PIN_MAP(0, 8)  /* Used as DW3000_MOSI_Pin. */
#define ARDUINO_10_PIN NRF_GPIO_PIN_MAP(1, 6)  /* Used as DW3000_CS_Pin. */
#define ARDUINO_9_PIN  NRF_GPIO_PIN_MAP(1, 19) /* Used as DW3000_WKUP_Pin. */
#define ARDUINO_8_PIN  NRF_GPIO_PIN_MAP(1, 2)  /* Used as DW3000_IRQ_Pin. */
#define ARDUINO_7_PIN  NRF_GPIO_PIN_MAP(0, 25) /* Used as DW3000_RST_Pin. */

#define DW3000_RST_Pin ARDUINO_7_PIN
#define DW3000_IRQ_Pin ARDUINO_8_PIN
#define DW3000_WUP_Pin ARDUINO_9_PIN

/* SPI defs. */
#define DW3000_CS_Pin           ARDUINO_10_PIN
#define DW3000_CLK_Pin          ARDUINO_13_PIN /* DWM3000 shield SPIM1 sck connected to UWB chip. */
#define DW3000_MOSI_Pin         ARDUINO_11_PIN /* DWM3000 shield SPIM1 mosi connected to UWB chip. */
#define DW3000_MISO_Pin         ARDUINO_12_PIN /* DWM3000 shield SPIM1 miso connected to UWB chip. */
#define DW3000_SPI_IRQ_PRIORITY APP_IRQ_PRIORITY_LOW

#define DW3000_RTS_PIN_NUM      UART_PIN_DISCONNECTED
#define DW3000_CTS_PIN_NUM      UART_PIN_DISCONNECTED

#define DW3000_MIN_SPI_FREQ     (NRF_SPIM_FREQ_4M)
#define DW3000_MAX_SPI_FREQ     (NRF_SPIM_FREQ_32M)
#define DW3000_SPI_IDX          (3)
#endif /* HAL_DW3000 */

#ifdef __cplusplus
}
#endif
