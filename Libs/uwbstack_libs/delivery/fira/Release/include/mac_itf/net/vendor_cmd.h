/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * enum llhw_vendor_cmd - Vendor command identifiers.
 * @LLHW_VENDOR_CMD_PCTT_SETUP_HW:
 *	PCTT: Setup hardware access.
 * @LLHW_VENDOR_CMD_PCTT_HANDLE_LOOPBACK:
 *	PCTT: Handle loop-back test.
 * @LLHW_VENDOR_CMD_PCTT_GET_LOOPBACK_INFO:
 *	PCTT: Get loop-back information.
 * @LLHW_VENDOR_CMD_PCTT_GET_FRAME_INFO:
 *	PCTT: Get the last received frame information.
 * @LLHW_VENDOR_CMD_RADAR_SWEEP_START:
 *	RADAR: Start radar operation.
 * @LLHW_VENDOR_CMD_RADAR_SWEEP_STOP:
 *	RADAR: Stop radar operation.
 * @LLHW_VENDOR_CMD_RADAR_SETUP:
 *	RADAR: Enables PLL Tx Prebuffers. Tech debt: UWB-13468
 * @LLHW_VENDOR_CMD_RADAR_CLEANUP:
 *	RADAR: Disables PLL Tx Prebuffers. Tech debt: UWB-13468
 */
enum llhw_vendor_cmd {
	LLHW_VENDOR_CMD_PCTT_SETUP_HW,
	LLHW_VENDOR_CMD_PCTT_HANDLE_LOOPBACK,
	LLHW_VENDOR_CMD_PCTT_GET_LOOPBACK_INFO,
	LLHW_VENDOR_CMD_PCTT_GET_FRAME_INFO,
	LLHW_VENDOR_CMD_RADAR_SWEEP_START,
	LLHW_VENDOR_CMD_RADAR_SWEEP_STOP,
	LLHW_VENDOR_CMD_RADAR_SETUP,
	LLHW_VENDOR_CMD_RADAR_CLEANUP,
};

/**
 * struct llhw_vendor_cmd_pctt_setup_hw - PCTT: direct HW access
 * vendor command.
 */
struct llhw_vendor_cmd_pctt_setup_hw {
	/**
	 * @enable_hw_assistance: Enable the HW filtering and the TX/RX FCS
	 * handling.
	 */
	bool enable_hw_assistance;
};

/**
 * struct llhw_vendor_cmd_pctt_handle_loopback - PCTT: handle loopback access.
 */
struct llhw_vendor_cmd_pctt_handle_loopback {
	/**
	 * @ant_set_id: antenna set index to use for transmit/receive.
	 */
	int8_t ant_set_id;
	/**
	 * @sts_mode: The sts mode to use: SP0, SP1, SP2 or SP3.
	 */
	uint8_t sts_mode;
	/**
	 * @rx_timeout_dtu: If negative, no timeout, if zero, use a default
	 * timeout value, else this is the timeout value in device time unit.
	 */
	int rx_timeout_dtu;
	/**
	 * @rx_frame_timeout_dtu: If no zero, timeout value for the full frame
	 * reception. This allow limiting the length of accepted frame. The
	 * timeout starts after rx_timeout_dtu value.
	 */
	int rx_frame_timeout_dtu;
	/**
	 * @data_payload: Skb of payload to send during loopback test.
	 */
	struct sk_buff *data_payload;
	/**
	 * @sts_param: Pointer to STS parameters for this loopback sequence.
	 * the STS is still only used if sts_mode is > 0.
	 */
	const struct mcps802154_sts_params *sts_param;
};

/**
 * struct llhw_vendor_cmd_pctt_get_loopback_info - PCTT: get access
 * info vendor command.
 */
struct llhw_vendor_cmd_pctt_get_loopback_info {
	/**
	 * @success: True when data sent match with received.
	 */
	bool success;
	/**
	 * @rx_timestamp_rctu: RX timestamp in RCTU units.
	 */
	uint64_t rx_timestamp_rctu;
	/**
	 * @tx_timestamp_rctu: TX timestamp in RCTU units.
	 */
	uint64_t tx_timestamp_rctu;
};

/**
 * struct llhw_vendor_cmd_pctt_get_frame_info - PCTT: last received frame
 * information.
 */
struct llhw_vendor_cmd_pctt_get_frame_info {
	/**
	 * @skb: sk buffer containing received data.
	 */
	struct sk_buff *skb;
	/**
	 * @info: frame information.
	 */
	struct mcps802154_rx_frame_info info;
};

/**
 * struct llhw_vendor_cmd_radar_configure - Radar: configuration
 */
struct llhw_vendor_cmd_radar_configure {
	/**
	 * @highest_tx_power: use highest possible TX power.
	 */
	bool highest_tx_power;
	/**
	 * @tx_profile_idx: Tx signal profile index if highest_tx_power=false.
	 */
	uint8_t tx_profile_idx;
	/**
	 * @rframe_config: rframe_config. SP0, SP1, SP3 are supported.
	 */
	uint8_t rframe_config;
	/**
	 * @sts_params: Pointer to STS parameters.
	 * The STS is still only used if rframe_config is not SP0.
	 */
	const struct mcps802154_sts_params *sts_params;
	/**
	 * @tx_data: Tx frame data pointer.
	 */
	uint8_t *tx_data;
	/**
	 * @tx_data_length: Tx frame data length.
	 */
	uint8_t tx_data_length;
};

#ifdef __cplusplus
}
#endif
