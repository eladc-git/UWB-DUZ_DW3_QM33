/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qblk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct quwbs - QUWBS context.
 *
 * Everything is private.
 */
struct quwbs {
	/**
	 * @msg_pool: Block pool for messages allocation.
	 */
	struct qblk_pool *msg_pool;
	/**
	 * @msg_blk_flags: Block allocation flags for messages allocation.
	 */
	enum qblk_flag msg_blk_flags;
};

#ifdef __cplusplus
}
#endif
