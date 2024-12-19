/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "linux/minmax.h"
#include "net/mcps802154.h"
#include "net/mcps802154_schedule.h"
#include "qerr.h"
#include "qtils.h"

#define LLD_IFS_MAX 8184
/* LLD_RSTU_MAX - 134217720 is ((2^24) - 1) * 8, to fit in a 24 bit field. */
#define LLD_RSTU_MAX 134217720
/* LLD_SB_RSTU_MAX - 8184 is 1023 * 8, to fit in a 10 bit field. */
#define LLD_SB_RSTU_MAX 8184
#define LLD_AGG_LIMIT_BYTES_MIN 63
#define LLD_AGG_LIMIT_BYTES_MAX 4095
#define LLD_NEXT_FRAME_REPETITION_MAX 7
#define LLD_BEACON_IFS_DEFAULT 1200
#define LLD_BEACON_INTERVAL_RSTU_DEFAULT 120000
#define LLD_BLOCK_RSTU_DEFAULT 2400
#define LLD_BURST_IFS_DEFAULT -48
#define LLD_CAP_INTERVAL_MAX_RSTU_DEFAULT 120000
#define LLD_CAP_SLOTS_PER_SECOND_DEFAULT 64
#define LLD_CAP_MAX_BE_DEFAULT 8
#define LLD_CAP_MIN_BE_DEFAULT 2
#define LLD_CAP_BE_MAX 15
#define LLD_NON_ASSOC_IFS_DEFAULT 1200
#define LLD_PHY_CHANNEL_DEFAULT 9
#define LLD_PHY_PREAMBLE_CODE_DEFAULT 25
#define LLD_PHY_PREAMBLE_CODE_MIN 9
#define LLD_PHY_PREAMBLE_CODE_MAX 32
#define LLD_PHY_PREAMBLE_CODE_HPRF_MIN 25
#define LLD_PHY_PSDU_SIZE_DEFAULT 0
#define LLD_PHY_PSDU_SIZE_MIN 0
#define LLD_PHY_PSDU_SIZE_MAX 2
#define LLD_PHY_PSR_DEFAULT 64
#define LLD_PHY_DATA_RATE_MIN 2
#define LLD_PHY_DATA_RATE_MAX 6
#define LLD_PHY_DATA_RATE_DEFAULT 2
#define LLD_PHY_SFD_SELECTOR_MIN 1
#define LLD_PHY_SFD_SELECTOR_MAX 4
#define LLD_PHY_SFD_SELECTOR_DEFAULT 2
#define LLD_PROBING_CAP_SLOTS_DEFAULT 8
#define LLD_PROBING_CAP_SLOTS_MAX 128
#define LLD_PROBING_INTERVAL_RSTU_DEFAULT 120000
#define LLD_SB_BEACON_CAP_SLOT_IFS_DEFAULT -48
#define LLD_SB_CAP_SLOT_RSTU_DEFAULT 600 /* spec = 120 */
#define LLD_SB_DL_CAP_SLOT_IFS_DEFAULT 600 /* spec = 240 */
#define LLD_SB_DL_SLOT_RSTU_DEFAULT 600 /* spec = 240 */
#define LLD_SB_MIN_RSTU_DEFAULT 1200
#define LLD_SB_AUX_MAX_DEFAULT 1
#define LLD_SB_UL_SLOT_RSTU_DEFAULT 600 /* spec = 120 */
#define LLD_DTU_RCTU_DEFAULT 4096
#define LLD_RESYNC_THRESHOLD_EXPONENT_DEFAULT 3
#define LLD_RESYNC_THRESHOLD_EXPONENT_MAX 15
#define LLD_BLOCK_SKIP_MIN_DELAY_BLOCKS 1
#define LLD_NON_ASSOCIATED_ACK_MAX 8
#define LLD_DEVICE_LOSS_THRESHOLD_MIN 1
#define LLD_DEVICE_LOSS_THRESHOLD_MAX 65535
#define LLD_DEVICE_LOSS_THRESHOLD_DEFAULT 8
#define LLD_DEVICES_MAX 31
#define LLD_BLOCK_STRUCTURE_CHANGE_INDEX_OFFSET_DEFAULT 5
#define LLD_BLOCK_STRUCTURE_CHANGE_START_OFFSET_DTU_DEFAULT 0
#define LLD_RANGING_EXPONENT_DEFAULT 15
#define LLD_RANGING_EXPONENT_MAX 15
#define LLD_RANGING_MULTI_NODE_MODE_DEFAULT 1
#define LLD_RANGING_MULTI_NODE_MODE_MIN 1
#define LLD_RANGING_MULTI_NODE_MODE_MAX 2
#define LLD_RANGING_ROUND_USAGE_DEFAULT 1
#define LLD_RANGING_ROUND_USAGE_MIN 1
#define LLD_RANGING_ROUND_USAGE_MAX 2
#define LLD_RANGING_STS_PACKET_CONFIG_DEFAULT 0
#define LLD_RANGING_STS_PACKET_CONFIG_MAX 3
#define LLD_RANGING_CAPABILITY_MANY_TO_MANY 0
#define LLD_RANGING_CAPABILITY_DS_TWR 0
#define LLD_RANGING_CAPABILITY_SECURED_SP1 0
#define LLD_RANGING_CAPABILITY_SECURED_SP3 0

/**
 * LLD_RX_MARGIN_RSTU - Margin until end of slot for frame reception.
 * TODO: Remove this and use anticip.
 */
#define LLD_RX_MARGIN_RSTU 300

/**
 * LLD_BEACON_PAYLOAD_LEN_MAX - Maximum length of beacon payload.
 */
#define LLD_BEACON_PAYLOAD_LEN_MAX 32

/**
 * LLD_DEVICE_EPHEMERAL_REQUEST_PUBLIC_KEY_LEN - Ephemeral request public key
 * length.
 */
#define LLD_DEVICE_EPHEMERAL_REQUEST_PUBLIC_KEY_LEN 32

/**
 * LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN - Device identity key length.
 */
#define LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN 32

/**
 * LLD_RESPONSE_WAIT_TIME_MS - Maximum time allowed for a response to a MAC
 * command.
 */
#define LLD_RESPONSE_WAIT_TIME_MS 300

/**
 * LLD_DEVICE_SHORT_ADDR_MAX - Maximum value for a device short address.
 */
#define LLD_DEVICE_SHORT_ADDR_MAX 255

/**
 * LLD_SHORT_ADDR_UNDEFINED - Used for a device when it is not associated, for
 * a coordinator when it is not known. This address has the same value
 * as IEEE802154_ADDR_SHORT_BROADCAST, but has a different meaning here.
 */
#define LLD_SHORT_ADDR_UNDEFINED 0xffff

/**
 * LLD_RANGING_EXPONENT_UNDEFINED - Used by device or coordinator to detect if
 * a ranging IE was present in the last incoming frame.
 */
#define LLD_RANGING_EXPONENT_UNDEFINED 0xff

/**
 * LLD_IFS_TO_INT() - Encode an IFS as a signed integer.
 * @ifs: IFS structure to encode.
 *
 * Return: Signed integer, in DTU, negative for gap-based.
 */
#define LLD_IFS_TO_INT(ifs) \
	((ifs).mode == LLD_IFS_MODE_GAP ? -(ifs).duration_dtu : (ifs).duration_dtu)

/**
 * LLD_IFS_TO_INT_8RSTU() - Encode an IFS as signed integer in 8 * RSTU, to
 * be used in frames.
 * @local: LLD context.
 * @ifs: IFS structure to encode.
 *
 * Return: Signed integer, in 8 * RSTU.
 */
#define LLD_IFS_TO_INT_8RSTU(local, ifs) (LLD_IFS_TO_INT(ifs) / (local)->llhw->rstu_dtu / 8)

/**
 * LLD_INT_TO_IFS() - Decode an IFS from a signed integer.
 * @x: Value to decode.
 * @f: Positive factor.
 *
 * Return: IFS structure.
 */
#define LLD_INT_TO_IFS(x, f)                                            \
	((struct lld_ifs){                                              \
		.mode = (x) < 0 ? LLD_IFS_MODE_GAP : LLD_IFS_MODE_SLOT, \
		.duration_dtu = (x) < 0 ? -(x) * (f) : (x) * (f),       \
	})

/**
 * LLD_INT_8RSTU_TO_IFS() - Decode an IFS from a signed integer in 8 * RSTU.
 * @local: LLD context.
 * @x_int_8rstu: Value to decode.
 *
 * Value is sign extended from a 11-bit field.
 *
 * Return: IFS structure.
 */
#define LLD_INT_8RSTU_TO_IFS(local, x_int_8rstu)                                \
	({                                                                      \
		uint32_t _x_int_8rstu_u = (uint32_t)(x_int_8rstu) << (32 - 11); \
		int32_t _x_int_8rstu_s = (int32_t)_x_int_8rstu_u >> (32 - 11);  \
		int _x_dtu = LLD_8RSTU_TO_DTU((local), _x_int_8rstu_s);         \
		LLD_INT_TO_IFS(_x_dtu, 1);                                      \
	})

/**
 * LLD_DTU_TO_8RSTU() - Convert from DTU to 8 * RSTU.
 * @local: LLD context.
 * @x_dtu: value in DTU.
 *
 * Return: value in 8 * RSTU.
 */
#define LLD_DTU_TO_8RSTU(local, x_dtu) ((x_dtu) / (local)->llhw->rstu_dtu / 8)

/**
 * LLD_8RSTU_TO_DTU() - Convert from 8 * RSTU to DTU.
 * @local: LLD context.
 * @x_8rstu: value in 8 * RSTU.
 *
 * Return: value in DTU.
 */
#define LLD_8RSTU_TO_DTU(local, x_8rstu) ((x_8rstu) * (local)->llhw->rstu_dtu * 8)

/**
 * enum lld_ifs_mode - Interframe spacing mode.
 * @LLD_IFS_MODE_GAP:
 * 	Gap based mode.
 * @LLD_IFS_MODE_SLOT:
 * 	Slot based mode.
 */
enum lld_ifs_mode {
	LLD_IFS_MODE_GAP,
	LLD_IFS_MODE_SLOT,
};

/**
 * enum lld_ranging_multi_node_mode - Ranging multi-node mode.
 * @LLD_RANGING_MODE_ONE_TO_MANY:
 * 	One-to-many mode.
 * @LLD_RANGING_MODE_MANY_TO_MANY:
 * 	Many-to-many mode.
 */
enum lld_ranging_multi_node_mode {
	LLD_RANGING_MODE_ONE_TO_MANY = 1,
	LLD_RANGING_MODE_MANY_TO_MANY = 2,
};

/**
 * enum lld_ranging_round_usage - Ranging roung usage.
 * @LLD_RANGING_ROUND_USAGE_SS_TWR:
 * 	Single-sided two-way ranging.
 * @LLD_RANGING_ROUND_USAGE_DS_TWR:
 * 	Dual-sided two-way ranging.
 */
enum lld_ranging_round_usage {
	LLD_RANGING_ROUND_USAGE_SS_TWR = 1,
	LLD_RANGING_ROUND_USAGE_DS_TWR = 2,
};

/**
 * enum lld_ranging_sts_packet_config - Secured ranging configuration.
 * @LLD_RANGING_STS_PACKET_CONFIG_SP0:
 * 	SP0 frame format.
 * @LLD_RANGING_STS_PACKET_CONFIG_SP1:
 * 	SP1 frame format.
 * @LLD_RANGING_STS_PACKET_CONFIG_SP3:
 * 	SP3 frame format.
 */
enum lld_ranging_sts_packet_config {
	LLD_RANGING_STS_PACKET_CONFIG_SP0 = 0,
	LLD_RANGING_STS_PACKET_CONFIG_SP1 = 1,
	LLD_RANGING_STS_PACKET_CONFIG_SP3 = 3,
};

/**
 * enum lld_ranging_capability_flags - Flags for device ranging capabilities.
 * @LLD_RANGING_CAPABILITY_FLAG_NONE:
 * 	No ranging capability.
 * @LLD_RANGING_CAPABILITY_FLAG_MANY_TO_MANY:
 * 	Many-to-many ranging.
 * @LLD_RANGING_CAPABILITY_FLAG_DS_TWR:
 * 	DS-TWR ranging.
 * @LLD_RANGING_CAPABILITY_FLAG_SECURED_SP1:
 * 	Secured ranging using SP1 frames.
 * @LLD_RANGING_CAPABILITY_FLAG_SECURED_SP3:
 * 	Secured ranging using SP3 frames.
 */
enum lld_ranging_capability_flags {
	LLD_RANGING_CAPABILITY_FLAG_NONE = 0,
	LLD_RANGING_CAPABILITY_FLAG_MANY_TO_MANY = 1 << 0,
	LLD_RANGING_CAPABILITY_FLAG_DS_TWR = 1 << 1,
	LLD_RANGING_CAPABILITY_FLAG_SECURED_SP1 = 1 << 2,
	LLD_RANGING_CAPABILITY_FLAG_SECURED_SP3 = 1 << 3,
};

/**
 * struct lld_ifs - Interframe spacing.
 */
struct lld_ifs {
	/**
	 * @mode: IFS mode.
	 */
	enum lld_ifs_mode mode;
	/**
	 * @duration_dtu: IFS duration.
	 */
	int duration_dtu;
};

static const enum mcps802154_hrp_uwb_psdu_size lld_conv_phy_psdu_size[LLD_PHY_PSDU_SIZE_MAX + 1] = {
	MCPS802154_HRP_UWB_PSDU_SIZE_1023, MCPS802154_HRP_UWB_PSDU_SIZE_2047,
	MCPS802154_HRP_UWB_PSDU_SIZE_4095
};

static const int lld_conv_phy_data_rate[LLD_PHY_DATA_RATE_MAX - LLD_PHY_DATA_RATE_MIN + 1] = {
	MCPS802154_DATA_RATE_6M81, MCPS802154_DATA_RATE_27M2, MCPS802154_DATA_RATE_7M80,
	MCPS802154_DATA_RATE_31M2, 54
};

static const enum mcps802154_sfd
	lld_conv_phy_sfd_selector[LLD_PHY_SFD_SELECTOR_MAX - LLD_PHY_SFD_SELECTOR_MIN + 1] = {
		MCPS802154_SFD_4Z_4,
		MCPS802154_SFD_4Z_8,
		MCPS802154_SFD_4Z_16,
		MCPS802154_SFD_4Z_32,
	};

/**
 * lld_ifs_dtu() - Compute interval between two frames.
 * @llhw: Low-level device pointer.
 * @first_frame_duration_dtu: Duration of the first frame.
 * @ifs: IFS.
 *
 * Return: The interval between the start date of the frames, in DTU.
 */
static inline int lld_ifs_dtu(struct mcps802154_llhw *llhw, int first_frame_duration_dtu,
			      const struct lld_ifs *ifs)
{
	int ifs_dtu = ifs->duration_dtu;

	if (ifs->mode == LLD_IFS_MODE_GAP)
		ifs_dtu += first_frame_duration_dtu;

	return qround_up(ifs_dtu, 8 * llhw->rstu_dtu);
}

/**
 * lld_check_phy_psr() - Check if the PSR and the preamble code are compatible.
 * @phy_psr: PSR.
 * @phy_preamble_code: Preamble code.
 *
 * Return: True if the PSR and the preamble code are compatible.
 */
static inline bool lld_check_phy_psr(int phy_psr, int phy_preamble_code)
{
	return (phy_preamble_code < LLD_PHY_PREAMBLE_CODE_HPRF_MIN &&
		phy_psr == MCPS802154_PSR_64) ||
	       (phy_preamble_code >= LLD_PHY_PREAMBLE_CODE_HPRF_MIN &&
		(phy_psr == MCPS802154_PSR_16 || phy_psr == MCPS802154_PSR_24 ||
		 phy_psr == MCPS802154_PSR_32 || phy_psr == MCPS802154_PSR_48 ||
		 phy_psr == MCPS802154_PSR_64 || phy_psr == MCPS802154_PSR_96 ||
		 phy_psr == MCPS802154_PSR_128 || phy_psr == MCPS802154_PSR_256));
}

/**
 * struct lld_network_params - Network parameters.
 */
struct lld_network_params {
	/**
	 * @block_dtu: Duration of a block.
	 */
	int block_dtu;
	/**
	 * @burst_ifs: Interval between burst DL frames, or between the last
	 * sub-block DL frame and the first burst DL frame.
	 */
	struct lld_ifs burst_ifs;
	/**
	 * @cap_max_be: Maximum backoff exponent.
	 */
	int cap_max_be;
	/**
	 * @cap_min_be: Maximum backoff exponent.
	 */
	int cap_min_be;
	/**
	 * @fast_ds_twr: TRUE to use the fast DS-TWR ranging procedure: disable
	 * smart power and ignore SB_MIN_RSTU.
	 */
	bool fast_ds_twr;
	/**
	 * @phy_channel: Channel in use. 5 or 9.
	 */
	int phy_channel;
	/**
	 * @phy_phr_hi_rate: For BPRF only, true to use the same rate as the
	 * data part for PHR.
	 */
	bool phy_phr_hi_rate;
	/**
	 * @phy_preamble_code: Preamble code index in use. 9-24: BPRF, 25-32:
	 * HPRF.
	 */
	int phy_preamble_code;
	/**
	 * @phy_psdu_size: For HPRF, specify the use of A0 and A1 fields of the
	 * PHR to extend the maximum PSDU size.
	 */
	enum mcps802154_hrp_uwb_psdu_size phy_psdu_size;
	/**
	 * @phy_psr: Number of preamble symbol repetitions in use.
	 */
	enum mcps802154_psr phy_psr;
	/**
	 * @phy_data_rate: For HPRF only, data rate in use. See &enum
	 * mcps802154_data_rate.
	 */
	int phy_data_rate;
	/**
	 * @phy_sfd_selector: SFD pattern in use.
	 */
	enum mcps802154_sfd phy_sfd_selector;
	/**
	 * @ranging_aoa_azimuth: TRUE to request measurement of angle of arrival
	 * on azimuth.
	 */
	bool ranging_aoa_azimuth;
	/**
	 * @ranging_aoa_elevation: TRUE to request measurement of angle of
	 * arrival on elevation.
	 */
	bool ranging_aoa_elevation;
	/**
	 * @ranging_exponent: If not 15, every block with an index which is a
	 * multiple of 2 to the power of rangingExponent is a ranging enabled
	 * block.
	 */
	int ranging_exponent;
	/**
	 * @ranging_multi_node_mode: Ranging multi-node mode.
	 */
	enum lld_ranging_multi_node_mode ranging_multi_node_mode;
	/**
	 * @ranging_report_immediate: TRUE to request immediate ranging report.
	 */
	bool ranging_report_immediate;
	/**
	 * @ranging_round_usage: Ranging round usage.
	 */
	enum lld_ranging_round_usage ranging_round_usage;
	/**
	 * @ranging_sts_packet_config: Secured ranging configuration.
	 */
	enum lld_ranging_sts_packet_config ranging_sts_packet_config;
	/**
	 * @resync_threshold_exponent: Threshold on the number of blocks since
	 * last successful DL frame reception to require a resynchronization
	 * for a device. The actual threshold value is 2 to the power of
	 * resyncThresholdExponent
	 */
	int resync_threshold_exponent;
	/**
	 * @sb_cap_slot_dtu: Duration of the additional CAP slots in a combined
	 * sub-block and CAP.
	 */
	int sb_cap_slot_dtu;
	/**
	 * @sb_dl_slot_dtu: Duration of the DL slot in a sub-block.
	 */
	int sb_dl_slot_dtu;
	/**
	 * @sb_min_dtu: Minimum duration of a sub-block.
	 */
	int sb_min_dtu;
	/**
	 * @sb_aux_max: Maximum number of auxiliary sub-block per block.
	 */
	int sb_aux_max;
	/**
	 * @sb_ul_slot_last_dtu: Duration of the last UL slot in a sub-block.
	 */
	int sb_ul_slot_last_dtu;
	/**
	 * @sb_ul_slot_dtu: Duration of the UL slot in a sub-block, except the
	 * last one.
	 */
	int sb_ul_slot_dtu;
};

/**
 * struct lld_block_skip - Block skip information.
 */
struct lld_block_skip {
	/**
	 * @index: Index of the next skipped blocked.
	 */
	uint32_t index;
	/**
	 * @n_blocks: Block skip duration in number of blocks or 0
	 * if no block skip is scheduled.
	 */
	int n_blocks;
};

/**
 * struct lld_non_assoc_ack_ie - Non-Associated Acknowledgement IE.
 */
struct lld_non_assoc_ack_ie {
	/**
	 * @repetition: Number of acknowledgement repetitions.
	 */
	int repetition;
	/**
	 * @src_extended_addr: Array of source extended addresses.
	 */
	uint64_t src_extended_addr[LLD_NON_ASSOCIATED_ACK_MAX];
	/**
	 * @seq_num: Array of sequence numbers.
	 */
	uint8_t seq_num[LLD_NON_ASSOCIATED_ACK_MAX];
};

/**
 * struct lld_block_struct_change - Block structure change information.
 */
struct lld_block_struct_change {
	/**
	 * @index_offset: Block structure change index offset.
	 */
	int index_offset;
	/**
	 * @n_ul_slots: Number of UL slots.
	 */
	int n_ul_slots;
	/**
	 * @ul_slot_device_short_addr: Short address of the device assigned to
	 * each slot.
	 */
	uint8_t ul_slot_device_short_addr[LLD_DEVICES_MAX];
	/**
	 * @start_offset_dtu: Block start offset.
	 */
	int start_offset_dtu;
	/**
	 * @network_params: Network parameters.
	 */
	struct lld_network_params network_params;
};

/**
 * struct lld_ranging_control - Ranging control information.
 */
struct lld_ranging_control {
	/**
	 * @ranging_aoa_azimuth: TRUE to request measurement of angle of arrival
	 * on azimuth.
	 */
	bool ranging_aoa_azimuth;
	/**
	 * @ranging_aoa_elevation: TRUE to request measurement of angle of
	 * arrival on elevation.
	 */
	bool ranging_aoa_elevation;
	/**
	 * @ranging_exponent: If not 15, every block with an index which is a
	 * multiple of 2 to the power of rangingExponent is a ranging enabled
	 * block.
	 */
	int ranging_exponent;
	/**
	 * @ranging_multi_node_mode: Multi-node mode: 1: One-to-many,
	 * 2: Many-to-many.
	 */
	int ranging_multi_node_mode;
	/**
	 * @ranging_report_immediate: TRUE to request immediate ranging report.
	 */
	bool ranging_report_immediate;
	/**
	 * @ranging_round_usage: 1: SS-TWR, 2: DS-TWR (Reserved).
	 */
	int ranging_round_usage;
	/**
	 * @ranging_sts_packet_config: Secured ranging configuration.
	 * 0: SP0 (not secured), 1: SP1, 3: SP3..
	 */
	int ranging_sts_packet_config;
};

/**
 * struct lld_ranging_measurement_element - Ranging measurement information
 * element.
 */
struct lld_ranging_measurement_element {
	/**
	 * @reply_or_round_trip_time_rctu: When a reply time, this is the time
	 * difference between the receive time of the received frame and the
	 * transmit time of the sent frame.
	 * When a round-trip time, this is the time difference between the
	 * transmit time of the sent frame and the receive time of the received
	 * frame. The unit is RCTU.
	 */
	int reply_or_round_trip_time_rctu;
	/**
	 * @aoa_azimuth_2pi: Estimated angle of arrival on azimuth.
	 */
	int aoa_azimuth_2pi;
	/**
	 * @aoa_azimuth_fom: Figure of merit of the azimuth AoA.
	 */
	int aoa_azimuth_fom;
	/**
	 * @aoa_elevation_2pi: Estimated angle of arrival on elevation.
	 */
	int aoa_elevation_2pi;
	/**
	 * @aoa_elevation_fom: Figure of merit of the elevation AoA.
	 */
	int aoa_elevation_fom;
};

/**
 * struct lld_ranging_measurement - Ranging measurement information.
 */
struct lld_ranging_measurement {
	/**
	 * @ranging_exponent: If not 15, every block with an index which is a
	 * multiple of 2 to the power of rangingExponent is a ranging enabled
	 * block.
	 */
	int ranging_exponent;
	/**
	 * @reply_or_round_trip_time: TRUE if reply or round-trip time field is
	 * present in each ranging measurement information element.
	 */
	bool reply_or_round_trip_time;
	/**
	 * @aoa_azimuth: TRUE if AoA azimuth field and AoA azimuth FoM field are
	 * present in each ranging measurement information element.
	 */
	bool aoa_azimuth;
	/**
	 * @aoa_elevation: TRUE if AoA elevation field and AoA elevation FoM
	 * field are present in each ranging measurement information element.
	 */
	bool aoa_elevation;
	/**
	 * @measurement_element: Ranging measurement information element.
	 */
	struct lld_ranging_measurement_element measurement_element;
};

/**
 * struct lld_rx_frame_payload_info - Info about received frame payload.
 */
struct lld_rx_frame_payload_info {
	/**
	 * @lqi: Link quality indicator.
	 */
	uint8_t lqi;
	/**
	 * @device_index: Device index if frame was received by coordinator.
	 */
	int device_index;
};

/**
 * is_before_seq() - Check if sequence number A is before sequence number B.
 * @a_seq: A sequence number.
 * @b_seq: B sequence number.
 *
 * Return: True if A sequence number is before B sequence number.
 */
static inline bool is_before_seq(uint8_t a_seq, uint8_t b_seq)
{
	return (int8_t)(a_seq - b_seq) < 0;
}

/**
 * is_before_index() - Check if index number A is before index number B.
 * @a_idx: A index number.
 * @b_idx: B index number.
 *
 * Return: True if A index number is before B index number.
 */
static inline bool is_before_index(uint32_t a_idx, uint32_t b_idx)
{
	return (int32_t)(a_idx - b_idx) < 0;
}

/**
 * lld_get_dl_start_dtu() - Get position of DL frame inside a sub-block.
 * @n_ul_slots: Number of UL slots.
 * @network_params: Network parameters.
 *
 * Return: Offset between the sub-block start and the DL frame.
 */
static inline int lld_get_dl_start_dtu(int n_ul_slots,
				       const struct lld_network_params *network_params)
{
	if (n_ul_slots == 0)
		return 0;

	return (n_ul_slots - 1) * network_params->sb_ul_slot_dtu +
	       network_params->sb_ul_slot_last_dtu;
}

/**
 * lld_get_subblock_duration_dtu() - Get sub-block duration.
 * @n_ul_slots: Number of UL slots.
 * @network_params: Network parameters.
 *
 * Return: The sub-block duration in dtu.
 */
static inline int lld_get_subblock_duration_dtu(int n_ul_slots,
						const struct lld_network_params *network_params)
{
	int sb_dtu =
		lld_get_dl_start_dtu(n_ul_slots, network_params) + network_params->sb_dl_slot_dtu;
	return max(sb_dtu, network_params->sb_min_dtu);
}

/**
 * lld_convert_dtu_to_4096rctu() - Convert time from dtu to 4096RCTU.
 * @timestamp_dtu: timestamp in DTU.
 * @dtu_rctu: Duration of one device time unit in ranging counter time.
 *
 * Return: Timestamp converted from dtu to 4096RCTU.
 */
static inline uint32_t lld_convert_dtu_to_4096rctu(uint32_t timestamp_dtu, int dtu_rctu)
{
	if (dtu_rctu != LLD_DTU_RCTU_DEFAULT)
		return (uint32_t)(((uint64_t)timestamp_dtu * dtu_rctu) /
				  (uint64_t)LLD_DTU_RCTU_DEFAULT);
	else
		return timestamp_dtu;
}

/*
 * lld_convert_4096rctu_to_dtu() - Convert time from 4096RCTU to DTU.
 * @timestamp_4096rctu: timestamp in 4096RCTU.
 * @dtu_rctu: Duration of one device time unit in ranging counter time.
 *
 * Return: Timestamp converted from 4096RCTU to DTU.
 */
static inline uint32_t lld_convert_4096rctu_to_dtu(uint32_t timestamp_4096rctu, int dtu_rctu)
{
	if (dtu_rctu != LLD_DTU_RCTU_DEFAULT)
		return (uint32_t)(((uint64_t)timestamp_4096rctu * LLD_DTU_RCTU_DEFAULT) /
				  (uint64_t)dtu_rctu);
	else
		return timestamp_4096rctu;
}

/*
 * lld_is_block_skipped() - Check if block index is inside the block skip
 * interval.
 * @block_index: Block index.
 * @block_skip: Block skip information.
 *
 * Return: True if block index is in the block skip interval.
 */
static inline bool lld_is_block_skipped(uint32_t block_index,
					const struct lld_block_skip *block_skip)
{
	return block_skip->n_blocks && !((int32_t)(block_index - block_skip->index) < 0) &&
	       (int32_t)(block_index - block_skip->index - block_skip->n_blocks) < 0;
}
