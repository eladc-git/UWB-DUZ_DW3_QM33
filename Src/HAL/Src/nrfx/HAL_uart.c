/**
 * @file      HAL_uart.c
 *
 * @brief     HAL implementation of UART functionalities
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "HAL_uart.h"

#include <stdint.h>
#include <stdbool.h>

#include "boards.h"
#include "nrf_uart.h"
#include "app_uart.h"
#include "circular_buffers.h"
#include "HAL_error.h"
#include "HAL_timer.h"

extern void NotifyControlTask(void);

static bool discard_next_symbol = false;
static uint32_t UART_timeout;
static bool UART_is_down = false;
static data_circ_buf_t _uartRx;
data_circ_buf_t *uartRx = &_uartRx;

void Restart_UART_timer()
{
    Timer.start(&UART_timeout);
}

bool IsUartDown(void)
{
    return UART_is_down;
}

void SetUartDown(bool val)
{
    UART_is_down = val;
}

/**
 * @brief Uart error handler
 * @param p_event Pointer to UART event.
 */
void deca_uart_error_handle(app_uart_evt_t *p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        error_handler(0, _ERR_General_Error);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        error_handler(0, _ERR_General_Error);
    }
    /* This event indicates that UART data has been received  */
    else if (p_event->evt_type == APP_UART_DATA_READY)
    {
        deca_uart_receive();
    }
}

/**
 * @brief Function for initializing the UART module.
 */
void deca_uart_init(void)
{
    uint32_t err_code;
    const app_uart_comm_params_t comm_params = {
        UART_0_RX_PIN,
        UART_0_TX_PIN,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud115200};

    /* Tx and Rx buffers have the same size. */
    APP_UART_FIFO_INIT(&comm_params, UART_RX_BUF_SIZE, UART_RX_BUF_SIZE, deca_uart_error_handle, APP_IRQ_PRIORITY_LOW, err_code);
    UNUSED_VARIABLE(err_code);
}

void deca_uart_close(void)
{
    app_uart_flush();
    app_uart_close();
}

/**
 * @brief  Function for transmitting data on UART
 *
 * @param  ptr Pointer is contain base address of data.
 */
int deca_uart_transmit(uint8_t *ptr, uint16_t size)
{
    int ret = NRF_SUCCESS;
    for (int i = 0; i < size; i++)
    {
        if (app_uart_put(ptr[i]) != NRF_SUCCESS)
        {
            ret = _ERR_UART_TX;
            break;
        }
    }
    return ret;
}

/**
 * @brief Function for receive data from UART and store into rx_buf (global array).
 */
void deca_uart_receive(void)
{
    uint32_t err_code;
    uint8_t rx_data;
    int head, tail, size;

    head = uartRx->head;
    tail = uartRx->tail;
    size = UART_RX_BUF_SIZE;

    err_code = app_uart_get(&rx_data);
    err_code = err_code; // for warning
    if (discard_next_symbol)
    {
        discard_next_symbol = false;
    }
    else
    {
        if (CIRC_SPACE(head, tail, size) > 0)
        {
            uartRx->buf[head] = rx_data;
            head = (head + 1) & (size - 1);
            uartRx->head = head;
        }
        else
        {
            /* USB RX packet can not fit free space in the buffer */
        }
        NotifyControlTask();
    }
}

/**
 * @brief Discard next incoming symbol, used while wakening up UART
 * from sleep as the first receiving symbol is a garbage).
 */
void deca_discard_next_symbol(void)
{
    discard_next_symbol = true;
}
