/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-Qorvo-2
 */

#include "deca_device_api.h"

#include <persistent_time.h>
#include <qassert.h>
#include <qrtc.h>
#include <qtimer.h>
#include <stdint.h>
#define LOG_TAG "persistent_time_nrfx"
#include <qlog.h>

#define BASE_TIMER_INSTANCE 0
#define MAX_BASE_TIME_COUNT 0xFFFFFFFF
#define PERSISTENT_FREQUENCY 1000000 /* 1Mhz freq. T = 1us. */

static struct qtimer *base_timer = NULL;
static uint64_t base_time_rollover = 0;

static const struct qtimer_config base_timer_config = { .freq_hz = PERSISTENT_FREQUENCY,
							.irq_priority = 7,
							.width = QTIMER_WIDTH_32_BIT };

static void base_time_rollover_handler(void *arg)
{
	base_time_rollover += MAX_BASE_TIME_COUNT;
}

void persistent_time_init(enum pm_state last_state)
{
	(void)last_state;
	enum qerr ret;
	QLOGD("persistent_time_init");

	base_timer = qtimer_init(BASE_TIMER_INSTANCE, &base_timer_config,
				 base_time_rollover_handler, NULL);

	QASSERT(base_timer);

	ret = qtimer_start(base_timer, MAX_BASE_TIME_COUNT, true);
	QASSERT(!ret);
}

static inline int64_t wait_synced_time(void)
{
	volatile uint32_t base_time_us;
	volatile uint32_t base_time_us_prev;

	qtimer_read(base_timer, &base_time_us_prev);
	/* Catch moment when timer counting change by one.
	 * Like that we know that we are on an edge changing start.
	 */
	do {
		qtimer_read(base_timer, &base_time_us);
	} while (base_time_us == base_time_us_prev);

	return (int64_t)(base_time_us + base_time_rollover);
}

int64_t persistent_time_get_rtc_us(void)
{
	volatile uint32_t base_time_us;

	qtimer_read(base_timer, &base_time_us);

	return (int64_t)(base_time_us + base_time_rollover);
}

void persistent_time_resync_rtc_systime(int64_t *rtc_us, uint32_t *systime)
{
	volatile uint32_t cur_systime;

	*rtc_us = wait_synced_time();
	cur_systime = dwt_readsystimestamphi32();
	*systime = cur_systime;
}

void persistent_time_update_rtc_systime(int64_t updated_rtc_us, uint32_t updated_systime)
{
}
