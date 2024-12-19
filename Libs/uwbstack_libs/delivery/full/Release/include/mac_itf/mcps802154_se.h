/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"
#include "qlist.h"
#include "qtils.h"

#include <stdint.h>

struct mcps802154_local;
struct mcps802154_llhw;

#define MCPS802154_SE_KEY_SIZE_MAX 32

/**
 * struct mcps802154_se_key - Description of a key result.
 */
struct mcps802154_se_key {
	/**
	 * @id: Identifier of the requested key.
	 */
	uint32_t id;
	/**
	 * @size: Byte size of the key value.
	 */
	uint8_t size;
	/**
	 * @value: Memory space for the requested key.
	 */
	uint8_t value[MCPS802154_SE_KEY_SIZE_MAX];
};

/**
 * struct mcps802154_se_request - Description of a key request.
 */
struct mcps802154_se_request {
	/**
	 * @entry: Entry in list of sessions.
	 */
	struct qlist_head entry;
	/**
	 * @llhw: Low-level device pointer.
	 */
	struct mcps802154_llhw *llhw;
	/**
	 * @region: Region instance returned to MCPS.
	 */
	struct mcps802154_region *region;
	/**
	 * @key: Descriptor for the result of the request.
	 */
	struct mcps802154_se_key key;
	/**
	 * @status: expecting status (currently pending cancellation)
	 */
	enum qerr status;
};

/*
 * @struct mcps802154_se: Private context of the secure element manager.
 */
struct mcps802154_se {
	/**
	 * @req: List of pending key request.
	 * @see struct mcps802154_se_request.
	 */
	struct qlist_head req;
	/**
	 * @current_req: Currently active request.
	 */
	struct mcps802154_se_request *current_req;
};

static inline struct mcps802154_se_request *
mcps802154_se_key_to_request(struct mcps802154_se_key *key)
{
	return qparent_of(key, struct mcps802154_se_request, key);
}

/**
 * mcps802154_se_init() - Initialize the private context of the secure
 * element manager.
 * @local: mcps802154 private data
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_se_init(struct mcps802154_local *local);

/**
 * mcps802154_se_send_request() - Post a key request to the se_manager.
 * @req: The key request description.
 *
 * NOTE: Any error is notified by the feedback event.
 */
void mcps802154_se_send_request(struct mcps802154_se_request *req);

/**
 * mcps802154_se_cancel_request() - Cancel a running key request.
 * @req: The key request description.
 *
 * NOTE: Any error is notified by the feedback event.
 *
 * Return: QERR_SUCCESS or QERR_BUSY if request is the current.
 */
enum qerr mcps802154_se_cancel_request(struct mcps802154_se_request *req);

/**
 * mcps802154_se_get_request() - Retrieve the request and then move forward
 * to the request in queue.
 * @llhw: Low-level device pointer.
 *
 * Return: The key request description.
 */
struct mcps802154_se_request *mcps802154_se_get_request(struct mcps802154_llhw *llhw);
