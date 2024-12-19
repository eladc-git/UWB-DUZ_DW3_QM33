/**
 * @file      HAL_usb.c
 *
 * @brief     HAL functions for usb interface
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "nrf_drv_usbd.h"
#include "app_error.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"
#include "boards.h"
#include "InterfUsb.h"
#include "HAL_usb.h"
#include "nrf_drv_clock.h"

#define LED_USB_RESUME   (BSP_BOARD_LED_0)
#define LED_CDC_ACM_OPEN (BSP_BOARD_LED_1)

/**
 * @brief Enable power USB detection
 *
 * Configure if example supports USB port connection
 */
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif

static volatile bool tx_pending = false;
static void (*rx_callback)(uint8_t *ptr, size_t len) = NULL;

static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const *p_inst, app_usbd_cdc_acm_user_event_t event);

#define CDC_ACM_COMM_INTERFACE 0
#define CDC_ACM_COMM_EPIN      NRF_DRV_USBD_EPIN2

#define CDC_ACM_DATA_INTERFACE 1
#define CDC_ACM_DATA_EPIN      NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT     NRF_DRV_USBD_EPOUT1

/**
 * @brief CDC_ACM class instance
 */
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm, cdc_acm_user_ev_handler, CDC_ACM_COMM_INTERFACE, CDC_ACM_DATA_INTERFACE, CDC_ACM_COMM_EPIN, CDC_ACM_DATA_EPIN, CDC_ACM_DATA_EPOUT, APP_USBD_CDC_COMM_PROTOCOL_AT_V250);

#define READ_SIZE 1

static char m_rx_buffer[NRF_DRV_USBD_EPSIZE];

/**
 * @brief User event handler @ref app_usbd_cdc_acm_user_ev_handler_t (headphones).
 */
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const *p_inst, app_usbd_cdc_acm_user_event_t event)
{
    app_usbd_cdc_acm_t const *p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

    switch (event)
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            tx_pending = false;
            bsp_board_led_on(LED_CDC_ACM_OPEN);

            /* Setup first transfer. */
            ret_code_t ret = app_usbd_cdc_acm_read_any(&m_app_cdc_acm, m_rx_buffer, sizeof(m_rx_buffer));
            UNUSED_VARIABLE(ret);
            break;
        }
        case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
            tx_pending = false;
            bsp_board_led_off(LED_CDC_ACM_OPEN);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
            tx_pending = false;
            break;
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            /* Get amount of data transferred. */
            size_t len = app_usbd_cdc_acm_rx_size(p_cdc_acm);

            /* Setup next transfer. */
            ret_code_t ret = app_usbd_cdc_acm_read_any(&m_app_cdc_acm, m_rx_buffer, sizeof(m_rx_buffer));
            rx_callback((uint8_t *)m_rx_buffer, len);
            (void)ret;
            break;
        }
        default:
            break;
    }
}

static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_DRV_SUSPEND:
            bsp_board_led_off(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_DRV_RESUME:
            bsp_board_led_on(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            bsp_board_leds_off();
            break;
        case APP_USBD_EVT_POWER_DETECTED:

            if (!nrf_drv_usbd_is_enabled())
            {
                /* Workaround for nRF SDK issue. When HFCLK is turned on, then USB hangs on
                 * initialization for ~14 seconds. To workaround this issue, HFCLK is disabled before
                 * enabling USB.

                 * Warning: Plugging USB cable while ranging is ongoing can affect measurement
                 * performance for a short moment of time. */
                if (nrf_drv_clock_hfclk_is_running())
                {
                    nrf_drv_clock_hfclk_release();
                    while (nrf_drv_clock_hfclk_is_running())
                        ;
                }
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            app_usbd_stop();
            /* Workaround for nRF SDK issue. HFCLK cannot be turned on all the time. It needs to be
             * explicitly turned on after USB has stopped (e.g. cable plugged off). */
            nrf_drv_clock_hfclk_request(NULL);
            break;
        case APP_USBD_EVT_POWER_READY:
            app_usbd_start();
            break;
        case APP_USBD_EVT_STATE_CHANGED:
            if (APP_USBD_STATE_Configured == app_usbd_core_state_get())
            {
                UsbSetState(USB_CONFIGURED);
            }
            break;
        default:
            break;
    }
}

static bool deca_usb_transmit(unsigned char *tx_buffer, int size)
{
    ret_code_t ret;

    for (int i = 0; i < 1; i++)
    {
        tx_pending = true;
        ret = app_usbd_cdc_acm_write(&m_app_cdc_acm, tx_buffer, size);
        if (ret != NRF_SUCCESS)
        {
            break;
        }
    }
    return ret == NRF_SUCCESS;
}

static void deca_usb_init(void (*rx_callback_)(uint8_t *ptr, size_t len))
{
    ret_code_t ret;
    rx_callback = rx_callback_;
    static const app_usbd_config_t usbd_config = {
        .ev_state_proc = usbd_user_ev_handler};

    app_usbd_serial_num_generate();

    tx_pending = false;
    ret = app_usbd_init(&usbd_config);
    APP_ERROR_CHECK(ret);

    app_usbd_class_inst_t const *class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
    ret = app_usbd_class_append(class_cdc_acm);
    APP_ERROR_CHECK(ret);

    if (USBD_POWER_DETECTION)
    {
#ifndef SOFTDEVICE_PRESENT
        ret = app_usbd_power_events_enable();
        APP_ERROR_CHECK(ret);
#endif
    }
    else
    {
        app_usbd_enable();
        app_usbd_start();
    }
}

static bool isTxBufferEmpty(void)
{
    return tx_pending == false;
}

static void InterfaceUsbUpdate(void)
{
}


/** @brief HAL USB API structure. */
const struct hal_usb_s Usb = {
    .init = &deca_usb_init,
    .deinit = NULL,
    .transmit = &deca_usb_transmit,
    .receive = NULL,
    .update = &InterfaceUsbUpdate,
    .isTxBufferEmpty = &isTxBufferEmpty};
