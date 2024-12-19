/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_ops.h"
#include "qerr.h"
#include "trace.h"

static inline enum qerr llhw_start(struct mcps802154_local *local)
{
	int r;

	trace_llhw_start(local);
	r = local->ops->start(&local->llhw);
	trace_llhw_return_int(local, r);
	return r;
}

static inline void llhw_stop(struct mcps802154_local *local)
{
	trace_llhw_stop(local);
	local->ops->stop(&local->llhw);
	trace_llhw_return_void(local);
}

static inline enum qerr llhw_tx_frame(struct mcps802154_local *local, struct sk_buff *skb,
				      const struct mcps802154_tx_frame_config *config,
				      int frame_idx, int next_delay_dtu)
{
	enum qerr r;

	trace_llhw_tx_frame(local, config, frame_idx, next_delay_dtu);
	r = local->ops->tx_frame(&local->llhw, skb, config, frame_idx, next_delay_dtu);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_tx_get_frame_info(struct mcps802154_local *local,
					       struct mcps802154_tx_frame_info *info)
{
	enum qerr r;

	trace_llhw_tx_get_frame_info(local);
	if (local->ops->tx_get_frame_info)
		r = local->ops->tx_get_frame_info(&local->llhw, info);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_tx_frame_info(local, r, info);
	return r;
}

static inline enum qerr llhw_tx_disable(struct mcps802154_local *local)
{
	enum qerr r;

	trace_llhw_tx_disable(local);
	if (local->ops->tx_disable)
		r = local->ops->tx_disable(&local->llhw);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_rx_enable(struct mcps802154_local *local,
				       const struct mcps802154_rx_frame_config *info, int frame_idx,
				       int next_delay_dtu)
{
	enum qerr r;

	trace_llhw_rx_enable(local, info, frame_idx, next_delay_dtu);
	r = local->ops->rx_enable(&local->llhw, info, frame_idx, next_delay_dtu);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_rx_disable(struct mcps802154_local *local)
{
	enum qerr r;

	trace_llhw_rx_disable(local);
	r = local->ops->rx_disable(&local->llhw);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_rx_get_frame(struct mcps802154_local *local, struct sk_buff **skb,
					  struct mcps802154_rx_frame_info *info)
{
	enum qerr r;

	trace_llhw_rx_get_frame(local, info);
	r = local->ops->rx_get_frame(&local->llhw, skb, info);
	trace_llhw_return_rx_frame(local, r, info);
	return r;
}

static inline enum qerr llhw_rx_get_error_frame(struct mcps802154_local *local,
						struct mcps802154_rx_frame_info *info)
{
	enum qerr r;

	trace_llhw_rx_get_error_frame(local, info);
	r = local->ops->rx_get_error_frame(&local->llhw, info);
	trace_llhw_return_rx_frame(local, r, info);
	return r;
}

static inline int llhw_idle(struct mcps802154_local *local, bool timestamp, uint32_t timestamp_dtu)
{
	enum qerr r;

	if (timestamp)
		trace_llhw_idle_timestamp(local, timestamp_dtu);
	else
		trace_llhw_idle(local);
	r = local->ops->idle(&local->llhw, timestamp, timestamp_dtu);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_reset(struct mcps802154_local *local)
{
	enum qerr r;

	trace_llhw_reset(local);
	r = local->ops->reset(&local->llhw);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_get_current_timestamp_dtu(struct mcps802154_local *local,
						       uint32_t *timestamp_dtu)
{
	enum qerr r;

	trace_llhw_get_current_timestamp_dtu(local);
	r = local->ops->get_current_timestamp_dtu(&local->llhw, timestamp_dtu);
	trace_llhw_return_timestamp_dtu(local, r, *timestamp_dtu);
	return r;
}

static inline enum qerr
llhw_tx_timestamp_dtu_to_rmarker_rctu(struct mcps802154_local *local,
				      const struct mcps802154_rmarker_info *info,
				      uint64_t *rmarker_rctu)
{
	/*
	 * Traces are not implemented to avoid to overload the global bandwidth
	 * with a superficial information.
	 */
	return local->ops->tx_timestamp_dtu_to_rmarker_rctu(&local->llhw, info, rmarker_rctu);
}

static inline int64_t llhw_difference_timestamp_rctu(struct mcps802154_local *local,
						     uint64_t timestamp_a_rctu,
						     uint64_t timestamp_b_rctu)
{
	/*
	 * Traces are not implemented to avoid to overload the global bandwidth
	 * with a superficial information.
	 */
	return local->ops->difference_timestamp_rctu(&local->llhw, timestamp_a_rctu,
						     timestamp_b_rctu);
}

static inline int
llhw_compute_frame_duration_dtu(struct mcps802154_local *local,
				const struct mcps802154_hrp_uwb_params *hrp_params,
				const struct mcps802154_sts_params *sts_params, int payload_bytes)
{
	/*
	 * Traces are not implemented to avoid to overload the global bandwidth
	 * with a superficial information.
	 */
	return local->ops->compute_frame_duration_dtu(&local->llhw, hrp_params, sts_params,
						      payload_bytes);
}

static inline int llhw_compute_max_payload(struct mcps802154_local *local,
					   const struct mcps802154_hrp_uwb_params *hrp_params,
					   const struct mcps802154_sts_params *sts_params,
					   int frame_duration_dtu)
{
	/*
	 * Traces are not implemented to avoid to overload the global bandwidth
	 * with a superficial information.
	 */
	if (!local->ops->compute_max_payload)
		return 0;
	return local->ops->compute_max_payload(&local->llhw, hrp_params, sts_params,
					       frame_duration_dtu);
}

static inline enum qerr llhw_set_channel(struct mcps802154_local *local, uint8_t page,
					 uint8_t channel, uint8_t preamble_code)
{
	enum qerr r;

	trace_llhw_set_channel(local, page, channel, preamble_code);
	r = local->ops->set_channel(&local->llhw, page, channel, preamble_code);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_set_hrp_uwb_params(struct mcps802154_local *local,
						const struct mcps802154_hrp_uwb_params *params)
{
	enum qerr r;

	trace_llhw_set_hrp_uwb_params(local, params);
	r = local->ops->set_hrp_uwb_params(&local->llhw, params);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_set_sts_params(struct mcps802154_local *local,
					    const struct mcps802154_sts_params *params)
{
	int r;

	trace_llhw_set_sts_params(local, params);
	if (local->ops->set_sts_params)
		r = local->ops->set_sts_params(&local->llhw, params);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_set_hw_addr_filt(struct mcps802154_local *local,
					      struct ieee802154_hw_addr_filt *filt,
					      unsigned long changed)
{
	enum qerr r;

	trace_llhw_set_hw_addr_filt(local, filt, changed);
	r = local->ops->set_hw_addr_filt(&local->llhw, filt, changed);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_set_txpower(struct mcps802154_local *local, int32_t mbm)
{
	enum qerr r;

	trace_llhw_set_txpower(local, mbm);
	r = local->ops->set_txpower(&local->llhw, mbm);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_set_cca_ed_level(struct mcps802154_local *local, int32_t mbm)
{
	enum qerr r;

	trace_llhw_set_cca_ed_level(local, mbm);
	r = local->ops->set_cca_ed_level(&local->llhw, mbm);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_set_promiscuous_mode(struct mcps802154_local *local, bool on)
{
	enum qerr r;

	trace_llhw_set_promiscuous_mode(local, on);
	r = local->ops->set_promiscuous_mode(&local->llhw, on);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_set_scanning_mode(struct mcps802154_local *local, bool on)
{
	enum qerr r;

	trace_llhw_set_scanning_mode(local, on);
	if (local->ops->set_scanning_mode)
		r = local->ops->set_scanning_mode(&local->llhw, on);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_set_calibration(struct mcps802154_local *local, const char *key,
					     void *value, size_t length)
{
	enum qerr r;

	trace_llhw_set_calibration(local, key);
	r = local->ops->set_calibration(&local->llhw, key, value, length);
	trace_llhw_return_int(local, r);
	return r;
}

static inline int llhw_get_calibration(struct mcps802154_local *local, const char *key, void *value,
				       size_t length)
{
	int r;

	trace_llhw_get_calibration(local, key);
	r = local->ops->get_calibration(&local->llhw, key, value, length);
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_get_calibration_key_name(struct mcps802154_local *local,
						      uint16_t key_idx, char *key)
{
	enum qerr r;

	trace_llhw_get_calibration_key_name(local, key_idx);
	if (local->ops->get_calibration_key_name)
		r = local->ops->get_calibration_key_name(&local->llhw, key_idx, key);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_reset_calibration(struct mcps802154_local *local)
{
	enum qerr r;

	trace_llhw_reset_calibration(local);
	if (local->ops->reset_calibration)
		r = local->ops->reset_calibration(&local->llhw);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_vendor_cmd(struct mcps802154_local *local, uint32_t vendor_id,
					uint32_t subcmd, void *data, size_t data_len)
{
	enum qerr r;

	trace_llhw_vendor_cmd(local, vendor_id, subcmd, data_len);
	if (local->ops->vendor_cmd)
		r = local->ops->vendor_cmd(&local->llhw, vendor_id, subcmd, data, data_len);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr
llhw_check_hrp_uwb_params(struct mcps802154_local *local,
			  const struct mcps802154_hrp_uwb_params *hrp_uwb_params)
{
	enum qerr r;

	trace_llhw_check_hrp_uwb_params(local, hrp_uwb_params);
	if (local->ops->check_hrp_uwb_params)
		r = local->ops->check_hrp_uwb_params(&local->llhw, hrp_uwb_params);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_rx_get_measurement(struct mcps802154_local *local, void *rx_ctx,
						struct mcps802154_rx_measurement_info *info)
{
	enum qerr r;
	trace_llhw_rx_get_measurement(local, rx_ctx);
	if (local->ops->rx_get_measurement)
		r = local->ops->rx_get_measurement(&local->llhw, rx_ctx, info);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_measurement(local, r, info);
	return r;
}

static inline enum qerr llhw_get_pwr_stats(struct mcps802154_local *local,
					   struct mcps802154_power_stats *pwr_stats)
{
	enum qerr r;

	trace_llhw_get_pwr_stats(local, pwr_stats);
	if (local->ops->get_power_stats)
		r = local->ops->get_power_stats(&local->llhw, pwr_stats);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr
llhw_get_uwb_device_stats(struct mcps802154_local *local,
			  struct mcps802154_uwb_device_stats *uwb_device_stats)
{
	enum qerr r;

	trace_llhw_get_uwb_device_stats(local, uwb_device_stats);
	if (local->ops->get_uwb_device_stats)
		r = local->ops->get_uwb_device_stats(&local->llhw, uwb_device_stats);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_testmode_cmd(struct mcps802154_local *local, void *data, int len)
{
	enum qerr r;

	trace_llhw_testmode_cmd(local);
	if (local->ops->testmode_cmd)
		r = local->ops->testmode_cmd(&local->llhw, data, len);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_query_gpio_timestamp(struct mcps802154_local *local,
						  int64_t *timestamp_us, uint8_t *sequence_number)
{
	enum qerr r;

	trace_llhw_query_gpio_timestamp(local, timestamp_us, sequence_number);
	if (local->ops->query_gpio_timestamp)
		r = local->ops->query_gpio_timestamp(&local->llhw, timestamp_us, sequence_number);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}

static inline enum qerr llhw_get_device_info(struct mcps802154_local *local,
					     struct mcps802154_device_info *device_info)
{
	enum qerr r;

	trace_llhw_get_device_info(local, device_info);
	if (local->ops->get_device_info)
		r = local->ops->get_device_info(&local->llhw, device_info);
	else
		r = QERR_ENOTSUP;
	trace_llhw_return_int(local, r);
	return r;
}
