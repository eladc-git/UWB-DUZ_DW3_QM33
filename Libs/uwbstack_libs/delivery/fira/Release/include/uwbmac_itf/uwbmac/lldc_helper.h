/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

/*
 * This file is generated automatically, edit with care.
 */

#pragma once

#include "lld_helper_stats.h"
#include "net/lld_region_common_nl.h"
#include "qerr.h"
#include "uwbmac/uwbmac.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct uwbmac_context;

#ifdef __cplusplus
extern "C" {
#endif

#define LLDC_HELPER_PAIRED_DEVICE_MAX 32
#define LLDC_HELPER_RANGING_MEASUREMENT_MAX 31

/**
 * struct lldc_helper_dev_assoc_state_changed - Information on device
 * association state changed.
 */
struct lldc_helper_dev_assoc_state_changed {
	/**
	 * @dev_key: Device identity key.
	 */
	uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN];
	/**
	 * @associated: True if the device is associated.
	 */
	bool associated;
	/**
	 * @device_short_addr: Short address of the device, if associated.
	 */
	uint16_t device_short_addr;
	/**
	 * @ranging_capabilities: Device ranging capabilities.
	 */
	struct {
		/**
		 * @many_to_many: True if the device supports many to many
		 * ranging.
		 */
		bool many_to_many;
		/**
		 * @ds_twr: True if the device supports DS-TWR ranging.
		 */
		bool ds_twr;
		/**
		 * @secured_sp1: True if the device supports secured SP1
		 * ranging.
		 */
		bool secured_sp1;
		/**
		 * @secured_sp3: True if the device supports secured SP3
		 * ranging.
		 */
		bool secured_sp3;
	} ranging_capabilities;
};

/**
 * struct lld_ranging_measurement - Ranging measurement information.
 */
struct lld_ranging_measurement {
	/**
	 * @short_addr: Participating device short address.
	 */
	uint16_t short_addr;
	/**
	 * @distance_mm: Distance in mm.
	 */
	int32_t distance_mm;
	/**
	 * @aoa_azimuth_2pi: Estimated angle of arrival on azimuth.
	 */
	int16_t aoa_azimuth_2pi;
	/**
	 * @aoa_azimuth_fom: Figure of merit of the azimuth AoA.
	 */
	uint8_t aoa_azimuth_fom;
	/**
	 * @aoa_elevation_2pi: Estimated angle of arrival on elevation.
	 */
	int16_t aoa_elevation_2pi;
	/**
	 * @aoa_elevation_fom: Figure of merit of the elevation AoA.
	 */
	uint8_t aoa_elevation_fom;
};

/**
 * struct lldc_helper_ranging_report - Information on ranging report.
 */
struct lldc_helper_ranging_report {
	/**
	 * @block_index: Current block index.
	 */
	uint32_t block_index;
	/**
	 * @n_ranging_measurements: Number of ranging measurements in the list.
	 */
	int n_ranging_measurements;
	/**
	 * @ranging_measurement: Ranging measurement information.
	 */
	struct lld_ranging_measurement ranging_measurement[LLDC_HELPER_RANGING_MEASUREMENT_MAX];
};

/**
 * struct lldc_helper_dev_pairing_request - Information on device pairing
 * request.
 */
struct lldc_helper_dev_pairing_request {
	/**
	 * @dev_key: Device identity key.
	 */
	uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN];
	/**
	 * @association_payload: Payload embedded in received association request.
	 */
	uint8_t association_payload[LLD_ASSOCIATION_PAYLOAD_LEN_MAX];
	/**
	 * @association_payload_len: Length of association payload.
	 */
	int association_payload_len;
};

/**
 * struct lldc_helper_ops - LLD coordinator operations.
 */
struct lldc_helper_ops {
	/**
	 * @dev_assoc_state_changed: Called when a device association state change. If NULL,
	 * ignored.
	 *
	 * This callback must return quickly and it must not reenter the MAC.
	 */
	void (*dev_assoc_state_changed)(
		void *user_data,
		const struct lldc_helper_dev_assoc_state_changed *dev_assoc_state_changed);
	/**
	 * @dev_pairing_request: Called when a device pairing request is received. If NULL,
	 * ignored.
	 *
	 * This callback must return quickly and it must not reenter the MAC.
	 */
	void (*dev_pairing_request)(
		void *user_data, const struct lldc_helper_dev_pairing_request *dev_pairing_request);
	/**
	 * @ranging_report: Called when a ranging report is received. If NULL,
	 * ignored.
	 *
	 * This callback must return quickly and it must not reenter the MAC.
	 */
	void (*ranging_report)(void *user_data,
			       const struct lldc_helper_ranging_report *ranging_report);
};

/*
 * struct lldc_context - LLD Coordinator context
 *
 * NOTE: content of this struct is considered private, do not use directly.
 */

struct lldc_context {
	struct uwbmac_channel channel;
	const struct lldc_helper_ops *ops;
	void *ops_user_data;
};

/**
 * struct lldc_stats - LLD coordinator per device statistics.
 */
struct lldc_stats {
	/**
	 * @interactive_mfs: Interactive MFS packet statistics.
	 */
	struct lld_mfs_stats interactive_mfs;
	/**
	 * @best_effort_mfs: Best effort MFS packet statistics.
	 */
	struct lld_mfs_stats best_effort_mfs;
	/**
	 * @rx_ul_frames: Number of good UL frames received.
	 */
	uint32_t rx_ul_frames;
	/**
	 * @rx_ul_bytes: Number of good UL frames bytes received, FCS not
	 * included, corresponding to rx_ul_frames.
	 */
	uint32_t rx_ul_bytes;
	/**
	 * @rx_ul_errors: Number of UL frames received with error, or not
	 * received.
	 */
	uint32_t rx_ul_errors;
	/**
	 * @rx_ul_aux_frames: Number of good UL frames received in an auxiliary
	 * sub-block.
	 */
	uint32_t rx_ul_aux_frames;
	/**
	 * @rx_ul_aux_bytes: Number of good UL frames bytes received in an
	 * auxiliary sub-block, FCS not included, corresponding to
	 * rx_ul_aux_frames.
	 */
	uint32_t rx_ul_aux_bytes;
	/**
	 * @rx_ul_aux_errors: Number of UL frames received with error in an
	 * auxiliary sub-block, or not received.
	 */
	uint32_t rx_ul_aux_errors;
	/**
	 * @blocks: Number of blocks in which the device participated (tracking
	 * or associated).
	 */
	uint32_t blocks;
	/**
	 * @lqi: Link quality indicator (LQI, 0-100) of all connected devices.
	 */
	uint8_t lqi;
};

/**
 * struct lldc_coord_stats - LLD global coordinator statistics.
 */
struct lldc_coord_stats {
	/**
	 * @tx_frame_non_assocs: Number of non associative frames successfully transmitted.
	 */
	uint32_t tx_frame_non_assocs;
	/**
	 * @tx_frame_non_assocs_errors: Number of non associative frames not transmitted.
	 */
	uint32_t tx_frame_non_assocs_errors;
	/**
	 * @tx_frame_beacon: Number of beacon frames successfully transmitted.
	 */
	uint32_t tx_frame_beacon;
	/**
	 * @tx_frame_beacon_errors: Number of beacon frames not transmitted.
	 */
	uint32_t tx_frame_beacon_errors;
	/**
	 * @tx_dl_frames: Number of DL frames successfully transmitted.
	 */
	uint32_t tx_dl_frames;
	/**
	 * @tx_dl_errors: Number of DL frames not transmitted.
	 */
	uint32_t tx_dl_errors;
	/**
	 * @tx_dl_bytes: Number of DL frames bytes transmitted
	 * successfully, corresponding to tx_dl_frames.
	 */
	uint32_t tx_dl_bytes;
	/**
	 * @stats: See &struct lldc_stats.
	 */
	struct lldc_stats stats;
};

/**
 * struct paired_dev_parameters - Paired devices list parameters.
 */
struct paired_dev_parameters {
	/**
	 * @dev_keys: List of device identity keys.
	 */
	uint8_t dev_keys[LLDC_HELPER_PAIRED_DEVICE_MAX][LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN];
	/**
	 * @n_dev_keys: Number of device identity keys in the list.
	 */
	int n_dev_keys;
};

/**
 * lldc_helper_open() - Initialize the internal resources of the helper.
 * @context: Lldc context to initialize.
 * @uwbmac_context: UWB MAC context.
 * @ops: Coordinator operations.
 * @ops_user_data: User data given to coordinator operations.
 *
 * NOTE: This function must be called first. @lldc_helper_close must be
 * called at the end of the application to ensure resources are freed.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_open(struct lldc_context *context, struct uwbmac_context *uwbmac_context,
			   const struct lldc_helper_ops *ops, void *ops_user_data);

/**
 * lldc_helper_close() - Free all internal resources of the helper.
 * @context: Lldc context to free.
 */
void lldc_helper_close(struct lldc_context *context);

/**
 * lldc_helper_set_scheduler() - Set the scheduler and the region.
 * @context: Lldc context.
 *
 * NOTE: This function must be called while the UWB MAC is stopped.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_scheduler(struct lldc_context *context);

/**
 * lldc_helper_start() - Start the coordinator.
 * @context: Lldc context.
 *
 * Return: 0 or error.
 */
int lldc_helper_start(struct lldc_context *context);

/**
 * lldc_helper_stop() - Stop the coordinator, return to the STANDBY
 * state.
 * @context: Lldc context.
 *
 * Return: 0 or error.
 */
int lldc_helper_stop(struct lldc_context *context);

/**
 * lldc_helper_add_paired_dev() - Add a device to the list of paired devices.
 * @context: Lldc context.
 * @paired_dev_parameters: Paired devices parameters.
 *
 * Return: 0 or error.
 */
int lldc_helper_add_paired_dev(struct lldc_context *context,
			       const struct paired_dev_parameters *paired_dev_parameters);

/**
 * lldc_helper_del_paired_dev() - Delete a device from the list of paired
 * devices.
 * @context: Lldc context.
 * @paired_dev_parameters: Paired devices parameters.
 *
 * Return: 0 or error.
 */
int lldc_helper_del_paired_dev(struct lldc_context *context,
			       const struct paired_dev_parameters *paired_dev_parameters);

/**
 * lldc_helper_get_paired_dev() - Get the list of paired devices.
 * @context: Lldc context.
 * @paired_dev_parameters: Paired devices parameters.
 *
 * Return: 0 or error.
 */
int lldc_helper_get_paired_dev(struct lldc_context *context,
			       struct paired_dev_parameters *paired_dev_parameters);

/**
 * lldc_helper_set_allow_pairing() - Set ALLOW_PAIRING parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Allow pairing with unknown devices, requests will be forwarded to higher layers.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_allow_pairing(struct lldc_context *context, bool value);

/**
 * lldc_helper_set_agg_limit_bytes() - Set AGG_LIMIT_BYTES parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Limit MPDU aggregated payload size after the first sub-frame.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_agg_limit_bytes(struct lldc_context *context, int value);

/**
 * lldc_helper_set_beacon_ifs() - Set BEACON_IFS parameter.
 * @context: Lldc context.
 * @slot_based: Use slot based mode if true, else gap based mode.
 * @value_rstu: Interval value in RSTU.
 *
 * Interval between the last sub-block DL frame and the beacon frame.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_beacon_ifs(struct lldc_context *context, bool slot_based, int value_rstu);

/**
 * lldc_helper_set_beacon_interval_rstu() - Set BEACON_INTERVAL_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Interval between two beacons transmissions when the coordinator is in ACTIVE state. Set to 0 to
 * disable beacon transmission.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_beacon_interval_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_beacon_payload() - Set BEACON_PAYLOAD parameter.
 * @context: Lldc context.
 * @data: Data value.
 * @size: Data size.
 *
 * Payload embedded in transmitted beacons, maximum size of 32 bytes.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_beacon_payload(struct lldc_context *context, const uint8_t *data,
					 size_t size);

/**
 * lldc_helper_set_block_rstu() - Set BLOCK_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Duration of a block.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_block_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_burst_ifs() - Set BURST_IFS parameter.
 * @context: Lldc context.
 * @slot_based: Use slot based mode if true, else gap based mode.
 * @value_rstu: Interval value in RSTU.
 *
 * Interval between burst DL frames, or between the last sub-block DL frame and the first burst DL
 * frame.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_burst_ifs(struct lldc_context *context, bool slot_based, int value_rstu);

/**
 * lldc_helper_set_cap_interval_max_rstu() - Set CAP_INTERVAL_MAX_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Maximum interval between two CAP when the coordinator is in ACTIVE state. Set to 0 to disable
 * CAP.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_cap_interval_max_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_cap_max_be() - Set CAP_MAX_BE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Maximum backoff exponent.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_cap_max_be(struct lldc_context *context, int value);

/**
 * lldc_helper_set_cap_min_be() - Set CAP_MIN_BE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Minimum backoff exponent.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_cap_min_be(struct lldc_context *context, int value);

/**
 * lldc_helper_set_cap_slots_per_second() - Set CAP_SLOTS_PER_SECOND parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Average number of contention slots per second.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_cap_slots_per_second(struct lldc_context *context, int value);

/**
 * lldc_helper_set_device_loss_threshold() - Set DEVICE_LOSS_THRESHOLD parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Number of consecutive failed blocks for a coordinator to declare a device loss.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_device_loss_threshold(struct lldc_context *context, int value);

/**
 * lldc_helper_set_fast_ds_twr() - Set FAST_DS_TWR parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * TRUE to use the fast DS-TWR ranging procedure: disable smart power and ignore SB_MIN_RSTU.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_fast_ds_twr(struct lldc_context *context, bool value);

/**
 * lldc_helper_set_non_assoc_ifs() - Set NON_ASSOC_IFS parameter.
 * @context: Lldc context.
 * @slot_based: Use slot based mode if true, else gap based mode.
 * @value_rstu: Interval value in RSTU.
 *
 * Interval between the last sub-block DL frame and the first frame of non-associated exchange.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_non_assoc_ifs(struct lldc_context *context, bool slot_based,
					int value_rstu);

/**
 * lldc_helper_set_phy_channel() - Set PHY_CHANNEL parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Channel in use: 5 or 9.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_phy_channel(struct lldc_context *context, int value);

/**
 * lldc_helper_set_phy_phr_hi_rate() - Set PHY_PHR_HI_RATE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * For BPRF only, true to use the same rate as the data part for PHR.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_phy_phr_hi_rate(struct lldc_context *context, bool value);

/**
 * lldc_helper_set_phy_preamble_code() - Set PHY_PREAMBLE_CODE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Preamble code index in use. 9-24: BPRF, 25-32: HPRF.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_phy_preamble_code(struct lldc_context *context, int value);

/**
 * lldc_helper_set_phy_psdu_size() - Set PHY_PSDU_SIZE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * For HPRF, specify the use of A0 and A1 fields of the PHR to extend the maximum PSDU size. 0:
 * 1023, 1: 2047, 2: 4095.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_phy_psdu_size(struct lldc_context *context, int value);

/**
 * lldc_helper_set_phy_psr() - Set PHY_PSR parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Number of preamble symbol repetitions in use. For BPRF: must be 64. For HPRF: must be one of 16,
 * 24, 32, 48, 64, 96, 128 or 256.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_phy_psr(struct lldc_context *context, int value);

/**
 * lldc_helper_set_phy_data_rate() - Set PHY_DATA_RATE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * For HPRF only, data rate index in use. 2: 6.81 Mbit/s, 3: 27 Mbit/s, 4: 7.8 Mbit/s, 5: 31 Mbit/s,
 * 6: 54 Mbit/s.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_phy_data_rate(struct lldc_context *context, int value);

/**
 * lldc_helper_set_phy_sfd_selector() - Set PHY_SFD_SELECTOR parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * SFD pattern in use. 1-4: SFD defined in 4z of length 4, 8, 16 or 32 symbols.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_phy_sfd_selector(struct lldc_context *context, int value);

/**
 * lldc_helper_set_probing_beacon_as_dl() - Set PROBING_BEACON_AS_DL parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * True to use a beacon frame to fill the role of both DL frame and beacon frame when the
 * coordinator is in the PROBING state.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_probing_beacon_as_dl(struct lldc_context *context, bool value);

/**
 * lldc_helper_set_probing_cap_slots() - Set PROBING_CAP_SLOTS parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Override CAP_SLOTS_PER_SECOND when the coordinator is in PROBING state.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_probing_cap_slots(struct lldc_context *context, int value);

/**
 * lldc_helper_set_probing_interval_rstu() - Set PROBING_INTERVAL_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Interval between two beacons transmissions when the coordinator is in PROBING state. The same
 * interval is used between two CAP.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_probing_interval_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_sb_beacon_cap_slot_ifs() - Set SB_BEACON_CAP_SLOT_IFS parameter.
 * @context: Lldc context.
 * @slot_based: Use slot based mode if true, else gap based mode.
 * @value_rstu: Interval value in RSTU.
 *
 * Interval between the beacon frame acting as a DL frame and the first additional CAP slot of a
 * combined main sub-block and CAP.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_sb_beacon_cap_slot_ifs(struct lldc_context *context, bool slot_based,
						 int value_rstu);

/**
 * lldc_helper_set_sb_cap_slot_rstu() - Set SB_CAP_SLOT_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Duration of the additional CAP slots in a combined sub-block and CAP.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_sb_cap_slot_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_sb_dl_cap_slot_ifs() - Set SB_DL_CAP_SLOT_IFS parameter.
 * @context: Lldc context.
 * @slot_based: Use slot based mode if true, else gap based mode.
 * @value_rstu: Interval value in RSTU.
 *
 * Interval between the last sub-block DL frame and the first additional CAP slot of a combined sub-
 * block and CAP.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_sb_dl_cap_slot_ifs(struct lldc_context *context, bool slot_based,
					     int value_rstu);

/**
 * lldc_helper_set_sb_dl_slot_rstu() - Set SB_DL_SLOT_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Duration of the DL slot in a sub-block.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_sb_dl_slot_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_sb_min_rstu() - Set SB_MIN_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Minimum duration of a sub-block.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_sb_min_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_sb_aux_max() - Set SB_AUX_MAX parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Maximum number of auxiliary sub-block per block.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_sb_aux_max(struct lldc_context *context, int value);

/**
 * lldc_helper_set_sb_ul_slot_last_rstu() - Set SB_UL_SLOT_LAST_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Duration of the last UL slot in a sub-block.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_sb_ul_slot_last_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_sb_ul_slot_rstu() - Set SB_UL_SLOT_RSTU parameter.
 * @context: Lldc context.
 * @value_rstu: Parameter value.
 *
 * Duration of the UL slot in a sub-block, except the last one.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_sb_ul_slot_rstu(struct lldc_context *context, int value_rstu);

/**
 * lldc_helper_set_smart_power() - Set SMART_POWER parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * True to enable smart power feature, false otherwise.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_smart_power(struct lldc_context *context, bool value);

/**
 * lldc_helper_set_ranging_aoa_azimuth() - Set RANGING_AOA_AZIMUTH parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * True to request measurement of angle of arrival on azimuth.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_ranging_aoa_azimuth(struct lldc_context *context, bool value);

/**
 * lldc_helper_set_ranging_aoa_elevation() - Set RANGING_AOA_ELEVATION parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * True to request measurement of angle of arrival on elevation.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_ranging_aoa_elevation(struct lldc_context *context, bool value);

/**
 * lldc_helper_set_ranging_exponent() - Set RANGING_EXPONENT parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * If not 15, every block with an index which is a multiple of 2 to the power of rangingExponent is
 * a ranging enabled block.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_ranging_exponent(struct lldc_context *context, int value);

/**
 * lldc_helper_set_ranging_multi_node_mode() - Set RANGING_MULTI_NODE_MODE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Multi-node mode: 1: One-to-many, 2: Many-to-many.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_ranging_multi_node_mode(struct lldc_context *context, int value);

/**
 * lldc_helper_set_ranging_report_immediate() - Set RANGING_REPORT_IMMEDIATE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * True to request immediate ranging report.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_ranging_report_immediate(struct lldc_context *context, bool value);

/**
 * lldc_helper_set_ranging_round_usage() - Set RANGING_ROUND_USAGE parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * 1: SS-TWR, 2: DS-TWR.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_ranging_round_usage(struct lldc_context *context, int value);

/**
 * lldc_helper_set_ranging_sts_packet_config() - Set RANGING_STS_PACKET_CONFIG parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Secured ranging configuration. 0: SP0  (not secured), 1: SP1, 3: SP3.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_ranging_sts_packet_config(struct lldc_context *context, int value);

/**
 * lldc_helper_set_resync_threshold_exponent() - Set RESYNC_THRESHOLD_EXPONENT parameter.
 * @context: Lldc context.
 * @value: Parameter value.
 *
 * Threshold on the number of blocks since last successful DL frame reception to require a
 * resynchronization for a device. The actual threshold value is 2 to the power of
 * resyncThresholdExponent.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_resync_threshold_exponent(struct lldc_context *context, int value);

/**
 * lldc_helper_set_mfs_parameters() - Set MFS parameters.
 * @context: Lldc context.
 * @mfs_id: MFS id (0: interactive, 1: best effort).
 * @mfs_expiration_ms: MSDU expiration delay in milliseconds. Expiration is disabled if 0.
 * @mfs_window_msdu: Size of the MFS window in number of MSDUs.
 * @mfs_window_bytes: Size of the MFS window in bytes.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_set_mfs_parameters(struct lldc_context *context, int mfs_id,
					 int mfs_expiration_ms, int mfs_window_msdu,
					 int mfs_window_bytes);

/**
 * lldc_helper_get_time() - Get current time and last time offset.
 * @context: Lldc context.
 * @local_time_dtu: Local time value in DTU.
 * @network_time_offset_dtu: Network time offset value in DTU, difference
 * between coordinator and device time.
 *
 * Return: QERR_SUCCESS or error.
 */
int lldc_helper_get_time(struct lldc_context *context, uint32_t *local_time_dtu,
			 uint32_t *network_time_offset_dtu);

/**
 * lldc_helper_get_stats() - Get coordinator network statistics.
 * @context: Lldc context.
 * @stats: Coordinator statistics.
 *
 * Return: QERR_SUCCESS or error.
 */
int lldc_helper_get_stats(struct lldc_context *context, struct lldc_coord_stats *stats);

/**
 * lldc_helper_get_device_stats() - Get device network statistics.
 * @context: Lldc context.
 * @short_addr: device's short address.
 * @stats: Device statistics.
 *
 * Return: QERR_SUCCESS or error.
 */
int lldc_helper_get_device_stats(struct lldc_context *context, uint16_t short_addr,
				 struct lldc_stats *stats);

/**
 * lldc_helper_reset_stats() - Reset statistics immediately. Caller must note
 * that the reset can occur during an lld sub-block.
 * @context: Lldc context.
 *
 * Return: QERR_SUCCESS or error.
 */
int lldc_helper_reset_stats(struct lldc_context *context);

/**
 * lldc_helper_get_short_addr() - Get local short address.
 * @context: Lldc context.
 * @short_addr: Local short address.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_helper_get_short_addr(struct lldc_context *context, uint16_t *short_addr);

#ifdef __cplusplus
}
#endif
