/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

enum mcps_task_event {
	MCPS_TASK_TX_DONE,
	MCPS_TASK_RX_TIMEOUT,
	MCPS_TASK_RX_ERROR,
	MCPS_TASK_RX,
	MCPS_TASK_STOP,
	MCPS_TASK_TIMER_EXPIRED,
	MCPS_TASK_SE_DONE,
};

/**
 * llhw_mcps_task_send_event() - Post an event into MCPS task.
 * @event: The identifier of the event.
 */
void llhw_mcps_task_send_event(enum mcps_task_event event);
