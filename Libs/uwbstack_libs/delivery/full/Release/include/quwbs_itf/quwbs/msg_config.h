/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "quwbs/config.h"
#include "quwbs/err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct quwbs_msg_config - Configuration identifier and value.
 */
struct quwbs_msg_config {
	/**
	 * @next: Next element in list or %NULL if last one.
	 */
	struct quwbs_msg_config *next;
	/**
	 * @id: Configuration identifier.
	 */
	enum quwbs_config_id id;
	/**
	 * @value: Configuration value.
	 */
	union {
		/**
		 * @value.device_state: Device state (read only).
		 */
		enum quwbs_device_state device_state;
		/**
		 * @value.low_power_mode: Low power mode
		 */
		bool low_power_mode;
		/**
		 * @value.phy_current_channel: phyCurrentChannel PIB.
		 */
		int phy_current_channel;
		/**
		 * @value.phy_current_code: phyCurrentCode PIB.
		 */
		int phy_current_code;
		/**
		 * @value.mac_pan_id: macPanId PIB.
		 */
		uint16_t mac_pan_id;
		/**
		 * @value.mac_short_addr: macShortAddress PIB.
		 */
		uint16_t mac_short_addr;
		/**
		 * @value.mac_extended_addr: macExtendedAddress PIB.
		 */
		uint64_t mac_extended_addr;
		/**
		 * @value.mac_pan_coord: PAN coordinator.
		 */
		bool mac_pan_coord;
		/**
		 * @value.mac_promiscuous_mode: macPromiscuousMode PIB.
		 */
		bool mac_promiscuous_mode;
		/**
		 * @value.mac_max_frame_retries: macMaxFrameRetries PIB.
		 */
		int mac_max_frame_retries;
	} value;
};

/**
 * struct quwbs_msg_config_status - Configuration identifier and status.
 */
struct quwbs_msg_config_status {
	/**
	 * @next: Next element in list or %NULL if last one.
	 */
	struct quwbs_msg_config_status *next;
	/**
	 * @id: Configuration identifier.
	 */
	enum quwbs_config_id id;
	/**
	 * @status: Status of setting or getting the parameter:
	 *
	 * - &quwbs_err.QUWBS_ERR_SUCCESS on success
	 * - &quwbs_err.QUWBS_ERR_INVALID_ID on invalid configuration identifier
	 * - &quwbs_err.QUWBS_ERR_INVALID on invalid configuration value to set
	 * - &quwbs_err.QUWBS_ERR_READ_ONLY when trying to set a read only configuration
	 * - &quwbs_err.QUWBS_ERR_BUSY when configuration can not be set at this moment
	 */
	enum quwbs_err status;
};

#ifdef __cplusplus
}
#endif
