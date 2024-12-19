/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

/**
 * DOC: QUWBS lower half API.
 *
 * This API is reserved for lower half implementation usage (MAC, UCI client, lower layers...).
 */

#include "qtils.h"
#include "quwbs/err.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct qnetbuf;
struct quwbs;
struct quwbs_msg;

/**
 * struct quwbs_lh - Structure shared between QUWBS lower half implementations.
 */
struct quwbs_lh {
	/**
	 * @ops: Lower half implementation operations.
	 */
	struct quwbs_lh_ops *ops;
};

/**
 * typedef quwbs_lh_init_cb() - Signal QUWBS initialization.
 * @lh: Lower half implementation context.
 * @quwbs: QUWBS context.
 *
 * Returns: QUWBS_ERR_SUCCESS or error.
 */
typedef enum quwbs_err (*quwbs_lh_init_cb)(struct quwbs_lh *lh, struct quwbs *quwbs);

/**
 * typedef quwbs_lh_exit_cb() - Signal QUWBS exit.
 * @lh: Lower half implementation context.
 */
typedef void (*quwbs_lh_exit_cb)(struct quwbs_lh *lh);

/**
 * typedef quwbs_lh_msg_recv_cb() - Receive a message from higher layers.
 * @lh: Lower half implementation context.
 * @msg: Received message, take ownership.
 *
 * Lower half implementation must release the message when done.
 */
typedef void (*quwbs_lh_msg_recv_cb)(struct quwbs_lh *lh, struct quwbs_msg *msg);

/**
 * typedef quwbs_lh_tx_cb() - Send a data frame.
 * @lh: Lower half implementation context.
 * @buf: Network buffer.
 * @queue_index: Corresponding queue.
 *
 * Take buffer ownership, must be given back using quwbs_lh_tx_done().
 */
typedef void (*quwbs_lh_tx_cb)(struct quwbs_lh *lh, struct qnetbuf *buf, int queue_index);

/**
 * typedef quwbs_lh_rx_queue() - Suspend or resume a queue.
 * @lh: Lower half implementation context.
 * @queue_index: Queue to suspend or resume.
 */
typedef void (*quwbs_lh_rx_queue)(struct quwbs_lh *lh, int queue_index);

/**
 * struct quwbs_lh_ops - Lower half implementation operations.
 *
 * This interface must be implemented by QUWBS lower half implementations.
 *
 * Examples: A message may be routed to the component responsible to handle it and pushed into the
 * corresponding queue. Pushing to the queue triggers the execution of the corresponding thread.
 * Messages may also be handled directly, then released.
 */
struct quwbs_lh_ops {
	/**
	 * @init: Called on QUWBS initialization.
	 */
	quwbs_lh_init_cb init;
	/**
	 * @exit: Called on QUWBS exit.
	 */
	quwbs_lh_exit_cb exit;
	/**
	 * @msg_recv: Called to receive a message from higher layers.
	 */
	quwbs_lh_msg_recv_cb msg_recv;
	/**
	 * @tx: Called to send a data frame.
	 */
	quwbs_lh_tx_cb tx;
	/**
	 * @rx_queue_suspend: Called to suspend a reception queue.
	 */
	quwbs_lh_rx_queue rx_queue_suspend;
	/**
	 * @rx_queue_resume: Called to resume a reception queue.
	 */
	quwbs_lh_rx_queue rx_queue_resume;
};

/**
 * quwbs_lh_msg_rsp_new_size() - Allocate and initialize a new message for a response.
 * @ctx: QUWBS context.
 * @cmd_msg: Command message, borrowed for the duration of the call.
 * @cmd_status: Command status.
 * @root_size: Size of the message root structure in bytes.
 *
 * When possible, use the &quwbs_lh_msg_rsp_new() macro instead.
 *
 * This copies information from the command message.
 *
 * Returns: A pointer to the message root structure, or %NULL if memory is exhausted.
 */
struct quwbs_msg *quwbs_lh_msg_rsp_new_size(struct quwbs *ctx, struct quwbs_msg *cmd_msg,
					    enum quwbs_err cmd_status, size_t root_size);

/**
 * quwbs_lh_msg_rsp_new() - Allocate and initialize a new message for a response with automatic
 * type handling.
 * @ctx: QUWBS context.
 * @cmd_msg: Command message, borrowed for the duration of the call.
 * @cmd_status: Command status.
 * @root_type: Type of the message root (not the pointer), must include a `msg` member for the base
 * message structure.
 *
 * This is a macro.
 *
 * This copies information from the command message.
 *
 * Returns: A pointer to the message root structure, or %NULL if memory is exhausted. Do not cast
 * the result so that the compiler can detect an erroneous parameter.
 */
#define quwbs_lh_msg_rsp_new(ctx, cmd_msg, cmd_status, root_type)                                 \
	(qparent_of(quwbs_lh_msg_rsp_new_size((ctx), (cmd_msg), (cmd_status), sizeof(root_type)), \
		    root_type, msg))

/**
 * quwbs_lh_msg_send() - Send a message to higher layers.
 * @ctx: QUWBS context.
 * @msg: Message to send, take ownership.
 *
 * This function takes ownership of the message in every cases, the caller must discard its pointer.
 *
 * Returns: QUWBS_ERR_SUCCESS or error.
 */
enum quwbs_err quwbs_lh_msg_send(struct quwbs *ctx, struct quwbs_msg *msg);

/**
 * quwbs_lh_tx_done() - Signal that a Tx buffer can be disposed.
 * @ctx: QUWBS context.
 * @buf: Buffer to be disposed, take ownership.
 * @success: This is true if transmission was done successfully.
 *
 * This function takes ownership of the buffer, the caller must discard its pointer.
 */
void quwbs_lh_tx_done(struct quwbs *ctx, struct qnetbuf *buf, bool success);

/**
 * quwbs_lh_tx_queue_suspended() - Signal a queue is suspended.
 * @ctx: QUWBS context.
 * @queue_index: Suspended queue index.
 */
void quwbs_lh_tx_queue_suspended(struct quwbs *ctx, int queue_index);

/**
 * quwbs_lh_tx_queue_resumed() - Signal a queue is resumed.
 * @ctx: QUWBS context.
 * @queue_index: Resumed queue index.
 */
void quwbs_lh_tx_queue_resumed(struct quwbs *ctx, int queue_index);

/**
 * quwbs_lh_rx() - Signal that a data frame has been received.
 * @ctx: QUWBS context.
 * @buf: Received frame buffer, take ownership.
 *
 * This function takes ownership of the buffer, the caller must discard its pointer.
 */
void quwbs_lh_rx(struct quwbs *ctx, struct qnetbuf *buf);

#ifdef __cplusplus
}
#endif
