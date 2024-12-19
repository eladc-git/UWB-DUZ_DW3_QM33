/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"

#include <stddef.h>
#include <stdint.h>

struct fira_session;
struct fbs_se_key_request;
struct fira_controlee;
struct fira_slot;
struct sk_buff;

/**
 * fira_sts_set_controlee_error_state() - Set a controlee to a given error
 * state.
 * @controlee: The controlee that will have its state changed.
 * @err: The error used to choose controlee state.
 */
void fira_sts_set_controlee_error_state(struct fira_controlee *controlee, enum qerr err);

/**
 * fira_sts_request_session_keys() - Request session and potential sub-session
 * keys. Post request to key-manager for dynamic keys, initialize corresponding
 * crypto context for others.
 * @session: The session requesting context init.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fira_sts_request_session_keys(struct fira_session *session);

/**
 * fira_sts_receive_key() - Initialize STS context with received dynamic key.
 * @session: The session requesting context init.
 * @se_key_req: The key-request result.
 * @r: The error code of key-request request.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fira_sts_receive_key(struct fira_session *session,
						struct fbs_se_key_request *se_key_req, enum qerr r);

/**
 * fira_sts_is_ready() - Check that all requested key have been acknowledged.
 * @session: The session requesting context init.
 *
 * Return: QERR_SUCCESS when session is ready to start, QERR_EBUSY if still
 * waiting for key and QERR_SE_ENOSUBKEY when no valid sub-session key received.
 */
enum qerr fira_sts_is_ready(const struct fira_session *session);

/**
 * fira_sts_deinit() - Deinitialize STS context and release its resources.
 * @session: The session requesting context deinit.
 */
void fira_sts_deinit(struct fira_session *session);

/**
 * fira_sts_request_sub_session_key() - Request sub-session key for a
 * controllee. Post request to key-manager for dynamic key or initialize
 * corresponding crypto context.
 * @session: The session requesting sub-session context init.
 * @controlee: The controlee containing the sub-session context init.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_sts_request_sub_session_key(struct fira_session *session,
					   struct fira_controlee *controlee);

/**
 * fira_sts_controlee_deinit() - Deinitialize sub-session's STS context for
 * a given controlee.
 * @controlee: The controlee containing the sub-session context to deinit.
 */
void fira_sts_controlee_deinit(struct fira_controlee *controlee);

/**
 * fira_sts_rotate_keys() - Rotate intermediate keys if needed.
 * @session: The session requesting key rotation.
 */
void fira_sts_rotate_keys(struct fira_session *session);

/**
 * fira_sts_get_sts_params() - Fetch sts_params to configure the current frame.
 * @session: The session requesting sts_params.
 * @slot: The slot on which the STS computation is performed.
 * @sts_v: STS vector to fill.
 * @sts_v_size: Size of STS vector.
 * @sts_key: STS key to set.
 * @sts_key_size: Size of STS key.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_sts_get_sts_params(const struct fira_session *session, const struct fira_slot *slot,
				  uint8_t *sts_v, uint32_t sts_v_size, uint8_t *sts_key,
				  uint32_t sts_key_size);

/**
 * fira_sts_get_phy_sts_index() - Compute phy sts index on a specific slot for a
 * given session.
 * @session: The session requesting phy sts index.
 * @slot: The slot associated with the desired phy sts index.
 *
 * Return: phy sts index on the current slot.
 */
uint32_t fira_sts_get_phy_sts_index(const struct fira_session *session,
				    const struct fira_slot *slot);

/**
 * fira_sts_get_segment_length() - Returns the length of sts segments based
 * on the session parameters.
 * @session: Current session.
 *
 * Return: length of sts segments.
 */
uint16_t fira_sts_get_segment_length(const struct fira_session *session);

/**
 * fira_sts_prepare_decrypt() - Prepare skb for header decryption and
 * verification.
 * @session: The session requesting frame decryption.
 * @slot: The slot for which the frame should be decrypted.
 * @skb: Buffer containing the frame to decrypt.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_sts_prepare_decrypt(struct fira_session *session, const struct fira_slot *slot,
				   struct sk_buff *skb);

/**
 * fira_sts_encrypt_frame() - Encrypt the given FiRa 802.15.4 frame.
 * @session: The session requesting frame encryption.
 * @slot: The slot with the frame to encrypt.
 * @skb: Buffer containing the frame to encrypt.
 * @header_len: Length of the 802.15.4 header. Can be used to find the start of
 * the payload.
 * @src_short_addr: Source short address.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_sts_encrypt_frame(const struct fira_session *session, const struct fira_slot *slot,
				 struct sk_buff *skb, int header_len, uint16_t src_short_addr);

/**
 * fira_sts_decrypt_frame() - Decrypt the given FiRa 802.15.4 frame.
 * @session: The session requesting frame decryption.
 * @slot: The slot with the frame to decrypt.
 * @skb: Buffer containing the frame to decrypt.
 * @header_len: Length of the 802.15.4 header. Used to find the start of the
 * frame payload.
 * @src_short_addr: Source short address.
 * @phy_sts_index: Phy sts index on the current slot.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_sts_decrypt_frame(const struct fira_session *session, const struct fira_slot *slot,
				 struct sk_buff *skb, int header_len, uint16_t src_short_addr,
				 uint32_t phy_sts_index);

/**
 * fira_sts_encrypt_hie() - Encrypt the HIE stored in a FiRa 802.15.4 frame.
 * @session: The session requesting HIE encryption.
 * @slot: The slot with the HIE to encrypt.
 * @skb: Buffer containing the HIE.
 * @hie_offset: Offset to the start of the HIE (relative to skb->data).
 * @hie_len: Length of the HIE.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_sts_encrypt_hie(const struct fira_session *session, const struct fira_slot *slot,
			       struct sk_buff *skb, int hie_offset, size_t hie_len);

/**
 * fira_sts_decrypt_hie() - Decrypt the HIE stored in a FiRa 802.15.4 frame.
 * @session: The session requesting HIE decryption.
 * @slot: The slot with the HIE to decrypt.
 * @skb: Buffer containing the frame.
 * @hie_offset: Offset to the start of the HIE (relative to skb->data).
 * @hie_len: Length of the HIE.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_sts_decrypt_hie(const struct fira_session *session, const struct fira_slot *slot,
			       struct sk_buff *skb, int hie_offset, size_t hie_len);
