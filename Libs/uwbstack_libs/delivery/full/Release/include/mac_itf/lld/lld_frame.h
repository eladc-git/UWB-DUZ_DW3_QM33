/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#include "lld/lld_common.h"
#include "net/af_ieee802154.h"
#include "net/lld_region_common_nl.h"

#include <stdbool.h>
#include <stdint.h>

#pragma once

#define LLD_FRAME_IE_VENDOR_OUI 0xc8b1ee
#define LLD_FRAME_IE_VENDOR_OUI_LEN 3

#define LLD_FRAME_DISPATCH 0x2c
#define LLD_FRAME_DISPATCH_LEN 1

enum lld_frame_type {
	LLD_FRAME_TYPE_DL_MAIN = 0,
	LLD_FRAME_TYPE_DL_AUX = 1,
	LLD_FRAME_TYPE_UL = 3,
	LLD_FRAME_TYPE_BEACON = 4,
};
#define LLD_FRAME_TYPE_LEN 1

enum lld_next_frame_type {
	LLD_NEXT_FRAME_TYPE_NOTHING = 0,
	LLD_NEXT_FRAME_TYPE_AUX = 1,
	LLD_NEXT_FRAME_TYPE_DATA_BURST = 2,
	LLD_NEXT_FRAME_TYPE_NON_ASSOCIATED = 3,
};

enum lld_frame_ie_id {
	LLD_FRAME_IE_ID_INACTIVE_DEVICE = 0x02,
	LLD_FRAME_IE_ID_BLOCK_SKIP_ADVERTISEMENT = 0x03,
	LLD_FRAME_IE_ID_NON_ASSOCIATED_ACK = 0x22,
	LLD_FRAME_IE_ID_BLOCK_STRUCTURE_CHANGE = 0x23,
	LLD_FRAME_IE_ID_RANGING_CONTROL = 0x24,
	LLD_FRAME_IE_ID_RANGING_MEASUREMENT = 0x25,
};

enum lld_association_response_status {
	LLD_ASSOCIATION_RESPONSE_STATUS_OK = 0,
	LLD_ASSOCIATION_RESPONSE_STATUS_PAN_AT_CAPACITY = 1,
	LLD_ASSOCIATION_RESPONSE_STATUS_PAN_ACCESS_DENIED = 2,
};

#define LLD_FRAME_DL_SYNC_TIMESTAMP_LEN 4

#define LLD_FRAME_DL_IE_HEADER_LEN_MIN \
	LLD_FRAME_DL_IE_HEADER_LEN(0, LLD_NEXT_FRAME_TYPE_AUX, false, 0)
#define LLD_FRAME_DL_IE_HEADER_LEN_MAX \
	LLD_FRAME_DL_IE_HEADER_LEN(LLD_DEVICES_MAX, LLD_NEXT_FRAME_TYPE_AUX, true, 0)
#define LLD_FRAME_DL_IE_HEADER_LEN(n_ul_slots, next_frame_type, next_sb_combined_with_cap, \
				   nested_ie_len)                                          \
	(LLD_FRAME_IE_VENDOR_OUI_LEN + LLD_FRAME_DISPATCH_LEN + LLD_FRAME_TYPE_LEN + 5 +   \
	 LLD_FRAME_DL_SYNC_TIMESTAMP_LEN + (((n_ul_slots) + 7) / 8) +                      \
	 ((next_frame_type) != LLD_NEXT_FRAME_TYPE_AUX ?                                   \
		  2 :                                                                      \
		  1 + ((next_sb_combined_with_cap) ? ((n_ul_slots) + 7) / 8 : 0)) +        \
	 nested_ie_len)
#define LLD_FRAME_UL_IE_HEADER_LEN(nested_ie_len)                                        \
	(LLD_FRAME_IE_VENDOR_OUI_LEN + LLD_FRAME_DISPATCH_LEN + LLD_FRAME_TYPE_LEN + 7 + \
	 (nested_ie_len))
#define LLD_FRAME_BEACON_IE_HEADER_LEN(cap_slots, network_params_len)                    \
	(LLD_FRAME_IE_VENDOR_OUI_LEN + LLD_FRAME_DISPATCH_LEN + LLD_FRAME_TYPE_LEN + 8 + \
	 ((cap_slots) ? 2 : 0) + (network_params_len))

#define LLD_FRAME_UL_STATE_SEL_ACK_0 0
#define LLD_FRAME_UL_STATE_SEL_ACK_1 8
#define LLD_FRAME_UL_STATE_SEL_ACK_2 16
#define LLD_FRAME_UL_STATE_SEL_ACK_3 24
#define LLD_FRAME_UL_STATE_SEL_ACK_4 32
#define LLD_FRAME_UL_STATE_INIT 7

#define LLD_FRAME_DL_NB_OF_UL_SLOTS (0x1fu << 0)
#define LLD_FRAME_DL_NEXT_FRAME_TYPE (0x7u << 0)
#define LLD_FRAME_DL_NEXT_FRAME_REPETITION (0x7u << 3)
#define LLD_FRAME_DL_NEXT_FRAME_IFS (0x3ffu << 6)
#define LLD_FRAME_DL_NEXT_SB_COMBINED_WITH_CAP (0x1u << 3)
#define LLD_FRAME_DL_NEXT_SB_DS_TWR_FINAL (0x1u << 4)
#define LLD_FRAME_DL_NEXT_SB_RESERVED (0x7u << 5)

#define LLD_FRAME_BEACON_N_CAP_SLOTS (0x7u << 0)
#define LLD_FRAME_BEACON_PAIRING_ALLOWED (0x1u << 3)

#define LLD_FRAME_BEACON_CAP_SLOT_IFS_8RSTU (0x3ffu << 0)

#define LLD_FRAME_BEACON_NETWORK_PARAMETERS_SIZE 17
#define LLD_FRAME_BEACON_PHY_CHANNEL (0x1u << 0)
#define LLD_FRAME_BEACON_PHY_PREAMBLE_CODE (0x1fu << 1)
#define LLD_FRAME_BEACON_PHY_SFD_SELECTOR (0x3u << 6)
#define LLD_FRAME_BEACON_PHY_PSR (0x7u << 8)
#define LLD_FRAME_BEACON_PHY_DATA_RATE (0x7u << 11)
#define LLD_FRAME_BEACON_PHY_PSDU_SIZE_OR_PHY_PHR_HI_RATE (0x3u << 14)
#define LLD_FRAME_BEACON_SB_UL_SLOT_8RSTU (0x3ffull << 0)
#define LLD_FRAME_BEACON_SB_UL_SLOT_LAST_8RSTU (0x3ffull << 10)
#define LLD_FRAME_BEACON_SB_DL_SLOT_8RSTU (0x3ffull << 20)
#define LLD_FRAME_BEACON_SB_MIN_8RSTU (0x3ffull << 30)
#define LLD_FRAME_BEACON_SB_CAP_SLOT_8RSTU (0x3ffull << 40)
#define LLD_FRAME_BEACON_BURST_IFS (0x7ffull << 50)
#define LLD_FRAME_BEACON_FAST_DS_TWR (1ull << 61)

#define LLD_FRAME_BEACON_CAP_MIN_BE (0xfu << 0)
#define LLD_FRAME_BEACON_CAP_MAX_BE (0xfu << 4)
#define LLD_FRAME_BEACON_RESYNC_THRESHOLD_EXPONENT (0xfu << 0)

#define LLD_FRAME_SUBFRAME_HEADER_LEN 4

#define LLD_FRAME_IE_SHORT_FORMAT_HEADER_LEN 1
#define LLD_FRAME_IE_LONG_FORMAT_HEADER_LEN 2
#define LLD_FRAME_IE_SHORT_FORMAT_LEN_MINUS_ONE 0x3
#define LLD_FRAME_IE_ID (0x3f << 2)
#define LLD_FRAME_IE_ID_SHORT_FORMAT_MIN 0x02
#define LLD_FRAME_IE_ID_SHORT_FORMAT_MAX 0x1f
#define LLD_FRAME_IE_ID_LONG_FORMAT_MIN 0x22
#define LLD_FRAME_IE_ID_LONG_FORMAT_MAX 0x3f

#define LLD_FRAME_IE_BLOCK_SKIP_ADVERTISEMENT_LEN 3
#define LLD_FRAME_IE_BLOCK_SKIP_ADVERTISEMENT_OFFSET 0xfffu
#define LLD_FRAME_IE_BLOCK_SKIP_ADVERTISEMENT_DURATION (0xfffu << 12)

#define LLD_FRAME_IE_INACTIVE_DEVICE_LEN 1
#define LLD_FRAME_IE_INACTIVE_DEVICE_OFFSET 0xf

#define LLD_FRAME_IE_NON_ASSOCIATED_ACK_LEN 9

#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_LEN(n_ul_slots, network_params_len) \
	(6 + (n_ul_slots) + (network_params_len))
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_INDEX_OFFSET (0xfffu << 0)
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_NB_OF_UL_SLOTS (0x1fu << 0)
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_NET_PARAMS_SIZE 12
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_SB_AUX_MAX (0xffull << 0)
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_SB_UL_SLOT_8RSTU (0x3ffull << 8)
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_SB_UL_SLOT_LAST_8RSTU (0x3ffull << 18)
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_SB_DL_SLOT_8RSTU (0x3ffull << 28)
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_SB_MIN_8RSTU (0x3ffull << 38)
#define LLD_FRAME_IE_BLOCK_STRUCTURE_CHANGE_SB_CAP_SLOT_8RSTU (0x3ffull << 48)

#define LLD_FRAME_IE_HEADER_ASSOC_REQ_LEN                           \
	(LLD_FRAME_IE_VENDOR_OUI_LEN + LLD_FRAME_DISPATCH_LEN + 1 + \
	 LLD_DEVICE_EPHEMERAL_REQUEST_PUBLIC_KEY_LEN)
#define LLD_FRAME_IE_PAYLOAD_ASSOC_REQ_LEN(payload_len)             \
	(LLD_FRAME_IE_VENDOR_OUI_LEN + LLD_FRAME_DISPATCH_LEN + 2 + \
	 LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN + payload_len)

#define LLD_FRAME_IE_RANGING_CONTROL_LEN 2
#define LLD_FRAME_IE_RANGING_CONTROL_RANGING_EXPONENT (0xfu << 0)
#define LLD_FRAME_IE_RANGING_CONTROL_RANGING_REPORT_IMMEDIATE (0x1u << 4)
#define LLD_FRAME_IE_RANGING_CONTROL_RANGING_MULTI_NODE_MODE (0x3u << 5)
#define LLD_FRAME_IE_RANGING_CONTROL_RANGING_ROUND_USAGE (0x3u << 7)
#define LLD_FRAME_IE_RANGING_CONTROL_RANGING_STS_PACKET_CONFIG (0x3u << 9)
#define LLD_FRAME_IE_RANGING_CONTROL_RANGING_AOA_AZIMUTH (0x1u << 11)
#define LLD_FRAME_IE_RANGING_CONTROL_RANGING_AOA_ELEVATION (0x1u << 12)

#define LLD_FRAME_IE_RANGING_MEASUREMENT_LEN(n_devices) (2 + (n_devices)*10)
#define LLD_FRAME_IE_RANGING_MEASUREMENT_RANGING_EXPONENT (0xfu << 0)
#define LLD_FRAME_IE_RANGING_MEASUREMENT_REPLY_OR_ROUND_TRIP (0x1u << 4)
#define LLD_FRAME_IE_RANGING_MEASUREMENT_AOA_AZIMUTH (0x1u << 5)
#define LLD_FRAME_IE_RANGING_MEASUREMENT_AOA_ELEVATION (0x1u << 6)

#define LLD_FRAME_IE_ASSOC_REQ_RANGING_CAPABILITY_MANY_TO_MANY (0x1u << 0)
#define LLD_FRAME_IE_ASSOC_REQ_RANGING_CAPABILITY_DS_TWR (0x1u << 1)
#define LLD_FRAME_IE_ASSOC_REQ_RANGING_CAPABILITY_SECURED_SP1 (0x1u << 2)
#define LLD_FRAME_IE_ASSOC_REQ_RANGING_CAPABILITY_SECURED_SP3 (0x1u << 3)
#define LLD_FRAME_IE_ASSOC_REQ_RANGING_CAPABILITY_RESERVED (0xfu << 4)

#define LLD_FRAME_IEEE802154_BEACON_FC(ie)                                                        \
	IEEE802154_FC_VALUE(IEEE802154_FC_TYPE_BEACON, 0, 0, 0, 0, 1, (ie), IEEE802154_ADDR_NONE, \
			    2, IEEE802154_ADDR_SHORT)

#define LLD_FRAME_IEEE802154_DL_FC(ie)                                                           \
	IEEE802154_FC_VALUE(IEEE802154_FC_TYPE_DATA, 1, 0, 0, 0, 1, (ie), IEEE802154_ADDR_SHORT, \
			    2, IEEE802154_ADDR_NONE)

#define LLD_FRAME_IEEE802154_UL_FC(ie)                                                             \
	IEEE802154_FC_VALUE(IEEE802154_FC_TYPE_DATA, 1, 0, 0, 0, 1, (ie), IEEE802154_ADDR_NONE, 2, \
			    IEEE802154_ADDR_SHORT)

#define LLD_FRAME_IEEE802154_ASSOCIATION_REQ_FC                                                  \
	IEEE802154_FC_VALUE(IEEE802154_FC_TYPE_MAC_CMD, 1, 0, 0, 1, 0, 1, IEEE802154_ADDR_SHORT, \
			    2, IEEE802154_ADDR_LONG)
#define LLD_FRAME_IEEE802154_ASSOCIATION_REQ_LEN                                    \
	((IEEE802154_FC_LEN) + (IEEE802154_SEQ_LEN) + (IEEE802154_SHORT_ADDR_LEN) + \
	 (IEEE802154_PAN_ID_LEN) + (IEEE802154_EXTENDED_ADDR_LEN) + (IEEE802154_SCF_LEN))

#define LLD_FRAME_IEEE802154_ASSOCIATION_RESP_FC                                                   \
	IEEE802154_FC_VALUE(IEEE802154_FC_TYPE_MAC_CMD, 1, 0, 0, 0, 1, 0, IEEE802154_ADDR_LONG, 2, \
			    IEEE802154_ADDR_LONG)
#define LLD_FRAME_IEEE802154_ASSOCIATION_RESP_LEN                                         \
	((IEEE802154_FC_LEN) + (IEEE802154_EXTENDED_ADDR_LEN) + (IEEE802154_PAN_ID_LEN) + \
	 (IEEE802154_EXTENDED_ADDR_LEN) + (IEEE802154_SCF_LEN) + 2 + (IEEE802154_SHORT_ADDR_LEN))

struct sk_buff;
struct mcps802154_llhw;

/**
 * lld_frame_msdu_put() - Append an MSDU to a DL or UL frame.
 * @skb: DL or UL frame.
 * @msdu: MSDU to append.
 *
 * Return: QERR_SUCCESS or error.
 */
int lld_frame_msdu_put(struct sk_buff *skb, struct sk_buff *msdu);

/**
 * lld_frame_msdu_check() - Check and read an MSDU from a DL or UL frame.
 * @llhw: Low-level device pointer.
 * @skb: DL or UL frame.
 * @frame_mtu: Frame MTU.
 * @mfs_id: Where to store MFS identifier.
 * @seq: Where to store MSDU sequence number (TODO: could be stored in the MSDU
 * skb, may be in the control block).
 * @msdu: Where to store MSDU buffer pointer.
 *
 * Return: 1 if end of frame reached, 0 on successfully decoded MSDU, else
 * negative error code.
 */
int lld_frame_msdu_check(struct mcps802154_llhw *llhw, struct sk_buff *skb, int frame_mtu,
			 int *mfs_id, uint8_t *seq, struct sk_buff **msdu);

/**
 * lld_frame_get_ie_header_len() - Get header length of IE.
 * @id: identifier of IE.
 *
 * Return: Header length of IE.
 */
static inline int lld_frame_get_ie_header_len(enum lld_frame_ie_id id)
{
	if (id >= LLD_FRAME_IE_ID_SHORT_FORMAT_MIN && id <= LLD_FRAME_IE_ID_SHORT_FORMAT_MAX)
		return LLD_FRAME_IE_SHORT_FORMAT_HEADER_LEN;
	else if (id >= LLD_FRAME_IE_ID_LONG_FORMAT_MIN && id <= LLD_FRAME_IE_ID_LONG_FORMAT_MAX)
		return LLD_FRAME_IE_LONG_FORMAT_HEADER_LEN;
	return 0;
}

/**
 * lld_frame_ie_header_put() - Add a header IE.
 * @p: LLD frame.
 * @id: Identifier of IE.
 * @len: Length of payload.
 *
 * Return: Address of reserved space to write payload, or NULL in case of error.
 */
void *lld_frame_ie_header_put(uint8_t *p, enum lld_frame_ie_id id, int len);

/**
 * lld_frame_ie_header_check() - Check and read an IE header.
 * @p: LLD frame.
 * @id: Identifier of IE.
 * @remaining_len: Remaining length in the frame.
 * @len: Length of payload.
 *
 * Return: Address of payload, or NULL in case of error.
 */
void *lld_frame_ie_header_check(uint8_t *p, enum lld_frame_ie_id *id, int remaining_len, int *len);
