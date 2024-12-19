/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-Qorvo-2
 */

#include <nrfx_timer.h>
#include <qtimer.h>

#define LOG_TAG "qtimer_nrfx"
#include <qlog.h>

#if !defined(NRFX_TIMER_ENABLED) || (NRFX_TIMER_ENABLED == 0)
#error "NRFX TIMER module should be enabled."
#endif

#define QTIMER_MAX NRFX_TIMER_ENABLED_COUNT

struct qtimer {
	nrfx_timer_t instance;
	uint8_t instance_number;
};

struct qtimer_instance {
	struct qtimer timer;
	bool is_used;
	qtimer_cb handler;
	void *arg;
};

#define READ_CHANNEL NRF_TIMER_CC_CHANNEL1
#define COMPARE_CHANNEL NRF_TIMER_CC_CHANNEL0
#define COMPARE_MASK_PERIODIC (NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK)
#define COMPARE_MASK_ONE_SHOT \
	(NRF_TIMER_SHORT_COMPARE0_STOP_MASK | NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK)

static struct qtimer_instance local_instance_timer[QTIMER_MAX] = { 0 };

static enum qerr set_timer_instance(uint8_t timer_id, nrfx_timer_t *timer)
{
	switch (timer_id) {
#if NRFX_TIMER0_ENABLED
	case 0:
		timer->p_reg = NRF_TIMER0;
		timer->instance_id = NRFX_TIMER0_INST_IDX;
		timer->cc_channel_count = TIMER0_CC_NUM;
		break;
#endif
#if NRFX_TIMER1_ENABLED
	case 1:
		timer->p_reg = NRF_TIMER1;
		timer->instance_id = NRFX_TIMER1_INST_IDX;
		timer->cc_channel_count = TIMER1_CC_NUM;
		break;
#endif
#if NRFX_TIMER2_ENABLED
	case 2:
		timer->p_reg = NRF_TIMER2;
		timer->instance_id = NRFX_TIMER2_INST_IDX;
		timer->cc_channel_count = TIMER2_CC_NUM;
		break;
#endif
#if NRFX_TIMER3_ENABLED
	case 3:
		timer->p_reg = NRF_TIMER3;
		timer->instance_id = NRFX_TIMER3_INST_IDX;
		timer->cc_channel_count = TIMER3_CC_NUM;
		break;
#endif
#if NRFX_TIMER4_ENABLED
	case 4:
		timer->p_reg = NRF_TIMER4;
		timer->instance_id = NRFX_TIMER4_INST_IDX;
		timer->cc_channel_count = TIMER4_CC_NUM;
		break;
#endif
	default:
		return QERR_EINVAL;
	}

	return QERR_SUCCESS;
}

static struct qtimer_instance *find_available_timer(uint8_t timer_id)
{
	uint8_t idx;
	bool found = true;

	for (idx = 0; idx < QTIMER_MAX; idx++) {
		if (!local_instance_timer[idx].is_used ||
		    (local_instance_timer[idx].timer.instance_number == timer_id)) {
			found = true;
			break;
		}
	}

	if (!found)
		return NULL;

	if (set_timer_instance(timer_id, &local_instance_timer[idx].timer.instance) != QERR_SUCCESS)
		return NULL;

	local_instance_timer[idx].is_used = true;
	return &local_instance_timer[idx];
}

static void qtimer_generic_handler(nrf_timer_event_t event_type, void *p_context)
{
	struct qtimer_instance *instance = (struct qtimer_instance *)p_context;
	if (instance->handler)
		instance->handler(instance->arg);
}

static nrf_timer_frequency_t qtimer_freq_to_nrf(uint32_t freq_hz)
{
	if (freq_hz >= 16000000) {
		return NRF_TIMER_FREQ_16MHz;
	} else if (freq_hz >= 8000000) {
		return NRF_TIMER_FREQ_8MHz;
	} else if (freq_hz >= 4000000) {
		return NRF_TIMER_FREQ_4MHz;
	} else if (freq_hz >= 2000000) {
		return NRF_TIMER_FREQ_2MHz;
	} else if (freq_hz >= 1000000) {
		return NRF_TIMER_FREQ_1MHz;
	} else if (freq_hz >= 5000000) {
		return NRF_TIMER_FREQ_500kHz;
	} else if (freq_hz >= 2500000) {
		return NRF_TIMER_FREQ_250kHz;
	} else if (freq_hz >= 1250000) {
		return NRF_TIMER_FREQ_125kHz;
	} else if (freq_hz >= 625000) {
		return NRF_TIMER_FREQ_62500Hz;
	}

	return NRF_TIMER_FREQ_31250Hz;
}

static nrf_timer_bit_width_t qtimer_width_to_nrf(enum qtimer_width width)
{
	switch (width) {
	case QTIMER_WIDTH_8_BIT:
		return NRF_TIMER_BIT_WIDTH_8;
	case QTIMER_WIDTH_16_BIT:
		return NRF_TIMER_BIT_WIDTH_16;
	case QTIMER_WIDTH_24_BIT:
		return NRF_TIMER_BIT_WIDTH_24;
	case QTIMER_WIDTH_32_BIT:
		return NRF_TIMER_BIT_WIDTH_32;
	default:
		break;
	}

	return NRF_TIMER_BIT_WIDTH_32;
}

struct qtimer *qtimer_init(uint8_t qtimer_id, const struct qtimer_config *config, qtimer_cb handler,
			   void *arg)
{
	if (!config)
		return NULL;

	struct qtimer_instance *timer_instance = find_available_timer(qtimer_id);

	if (!timer_instance) {
		QLOGW("No more available instances. Check NRFX_TIMER_ENABLED_COUNT.");
		return NULL;
	}

	nrfx_timer_uninit(&timer_instance->timer.instance);

	nrfx_timer_config_t p_config;
	p_config.frequency = qtimer_freq_to_nrf(config->freq_hz);
	p_config.mode = NRF_TIMER_MODE_TIMER;
	p_config.bit_width = qtimer_width_to_nrf(config->width);
	p_config.interrupt_priority = config->irq_priority;
	p_config.p_context = (void *)timer_instance;

	timer_instance->handler = handler;
	timer_instance->arg = arg;
	timer_instance->timer.instance_number = qtimer_id;

	if (nrfx_timer_init(&timer_instance->timer.instance, &p_config, qtimer_generic_handler) !=
	    NRFX_SUCCESS) {
		timer_instance->is_used = false;
		QLOGW("qtimer: initialization failed for id %d.", qtimer_id);
		return NULL;
	}

	return &timer_instance->timer;
}

enum qerr qtimer_start(const struct qtimer *timer, uint32_t us, bool periodic)
{
	if (!timer)
		return QERR_EINVAL;

	/* If no `us` paremters, timer is a free running timer. */
	if (us) {
		/* Program an interrupt on specified usec time.*/
		nrf_timer_short_mask_t timer_short_mask;
		uint32_t time_ticks = nrfx_timer_us_to_ticks(&timer->instance, us);

		if (periodic)
			timer_short_mask = COMPARE_MASK_PERIODIC;
		else
			timer_short_mask = COMPARE_MASK_ONE_SHOT;

		nrf_timer_cc_channel_t timer_cc_channel = COMPARE_CHANNEL;

		nrfx_timer_extended_compare(&timer->instance, timer_cc_channel, time_ticks,
					    timer_short_mask, true);
	}

	nrfx_timer_enable(&timer->instance);

	return QERR_SUCCESS;
}

enum qerr qtimer_stop(const struct qtimer *timer)
{
	if (!timer)
		return QERR_EINVAL;

	nrfx_timer_disable(&timer->instance);

	return QERR_SUCCESS;
}

enum qerr qtimer_read(const struct qtimer *timer, volatile uint32_t *value)
{
	if (!timer || !value)
		return QERR_EINVAL;

	*value = nrfx_timer_capture(&timer->instance, READ_CHANNEL);

	return QERR_SUCCESS;
}
