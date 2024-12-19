/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdbool.h>

/**
 * DOC: QUWBS data API
 *
 * Overview
 * --------
 *
 * Data API is used to exchange data frames, like a network adapter.
 */

#ifdef __cplusplus
extern "C" {
#endif

struct quwbs;
struct qnetbuf;

/**
 * struct quwbs_data_ops - Data operations.
 *
 * The same interface is used for any data transfer when at least one of the active regions
 * implements it.
 *
 * Data is sent and received as MPDU without the FCS, this means that the MAC header must be
 * included, but not the MAC footer. The data must be included in a &struct qnetbuf.
 *
 * Transmission:
 *
 * To send a data frame, use the quwbs_data_tx() function. The MAC will handle all the timing
 * details and send the frame when possible. Once the frame has been sent, or when the MAC
 * determined that the frame cannot be sent, the &quwbs_data_ops.tx_done callback is called so that
 * the application can have a status of the transmission and reclaim memory.
 *
 * The MAC can handle several queues. Frame ordering for a recipient inside a queue is guaranteed,
 * but not between two different recipients or between two different queues.
 *
 * A queue can be suspended or resumed. When a queue is suspended, the application is expected to
 * refrain transmission of any other frame for the same queue. Any transmission attempt will result
 * in a error returned by quwbs_data_tx(). Queue state change is signaled by
 * &quwbs_data_ops.tx_queue_suspended and &quwbs_data_ops.tx_queue_resumed callbacks. Queues start
 * in the resumed state.
 *
 * Reception:
 *
 * When a data frame is received by the MAC, the &quwbs_data_ops.rx callback is called. Once the
 * frame data has been processed, the application must release the associated memory.
 *
 * Application can also control the flow of data reception by calling the
 * quwbs_data_rx_queue_suspend() and quwbs_data_rx_queue_resume() function. Queues start in the
 * resumed state.
 */
struct quwbs_data_ops {
	/**
	 * @tx_done: Called when a buffer given to quwbs_data_tx() can be disposed.
	 *
	 * If function pointer is %NULL, buffer is released.
	 *
	 * The success parameter is true if the transmission was done successfully.
	 */
	void (*tx_done)(void *user_data, struct qnetbuf *buf, bool success);
	/**
	 * @tx_queue_suspended: Called to signal a queue is suspended. Application should refrain
	 * from transmitting more data frame on this queue.
	 *
	 * If function pointer is %NULL, ignored.
	 */
	void (*tx_queue_suspended)(void *user_data, int queue_index);
	/**
	 * @tx_queue_resumed: Called to signal a queue is resumed. Application can resume data frame
	 * transmission on this queue.
	 *
	 * If function pointer is %NULL, ignored.
	 */
	void (*tx_queue_resumed)(void *user_data, int queue_index);
	/**
	 * @rx: Called when a data frame has been received and that the receiving queue is not
	 * suspended.
	 *
	 * If function pointer is %NULL, buffer is released.
	 */
	void (*rx)(void *user_data, struct qnetbuf *buf, int queue_index);
};

/**
 * quwbs_data_register_ops() - Set callbacks used for data transfer.
 * @ctx: QUWBS context.
 * @ops: Structure with the data callbacks. Must be kept valid. %NULL to clear callbacks.
 * @user_data: User data to pass to callback functions.
 *
 * Please see &struct quwbs_data_ops for details.
 */
void quwbs_data_register_ops(struct quwbs *ctx, const struct quwbs_data_ops *ops, void *user_data);

/**
 * quwbs_data_tx() - Send a data frame.
 * @ctx: QUWBS context.
 * @buf: Network buffer.
 * @queue_index: Corresponding queue.
 *
 * Please see &struct quwbs_data_ops for details.
 *
 * Take buffer ownership, will be given back using &quwbs_data_ops.tx_done callback.
 *
 * Returns: QUWBS_ERR_SUCCESS or error.
 */
enum quwbs_err quwbs_data_tx(struct quwbs *ctx, struct qnetbuf *buf, int queue_index);

/**
 * quwbs_data_rx_queue_suspend() - Suspend a reception queue.
 * @ctx: QUWBS context.
 * @queue_index: Corresponding queue.
 *
 * Please see &struct quwbs_data_ops for details.
 */
void quwbs_data_rx_queue_suspend(struct quwbs *ctx, int queue_index);

/**
 * quwbs_data_rx_queue_resume() - Resume a reception queue.
 * @ctx: QUWBS context.
 * @queue_index: Corresponding queue.
 *
 * Please see &struct quwbs_data_ops for details.
 */
void quwbs_data_rx_queue_resume(struct quwbs *ctx, int queue_index);

#ifdef __cplusplus
}
#endif
