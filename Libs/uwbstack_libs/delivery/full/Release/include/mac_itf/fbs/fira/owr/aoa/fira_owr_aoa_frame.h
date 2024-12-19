/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fira/fira_frame_defines.h"
#include "net/af_ieee802154.h"
#include "net/mcps802154.h"
#include "net/mcps802154_frame.h"
#include "qerr.h"
#include "quwbs/fbs/defs.h"

struct fira_session;
struct fira_slot;
struct fira_data_payload_segment;
struct sk_buff;

#define FIRA_OWR_AOA_MAC_FRAME_FC_VALUE                                                          \
	IEEE802154_FC_MP_VALUE_LONG(IEEE802154_ADDR_SHORT, IEEE802154_ADDR_SHORT, 0, 1, 0, 0, 0, \
				    0, 1)

#define FIRA_OWR_AOA_MAC_FRAME_HEADER_LEN                                         \
	(IEEE802154_FC_LEN + IEEE802154_SHORT_ADDR_LEN * 2 + IEEE802154_SCF_LEN + \
	 IEEE802154_SEQ_LEN)
#define FIRA_OWR_AOA_MAC_FRAME_FULL_LEN                                              \
	(IEEE802154_FC_LEN + IEEE802154_SEQ_LEN + IEEE802154_SHORT_ADDR_LEN +        \
	 IEEE802154_SHORT_ADDR_LEN + IEEE802154_SCF_LEN + IEEE802154_IE_HEADER_LEN + \
	 FIRA_IE_PAYLOAD_OWR_AOA_MEASUREMENT_LEN + IEEE802154_IE_HEADER_LEN + IEEE802154_FCS_LEN)

#define FIRA_IE_PAYLOAD_OWR_COMMON_LEN (FIRA_IE_VENDOR_OUI_LEN + 1)
#define FIRA_IE_PAYLOAD_OWR_AOA_TYPE_DEPENDENT_LEN 6
#define FIRA_IE_PAYLOAD_OWR_AOA_MEASUREMENT_LEN \
	(FIRA_IE_PAYLOAD_OWR_COMMON_LEN + FIRA_IE_PAYLOAD_OWR_AOA_TYPE_DEPENDENT_LEN)
#define FIRA_IE_PAYLOAD_DATA_SIZE 2

#define FIRA_OWR_AOA_MEASUREMENT_MESSAGE_NUM_OF_RFRAMES_MASK (0xf << 0)

/**
 * fira_owr_aoa_frame_header_put() - Fill MAC frame header for OWR for AoA Message.
 * @session: FiRa session context.
 * @skb: Frame buffer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_owr_aoa_frame_header_put(struct fira_session *session, struct sk_buff *skb);

/**
 * fira_owr_aoa_frame_header_check() - Parse MAC frame header for OWR for AoA Message.
 * @session: FiRa session context.
 * @skb: Frame buffer.
 * @src_addr: Pointer to where the source address will be stored.
 * @ie: Context for IE decoding.
 *
 * Return: FiRa status code.
 */
enum quwbs_fbs_status fira_owr_aoa_frame_header_check(struct fira_session *session,
						      struct sk_buff *skb,
						      mcps802154_device_address *src_addr,
						      struct mcps802154_ie *ie);

/**
 * fira_owr_aoa_frame_header_check_decrypt() - Check and consume MAC frame
 * header and decrypt the FiRa payload.
 * @session: FiRa session context.
 * @slot: Corresponding slot.
 * @skb: Frame buffer.
 * @src_address: Current frame source device address.
 * @ie: Context for IE decoding.
 *
 * Return: FiRa status code.
 */
enum quwbs_fbs_status
fira_owr_aoa_frame_header_check_decrypt(struct fira_session *session, const struct fira_slot *slot,
					struct sk_buff *skb, mcps802154_device_address *src_address,
					struct mcps802154_ie *ie);

/**
 * fira_owr_aoa_frame_measurement_put() - Fill FiRa frame payload for
 * OWR for AoA measurement message.
 * @session: FiRa session context.
 * @skb: Frame buffer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_owr_aoa_frame_measurement_put(const struct fira_session *session,
					     struct sk_buff *skb);

/**
 * fira_owr_aoa_frame_payload_check() - Check and consume all the payloads from
 * OWR for AoA Message (AoA Measurement Message, Data Message).
 * @session: FiRa session context.
 * @skb: Frame buffer.
 * @ie: Context for IE decoding.
 * @segment: Pointer to where the current data segment will be stored.
 *
 * Return: FiRa status code.
 */
enum quwbs_fbs_status fira_owr_aoa_frame_payload_check(struct fira_session *session,
						       struct sk_buff *skb,
						       struct mcps802154_ie *ie,
						       struct fira_data_payload_segment **segment);
