/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

struct mcps802154_local;
struct mcps802154_scheduler;
struct genl_info;
struct netlink_ext_ack;
struct nlattr;

/**
 * mcps802154_scheduler_open() - Open a scheduler, and set parameters.
 * @local: MCPS private data.
 * @name: Name of scheduler to open.
 * @params_attr: Nested attribute containing scheduler parameters, may be NULL.
 * @extack: Extended ACK report structure.
 *
 * Return: The open scheduler or NULL on error.
 */
struct mcps802154_scheduler *mcps802154_scheduler_open(struct mcps802154_local *local,
						       const char *name,
						       const struct nlattr *params_attr,
						       struct netlink_ext_ack *extack);

/**
 * mcps802154_scheduler_close() - Close a scheduler.
 * @scheduler: Pointer to the scheduler.
 */
void mcps802154_scheduler_close(struct mcps802154_scheduler *scheduler);

/**
 * mcps802154_scheduler_notify_stop() - Notify a scheduler that device has been
 * stopped.
 * @scheduler: Pointer to the scheduler.
 */
void mcps802154_scheduler_notify_stop(struct mcps802154_scheduler *scheduler);

/**
 * mcps802154_scheduler_set_parameters() - Set parameters of an open scheduler.
 * @scheduler: Pointer to the scheduler.
 * @params_attr: Nested attribute containing scheduler parameters, may be NULL.
 * @extack: Extended ACK report structure.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_scheduler_set_parameters(struct mcps802154_scheduler *scheduler,
					      const struct nlattr *params_attr,
					      struct netlink_ext_ack *extack);

/**
 * mcps802154_scheduler_call() - Call scheduler specific procedure.
 * @scheduler: Pointer to the scheduler.
 * @call_id: Identifier of the procedure, scheduler specific.
 * @params_attr: Nested attribute containing procedure parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_scheduler_call(struct mcps802154_scheduler *scheduler, uint32_t call_id,
				    const struct nlattr *params_attr, const struct genl_info *info);
