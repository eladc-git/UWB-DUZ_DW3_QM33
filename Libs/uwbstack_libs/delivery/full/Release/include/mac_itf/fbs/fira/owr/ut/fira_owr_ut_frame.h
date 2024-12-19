/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fira/fira_frame_defines.h"
#include "net/af_ieee802154.h"
#include "net/fira_region_params.h"
#include "net/mcps802154_frame.h"
#include "qbitfield.h"
#include "quwbs/fbs/defs.h"

#include <stdint.h>

struct fira_owr_ut_frame;
struct fira_owr_ut_frame_device_id;
struct fira_owr_ut_frame_tx_timestamp;
struct sk_buff;

/**
 * FIRA_OWR_UTM_MAC_FRAME_FC_VALUE - Definitions from FiRa v2.0 6.3.7.1.7.
 * address type short.
 * lfc=1, dam=none, sam=Short, panid=0, secen=1,
 * seq=1, frpen=0, ver=0, ack=0, ie=1
 */
#define FIRA_OWR_UTM_MAC_FRAME_FC_VALUE                                                            \
	IEEE802154_FC_MP_VALUE_LONG(IEEE802154_ADDR_NONE, IEEE802154_ADDR_SHORT, 0, 1, 1, 0, 0, 0, \
				    1)
/**
 * FIRA_OWR_UTM_MAC_FRAME_HEADER_LEN - MAC header is composed of:
 * - Normal size Multipurpose Frame Control Field
 * - Source address.
 * - Security Control Field 802.15.4 v2.0 9.4.2.
 */
#define FIRA_OWR_UTM_MAC_FRAME_HEADER_LEN \
	(IEEE802154_FC_LEN + IEEE802154_SHORT_ADDR_LEN + IEEE802154_SCF_LEN)

/**
 * FIRA_OWR_UTM_MESSAGE_ID_TYPE_VALUE() - Definitions from FiRa v2.0 5.9.13.
 * @_t_: is the UL-TDoA Message Type.
 */
#define FIRA_OWR_UTM_MESSAGE_ID_TYPE_VALUE(_t_)                                       \
	(QFIELD_PREP(FIRA_MESSAGE_UWB_MSG_ID_MASK, FIRA_MESSAGE_ID_ONE_WAY_RANGING) | \
	 QFIELD_PREP(FIRA_MESSAGE_OWR_MSG_TYPE_MASK, _t_))

/* Definitions from FiRa v2.0 5.9.13.1 Uplink Tdoa Message (UTM). */

#define FIRA_OWR_UTM_MESSAGE_CONTROL_LEN 1
#define FIRA_OWR_UTM_FRAME_NUMBER_LEN 4

#define FIRA_OWR_UTM_MESSAGE_CONTROL_LEN_MASK 0x1
#define FIRA_OWR_UTM_FRAME_NUMBER_LEN_MASK 0xf

#define FIRA_OWR_UTM_MESSAGE_HEADER_LEN \
	(FIRA_IE_VENDOR_OUI_LEN + FIRA_MESSAGE_UWB_MSG_ID_LEN + FIRA_OWR_UTM_MESSAGE_CONTROL_LEN)

#define FIRA_OWR_UTM_TX_TIMESTAMP_LEN(_s_)                                   \
	(((_s_).tx_timestamp.len == FIRA_OWR_UTM_TIMESTAMP_NOT_PRESENT ? 0 : \
	  (_s_).tx_timestamp.len == FIRA_OWR_UTM_TIMESTAMP_40BITS      ? 5 : \
									 8))

#define FIRA_OWR_UTM_DEVICE_ID_LEN(_s_) \
	(((_s_).device_id.len == FIRA_OWR_UTM_DEVICE_ID_64BITS ? 8 : (_s_).device_id.len * 2))

#define FIRA_OWR_UTM_DEVICE_ID_LEN_MASK 0x3
#define FIRA_OWR_UTM_TX_TIMESTAMP_LEN_MASK 0x3
#define FIRA_OWR_UTM_CTRL_RFU_MASK 0xf

#define FIRA_OWR_UTM_CONTROL_DEVICE_ID_LEN_MASK (FIRA_OWR_UTM_DEVICE_ID_LEN_MASK << 0)
#define FIRA_OWR_UTM_CONTROL_TX_TIMESTAMP_LEN_MASK (FIRA_OWR_UTM_TX_TIMESTAMP_LEN_MASK << 2)

/**
 * FIRA_OWR_UTM_CONTROL_PREP() - Build field in bitmap.
 * @_x_: is the discriminant part of the name of bitmask.
 * @_v_: is the value of the field.
 */
#define FIRA_OWR_UTM_CONTROL_PREP(_x_, _v_) QFIELD_PREP(FIRA_OWR_UTM_CONTROL_##_x_##_MASK, (_v_))

/**
 * FIRA_OWR_UTM_CONTROL_GET() - Extract a field from a bitmap.
 * @_x_: is the discriminant part of the name of bitmask.
 * @_v_: is the value of the bitmap.
 */
#define FIRA_OWR_UTM_CONTROL_GET(_x_, _v_) QFIELD_GET(FIRA_OWR_UTM_CONTROL_##_x_##_MASK, _v_)

/*
 * FIRA_OWR_UTM_PAYLOAD_LEN() - Compute the BLINK or SYNC frame byte size
 * from fields of input struct.
 * @_s_: the input parameter struct.
 */
#define FIRA_OWR_UTM_PAYLOAD_LEN(_s_)                                      \
	(FIRA_OWR_UTM_MESSAGE_HEADER_LEN + FIRA_OWR_UTM_FRAME_NUMBER_LEN + \
	 FIRA_OWR_UTM_DEVICE_ID_LEN(_s_) + FIRA_OWR_UTM_TX_TIMESTAMP_LEN(_s_))

/**
 * struct fira_owr_ut_frame_tx_timestamp - Tx Timestamp description
 * @len: Tx Timestamp length (0, 40 or 64 bits).
 * @rctu: Tx Timestamp value.
 */
struct fira_owr_ut_frame_tx_timestamp {
	enum fira_owr_utm_timestamp_len len;
	uint64_t rctu;
};

/**
 * struct fira_owr_ut_frame_device_id - Device ID description
 * @len: Device ID length (0, 16, 32 or 64 bits).
 * @value: Device ID value.
 */
struct fira_owr_ut_frame_device_id {
	enum fira_owr_utm_device_id_len len;
	uint64_t value;
};

/**
 * struct fira_owr_ut_frame - FiRa UL-TDoA Message Data.
 * @frame_number: UTM frame number.
 * @device_id: Device ID.
 * @tx_timestamp: Tx Timestamp.
 */
struct fira_owr_ut_frame {
	uint32_t frame_number;
	struct fira_owr_ut_frame_device_id device_id;
	struct fira_owr_ut_frame_tx_timestamp tx_timestamp;
};

/**
 * fira_owr_ut_frame_blink_payload_put() - Fill FiRa frame payload for
 * a One Way Ranging Blink UL-TDoA Message (Blink UTM).
 * @ut_blink: incoming parameters.
 * @src_short_addr: Source short address.
 * @skb: Frame buffer.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_ut_frame_blink_payload_put(const struct fira_owr_ut_frame *ut_blink,
							  uint16_t src_short_addr,
							  struct sk_buff *skb);

/**
 * fira_owr_ut_frame_sync_payload_put() - Fill FiRa frame payload for
 * a One Way Ranging Sync UL-TDoA Message (Synchronization UTM).
 * @ut_sync: incoming parameters.
 * @src_short_addr: Source short address.
 * @skb: Frame buffer.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_ut_frame_sync_payload_put(const struct fira_owr_ut_frame *ut_sync,
							 uint16_t src_short_addr,
							 struct sk_buff *skb);

/**
 * fira_owr_ut_frame_blink_payload_check() - Check OWR Message Type-dependent
 * Payload in One Way Ranging Blink UL-TDoA Message (Blink UTM).
 * @skb: Incoming Frame buffer.
 * @ut_blink: Outgoing parameters.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_ut_frame_blink_payload_check(struct sk_buff *skb,
							    struct fira_owr_ut_frame *ut_blink);

/**
 * fira_owr_ut_frame_sync_payload_check() - Check OWR Message Type-dependent
 * Payload in One Way Ranging Sync UL-TDoA Message (Synchronization UTM).
 * @skb: Incoming Frame buffer.
 * @ut_sync: Outgoing parameters.
 *
 * Return: Status code.
 */
enum quwbs_fbs_status fira_owr_ut_frame_sync_payload_check(struct sk_buff *skb,
							   struct fira_owr_ut_frame *ut_sync);
