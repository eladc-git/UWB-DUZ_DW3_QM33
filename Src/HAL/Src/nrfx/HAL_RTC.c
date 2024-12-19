/**
 * @file      HAL_RTC.c
 *
 * @brief     Hardware abstraction layer for RTC
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

/* nRF5340dk Hardware Abstraction Layer for RTC */
#include <stdint.h>

#include "HAL_rtc.h"
#include "int_priority.h"

#include <nrfx_rtc.h>

/* WKUP timer counts Super Frame period.
 * The WKUP timer resolution is (30517.5) counts in 1 ns. */
#define WKUP_RESOLUTION_NS (1e9f / 32768.0f)

/* WKUP timer used at Ranging phase.
 * It is counts the Super Frame period, received in the Ranging Config message
 * in Tag's local time domain. */
#define WKUP_RESOLUTION_US (WKUP_RESOLUTION_NS / 1e3f)

#define WKUP_RESOLUTION_MS (WKUP_RESOLUTION_US / 1e3f)

/* RTC WKUP timer counts Super Frame period.
 * The RTC WKUP timer prescaler is configured as each Tick count is 30.517 us. */
#define RTC_WKUP_PRESCALER (0)

/* RTC WKUP timer counts Super Frame period.
 * The RTC WKUP timer is 24 bit counter. Counter overflows at 2^24 - 16777216. */
#define RTC_WKUP_CNT_OVFLW      (16777216)
#define RTC_WKUP_CNT_OVFLW_MASK (RTC_WKUP_CNT_OVFLW - 1)


#if !defined(NRFX_RTC_ENABLED)
#error "RTC flag not defined"
#endif

#if !NRFX_RTC_ENABLED
#error "RTC defined but not enabled"
#endif

#if (NRFX_RTC2_ENABLED == 1)
/* SD using 0; FreeRTOS using 1; */
#define RTC_ID 2
#else
#define RTC_ID 0
#endif
#define RTC_CHAN     0
#define RTC_COMP_EVT NRFX_RTC_INT_COMPARE0
#define RTC_IRQ      RTC0_IRQn
#define RTC_HDLR     nrfx_rtc_0_irq_handler

static nrfx_rtc_t grtc = NRFX_RTC_INSTANCE(RTC_ID);

static void (*rtc_callback)(void);
static uint32_t sRTC_SF_PERIOD = 3276;

static void rtc_disable_irq(void)
{
    uint32_t msk = RTC_CHANNEL_INT_MASK(RTC_CHAN);
    nrfx_rtc_int_disable(&grtc, &msk);
}

static void rtc_enable_irq(void)
{
    uint32_t msk = RTC_CHANNEL_INT_MASK(RTC_CHAN);
    nrfx_rtc_int_enable(&grtc, msk);
}

static void rtc_set_priority_irq(void)
{
    /* Configure the RTC Wakeup timer with a high priority;
     * this timer is saving global Super Frame Timestamp,
     * so we want this timestamp as stable as we can. */
    NVIC_SetPriority(RTC_IRQ, PRIO_RTC_WKUP_IRQn);
}

static uint32_t rtc_get_counter(void)
{
    return nrfx_rtc_counter_get(&grtc);
}

static uint32_t rtc_get_time_elapsed(uint32_t start, uint32_t stop)
{
    /* RTC is counting up, so we do stop - start to get elapsed time. */
    int32_t tmp = stop - start;

    /* Check roll over. */
    if (tmp < 0)
    {
        /* RTC Timer overflow - 24 bit counter. */
        tmp += RTC_WKUP_CNT_OVFLW;
    }
    return tmp;
}

/**
 * @brief Reload RTC registers after interrupt to prepare next compare match event.
 */
static void rtc_reload(uint32_t base_cnt)
{
    uint32_t new_cc = base_cnt + sRTC_SF_PERIOD;
    new_cc &= RTC_WKUP_CNT_OVFLW_MASK;
    nrfx_rtc_cc_set(&grtc, RTC_CHAN, new_cc, true);
}

/**
 * @brief RTC callback called by the RTC interrupt.
 */
static void rtc_handler(nrfx_rtc_int_type_t int_type)
{
    if (int_type == RTC_COMP_EVT)
    {
        if (rtc_callback)
            rtc_callback();
    }
}

static void rtc_set_callback(void (*cb)(void))
{
    rtc_callback = cb;
}

static float rtc_get_wakup_resolution_ns(void)
{
    return WKUP_RESOLUTION_NS;
}

/**
 * @brief Setup RTC Wakeup timer period_ns is awaiting time in ns.
 */
static void rtc_configure_wakeup(void)
{
    nrfx_err_t err_code;
    uint32_t old_cc, new_cc;
    nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;

    config.prescaler = RTC_WKUP_PRESCALER;

    /* the timer was initialized in the rtc_init(). */
    nrfx_rtc_disable(&grtc);

    /* rtc_handler */
    nrfx_rtc_uninit(&grtc);
    err_code = nrfx_rtc_init(&grtc, &config, rtc_handler);
    if (err_code != NRFX_SUCCESS)
    {
        return;
    }

    /* Disable tick interrupt. */
    nrfx_rtc_tick_disable(&grtc);

    /* Enable Counter Compare interrupt. */
    old_cc = nrfx_rtc_counter_get(&grtc);
    new_cc = old_cc + sRTC_SF_PERIOD;
    new_cc &= RTC_WKUP_CNT_OVFLW_MASK;
    nrfx_rtc_cc_set(&grtc, 0, new_cc, true);

    /* Power on RTC instance. */
    nrfx_rtc_enable(&grtc);
}

/**
 * @brief Setup RTC Wakeup timer period_ms is awaiting time in ms.
 */
void rtc_configure_wakeup_ns(uint32_t period_ns)
{
    sRTC_SF_PERIOD = period_ns / WKUP_RESOLUTION_NS;
    rtc_configure_wakeup();
}

/**
 * @brief Setup RTC Wakeup timer period_ms is awaiting time in ms.
 */
void rtc_configure_wakeup_ms(uint32_t period_ms)
{
    sRTC_SF_PERIOD = period_ms / WKUP_RESOLUTION_MS;
    rtc_configure_wakeup();
}

/**
 * @brief Initialization of the RTC driver instance.
 */
static void rtc_init(void)
{
    nrfx_err_t err_code;
    nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;
    /* WKUP_RESOLUTION_US counter period. */
    config.prescaler = RTC_WKUP_PRESCALER;

    rtc_callback = NULL;

    err_code = nrfx_rtc_init(&grtc, &config, rtc_handler);
    if (err_code != NRFX_SUCCESS)
    {
        return;
    }

    /* Power on RTC instance. */
    nrfx_rtc_enable(&grtc);
}

static void rtc_deinit(void)
{
    /* stop the RTC timer. */
    nrfx_rtc_disable(&grtc);
    nrfx_rtc_uninit(&grtc);
    rtc_callback = NULL;
}

static void rtc_wkup_stop(void)
{
}

static uint32_t rtc_get_unwrapped_sec(uint32_t rtc_cnt)
{
    /* Used to detect wraparound (every 512 sec with current nRF RTC config). */
    static int32_t prev_n_sec = 0;
    static int32_t nb_rtc_wraparound = 0;
    /* 24 bits counter, 15 bits for subsec as CLK = 32768 Hz */
#define NRF_RTC_WRAPAROUND_SEC 512
    uint32_t n_sec, unwrapped_sec;

    n_sec = rtc_cnt >> 15;

    if (n_sec < prev_n_sec)
    {
        nb_rtc_wraparound++;
    }
    prev_n_sec = n_sec;
    unwrapped_sec = n_sec + (nb_rtc_wraparound * NRF_RTC_WRAPAROUND_SEC);

    return unwrapped_sec;
}

/**
 * @brief wait for next RTC clk edge (SSR reg change) and read RTC Time
 * This function may block up to (1/SecondFraction+1) = 1/32768 sec ~30
 * It returns the read RTC time in the following format :
 * @param uint32_t* p_sec : counter of seconds
 * @param uint32_t* p_subsec : counter of subseconds
 * @param uint32_t* p_read_subsec : granularity of the counter of subseconds
 */
static void rtc_get_time_non_blocking(uint32_t *p_sec, uint32_t *p_subsec, uint32_t *p_rtc_subsec_hz)
{
    int32_t rtc_cnt;

    rtc_cnt = nrfx_rtc_counter_get(&grtc);

    *p_sec = rtc_get_unwrapped_sec((uint32_t)rtc_cnt);
    /* Valid assuming that NRFX_RTC_DEFAULT_CONFIG_FREQUENCY == 32768. */
    *p_subsec = rtc_cnt & 0x7FFF;
    /* Return the granularity as the divider of the second. */
    *p_rtc_subsec_hz = NRFX_RTC_DEFAULT_CONFIG_FREQUENCY;
    return;
}


static void rtc_get_time(uint32_t *p_sec, uint32_t *p_subsec, uint32_t *p_rtc_subsec_hz)
{
    /* Waiting until next clk edge is not supported yet on nRF.
     * The UWB clk / RTC clk resync is impacted, with a accuracy of 30us instead of 1-2us obtained on STM32
     * This is still compliant with CCC/FiRa inter block timing requirements, for most of the
     * MAC time grid settings. */
    int32_t rtc_cnt_initial, rtc_cnt;
    rtc_cnt_initial = nrfx_rtc_counter_get(&grtc);
    /* Loop waiting for ther next CLK edge i.e rtc_cnt change */
    for (uint8_t k = 0; k < 100; k++)
    {
        rtc_cnt = nrfx_rtc_counter_get(&grtc);
        if (rtc_cnt != rtc_cnt_initial)
        {
            break;
        }
    }

    *p_sec = rtc_get_unwrapped_sec((uint32_t)rtc_cnt);
    /* Valid assuming that NRFX_RTC_DEFAULT_CONFIG_FREQUENCY == 32768. */
    *p_subsec = rtc_cnt & 0x7FFF;
    /* Return the granularity as the divider of the second. */
    *p_rtc_subsec_hz = NRFX_RTC_DEFAULT_CONFIG_FREQUENCY;
    return;
}


/** @brief HAL RTC API structure. */
const struct hal_rtc_s Rtc = {
    .init = &rtc_init,
    .deinit = &rtc_deinit,
    .enableIRQ = &rtc_enable_irq,
    .disableIRQ = &rtc_disable_irq,
    .setPriorityIRQ = &rtc_set_priority_irq,
    .getTimestamp = &rtc_get_counter,
    .getTime = &rtc_get_time,
    .getTimeNonBlocking = &rtc_get_time_non_blocking,
    .getTimeElapsed = &rtc_get_time_elapsed,
    .reload = &rtc_reload,
    .configureWakeup_ms = &rtc_configure_wakeup_ms,
    .configureWakeup_ns = &rtc_configure_wakeup_ns,
    .getWakeupResolution_ns = &rtc_get_wakup_resolution_ns,
    .setCallback = &rtc_set_callback,
    .WakeupStop = &rtc_wkup_stop};
