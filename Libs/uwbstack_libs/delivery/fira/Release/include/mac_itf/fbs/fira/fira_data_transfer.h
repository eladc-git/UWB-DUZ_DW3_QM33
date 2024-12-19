/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154.h"
#include "qerr.h"

#include <stdint.h>

struct fira_data_payload_segment;
struct fira_session;
struct qlist_head;

/*
 * Maximum credits available for one session.
 * The HPRF value is adjusted to handle transfers up to 4KB and is equal to
 * ceil(4096 / UCI_MAX_DATA_PAYLOAD_SIZE)
 */
#define FIRA_DATA_MAX_BPRF_CREDITS 3
#define FIRA_DATA_MAX_HPRF_CREDITS 17

/**
 * fira_data_transfer_init_buffers() - Initialize both RX and TX buffer list.
 * @session: FiRa session context.
 */
void fira_data_transfer_init_buffers(struct fira_session *session);

/**
 * fira_data_transfer_add_rx_payload_segment() - Add piggybacked DM payload
 * IE into the corresponding RX buffer. It creates a new RX buffer if the sender
 * address is not found in the RX buffer list.
 *
 * @session: FiRa session context.
 * @src_address: Address from the sender device of this data segment.
 * @segment: Application data segment to be added into a rx buffer.
 *
 * Return: QERR_SUCCESS or error. If a new buffer allocation fails, the callee
 * function must free the segment.
 */
enum qerr fira_data_transfer_add_rx_payload_segment(struct fira_session *session,
						    mcps802154_device_address *src_address,
						    struct fira_data_payload_segment **segment);

/**
 * fira_data_transfer_add_tx_payload_segment() - Add a new segment into TX
 * buffer.
 * @session: FiRa session context.
 * @app_data: Pointer to the application data in the segment.
 * @len: Length of the segment.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_data_transfer_add_tx_payload_segment(struct fira_session *session, uint8_t *app_data,
						    uint16_t len);

/**
 * fira_data_transfer_flush_rx_buffers() - Free all remaining rx buffers
 * including all payload segments.
 * @session: FiRa session context.
 */
void fira_data_transfer_flush_rx_buffers(struct fira_session *session);

/**
 * fira_data_transfer_flush_tx_buffer() - Free all remaining segment in tx
 * buffer.
 * @session: FiRa session context.
 */
void fira_data_transfer_flush_tx_buffer(struct fira_session *session);

/**
 * fira_data_transfer_flush_payload_segments() - Free all remaining payload
 * segments.
 * @payload_segments: List of payload segments to be freed.
 */
void fira_data_transfer_flush_payload_segments(struct qlist_head *payload_segments);

/**
 * fira_session_set_new_tx_app_data() - Handle new application data to be sent.
 * @session: FiRa session context.
 * @app_data: New application data segment.
 * @len: Length of the new application data segment.
 * @seq_num: Sequence number of the payload the current data segment belongs to.
 * @segment_info: Information about the received segment.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_set_new_tx_app_data(struct fira_session *session, uint8_t *app_data,
					   uint16_t len, uint16_t seq_num, uint8_t segment_info);

/**
 * fira_data_transfer_get_ranging_round_controller_data_size() - Get maximum user data size to be
 * sent in a ranging round for a controller.
 * @session: FiRa session context.
 * @payload_size_max: Remaining payload size
 *
 * Return: Maximum data size.
 */
uint16_t
fira_data_transfer_get_ranging_round_controller_data_size(const struct fira_session *session,
							  uint16_t payload_size_max);

/**
 * fira_data_transfer_get_ranging_round_controlee_data_size() - Get maximum user data size to be
 * sent in a ranging round for a controlee.
 * @session: FiRa session context.
 * @payload_size_max: Remaining payload size
 *
 * Return: Maximum data size.
 */
uint16_t
fira_data_transfer_get_ranging_round_controlee_data_size(const struct fira_session *session,
							 uint16_t payload_size_max);
