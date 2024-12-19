/**
 * @file      log_processing.c
 *
 * @brief     Log processing task
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "log_processing.h"

#include <nrf_log_ctrl.h>
#include <nrf_log_default_backends.h>
#include <qtimer.h>
#include <qassert.h>
#include "qtime.h"
#include "qthread.h"
#include "task_signal.h"
#include "qmalloc.h"
#include "int_priority.h"

#define LOG_FLUSH_MS (5)

#if defined(CONFIG_LOG) && defined(NRF_LOG_ENABLED)

#ifndef NRF_LOG_TIMESTAMP_DEFAULT_FREQUENCY
#error "Should define log timestamp frequency, set as 1Mhz to have 1usec resolution"
#endif
#define LOG_TIMER_INSTANCE  4
#define MAX_LOG_TIMER_COUNT 0xFFFFFFFF
/* 1Mhz freq. T = 1us. */
#define LOG_TIMESTAMP_FREQ NRF_LOG_TIMESTAMP_DEFAULT_FREQUENCY

static struct qtimer *log_timer = NULL;

static const struct qtimer_config log_timer_config = {
    .freq_hz = LOG_TIMESTAMP_FREQ,
    .irq_priority = 7,
    .width = QTIMER_WIDTH_32_BIT};

static uint32_t get_timestamp(void)
{
    volatile uint32_t timestamp;

    qtimer_read(log_timer, &timestamp);

    return timestamp;
}

static void init_log(void)
{
    enum qerr ret;

    /* Init log timestamp timer. */
    log_timer = qtimer_init(LOG_TIMER_INSTANCE, &log_timer_config, NULL, NULL);
    QASSERT(log_timer);

    ret = qtimer_start(log_timer, MAX_LOG_TIMER_COUNT, true);
    QASSERT(!ret);

    ret_code_t err_code = NRF_LOG_INIT(get_timestamp);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void log_processing_task(void *pvParameters)
{
    for (;;)
    {
        if (!NRF_LOG_PROCESS())
        {
            qtime_msleep_yield(LOG_FLUSH_MS);
        }
    }
}

static task_signal_t logger_task;

void create_log_processing_task(void)
{
    init_log();

    size_t task_size = 768;
    logger_task.task_stack = qmalloc(task_size);

    /* Create the task, storing the handle. */
    logger_task.thread = qthread_create(log_processing_task, NULL, "Logger", logger_task.task_stack, task_size, PRIO_LogProcessingTask);
}

#endif
