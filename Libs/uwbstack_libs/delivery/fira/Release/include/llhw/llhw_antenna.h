/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "llhw.h"
#include "llhw_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_L1_CONFIG_OLD_ANTENNA_MANAGEMENT_MODEL

/**
 * llhw_prepare_tx_antennas() - Prepare TX antennas: read calibration related to
 * antenna management and compute the Antenna ID from RF path configuration.
 * @uwb: the UWB device.
 *
 * Note: that function must always be called before llhw_set_tx_antenna.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_prepare_tx_antennas(struct uwb_chip *uwb);

/**
 * llhw_prepare_rx_antennas() - Prepare RX antennas: read calibration related to
 * antenna management and compute the Antenna ID from RF path configuration.
 * @uwb: the UWB device.
 *
 * Note: that function must always be called before llhw_set_rx_antenna.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_prepare_rx_antennas(struct uwb_chip *uwb);

/**
 * llhw_prepare_full_duplex_antennas() - configure antennas for full-duplex.
 * @uwb: the UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_prepare_full_duplex_antennas(struct uwb_chip *uwb);

#else /* !CONFIG_L1_CONFIG_OLD_ANTENNA_MANAGEMENT_MODEL. */

/**
 * llhw_get_antenna_pair() - Get RX antenna pair regarding index.
 * @uwb: the UWB device.
 * @ant_pair_idx: Antenna pair index in calibration.
 * @ant_pair: Antenna pair.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_get_antenna_pair(const struct uwb_chip *uwb, int8_t ant_pair_idx,
				struct antenna_pair *ant_pair);

/**
 * llhw_get_antenna_path - Fill antenna path information.
 * @uwb: the UWB device.
 * @ant_path_idx: antena path index.
 * @ant_path: antenna path.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_get_antenna_path(const struct uwb_chip *uwb, int8_t ant_path_idx,
				struct antenna_path_def *ant_path);

/**
 * llhw_get_rx_antenna_set() - Get RX antenna set regarding index.
 * @uwb: the UWB device.
 * @ant_set_idx: Antenna set index in calibration.
 * @rx_ant_set: Antenna set.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_get_rx_antenna_set(const struct uwb_chip *uwb, int8_t ant_set_idx,
				  struct rx_antenna_set *rx_ant_set);

/**
 * llhw_get_rx_pair_ant_paths() - Get Antenna Paths contained in an Antenna Pair.
 * @uwb: The UWB device.
 * @ant_pair_idx: Index of the Antenna Pair.
 * @ant_paths: Returned table containing the 2 Antenna Paths of the pair.
 * @ant_paths_idx: Returned table containing indexes of the 2 Antenna Paths of the pair.
 * @ext_sw_cfg: Returned External Switch configuration.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_get_rx_pair_ant_paths(const struct uwb_chip *uwb, uint8_t ant_pair_idx,
				     struct antenna_path_def ant_paths[2], uint8_t ant_paths_idx[2],
				     uint8_t *ext_sw_cfg);

/**
 * llhw_generate_rf_config_half_duplex() - Generate RF config for half-duplex,
 * based on Antenna Set provided.
 * @uwb: The UWB device.
 * @is_sip: True if device is a SiP, false otherwise.
 * @rx: true if RX, else if TX.
 * @rffe_cfg: The returned RFFE config.
 * @rfsw_cfg: The returned RF Switch configuraton for each LUT used in AUTO
 * mode.
 * @rx_cfg: The returned RX Receiver(s) configuration.
 * @ext_sw_cfg: The returned External Switch configuration.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_generate_rf_config_half_duplex(struct uwb_chip *uwb, bool is_sip, bool rx,
					      struct uwb_rffe_config *rffe_cfg,
					      dwt_rfsw_config_t *rfsw_cfg, uint32_t *rx_cfg,
					      uint8_t *ext_sw_cfg);

/**
 * llhw_generate_rf_config_full_duplex() - Generate RF config for full-duplex,
 * based on Antenna Set provided.
 * @uwb: The UWB device.
 * @is_sip: True if device is a SiP, false otherwise.
 * @rffe_cfg: The returned RFFE config.
 * @rx_cfg: The returned RX Receiver(s) configuration.
 * @ext_sw_cfg: The returned External Switch configuration.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_generate_rf_config_full_duplex(struct uwb_chip *uwb, bool is_sip,
					      struct uwb_rffe_config *rffe_cfg, uint32_t *rx_cfg,
					      uint8_t *ext_sw_cfg);

#endif /* CONFIG_L1_CONFIG_OLD_ANTENNA_MANAGEMENT_MODEL. */

/**
 * llhw_set_tx_antenna() - configure TX antenna
 * @uwb: the UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_set_tx_antenna(struct uwb_chip *uwb);

/**
 * llhw_set_rx_antenna() - configure RX antenna
 * @uwb: the UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_set_rx_antenna(struct uwb_chip *uwb);

/**
 * llhw_set_full_duplex_antenna() - Configure RF for full-duplex.
 * @uwb: The UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_set_full_duplex_antenna(struct uwb_chip *uwb);

/**
 * llhw_read_tx_delay_from_ant_set - Read tx delay.
 * @uwb: the UWB device.
 * @channel: UWB channel used.
 * @ant_set_id: Antenna set ID.
 * @ant_delay: Returned tx antenna delay.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_read_tx_delay_from_ant_set(struct uwb_chip *uwb, int channel, int8_t ant_set_id,
					  uint32_t *ant_delay);

/**
 * llhw_get_tx_antenna_id() - get TX antenna ID from Antenna configuration.
 * @uwb: the UWB device.
 * @ant_set_idx: Antenna Set index.
 * @tx_ant_id: Antenna TX index.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_get_tx_antenna_id(struct uwb_chip *uwb, int8_t ant_set_idx, uint8_t *tx_ant_id);

#ifdef CONFIG_UWB_MCPS_POST_TX_TONE
/**
 * llhw_set_tx_antenna_post_tone() - configure TX antenna for post Tx tone
 * @uwb: the UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_set_tx_antenna_post_tone(struct uwb_chip *uwb);
#endif

/**
 * llhw_get_toa_segments() - Get segment(s) used for ToA measurement.
 * @uwb: the UWB chip.
 * @is_master_forced: indicates if the master is forced in case of dual RX.
 * @dual_receiver: true if dual receiver is used.
 *
 * The IDs of the segments used for ToA are stored in the field `segment_id`
 * of `uwb->rx`, at indexes 0 and 1.
 */
void llhw_get_toa_segments(struct uwb_chip *uwb, bool is_master_forced, bool dual_receiver);

/**
 * llhw_set_trx_antenna() - Configure TX/RX antenna.
 * @uwb: the UWB device.
 * @is_rx: is RX or TX.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_set_trx_antenna(struct uwb_chip *uwb, bool is_rx);

#ifdef __cplusplus
}
#endif
