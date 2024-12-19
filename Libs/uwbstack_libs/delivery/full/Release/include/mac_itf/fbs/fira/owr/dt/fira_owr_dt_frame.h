/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fira/fira_frame_defines.h"
#include "net/af_ieee802154.h"
#include "net/fira_region_params.h"
#include "net/mcps802154.h"
#include "net/mcps802154_frame.h"
#include "qbitfield.h"
#include "quwbs/fbs/defs.h"

#include <stdbool.h>
#include <stdint.h>

struct fira_owr_dt_frame_active_rr_info;
struct fira_owr_dt_frame_final;
struct fira_owr_dt_frame_poll;
struct fira_owr_dt_frame_response;
struct fira_owr_dt_frame_tx_timestamp;
struct fira_workspace;
struct sk_buff;

/**
 * FIRA_OWR_DTM_MAC_FRAME_FC_VALUE - Definitions from FiRa-v2.0.0 6.3.7.1.7.
 * TODO: Why is address type short?
 * lfc=1, dam=sam=Short, panid=0, secen=1,
 * seq=1, frpen=0, ver=0, ack=0, ie=1
 */
#define FIRA_OWR_DTM_MAC_FRAME_FC_VALUE                                                          \
	IEEE802154_FC_MP_VALUE_LONG(IEEE802154_ADDR_SHORT, IEEE802154_ADDR_SHORT, 0, 1, 1, 0, 0, \
				    0, 1)

/**
 * FIRA_OWR_DTM_MAC_FRAME_HEADER_LEN - MAC header is composed of:
 * - Normal size Multipurpose Frame Control Field
 * - Dest and source addresses.
 * - Security Control Field 802.15.4-2020 9.4.2.
 */
#define FIRA_OWR_DTM_MAC_FRAME_HEADER_LEN \
	(IEEE802154_FC_LEN + IEEE802154_SHORT_ADDR_LEN * 2 + IEEE802154_SCF_LEN)

/**
 * FIRA_OWR_DTM_MESSAGE_ID_TYPE_VALUE() - Definitions from FiRa-v2.0.0 5.9.13.
 * @_t_: Is the DL-TDoA Message Type.
 */
#define FIRA_OWR_DTM_MESSAGE_ID_TYPE_VALUE(_t_)                                       \
	(QFIELD_PREP(FIRA_MESSAGE_UWB_MSG_ID_MASK, FIRA_MESSAGE_ID_ONE_WAY_RANGING) | \
	 QFIELD_PREP(FIRA_MESSAGE_OWR_MSG_TYPE_MASK, _t_))

/* Definitions from FiRa-v2.0.0 5.9.13.2 Downlink Tdoa Message (DTM). */
#define FIRA_OWR_DTM_MESSAGE_CONTROL_LEN 3
#define FIRA_OWR_DTM_ROUND_INDEX_LEN 1
#define FIRA_OWR_DTM_BLOCK_INDEX_LEN 2

#define FIRA_OWR_DTM_TX_TIMESTAMP_LEN(_s_) \
	(5 + 3 * ((_s_).tx_timestamp.len == FIRA_OWR_DTM_TIMESTAMP_64BITS))

#define FIRA_OWR_DTM_MGMT_LEN(_s_)                                          \
	(1 * (_s_).mgmt_slot_index_present + 2 * !(_s_).final_dtm_present + \
	 IEEE802154_SHORT_ADDR_LEN +                                        \
	 (IEEE802154_EXTENDED_ADDR_LEN - IEEE802154_SHORT_ADDR_LEN) * (_s_).extended_addresses)
#define FIRA_OWR_DTM_MGMT_LIST_LEN(_s_) (FIRA_OWR_DTM_MGMT_LEN(_s_) * (_s_).mgmt_list_len)

#define FIRA_OWR_DTM_RESP_CFO_LEN(_s_) (2 * (_s_).cfo_present)

#define FIRA_OWR_DTM_REPLY_TIME_LEN(_s_) (6 + 6 * (_s_).extended_addresses)
#define FIRA_OWR_DTM_REPLY_TIME_LIST_LEN(_s_) \
	(FIRA_OWR_DTM_REPLY_TIME_LEN(_s_) * (_s_).reply_time_list_len)

#define FIRA_OWR_DTM_RESP_REPLY_TIME_LEN 4

#define FIRA_OWR_DTM_RESP_TOF_RESULT_LEN(_s_) (2 * (_s_).tof_present)

#define FIRA_OWR_DTM_HOP_COUNT_LEN(_s_) (1 * (_s_).hop_count_present)

#define FIRA_OWR_DT_ANCHOR_LOCATION_PRESENT(_t_) \
	((_t_) == FIRA_DT_LOCATION_COORD_RELATIVE || (_t_) == FIRA_DT_LOCATION_COORD_WGS84)
#define FIRA_OWR_DTM_PAYLOAD_ANCHOR_LOCATION_LEN(_s_)                      \
	(((_s_).anchor_location_type == FIRA_DT_LOCATION_COORD_RELATIVE) ? \
		 1 + FIRA_OWR_DTM_LOCATION_COORD_RELATIVE_LEN :            \
	 ((_s_).anchor_location_type == FIRA_DT_LOCATION_COORD_WGS84) ?    \
		 1 + FIRA_OWR_DTM_LOCATION_COORD_WGS84_LEN :               \
		 0)

#define FIRA_OWR_DTM_PAYLOAD_ACTIVE_RR_INFO_LEN(_s_) \
	((_s_).active_rr_info_present + (_s_).active_rr_info.len)

#define FIRA_OWR_DTM_MESSAGE_HEADER_LEN                                                            \
	(FIRA_IE_VENDOR_OUI_LEN + FIRA_MESSAGE_UWB_MSG_ID_LEN + FIRA_OWR_DTM_MESSAGE_CONTROL_LEN + \
	 FIRA_OWR_DTM_ROUND_INDEX_LEN + FIRA_OWR_DTM_BLOCK_INDEX_LEN)

/*
 * FiRa Consortium UWB MAC Technical Requirements v2.0.0_0.9r0
 * 5.9.13.2 Downlink TDoA Message
 * Table 41 - Message Control field
 *
 * Prefix: FIRA_OWR_DTM_CONTROL_
 */
#define FIRA_OWR_DTM_CONTROL_MGMT_LIST_LEN_WIDTH 0xfu
#define FIRA_OWR_DTM_CONTROL_MGMT_LIST_LEN_SHIFT 0u
#define FIRA_OWR_DTM_CONTROL_MGMT_LIST_LEN_MASK GET_MASK(FIRA_OWR_DTM_CONTROL, MGMT_LIST_LEN)

#define FIRA_OWR_DTM_CONTROL_RANGING_SLOT_INDEX_PRESENT_WIDTH 0x1u
#define FIRA_OWR_DTM_CONTROL_RANGING_SLOT_INDEX_PRESENT_SHIFT 4u
#define FIRA_OWR_DTM_CONTROL_RANGING_SLOT_INDEX_PRESENT_MASK \
	GET_MASK(FIRA_OWR_DTM_CONTROL, RANGING_SLOT_INDEX_PRESENT)

#define FIRA_OWR_DTM_CONTROL_REPLY_TIME_LIST_LEN_WIDTH 0xfu
#define FIRA_OWR_DTM_CONTROL_REPLY_TIME_LIST_LEN_SHIFT 6u
#define FIRA_OWR_DTM_CONTROL_REPLY_TIME_LIST_LEN_MASK \
	GET_MASK(FIRA_OWR_DTM_CONTROL, REPLY_TIME_LIST_LEN)

#define FIRA_OWR_DTM_CONTROL_FINAL_DTM_PRESENT_WIDTH 0x1u
#define FIRA_OWR_DTM_CONTROL_FINAL_DTM_PRESENT_SHIFT 10u
#define FIRA_OWR_DTM_CONTROL_FINAL_DTM_PRESENT_MASK \
	GET_MASK(FIRA_OWR_DTM_CONTROL, FINAL_DTM_PRESENT)

#define FIRA_OWR_DTM_CONTROL_TX_TIMESTAMP_LEN_WIDTH 0x3u
#define FIRA_OWR_DTM_CONTROL_TX_TIMESTAMP_LEN_SHIFT 11u
#define FIRA_OWR_DTM_CONTROL_TX_TIMESTAMP_LEN_MASK GET_MASK(FIRA_OWR_DTM_CONTROL, TX_TIMESTAMP_LEN)

#define FIRA_OWR_DTM_CONTROL_TX_TIMESTAMP_TYPE_WIDTH 0x1u
#define FIRA_OWR_DTM_CONTROL_TX_TIMESTAMP_TYPE_SHIFT 13u
#define FIRA_OWR_DTM_CONTROL_TX_TIMESTAMP_TYPE_MASK \
	GET_MASK(FIRA_OWR_DTM_CONTROL, TX_TIMESTAMP_TYPE)

#define FIRA_OWR_DTM_CONTROL_CFO_PRESENT_WIDTH 0x1u
#define FIRA_OWR_DTM_CONTROL_CFO_PRESENT_SHIFT 14u
#define FIRA_OWR_DTM_CONTROL_CFO_PRESENT_MASK GET_MASK(FIRA_OWR_DTM_CONTROL, CFO_PRESENT)

#define FIRA_OWR_DTM_CONTROL_TOF_PRESENT_WIDTH 0x1u
#define FIRA_OWR_DTM_CONTROL_TOF_PRESENT_SHIFT 15u
#define FIRA_OWR_DTM_CONTROL_TOF_PRESENT_MASK GET_MASK(FIRA_OWR_DTM_CONTROL, TOF_PRESENT)

#define FIRA_OWR_DTM_CONTROL_HOP_COUNT_PRESENT_WIDTH 0x1u
#define FIRA_OWR_DTM_CONTROL_HOP_COUNT_PRESENT_SHIFT 16u
#define FIRA_OWR_DTM_CONTROL_HOP_COUNT_PRESENT_MASK \
	GET_MASK(FIRA_OWR_DTM_CONTROL, HOP_COUNT_PRESENT)

#define FIRA_OWR_DTM_CONTROL_ANCHOR_LOCATION_PRESENT_WIDTH 0x1u
#define FIRA_OWR_DTM_CONTROL_ANCHOR_LOCATION_PRESENT_SHIFT 17u
#define FIRA_OWR_DTM_CONTROL_ANCHOR_LOCATION_PRESENT_MASK \
	GET_MASK(FIRA_OWR_DTM_CONTROL, ANCHOR_LOCATION_PRESENT)

#define FIRA_OWR_DTM_CONTROL_ACTIVE_RR_INFO_PRESENT_WIDTH 0x1u
#define FIRA_OWR_DTM_CONTROL_ACTIVE_RR_INFO_PRESENT_SHIFT 18u
#define FIRA_OWR_DTM_CONTROL_ACTIVE_RR_INFO_PRESENT_MASK \
	GET_MASK(FIRA_OWR_DTM_CONTROL, ACTIVE_RR_INFO_PRESENT)

/**
 * FIRA_OWR_DTM_CONTROL_PREP() - Build field in bitmap.
 * @_x_: Is the discriminant part of the name of bitmask.
 * @_v_: Is the value of the field.
 */
#define FIRA_OWR_DTM_CONTROL_PREP(_x_, _v_) QFIELD_PREP(FIRA_OWR_DTM_CONTROL_##_x_##_MASK, (_v_))

/**
 * FIRA_OWR_DTM_CONTROL_GET() - Extract a field from a bitmap.
 * @_x_: Is the discriminant part of the name of bitmask.
 * @_v_: Is the value of the bitmap.
 */
#define FIRA_OWR_DTM_CONTROL_GET(_x_, _v_) QFIELD_GET(FIRA_OWR_DTM_CONTROL_##_x_##_MASK, _v_)

/**
 * struct fira_owr_dt_frame_tx_timestamp -- Anchor TimeStamp description
 * @type: Local (0) or COMMON (1) if emiting anchor is synchronized.
 * @len: Tx Timestamp length (40 or 64 bits).
 * @rctu: Ranging Counter Time Unit value (802.15.4z-2020 6.9.1.4).
 */
struct fira_owr_dt_frame_tx_timestamp {
	enum fira_owr_dtm_timestamp_type type;
	enum fira_owr_dtm_timestamp_len len;
	uint64_t rctu;
};

/**
 * struct fira_owr_dt_frame_active_rr_info -- Active Ranging Round Information
 * @len: The total number of ranging round.
 * @index: The total number of ranging round indexes.
 */
struct fira_owr_dt_frame_active_rr_info {
	uint8_t len;
	uint8_t *index;
};

/**
 * struct fira_owr_dt_frame_mgmt_entry -- Response request parameters.
 * @slot_index: Assigned ranging slot in which Initiator will wait for the
 * response.
 * @tof_result_rctu: Result of SS-TWR with the Responder by Initiator in the
 * previous ranging block.
 * @addr: Address of requested Responder.
 */
struct fira_owr_dt_frame_mgmt_entry {
	uint8_t slot_index;
	uint16_t tof_result_rctu;
	mcps802154_device_address addr;
};

/**
 * struct fira_owr_dt_frame_poll -- FiRa Down Link TDoA Message Data.
 * @extended_addresses: Use extended addresses.
 * @mgmt_slot_index_present: Signals Ranging Slot Index in Management List.
 * @final_dtm_present: Indicates if Initiator will transmit Final DTM.
 * In other word, indicates Double Sided ranging between anchors.
 * @hop_count_present: True if Hop Count is present.
 * @active_rr_info_present: True if Active Ranging Round Information is present.
 * @mgmt_list_len: Management List length.
 * @hop_count: Hop Count between Reference Initiator and current Initiator.
 * @last_slot_index: Slot Index of the last responder.
 * @round_index: Round Index of the current ranging round.
 * @block_index: Block Index of the current ranging block.
 * @tx_timestamp:  Anchor TimeStamp description.
 * @mgmt_list: Management List.
 * @anchor_location_type: Anchor Location Coordinate System
 * (0: WGS-84, 1: relative, 0xff: invalid - no location)
 * @anchor_location: Anchor location (if present, Poll/Response DTM).
 * @active_rr_info: Active Ranging Rounds Info List (if present).
 */
struct fira_owr_dt_frame_poll {
	uint8_t extended_addresses : 1;
	uint8_t mgmt_slot_index_present : 1;
	uint8_t final_dtm_present : 1;
	uint8_t hop_count_present : 1;
	uint8_t active_rr_info_present : 1;
	uint8_t mgmt_list_len;
	uint8_t hop_count;
	uint8_t last_slot_index;
	uint8_t round_index;
	uint16_t block_index;
	struct fira_owr_dt_frame_tx_timestamp tx_timestamp;
	struct fira_owr_dt_frame_mgmt_entry mgmt_list[FIRA_RESPONDERS_MAX];
	enum fira_dt_location_coord_system_type anchor_location_type;
	uint8_t anchor_location[FIRA_OWR_DTM_LOCATION_SIZE_MAX];
	struct fira_owr_dt_frame_active_rr_info active_rr_info;
};

/*
 * FIRA_OWR_DTM_POLL_PAYLOAD_LEN() - Compute the POLL frame byte size
 * from fields of input struct.
 * @_s_: The input parameter struct.
 */
#define FIRA_OWR_DTM_POLL_PAYLOAD_LEN(_s_)                                      \
	(FIRA_OWR_DTM_MESSAGE_HEADER_LEN + FIRA_OWR_DTM_TX_TIMESTAMP_LEN(_s_) + \
	 FIRA_OWR_DTM_MGMT_LIST_LEN(_s_) + FIRA_OWR_DTM_HOP_COUNT_LEN(_s_) +    \
	 FIRA_OWR_DTM_PAYLOAD_ANCHOR_LOCATION_LEN(_s_) +                        \
	 FIRA_OWR_DTM_PAYLOAD_ACTIVE_RR_INFO_LEN(_s_))

/**
 * struct fira_owr_dt_frame_response -- FiRa Down Link TDoA Message Data.
 * @extended_addresses: Use extended addresses if true, else false.
 * @final_dtm_present: Indicates if Initiator will transmit Final DTM.
 * @cfo_present: True if Responder CFO is present.
 * @tof_present: True if Responder ToF is present.
 * @active_rr_info_present: True if Active Ranging Round Information is present.
 * @round_index: Round Index of the current ranging round.
 * @block_index: Block Index of the current ranging block.
 * @cfo_ppm_q10: Responder Clock Frequency Offset with respect to the Initiator
 * in the unit of ppm. This value shall be reported in Q6.10 format, which gives
 * a resolution of approximately 0.0010ppm and a range of [-32ppm, 32ppm).
 * @tof_rctu: Time Of Flight value between the Initiator and the Responder
 * measured by the Responder based on DS-TWR in the previous ranging block.
 * @reply_time_rctu: Elapsed time between the reception of the Poll DTM and the
 * transmission of the Response DTM..
 * @tx_timestamp:  Anchor TimeStamp description.
 * @anchor_location_type: Anchor Location Coordinate System
 * (0: WGS-84, 1: relative, 0xff: invalid - no location)
 * @anchor_location: Anchor location (if present, Poll/Response DTM).
 * @active_rr_info: Active Ranging Rounds Info List (if present).
 */
struct fira_owr_dt_frame_response {
	uint8_t extended_addresses : 1;
	uint8_t final_dtm_present : 1;
	uint8_t cfo_present : 1;
	uint8_t tof_present : 1;
	uint8_t active_rr_info_present : 1;
	uint8_t round_index;
	uint16_t block_index;
	int16_t cfo_ppm_q10;
	uint16_t tof_rctu;
	uint32_t reply_time_rctu;
	struct fira_owr_dt_frame_tx_timestamp tx_timestamp;
	enum fira_dt_location_coord_system_type anchor_location_type;
	uint8_t anchor_location[FIRA_OWR_DTM_LOCATION_SIZE_MAX];
	struct fira_owr_dt_frame_active_rr_info active_rr_info;
};

/*
 * FIRA_OWR_DTM_RESPONSE_PAYLOAD_LEN() - Compute the RESPONSE frame byte size
 * from fields of input struct.
 * @_s_: The input parameter struct.
 */
#define FIRA_OWR_DTM_RESPONSE_PAYLOAD_LEN(_s_)                                                   \
	(FIRA_OWR_DTM_MESSAGE_HEADER_LEN + FIRA_OWR_DTM_TX_TIMESTAMP_LEN(_s_) +                  \
	 FIRA_OWR_DTM_RESP_CFO_LEN(_s_) + FIRA_OWR_DTM_RESP_REPLY_TIME_LEN +                     \
	 FIRA_OWR_DTM_RESP_TOF_RESULT_LEN(_s_) + FIRA_OWR_DTM_PAYLOAD_ANCHOR_LOCATION_LEN(_s_) + \
	 FIRA_OWR_DTM_PAYLOAD_ACTIVE_RR_INFO_LEN(_s_))

/**
 * struct fira_owr_dt_frame_reply_time -- Time of response measured by
 * Initiator.
 * @addr: Address of Responder.
 * @rctu: Time difference between the reception of the Response DTM and
 * the transmission of the Final DTM.
 */
struct fira_owr_dt_frame_reply_time {
	mcps802154_device_address addr;
	uint32_t rctu;
};

/**
 * struct fira_owr_dt_frame_final -- FiRa Down Link TDoA Message Data.
 * @extended_addresses: Use extended addresses if true, else false.
 * @reply_time_list_len: Reply Time List length (if present).
 * @round_index: Round Index of the current ranging round.
 * @block_index: Block Index of the current ranging block.
 * @tx_timestamp:  Anchor TimeStamp description.
 * @reply_time_list: Reply Time List.
 */
struct fira_owr_dt_frame_final {
	uint8_t extended_addresses : 1;
	uint8_t reply_time_list_len;
	uint8_t round_index;
	uint16_t block_index;
	struct fira_owr_dt_frame_tx_timestamp tx_timestamp;
	struct fira_owr_dt_frame_reply_time reply_time_list[FIRA_RESPONDERS_MAX];
};

/*
 * FIRA_OWR_DTM_FINAL_PAYLOAD_LEN() - Compute the FINAL frame byte size
 * from fields of input struct.
 * @_s_: The input parameter struct.
 */
#define FIRA_OWR_DTM_FINAL_PAYLOAD_LEN(_s_)                                     \
	(FIRA_OWR_DTM_MESSAGE_HEADER_LEN + FIRA_OWR_DTM_TX_TIMESTAMP_LEN(_s_) + \
	 FIRA_OWR_DTM_REPLY_TIME_LIST_LEN(_s_))

/**
 * fira_owr_dt_frame_put_mac_header() - Write content of MAC header.
 * @ws: FiRa workspace context.
 * @skb: The input frame buffer.
 */
void fira_owr_dt_frame_put_mac_header(const struct fira_workspace *ws, struct sk_buff *skb);

/**
 * fira_owr_dt_frame_pull_mac_header() - Read and check content of MAC header.
 * @skb: The input frame buffer.
 * @src_addr: Buffer to retrieve address of sender.
 * @ie: Buffer to get the Header IE of type Termination 1 which has to be
 * present.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_dt_frame_pull_mac_header(struct sk_buff *skb,
							mcps802154_device_address *src_addr,
							struct mcps802154_ie *ie);

/**
 * fira_owr_dt_frame_validate_mgmt_list() - Validates the management list.
 * @len: Length of the management list.
 * @mgmt_list: Management list.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status
fira_owr_dt_frame_validate_mgmt_list(uint8_t len, struct fira_owr_dt_frame_mgmt_entry *mgmt_list);
/**
 * fira_owr_dt_frame_poll_put() - Fill FiRa frame for a One Way Ranging
 * Poll DL-TDoA Message (Poll DTM).
 * @dt_poll: Incoming parameters.
 * @skb: Frame buffer.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_dt_frame_poll_put(const struct fira_owr_dt_frame_poll *dt_poll,
						 struct sk_buff *skb);

/**
 * fira_owr_dt_frame_response_put() - Fill FiRa frame for a One Way Ranging
 * Response DL-TDoA Message (Response DTM).
 * @dt_response: Incoming parameters.
 * @skb: Frame buffer.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status
fira_owr_dt_frame_response_put(const struct fira_owr_dt_frame_response *dt_response,
			       struct sk_buff *skb);

/**
 * fira_owr_dt_frame_final_put() - Fill FiRa frame for a One Way Ranging
 * Final DL-TDoA Message (Final DTM).
 * @dt_final: Incoming parameters.
 * @skb: Frame buffer.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_dt_frame_final_put(const struct fira_owr_dt_frame_final *dt_final,
						  struct sk_buff *skb);

/**
 * fira_owr_dt_frame_poll_check() - Check OWR Message Type-dependent
 * in One Way Ranging Poll DL-TDoA Message (Poll DTM).
 * @ws: Fira workspace context.
 * @filtered: Filtered mode for DT-Anchor.
 * @skb: Incoming Frame buffer.
 * @ie: Context for IE decoding.
 * @dt_poll: Outgoing parameters.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_dt_frame_poll_check(const struct fira_workspace *ws, bool filtered,
						   struct sk_buff *skb, struct mcps802154_ie *ie,
						   struct fira_owr_dt_frame_poll *dt_poll);

/**
 * fira_owr_dt_frame_response_check() - Check OWR Message Type-dependent
 * in One Way Ranging Response DL-TDoA Message (Response DTM).
 * @skb: Incoming Frame buffer.
 * @ie: Context for IE decoding.
 * @dt_response: Outgoing parameters.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status
fira_owr_dt_frame_response_check(struct sk_buff *skb, struct mcps802154_ie *ie,
				 struct fira_owr_dt_frame_response *dt_response);

/**
 * fira_owr_dt_frame_final_check() - Check OWR Message Type-dependent
 * in One Way Ranging Final DL-TDoA Message (Final DTM).
 * @ws: Fira workspace context.
 * @filtered: Filtered mode for DT-Anchor.
 * @skb: Incoming Frame buffer.
 * @ie: Context for IE decoding.
 * @dt_final: Outgoing parameters.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_dt_frame_final_check(const struct fira_workspace *ws, bool filtered,
						    struct sk_buff *skb, struct mcps802154_ie *ie,
						    struct fira_owr_dt_frame_final *dt_final);
