/**
 * @file      HAL_timer.c
 *
 * @brief     HAL functions for timer
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "HAL_timer.h"

#include <stdint.h>

#include "nrf_error.h"
#include "nrf_timer.h"
#include "nrf_drv_timer.h"
#include "HAL_gpio.h"

#define TIMER_ID    2
#define FS_TIMER_ID 3

uint32_t HAL_GetTick(void);
void no_os_tick_handler(void);

static volatile uint32_t time32_incr = 0;

/**
 * @brief Wrapper for to read a SysTickTimer, which is incremented with
 * CLOCKS_PER_SEC frequency.
 * The resolution of time32_incr is usually 1/1000 sec.
 * */
uint32_t HAL_GetTick(void)
{
    return time32_incr;
}

void no_os_tick_handler(void)
{
    time32_incr++;
}

/* Timer for tick counting. */
const nrf_drv_timer_t timer1 = NRF_DRV_TIMER_INSTANCE(TIMER_ID);

/**
 * @brief Handler for timer events.
 * @param event_type: Timer event type.
 * @param p_context: General purpose parameter.
 *
 * @return Error code
 */
static void timer1_event_handler(nrf_timer_event_t event_type, void *p_context)
{
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            no_os_tick_handler();
            break;

        default:
            break;
    }
}

/**
 * @brief Initialize timer.
 *
 * @return Error code
 */
static uint32_t init_timer(void)
{
    uint32_t time_ms = 1; /* Time(in miliseconds) between consecutive compare events. */
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.frequency = NRF_TIMER_FREQ_125kHz;
    err_code = nrf_drv_timer_init(&timer1, &timer_cfg, timer1_event_handler);
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_ms_to_ticks(&timer1, time_ms);

    nrf_drv_timer_extended_compare(&timer1, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&timer1);

    return err_code;
}

/**
 * @brief Save system timestamp (in CLOCKS_PER_SEC).
 */
static void start_timer(volatile uint32_t *p_timestamp)
{
    *p_timestamp = HAL_GetTick();
}

/**
 * @brief Check if time from current timestamp over expectation.
 *
 * @return true  - time is over,
 *         false - time is not over yet.
 */
bool check_timer(uint32_t timestamp, uint32_t time)
{
    uint32_t time_passing;
    uint32_t temp_tick_time = HAL_GetTick();

    if (temp_tick_time >= timestamp)
    {
        time_passing = temp_tick_time - timestamp;
    }
    else
    {
        time_passing = 0xffffffffUL - timestamp + temp_tick_time;
    }

    if (time_passing >= time)
    {
        return (true);
    }

    return (false);
}

/* Fast Sleep Timer section, timer used for low-power UWB. */
#if (TIMER3_ENABLED == 1)

/* This struct is to decouple fs_timer_cb() from other API. */
static struct fs_timer_context_s
{
    void *tmr;
    void *dw;
} fs_timer_context;

/**
 * @brief   Internal instance callback. Calls "timerIsr" on CC event, if present.
 */
static void fs_timer_cb(nrf_timer_event_t event_type, void *p_context)
{
    struct fs_timer_context_s *context = (struct fs_timer_context_s *)p_context;
    hal_fs_timer_t *htimer = (hal_fs_timer_t *)context->tmr;

    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            if (htimer->timerIsr)
            {
                LP_DEBUG_D1();
                htimer->timerIsr(context->dw);
            }
            break;
        default:
            // Do nothing.
            break;
    }
}

/**
 * @brief Initialization of the NRF Fast Sleep Timer driver instance.
 *
 * Timer is running on 1MHz:
 *  - in continuous mode with Counter Compare events
 *  - clearing its counter (and keep counting) on CC.
 */
static void fs_timer_init(void *tmr, void *dw, void (*cb)(void *))
{
    hal_fs_timer_t *htimer = (hal_fs_timer_t *)tmr;
    nrf_drv_timer_t *timer = htimer->timer;
    nrf_drv_timer_config_t timer_cfg;
    nrfx_err_t err_code;

    assert(htimer->freq == 1000000);

    fs_timer_context.tmr = tmr;
    fs_timer_context.dw = dw;

    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    timer_cfg.frequency = NRF_TIMER_FREQ_1MHz;
    timer_cfg.mode = NRF_TIMER_MODE_TIMER;
    timer_cfg.interrupt_priority = 7;
    timer_cfg.p_context = &fs_timer_context;

    if (cb)
    {
        htimer->timerIsr = cb;
        err_code = nrf_drv_timer_init(timer, &timer_cfg, fs_timer_cb);

        if (err_code == NRFX_ERROR_INVALID_STATE)
        {
            nrfx_timer_uninit(timer);
            nrf_drv_timer_init(timer, &timer_cfg, fs_timer_cb);
        }

        /* Starting fs timer to run without IRQ to avoid jitter on Start/Stop */
        htimer->start(htimer, false, UINT32_MAX, 0);
    }
}

/**
 * @brief De-Initialization of the NRF Fast Sleep Timer driver instance.
 *
 * Timer is running on 1MHz:
 *  - in continuous mode with Counter Compare events
 *  - clearing its counter (and keep counting) on CC.
 */
static void fs_timer_deinit(void *tmr)
{
    hal_fs_timer_t *htimer = (hal_fs_timer_t *)tmr;
    nrf_drv_timer_t *timer = htimer->timer;
    htimer->stop(htimer);
    nrfx_timer_uninit(timer);
}

static uint32_t fs_timer_get_tick(void *tmr)
{
    hal_fs_timer_t *htimer = (hal_fs_timer_t *)tmr;
    nrf_drv_timer_t *timer = (nrf_drv_timer_t *)htimer->timer;

    return (uint32_t)(nrf_drv_timer_capture(timer, NRF_TIMER_CC_CHANNEL0));
}


static void fs_timer_stop(void *tmr)
{
    hal_fs_timer_t *htimer = (hal_fs_timer_t *)tmr;
    nrf_drv_timer_t *timer = (nrf_drv_timer_t *)htimer->timer;
    nrf_drv_timer_disable(timer);
}


static void fs_timer_start_us(void *tmr, bool int_en, uint32_t time_us, int corr_tick)
{
    hal_fs_timer_t *htimer = (hal_fs_timer_t *)tmr;
    nrf_drv_timer_t *timer = (nrf_drv_timer_t *)htimer->timer;
    uint32_t time_ticks;

    time_ticks = nrf_drv_timer_us_to_ticks(timer, time_us) + corr_tick;
    nrf_drv_timer_extended_compare(timer, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, int_en);

    if (!nrf_drv_timer_is_enabled(timer))
    {
        nrf_drv_timer_enable(timer);
    }
}

/* Instance of the fast sleep timer
 * this timer is used to enable low-power sleep of UWB subsystem for the price of its own power consumption
 */
static nrf_drv_timer_t fs_timer = NRF_DRV_TIMER_INSTANCE(FS_TIMER_ID);

hal_fs_timer_t hal_fs_timer = {
    .timer = &fs_timer,
    .timerIsr = NULL,
    .init = fs_timer_init,
    .deinit = fs_timer_deinit,
    .start = fs_timer_start_us,
    .stop = fs_timer_stop,
    .get_tick = fs_timer_get_tick,
    .freq = 1000000,
};

#else
hal_fs_timer_t hal_fs_timer = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0};
#endif

/** @brief HAL Timer API structure. */
const struct hal_timer_s Timer = {
    .init = &init_timer,
    .start = &start_timer,
    .check = &check_timer};
