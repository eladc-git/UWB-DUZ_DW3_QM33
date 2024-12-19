/**
 * @file      controlTask.c
 *
 * @brief     Control task for USB/UART
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "usb_uart_rx.h"
#include "int_priority.h"
#include "circular_buffers.h"
#include "cmd_fn.h"
#include "HAL_error.h"
#include "task_signal.h"
#include "qtime.h"
#include "qmalloc.h"
#include "qmutex.h"

#ifdef UCI_BUILD
#define CONTROL_TASK_STACK_SIZE_BYTES 512
#else
#define CONTROL_TASK_STACK_SIZE_BYTES 2048
#endif

/* Communication RX buffer size. */
#define COM_RX_BUF_SIZE UART_RX_BUF_SIZE
/* Signal value when data is received. */
#define CTRL_DATA_RECEIVED 0x02
/* Signal value when STOP is required. */
#define CTRL_STOP_APP 0x04

/* Control task handler. */
task_signal_t ctrlTask;

/* Mutex for synchronizing rx buffer with UCI task. */
struct qmutex *rx_buffer_mutex = NULL;

/* Communication RX buffer size from usb_uart_rx parser to application for RX from USB/USART. */
#define COM_RX_BUF_SIZE UART_RX_BUF_SIZE
uint16_t local_buff_length;
uint8_t local_buff[COM_RX_BUF_SIZE];


/**
 * @brief This is a Command Control and Data task to handle received data.
 * This task is activated on the startup.
 * There are 2 sources of control data: Uart and Usb.
 * */
static void CtrlTask(void *arg)
{
    usb_data_e res;
    int signal_value;

    if (!rx_buffer_mutex)
    {
        rx_buffer_mutex = qmutex_init();
        if (!rx_buffer_mutex)
            return;
    }

    while (1)
    {
        /* Wait data from USB/UART. */
        if (qsignal_wait(ctrlTask.signal, &signal_value, QOSAL_WAIT_FOREVER) == QERR_SUCCESS)
        {
            switch (signal_value)
            {
#ifdef CLI_BUILD
                case CTRL_STOP_APP:
                    /* Time for flushing, TODO: create a function for testing flushing is over. */
                    qtime_msleep(500);
                    command_stop_received();
                    break;
#endif
                case CTRL_DATA_RECEIVED:
                    qmutex_lock(rx_buffer_mutex, QOSAL_WAIT_FOREVER);
                    res = usb_uart_rx();
                    qmutex_unlock(rx_buffer_mutex);
                    AppGet()->command_parser(res, (char *)local_buff);
            }
        }
    }
}

/**
 * @brief Creation of Control task and signal.
 * Note. The Control task awaits an input on a USB and/or UART interfaces.
 */
void ControlTaskInit(void)
{
    /* Create Control Task. */
    size_t task_size = CONTROL_TASK_STACK_SIZE_BYTES;
    ctrlTask.task_stack = qmalloc(task_size);

    ctrlTask.thread = qthread_create(CtrlTask, NULL, "Control", ctrlTask.task_stack, task_size, PRIO_CtrlTask);
    if (!ctrlTask.thread)
    {
        error_handler(1, _ERR_Create_Task_Bad);
    }

    /* Create Control Signal. */
    ctrlTask.signal = qsignal_init();
    if (!ctrlTask.signal)
    {
        error_handler(1, _ERR_Create_Task_Bad);
    }
}

/** @brief Notify that a new data is received. */
void NotifyControlTask(void)
{
    /* Send signal only if Control Task is started. */
    if (ctrlTask.signal != NULL)
    {
        qsignal_raise(ctrlTask.signal, CTRL_DATA_RECEIVED);
    }
}

/** @brief Notify that STOP App is required. */
void NotifyControlTaskStopApp(void)
{
    /* Send signal only if Control Task is started. */
    if (ctrlTask.signal != NULL)
    {
        qsignal_raise(ctrlTask.signal, CTRL_STOP_APP);
    }
}
