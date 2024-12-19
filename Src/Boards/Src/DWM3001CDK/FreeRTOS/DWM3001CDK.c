/**
 * @file      DWM3001CDK.c
 *
 * @brief     Board specific initialization
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "app_error.h"
#include "boards.h"
#include "thisBoard.h"
#include "nrf_drv_clock.h"
#include "HAL_error.h"
#if defined(USB_ENABLE)
#include "HAL_usb.h"
#endif
#include "usb_uart_rx.h"
#include "driver_app_config.h"
#include "HAL_uart.h"
#include "HAL_rtc.h"
#include "HAL_timer.h"
#include "HAL_watchdog.h"
#include "nrf52_board.h"

#include <qerr.h>
#include <llhw.h>
#include <persistent_time.h>

void peripherals_init(void)
{
    ret_code_t err_code;

    /* With this change, Reset After Power Cycle is not required. */
    nrf_gpio_cfg_input(UART_0_RX_PIN, NRF_GPIO_PIN_PULLUP);

    if (!nrf_drv_clock_init_check())
    {
        err_code = nrf_drv_clock_init();
        APP_ERROR_CHECK(err_code);
    }

#if defined(CONFIG_LOG)
    /* Enable RTC clock to flush RTT logs via log_processing task. */

    /* Start explicelty LFCLK before using the RTC
     * LFXO is highly recommended as the LFCLK clock source, since the
     * LFRC has a large frequency variation. */
    nrfx_clock_lfclk_start();
#endif

#if !defined(USB_ENABLE)
    /* Workaround for nRF SDK issue. We cannot turn on HFCLK for all the time when we use USB.
     * When USB_ENABLE flag is on, then LFCLK request is being handled in the HAL_usb.c */
    nrf_drv_clock_hfclk_request(NULL);
    while (!nrf_drv_clock_hfclk_is_running())
        ;
#endif

    Rtc.init();
    Timer.init();

    /* Watchdog 60sec. */
    Watchdog.init(60000);
}

void BoardInit(void)
{
    handle_approtect();
    bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS);
    peripherals_init();
}

void board_interface_init(void)
{
#if defined(USB_ENABLE)
    Usb.init(usb_rx_callback_circular_buf);
#endif
    if (is_uart_allowed())
    {
        deca_uart_init();
    }
    persistent_time_init(0);
}
