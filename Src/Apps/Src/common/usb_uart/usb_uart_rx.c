/**
 * @file      usb_uart_rx.c
 *
 * @brief     Module receiving data from USB/UART
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "usb_uart_rx.h"

#include <string.h>
#include <stdint.h>

#include "app.h"
#include "driver_app_config.h"
#include "controlTask.h"
#include "cmd.h"
#include "usb_uart_tx.h"
#ifdef USB_ENABLE
#include "HAL_usb.h"
#include "InterfUsb.h"
#endif
/* For bare-metal implementation this critical defines may be required. */
#define USB_UART_ENTER_CRITICAL()
#define USB_UART_EXIT_CRITICAL()


extern data_circ_buf_t *uartRx;
extern data_circ_buf_t *btRx;

#define MAX_CMD_LENGTH 0x400

static data_circ_buf_t _usbRx;
/* This is temporary. We must investigate how to share this buffer accrss task and here. */
data_circ_buf_t *usbRx = &_usbRx;

/* Receiving command type status. */
typedef enum
{
    cmdREGULAR = 0,
    cmdJSON,
    cmdUNKNOWN_TYPE
} command_type_e;

/**
 * @brief Waits only commands from incoming stream.
 * The binary interface (deca_usb2spi stream) is not allowed.
 *
 * @return COMMAND_READY : the data for future processing can be found in app.local_buff : app.local_buff_len
 * NO_DATA : no command yet
 */
usb_data_e waitForCommand(uint8_t *pBuf, uint16_t len, uint16_t *read_offset, uint16_t cyclic_size)
{
    usb_data_e ret = NO_DATA;
    static uint16_t cmdLen = 0;
    static uint8_t cmdBuf[MAX_CMD_LENGTH];
    uint16_t cnt;
    static command_type_e command_type = cmdUNKNOWN_TYPE;
    static uint8_t brackets_cnt;
    uint16_t local_buff_length = 0;

    /* Loop over the buffer rx data. */
    for (cnt = 0; cnt < len; cnt++)
    {
        /* Erase of a char in the terminal. */
        if (pBuf[*read_offset] == '\b')
        {
            port_tx_msg((uint8_t *)"\b\x20\b", 3);
            if (cmdLen)
                cmdLen--;
        }
        else
        {
            port_tx_msg(&pBuf[*read_offset], 1);
            if (pBuf[*read_offset] == '\n' || pBuf[*read_offset] == '\r')
            {
                /* Checks if need to handle regular command. */
                if ((cmdLen != 0) && (command_type == cmdREGULAR))
                {
                    memcpy(&local_buff[local_buff_length], cmdBuf, cmdLen);
                    local_buff[local_buff_length + cmdLen] = '\n';
                    local_buff_length += (cmdLen + 1);
                    cmdLen = 0;
                    command_type = cmdUNKNOWN_TYPE;
                    ret = COMMAND_READY;
                }
            }
            else if (command_type == cmdUNKNOWN_TYPE)
            {
                /* Find out if getting regular command or JSON. */
                cmdBuf[cmdLen] = pBuf[*read_offset];
                if (pBuf[*read_offset] == '{')
                {
                    /* Start Json command. */
                    command_type = cmdJSON;
                    brackets_cnt = 1;
                }
                else
                {
                    /* Start regular command. */
                    command_type = cmdREGULAR;
                }
                cmdLen++;
            }
            else if (command_type == cmdREGULAR)
            {
                /* Regular command. */
                cmdBuf[cmdLen] = pBuf[*read_offset];
                cmdLen++;
            }
            else
            {
                /* Json command. */
                cmdBuf[cmdLen] = pBuf[*read_offset];
                cmdLen++;
                if (pBuf[*read_offset] == '{')
                {
                    brackets_cnt++;
                }
                else if (pBuf[*read_offset] == '}')
                {
                    brackets_cnt--;
                    /* Got a full Json command. Update the app commands buffer. */
                    if (brackets_cnt == 0)
                    {
                        memcpy(&local_buff[local_buff_length], cmdBuf, cmdLen);
                        local_buff[local_buff_length + cmdLen] = '\n';
                        local_buff_length += (cmdLen + 1);
                        cmdLen = 0;
                        command_type = cmdUNKNOWN_TYPE;
                        ret = COMMAND_READY;
                    }
                }
            }
        }
        *read_offset = (*read_offset + 1) & cyclic_size;
        /* Checks if command too long and we need to reset it. */
        if (cmdLen >= sizeof(cmdBuf))
        {
            cmdLen = 0;
            command_type = cmdUNKNOWN_TYPE;
        }
    }

    if (ret == COMMAND_READY)
    {
        /* If there is at least 1 command, add 0 at the end. */
        local_buff[local_buff_length] = 0;
        local_buff_length++;
    }
    return (ret);
}

void usb_rx_callback_circular_buf(uint8_t *ptr, size_t len)
{
    int head = usbRx->head;
    int tail = usbRx->tail;
    int size = sizeof(usbRx->buf);

    if (CIRC_SPACE(head, tail, size) > len)
    {
        for (int i = 0; i < len; i++)
        {
            usbRx->buf[head] = ptr[i];
            head = (head + 1) & (size - 1);
        }

        usbRx->head = head;
    }
    else
    {
        /* USB RX packet can not fit free space in the buffer. */
    }
    extern void NotifyControlTask(void);
    NotifyControlTask();
}

/** @brief This should be calling on a reception of a data from UART or USB. */
usb_data_e usb_uart_rx(void)
{
    usb_data_e ret = NO_DATA;
    uint16_t uartLen, headUart, tailUart;
#ifdef USB_ENABLE
    uint16_t usbLen, headUsb, tailUsb;
#endif
    /* USART control prevails over USB control if both at the same time. */
    USB_UART_ENTER_CRITICAL();
    headUart = uartRx->head;
#ifdef USB_ENABLE
    headUsb = usbRx->head;
#endif

    USB_UART_EXIT_CRITICAL();

    tailUart = uartRx->tail;
    uartLen = CIRC_CNT(headUart, tailUart, sizeof(uartRx->buf));
#ifdef USB_ENABLE
    tailUsb = usbRx->tail;
    usbLen = CIRC_CNT(headUsb, tailUsb, sizeof(usbRx->buf));
#endif

    if ((uartLen > 0) && is_uart_allowed())
    {
        ret = AppGet()->on_rx(uartRx->buf, uartLen, &tailUart, sizeof(uartRx->buf) - 1);
        USB_UART_ENTER_CRITICAL();
        uartRx->tail = tailUart;
        USB_UART_EXIT_CRITICAL();
    }
#ifdef USB_ENABLE
    else if ((usbLen > 0) && (UsbGetState() == USB_CONFIGURED))
    {
        ret = AppGet()->on_rx(usbRx->buf, usbLen, &tailUsb, sizeof(usbRx->buf) - 1);
        USB_UART_ENTER_CRITICAL();
        usbRx->tail = tailUsb;
        USB_UART_EXIT_CRITICAL();
    }
#endif
    return ret;
}
