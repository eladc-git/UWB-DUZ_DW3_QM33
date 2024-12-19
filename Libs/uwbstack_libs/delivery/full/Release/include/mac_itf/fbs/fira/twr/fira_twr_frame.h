/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fira/fira_frame_defines.h"
#include "net/mcps802154.h"
#include "qerr.h"

#include <stdbool.h>
#include <stdint.h>

struct fira_session;
struct fira_slot;
struct sk_buff;
struct mcps802154_ie;

#define FIRA_IE_PAYLOAD_CONTROL_TYPE1_LEN(n_mngt) (FIRA_IE_VENDOR_OUI_LEN + 4 + 4 * (n_mngt))
#define FIRA_IE_PAYLOAD_CONTROL_TYPE2_LEN(rml_present, rml_size) \
	(FIRA_IE_VENDOR_OUI_LEN + 6 + 2 * (rml_present) + rml_size)
#define FIRA_IE_PAYLOAD_CONTROL_UPDATE_LEN (FIRA_IE_VENDOR_OUI_LEN + 3)
#define FIRA_IE_PAYLOAD_MEASUREMENT_REPORT_TYPE1_LEN(round_index_present, n_reply_time) \
	(FIRA_IE_VENDOR_OUI_LEN + 2 + 2 * (round_index_present) + 4 + 6 * (n_reply_time))
#define FIRA_IE_PAYLOAD_MEASUREMENT_REPORT_TYPE2_LEN(round_index_present, reply_time_present, \
						     n_reply_time)                            \
	(FIRA_IE_VENDOR_OUI_LEN + 3 + 2 * (round_index_present) + 4 * (reply_time_present) +  \
	 6 * (n_reply_time))
#define FIRA_IE_PAYLOAD_MEASUREMENT_REPORT_TYPE3_LEN(aoa_azimuth_present, aoa_elevation_present, \
						     aoa_fom_present, reply_time_present)        \
	(FIRA_IE_VENDOR_OUI_LEN + 2 + 4 * (reply_time_present) + 2 * (aoa_azimuth_present) +     \
	 2 * (aoa_elevation_present) +                                                           \
	 (aoa_fom_present) * (1 * (aoa_azimuth_present) + 1 * (aoa_elevation_present)))
#define FIRA_IE_PAYLOAD_RESULT_REPORT_LEN(tof_present, aoa_azimuth_present, aoa_elevation_present, \
					  aoa_fom_present, neg_tof_present)                        \
	(FIRA_IE_VENDOR_OUI_LEN + 2 + 4 * (tof_present) + 2 * (aoa_azimuth_present) +              \
	 2 * (aoa_elevation_present) +                                                             \
	 (aoa_fom_present) * (1 * (aoa_azimuth_present) + 1 * (aoa_elevation_present)) +           \
	 1 * (neg_tof_present))

#define FIRA_MNGT_RANGING_ROLE (1 << 0)
#define FIRA_MNGT_SLOT_INDEX (0xff << 1)
#define FIRA_MNGT_SHORT_ADDR (0xffff << 9)
#define FIRA_MNGT_MESSAGE_ID (0xf << 25)
#define FIRA_MNGT_STOP (1 << 29)
#define FIRA_MNGT_RESERVED (0x3U << 30)

#define FIRA_CONTROL_MESSAGE_TYPE2_CONTROL_CAP_SIZE (0xff << 0)
#define FIRA_CONTROL_MESSAGE_TYPE2_CONTROL_REPORT_PHASE (0x01 << 8)
#define FIRA_CONTROL_MESSAGE_TYPE2_CONTROL_NUM_TX_OFFSETS (0x03 << 10)
#define FIRA_CONTROL_MESSAGE_TYPE2_CONTROL_HOPPING_MODE (0x01 << 12)
#define FIRA_CONTROL_MESSAGE_TYPE2_CONTROL_STOP_RANGING (0x01 << 13)
#define FIRA_CONTROL_MESSAGE_TYPE2_CONTROL_RML_CONFIG (0x03 << 14)
#define FIRA_CONTROL_MESSAGE_TYPE2_CONTROL_ALLOWED_TX_OFFSETS (0xff << 16)

#define FIRA_CONTROL_MESSAGE_TYPE2_NUM_TX_OFFSETS_NONE (0x0)
#define FIRA_CONTROL_MESSAGE_TYPE2_NUM_TX_OFFSETS_2 (0x1)
#define FIRA_CONTROL_MESSAGE_TYPE2_NUM_TX_OFFSETS_4 (0x2)
#define FIRA_CONTROL_MESSAGE_TYPE2_NUM_TX_OFFSETS_8 (0x3)

#define FIRA_CONTROL_MESSAGE_TYPE2_RML_CONFIG_ABSENT (0x0)
#define FIRA_CONTROL_MESSAGE_TYPE2_RML_CONFIG_SHORT_ADDR (0x1)
#define FIRA_CONTROL_MESSAGE_TYPE2_RML_CONFIG_EXTENDED_ADDR (0x2)

#define FIRA_MEASUREMENT_REPORT_CONTROL_HOPPING_MODE (1 << 0)
#define FIRA_MEASUREMENT_REPORT_CONTROL_ROUND_INDEX_PRESENT (1 << 1)
#define FIRA_MEASUREMENT_REPORT_CONTROL_N_REPLY_TIME (0x3f << 2)

#define FIRA_MEASUREMENT_REPORT_2_CONTROL_REPLY_TIME_PRESENT (1 << 0)

#define FIRA_MEASUREMENT_REPORT_3_CONTROL_AOA_AZIMUTH_PRESENT (1 << 0)
#define FIRA_MEASUREMENT_REPORT_3_CONTROL_AOA_ELEVATION_PRESENT (1 << 1)
#define FIRA_MEASUREMENT_REPORT_3_CONTROL_REPLY_TIME_PRESENT (1 << 2)
#define FIRA_MEASUREMENT_REPORT_3_CONTROL_AOA_FOM_PRESENT (1 << 3)

#define FIRA_RESULT_REPORT_CONTROL_TOF_PRESENT (1 << 0)
#define FIRA_RESULT_REPORT_CONTROL_AOA_AZIMUTH_PRESENT (1 << 1)
#define FIRA_RESULT_REPORT_CONTROL_AOA_ELEVATION_PRESENT (1 << 2)
#define FIRA_RESULT_REPORT_CONTROL_AOA_FOM_PRESENT (1 << 3)
#define FIRA_RESULT_REPORT_CONTROL_NEG_TOF_PRESENT (1 << 4)

#define FIRA_CONTROL_UPDATE_MESSAGE_UWB_MSG_ID (0x0f << 0)
#define FIRA_CONTROL_UPDATE_MESSAGE_HOPPING_MODE (1 << 4)

/**
 * fira_twr_frame_control_update_payload_put() - Fill FiRa frame payload for a
 * control update message.
 * @session: FiRa session context.
 * @slot: Slot information.
 * @skb: Frame buffer.
 */
void fira_twr_frame_control_update_payload_put(const struct fira_session *session,
					       const struct fira_slot *slot, struct sk_buff *skb);

/**
 * fira_twr_frame_control_update_payload_check() - Check and consume FiRa frame
 * payload for a control update message.
 * @session: FiRa session context.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must have been used to read header first.
 * @src_address: Address from the sender of the current frame.
 *
 * Return: true if message is correct. Extra payload is accepted.
 */
bool fira_twr_frame_control_update_payload_check(struct fira_session *session, struct sk_buff *skb,
						 struct mcps802154_ie *ie,
						 mcps802154_device_address *src_address);

/**
 * fira_twr_frame_control_payload_put() - Fill FiRa frame payload for a control message.
 * @session: FiRa session context.
 * @slot: Slot information.
 * @skb: Frame buffer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_twr_frame_control_payload_put(const struct fira_session *session,
					     const struct fira_slot *slot, struct sk_buff *skb);

/**
 * fira_twr_frame_control_payload_check() - Check FiRa frame payload for a
 * control message.
 * @session: FiRa session context.
 * @slot: Slot associated to the frame to check.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must have been used to read header first.
 * @n_slots: Pointer where to store number of used slots.
 * @src_address: Address from the sender of the current frame.
 *
 * Return: true if message is correct. Extra payload is accepted.
 */
bool fira_twr_frame_control_payload_check(struct fira_session *session,
					  const struct fira_slot *slot, struct sk_buff *skb,
					  struct mcps802154_ie *ie, unsigned int *n_slots,
					  mcps802154_device_address *src_address);

/**
 * fira_twr_frame_measurement_report_payload_put() - Fill FiRa frame payload for
 * a measurement report message.
 * @session: FiRa session context.
 * @slot: Slot information.
 * @skb: Frame buffer.
 */
void fira_twr_frame_measurement_report_payload_put(const struct fira_session *session,
						   const struct fira_slot *slot,
						   struct sk_buff *skb);

/**
 * fira_twr_frame_measurement_report_payload_check() - Check FiRa frame payload
 * for a measurement report message.
 * @session: FiRa session context.
 * @slot: Slot information.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must have been used to read header first.
 * @src_address: Address from the sender of the current frame.
 *
 * Return: true if message is correct. Extra payload is accepted.
 */
bool fira_twr_frame_measurement_report_payload_check(struct fira_session *session,
						     const struct fira_slot *slot,
						     struct sk_buff *skb, struct mcps802154_ie *ie,
						     mcps802154_device_address *src_address);

/**
 * fira_twr_frame_result_report_payload_put() - Fill FiRa frame payload for a
 * result report message.
 * @session: FiRa session context.
 * @slot: Slot information.
 * @skb: Frame buffer.
 */
void fira_twr_frame_result_report_payload_put(const struct fira_session *session,
					      const struct fira_slot *slot, struct sk_buff *skb);

/**
 * fira_twr_frame_result_report_payload_check() - Check FiRa frame payload for
 * a result report message.
 * @session: FiRa session context.
 * @slot: Slot information.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must have been used to read header first.
 * @src_address: Short address from the sender of the current frame.
 *
 * Return: true if message is correct. Extra payload is accepted.
 */
bool fira_twr_frame_result_report_payload_check(struct fira_session *session,
						const struct fira_slot *slot, struct sk_buff *skb,
						struct mcps802154_ie *ie,
						mcps802154_device_address *src_address);

/**
 * fira_twr_frame_header_put() - Fill FiRa frame header for a Two Way Ranging (TWR) Message.
 * @session: FiRa session context.
 * @slot: Slot information.
 * @skb: Frame buffer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_twr_frame_header_put(const struct fira_session *session,
				    const struct fira_slot *slot, struct sk_buff *skb);

/**
 * fira_twr_frame_header_check() - Check and consume FiRa header for
 * Two Way Ranging (TWR) Message
 * @session: FiRa session context.
 * @slot: Slot information.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must be zero initialized.
 * @phy_sts_index: STS index read from header.
 * @session_id: Session id read from header.
 *
 * Return: true if header is correct.
 */
bool fira_twr_frame_header_check(struct fira_session *session, const struct fira_slot *slot,
				 struct sk_buff *skb, struct mcps802154_ie *ie,
				 uint32_t *phy_sts_index, uint32_t *session_id);

/**
 * fira_twr_frame_header_control_check() - Check control frame, consume
 * header and capture the session context.
 * @session: FiRa session context.
 * @slot: Corresponding slot.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must be zero initialized.
 * @phy_sts_index: STS index received.
 *
 * Return: true if the session context was captured successfully, false
 * otherwise.
 */
bool fira_twr_frame_header_control_check(struct fira_session *session, const struct fira_slot *slot,
					 struct sk_buff *skb, struct mcps802154_ie *ie,
					 uint32_t *phy_sts_index);

/**
 * fira_twr_frame_header_check_decrypt() - Check and consume header, and decrypt payload.
 * @session: FiRa session context.
 * @slot: Corresponding slot.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must be zero initialized.
 * @src_address: Address from the sender of the current frame.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_twr_frame_header_check_decrypt(struct fira_session *session,
					      const struct fira_slot *slot, struct sk_buff *skb,
					      struct mcps802154_ie *ie,
					      mcps802154_device_address *src_address);

/**
 * fira_twr_frame_data_payload_check() - Check FiRa frame payload for a data data message.
 * @session: FiRa session context.
 * @slot: Corresponding slot.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must be zero initialized.
 * @src_address: Address from the sender of the current frame.
 *
 * Return: true if message is correct.
 */
bool fira_twr_frame_data_payload_check(struct fira_session *session, const struct fira_slot *slot,
				       struct sk_buff *skb, struct mcps802154_ie *ie,
				       mcps802154_device_address *src_address);

/**
 * fira_twr_frame_header_ie_check() - Check and consume FiRa header IE.
 * @session: FiRa session context.
 * @slot: Corresponding slot.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must be zero initialized.
 * @phy_sts_index: STS index read from header IE.
 * @session_id: Session id read from header IE.
 *
 * Return: true if message is correct.
 */
bool fira_twr_frame_header_ie_check(struct fira_session *session, const struct fira_slot *slot,
				    struct sk_buff *skb, struct mcps802154_ie *ie,
				    uint32_t *phy_sts_index, uint32_t *session_id);
