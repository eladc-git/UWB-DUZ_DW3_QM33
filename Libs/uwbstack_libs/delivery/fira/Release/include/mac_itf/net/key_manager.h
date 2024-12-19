/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/fira_region_params.h"
#include "qerr.h"

#include <stdint.h>

struct mcps802154_llhw;

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_MANAGER_KEY_SIZE_MAX FIRA_KEY_SIZE_MAX

/**
 * key_manager_init() - Initialise communication between key manager and the SE.
 * @rds_provider: RDS provider to use.
 *
 * NOTE: This API should be implemented by the platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr key_manager_init(void *rds_provider);

/**
 * key_manager_add_key() - This function is used to add a session key in the SE
 * @session_id: id of the session (can be any type).
 * @session_key: output session key to add.
 * @key_len: [in/out] in is the buffer size, out is the actual size of the
 * session key.
 * @se_status: status from the SE.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr key_manager_add_key(uint32_t session_id, uint8_t *session_key, uint8_t key_len,
			      uint16_t *se_status);

/**
 * key_manager_request_key() - Post a request to receive a dynamic key.
 * @id: Session or sub-session identifier requesting the key.
 * @key: Allocated buffer needed to store the session or sub-session key.
 * @key_len: Length of the key.
 *
 * NOTE: This API should be implemented by the platform.
 * NOTE2: Any error is notified by the feedback event.
 */
void key_manager_request_key(uint32_t id, uint8_t *key, uint8_t *key_len);

/**
 * key_manager_cancel_key_request() - Used to cancel session/sub-session key
 * retrieval.
 * @id: Session or sub-session identifier requesting the key.
 *
 * NOTE: This API should be implemented by the platform.
 * NOTE2: Any error is notified by the feedback event.
 * NOTE3: After the call, the related resources key and key_len cannot be
 * accessed anymore.
 */
void key_manager_cancel_key_request(uint32_t id);

/**
 * key_manager_event() - callback called when the SE fetching event finished.
 * @id: Session or sub-session identifier requesting the key.
 * @status: Status of the request from key manager.
 *
 * NOTE: This API should be called asynchronously when key is retrieved.
 *
 */
void key_manager_event(uint32_t id, enum qerr status);

/**
 * key_manager_se_done() - Signals that key manager request is finished.
 * @llhw: Low-level device pointer.
 *
 * NOTE: This API should be called asynchronously when key is retrieved.
 *
 */
void key_manager_se_done(struct mcps802154_llhw *llhw);

#ifdef __cplusplus
}
#endif
