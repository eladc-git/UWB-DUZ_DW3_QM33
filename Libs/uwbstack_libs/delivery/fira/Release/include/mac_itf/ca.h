/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_schedule.h"
#include "qerr.h"
#include "qlist.h"
#include "schedule.h"

#include <stdbool.h>
#include <stdint.h>

struct mcps802154_local;
struct genl_info;
struct netlink_ext_ack;
struct nlattr;
struct sk_buff;

/**
 * struct mcps802154_ca - CA private data.
 */
struct mcps802154_ca {
	/**
	 * @schedule: Current schedule.
	 */
	struct mcps802154_schedule schedule;
	/**
	 * @scheduler: Scheduler responsible to maintain the schedule
	 * or NULL when not chosen yet.
	 */
	struct mcps802154_scheduler *scheduler;
	/**
	 * @regions: List of regions currently available in the schedule.
	 */
	struct qlist_head regions;
	/**
	 * @n_regions: current number of opened regions.
	 */
	int n_regions;
	/**
	 * @reset: Whether the schedule was invalidated and need to be changed.
	 */
	bool reset;
	/**
	 * @idle_access: Access used to wait when there is nothing to do.
	 */
	struct mcps802154_access idle_access;
};

/**
 * mcps802154_ca_init() - Initialize CA.
 * @local: MCPS private data.
 */
void mcps802154_ca_init(struct mcps802154_local *local);

/**
 * mcps802154_ca_uninit() - Uninitialize CA.
 * @local: MCPS private data.
 */
void mcps802154_ca_uninit(struct mcps802154_local *local);

/**
 * mcps802154_ca_start() - Start device.
 * @local: MCPS private data.
 *
 * FSM mutex should be locked.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_ca_start(struct mcps802154_local *local);

/**
 * mcps802154_ca_stop() - Request to stop device.
 * @local: MCPS private data.
 *
 * FSM mutex should be locked.
 *
 * This is asynchronous, caller needs to wait !local->started.
 */
void mcps802154_ca_stop(struct mcps802154_local *local);

/**
 * mcps802154_ca_notify_stop() - Notify that device has been stopped.
 * @local: MCPS private data.
 *
 * FSM mutex should be locked.
 *
 */
void mcps802154_ca_notify_stop(struct mcps802154_local *local);

/**
 * mcps802154_ca_close() - Request to close all the schedules.
 * @local: MCPS private data.
 *
 * FSM mutex should be locked.
 */
void mcps802154_ca_close(struct mcps802154_local *local);

/**
 * mcps802154_ca_set_scheduler() - Set the scheduler responsible for managing
 * the schedule, and configure its parameters.
 * @local: MCPS private data.
 * @name: Scheduler name.
 * @params_attr: Nested attribute containing region parameters. May be NULL.
 * @extack: Extended ACK report structure.
 *
 * FSM mutex should be locked.
 *
 * Device should not be started for the moment.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_ca_set_scheduler(struct mcps802154_local *local, const char *name,
				      const struct nlattr *params_attr,
				      struct netlink_ext_ack *extack);

/**
 * mcps802154_ca_set_region() - Set scheduler's region.
 * @local: MCPS private data.
 * @scheduler_name: Scheduler name.
 * @region_id: Identifier of the region, scheduler specific.
 * @region_name: Name of region to attach to the scheduler.
 * @params_attr: Nested attribute containing region parameters.
 * @extack: Extended ACK report structure.
 *
 * FSM mutex should be locked.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_ca_set_region(struct mcps802154_local *local, const char *scheduler_name,
				   uint32_t region_id, const char *region_name,
				   const struct nlattr *params_attr,
				   struct netlink_ext_ack *extack);

/**
 * mcps802154_ca_scheduler_set_parameters() - Set the scheduler parameters.
 * @local: MCPS private data.
 * @name: Scheduler name.
 * @params_attr: Nested attribute containing region parameters.
 * @extack: Extended ACK report structure.
 *
 * FSM mutex should be locked.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_ca_scheduler_set_parameters(struct mcps802154_local *local, const char *name,
						 const struct nlattr *params_attr,
						 struct netlink_ext_ack *extack);

/**
 * mcps802154_ca_scheduler_call() - Call scheduler specific procedure.
 * @local: MCPS private data.
 * @scheduler_name: Scheduler name.
 * @call_id: Identifier of the procedure, scheduler specific.
 * @params_attr: Nested attribute containing procedure parameters.
 * @info: Request information.
 *
 * FSM mutex should be locked.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_ca_scheduler_call(struct mcps802154_local *local, const char *scheduler_name,
				       uint32_t call_id, const struct nlattr *params_attr,
				       const struct genl_info *info);

/**
 * mcps802154_ca_set_region_parameters() - Set the region parameters.
 * @local: MCPS private data.
 * @scheduler_name: Scheduler name.
 * @region_id: Identifier of the region, scheduler specific.
 * @region_name: Name of the region to call.
 * @params_attr: Nested attribute containing region parameters.
 * @extack: Extended ACK report structure.
 *
 * FSM mutex should be locked.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_ca_set_region_parameters(struct mcps802154_local *local,
					      const char *scheduler_name, uint32_t region_id,
					      const char *region_name,
					      const struct nlattr *params_attr,
					      struct netlink_ext_ack *extack);

/**
 * mcps802154_ca_get_region_parameters() - Get the region parameters.
 * @local: MCPS private data.
 * @scheduler_name: Scheduler name.
 * @region_id: Identifier of the region, scheduler specific.
 * @region_name: Name of the region to call.
 * @params_reply: Message to be filled with parameters by region op.
 *
 * FSM mutex should be locked.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_ca_get_region_parameters(struct mcps802154_local *local,
					      const char *scheduler_name, uint32_t region_id,
					      const char *region_name,
					      struct sk_buff *params_reply);

/**
 * mcps802154_ca_call_region() - Call region specific procedure.
 * @local: MCPS private data.
 * @scheduler_name: Scheduler name.
 * @region_id: Identifier of the region, scheduler specific.
 * @region_name: Name of the region to call.
 * @call_id: Identifier of the procedure, region specific.
 * @params_attr: Nested attribute containing procedure parameters.
 * @info: Request information.
 *
 * FSM mutex should be locked.
 *
 * Return: QERR_SUCCESS or error.
 */
int mcps802154_ca_call_region(struct mcps802154_local *local, const char *scheduler_name,
			      uint32_t region_id, const char *region_name, uint32_t call_id,
			      const struct nlattr *params_attr, const struct genl_info *info);

/**
 * mcps802154_ca_xmit_skb() - Transmit the buffer through the first region
 * that accepts it.
 * @local: MCPS private data.
 * @skb: Buffer to be transmitted.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_ca_xmit_skb(struct mcps802154_local *local, struct sk_buff *skb);

/**
 * mcps802154_ca_get_access() - Compute and return access.
 * @local: MCPS private data.
 * @next_timestamp_dtu: Date of next access opportunity.
 *
 * Return: A pointer to current access.
 */
struct mcps802154_access *mcps802154_ca_get_access(struct mcps802154_local *local,
						   uint32_t next_timestamp_dtu);

/**
 * mcps802154_ca_reschedule() - Request FProc to change access.
 * @local: MCPS private data.
 *
 * FSM mutex should be locked.
 *
 * When something has changed that could impact the current access, this
 * function should be called to evaluate the change and notify FProc. This
 * should be done for example when a new frame is queued.
 */
void mcps802154_ca_reschedule(struct mcps802154_local *local);

/**
 * mcps802154_ca_invalidate_schedule() - Invalidate the schedule and force
 * update.
 * @local: MCPS private data.
 *
 * FSM mutex should be locked.
 *
 * When something has changed that impact the current schedule, this function
 * can be called to invalidate the schedule and force an update.
 * The update will be done after the end of current access.
 * This API should be called for example when a region parameter changes.
 */
void mcps802154_ca_invalidate_schedule(struct mcps802154_local *local);
