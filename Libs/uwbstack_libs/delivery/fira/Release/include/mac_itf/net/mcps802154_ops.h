/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct sk_buff;
struct mcps802154_llhw;
struct mcps802154_tx_frame_config;
struct mcps802154_tx_frame_info;
struct mcps802154_rx_frame_config;
struct mcps802154_rx_frame_info;
struct mcps802154_rx_measurement_info;
struct mcps802154_hrp_uwb_params;
struct mcps802154_channel;
struct mcps802154_sts_params;
struct ieee802154_hw_addr_filt;
struct wpan_phy_cca;
struct mcps802154_power_stats;
struct mcps802154_uwb_device_stats;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct mcps802154_ops - Callback from MCPS to the driver.
 */
struct mcps802154_ops {
	/**
	 * @start: Initialize device. Reception should not be activated.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*start)(struct mcps802154_llhw *llhw);
	/**
	 * @stop: Stop device. Should stop any transmission or reception and put
	 * the device in a low power mode.
	 */
	void (*stop)(struct mcps802154_llhw *llhw);
	/**
	 * @tx_frame: Transmit a frame. skb contains the buffer starting from
	 * the IEEE 802.15.4 header. The low-level driver should send the frame
	 * as specified in config. Receiver should be disabled automatically
	 * unless a frame is being received.
	 *
	 * The &frame_idx parameter gives the index of the frame in a "block".
	 * Frames from the same block (aka frame_idx > 0) should maintain the
	 * same synchronization.
	 *
	 * The &next_delay_dtu parameter gives the expected delay between the
	 * start of the transmitted frame and the next action.
	 *
	 * Return: QERR_SUCCESS, QERR_ETIME if frame can not be sent at
	 * specified timestamp, QERR_EBUSY if a reception is happening right
	 * now, or any other error.
	 */
	enum qerr (*tx_frame)(struct mcps802154_llhw *llhw, struct sk_buff *skb,
			      const struct mcps802154_tx_frame_config *config, int frame_idx,
			      int next_delay_dtu);
	/**
	 * @tx_disable: Disable a programmed transmission.
	 *
	 * Return: QERR_SUCCESS, QERR_EBUSY if the Tx can not be cancelled (time
	 * left for delayed Tx is too short) or any other error.
	 */
	enum qerr (*tx_disable)(struct mcps802154_llhw *llhw);
	/**
	 * @tx_get_frame_info: Get information about a transmitted frame
	 * frame.
	 *
	 * Return: QERR_SUCCESS, QERR_EBUSY if no longer available, or any other
	 * error.
	 */
	enum qerr (*tx_get_frame_info)(struct mcps802154_llhw *llhw,
				       struct mcps802154_tx_frame_info *info);
	/**
	 * @rx_enable: Enable receiver.
	 *
	 * The &frame_idx parameter gives the index of the frame in a "block".
	 * Frames from the same block (aka frame_idx > 0) should maintain the
	 * same synchronization.
	 *
	 * The &next_delay_dtu parameter gives the expected delay between the
	 * start of the received frame or timeout event and the next action.
	 *
	 * Return: QERR_SUCCESS, QERR_ETIME if receiver can not be enabled at
	 * specified timestamp, or any other error.
	 */
	enum qerr (*rx_enable)(struct mcps802154_llhw *llhw,
			       const struct mcps802154_rx_frame_config *config, int frame_idx,
			       int next_delay_dtu);
	/**
	 * @rx_disable: Disable receiver, or a programmed receiver enabling,
	 * unless a frame reception is happening right now.
	 *
	 * Return: QERR_SUCCESS, QERR_EBUSY if a reception is happening right
	 * now, or any other error.
	 */
	enum qerr (*rx_disable)(struct mcps802154_llhw *llhw);
	/**
	 * @rx_get_frame: Get previously received frame. MCPS calls this handler
	 * after a frame reception has been signaled by the low-level driver.
	 *
	 * The received buffer is owned by MCPS after this call. Only the
	 * requested information need to be filled in the information structure.
	 *
	 * Return: QERR_SUCCESS, QERR_EBUSY if no longer available, or any other
	 * error.
	 */
	enum qerr (*rx_get_frame)(struct mcps802154_llhw *llhw, struct sk_buff **skb,
				  struct mcps802154_rx_frame_info *info);
	/**
	 * @rx_get_error_frame: Get information on rejected frame. MCPS can call
	 * this handler after a frame rejection has been signaled by the
	 * low-level driver.
	 *
	 * In case of error, info flags must be cleared by this callback.
	 *
	 * Return: QERR_SUCCESS, QERR_EBUSY if no longer available, or any other
	 * error.
	 */
	enum qerr (*rx_get_error_frame)(struct mcps802154_llhw *llhw,
					struct mcps802154_rx_frame_info *info);
	/**
	 * @rx_get_measurement: Get measurement associated with a received
	 * frame.
	 *
	 * Return: QERR_SUCCESS, QERR_EBUSY if no longer available, or any other
	 * error.
	 */
	enum qerr (*rx_get_measurement)(struct mcps802154_llhw *llhw, void *rx_ctx,
					struct mcps802154_rx_measurement_info *info);
	/**
	 * @idle: Put the device into idle mode without time limit or until the
	 * given timestamp.  The driver should call &mcps802154_timer_expired()
	 * before the given timestamp so that an action can be programmed at the
	 * given timestamp.
	 *
	 * The &mcps802154_timer_expired() function must not be called
	 * immediately from this callback, but should be scheduled to be called
	 * later.
	 *
	 * If the driver is late, the regular handling of late actions will take
	 * care of the situation.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*idle)(struct mcps802154_llhw *llhw, bool timestamp, uint32_t timestamp_dtu);
	/**
	 * @reset: Reset device after an unrecoverable error.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*reset)(struct mcps802154_llhw *llhw);
	/**
	 * @get_current_timestamp_dtu: Get current timestamp in device time
	 * unit.
	 *
	 * If the device is currently in a low power state, the eventual wake up
	 * delay should be added to the returned timestamp.
	 *
	 * If the current timestamp can not be determined precisely, it should
	 * return a pessimistic value, i.e. rounded up.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*get_current_timestamp_dtu)(struct mcps802154_llhw *llhw,
					       uint32_t *timestamp_dtu);
	/**
	 * @tx_timestamp_dtu_to_rmarker_rctu: Compute the RMARKER timestamp in
	 * ranging counter time unit for a frame transmitted at given timestamp
	 * in device time unit (RDEV only).
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*tx_timestamp_dtu_to_rmarker_rctu)(struct mcps802154_llhw *llhw,
						      const struct mcps802154_rmarker_info *info,
						      uint64_t *rmarker_rctu);
	/**
	 * @difference_timestamp_rctu: Compute the difference between two
	 * timestamp values.
	 *
	 * Return: The difference between A and B.
	 */
	int64_t (*difference_timestamp_rctu)(struct mcps802154_llhw *llhw,
					     uint64_t timestamp_a_rctu, uint64_t timestamp_b_rctu);
	/**
	 * @compute_frame_duration_dtu: Compute the duration of a frame with
	 * given payload length (header and checksum included) using given
	 * radio and sts parameters. For SP0 sts_params could be NULL
	 * or a valid pointer to a zero-filled structure. For SP3 payload_bytes
	 * should be zero.
	 *
	 * Return: The duration in device time unit.
	 */
	int (*compute_frame_duration_dtu)(struct mcps802154_llhw *llhw,
					  const struct mcps802154_hrp_uwb_params *hrp_params,
					  const struct mcps802154_sts_params *sts_params,
					  int payload_bytes);
	/**
	 * @compute_max_payload: Compute maximal possible payload length
	 * (full PSDU, including MHR and MFR) for the given frame duration
	 * using given radio and sts parameters. For SP0 sts_params could be
	 * NULL or a valid pointer to a zero-filled structure.
	 *
	 * Return: Maximal possible payload length (header and checksum
	 * included).
	 */
	int (*compute_max_payload)(struct mcps802154_llhw *llhw,
				   const struct mcps802154_hrp_uwb_params *hrp_params,
				   const struct mcps802154_sts_params *sts_params,
				   int frame_duration_dtu);
	/**
	 * @set_channel: Set channel parameters.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_channel)(struct mcps802154_llhw *llhw, uint8_t page, uint8_t channel,
				 uint8_t preamble_code);
	/**
	 * @set_hrp_uwb_params: Set radio parameters for HRP UWB.
	 *
	 * The parameters in &mcps802154_llhw can change according to radio
	 * parameters.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_hrp_uwb_params)(struct mcps802154_llhw *llhw,
					const struct mcps802154_hrp_uwb_params *params);
	/**
	 * @check_hrp_uwb_params: Check that the HRP parameters are compatible
	 * with the hardware capabilities.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*check_hrp_uwb_params)(struct mcps802154_llhw *llhw,
					  const struct mcps802154_hrp_uwb_params *params);
	/**
	 * @set_sts_params: Set STS parameters (ERDEV only).
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_sts_params)(struct mcps802154_llhw *llhw,
				    const struct mcps802154_sts_params *params);
	/**
	 * @set_hw_addr_filt: Set hardware filter parameters.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_hw_addr_filt)(struct mcps802154_llhw *llhw,
				      struct ieee802154_hw_addr_filt *filt, unsigned long changed);
	/**
	 * @set_txpower: Set transmission power.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_txpower)(struct mcps802154_llhw *llhw, int32_t mbm);
	/**
	 * @set_cca_mode: Set CCA mode.
	 *
	 * The CCA duration in &mcps802154_llhw can change according to CCA
	 * mode.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_cca_mode)(struct mcps802154_llhw *llhw, const struct wpan_phy_cca *cca);
	/**
	 * @set_cca_ed_level: Set CCA energy detection threshold.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_cca_ed_level)(struct mcps802154_llhw *llhw, int32_t mbm);
	/**
	 * @set_promiscuous_mode: Set promiscuous mode.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_promiscuous_mode)(struct mcps802154_llhw *llhw, bool on);
	/**
	 * @set_scanning_mode: Set SW scanning mode.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_scanning_mode)(struct mcps802154_llhw *llhw, bool on);
	/**
	 * @set_calibration: Set calibration value.
	 *
	 * Set the calibration parameter specified by the key string with the
	 * value specified in the provided buffer. The provided length must
	 * match the length returned by the @get_calibration() callback.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*set_calibration)(struct mcps802154_llhw *llhw, const char *key, void *value,
				     size_t length);
	/**
	 * @get_calibration: Get calibration value.
	 *
	 * Get the calibration parameter specified by the key string into the
	 * provided buffer.
	 *
	 * Return: size of parameter written in buffer or error.
	 */
	int (*get_calibration)(struct mcps802154_llhw *llhw, const char *key, void *value,
			       size_t length);
	/**
	 * @get_calibration_key_name: Get calibration key name.
	 *
	 * Get the name of the calibration key at index `key_idx`. Allows to
	 * iterate over all calibration keys, and retrieve their values.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*get_calibration_key_name)(struct mcps802154_llhw *llhw, uint16_t key_idx,
					      char *key);
	/**
	 * @reset_calibration: Reset values of all calibration keys.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*reset_calibration)(struct mcps802154_llhw *llhw);
	/**
	 * @vendor_cmd: Run a vendor specific command.
	 *
	 * Do not (ab)use this feature to implement features that could be
	 * openly shared across drivers.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*vendor_cmd)(struct mcps802154_llhw *llhw, uint32_t vendor_id, uint32_t subcmd,
				void *data, size_t data_len);
	/**
	 * @get_antenna_caps: Return antenna set capabilites.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*get_antenna_caps)(struct mcps802154_llhw *llhw, int8_t ant_set_id,
				      uint32_t *caps);
	/**
	 * @get_power_stats: Get the power statistics.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*get_power_stats)(struct mcps802154_llhw *llhw,
				     struct mcps802154_power_stats *pwr_stats);
	/**
	 * @get_uwb_device_stats: Get the UWBS stats.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*get_uwb_device_stats)(struct mcps802154_llhw *llhw,
					  struct mcps802154_uwb_device_stats *uwb_device_stats);
	/**
	 * @testmode_cmd: Run a testmode command.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*testmode_cmd)(struct mcps802154_llhw *llhw, void *data, int len);
	/**
	 * @query_gpio_timestamp: Query the GPIO timestamp information.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*query_gpio_timestamp)(struct mcps802154_llhw *llhw, int64_t *timestamp_us,
					  uint8_t *sequence_number);
	/**
	 * @get_mac_session_scheduler_id: Get the session scheduler id stored in l1_config.
	 *
	 * Return: The value of the session scheduler id, using `enum mac_session_scheduler_id`.
	 */
	uint8_t (*get_mac_session_scheduler_id)(struct mcps802154_llhw *llhw);
	/**
	 * @get_device_info: Get device information.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*get_device_info)(struct mcps802154_llhw *llhw,
				     struct mcps802154_device_info *device_info);
};

#ifdef __cplusplus
}
#endif
