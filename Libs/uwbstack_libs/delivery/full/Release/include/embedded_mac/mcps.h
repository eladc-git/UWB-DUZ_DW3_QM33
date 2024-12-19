/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <net/genetlink.h>
#include <net/mac802154.h>
#include <net/mcps802154.h>
#include <net/netlink.h>

/**
 * mcps_set_scheduler() - Set the scheduler responsible for managing
 * the schedule, and configure its parameters.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @name: Scheduler name.
 * @params_attr: Scheduler parameters.
 *
 * Device should not be started for the moment.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_set_scheduler(struct ieee802154_hw *hw, const char *name,
			     const struct nlattr *params_attr);

/**
 * mcps_set_scheduler_parameters() - Set the scheduler parameters.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @name: Scheduler name.
 * @params_attr: Scheduler parameters.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_set_scheduler_parameters(struct ieee802154_hw *hw, const char *name,
					const struct nlattr *params_attr);

/**
 * mcps_call_scheduler() - Call scheduler specific procedure.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @name: Scheduler name.
 * @call_id: Identifier of the procedure, scheduler specific.
 * @params_attr: Call procedure parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_call_scheduler(struct ieee802154_hw *hw, const char *name, uint32_t call_id,
			      const struct nlattr *params_attr, const struct genl_info *info);

/**
 * mcps_set_regions() - Set regions that populate the schedule.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @scheduler_name: Scheduler name.
 * @region_id: Identifier of the region, scheduler specific.
 * @region_name: Name of region to attach to the scheduler.
 * @params_attr: Region parameters.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_set_regions(struct ieee802154_hw *hw, const char *scheduler_name, uint32_t region_id,
			   const char *region_name, const struct nlattr *params_attr);

/**
 * mcps_set_region_parameters() - Set the region parameters.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @scheduler_name: Scheduler name.
 * @region_id: Identifier of the region, scheduler specific.
 * @region_name: Name of the region to call.
 * @params_attr: Call procedure parameters.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_set_region_parameters(struct ieee802154_hw *hw, const char *scheduler_name,
				     uint32_t region_id, const char *region_name,
				     const struct nlattr *params_attr);

/**
 * mcps_get_region_parameters() - Get the region parameters.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @scheduler_name: Scheduler name.
 * @region_id: Identifier of the region, scheduler specific.
 * @region_name: Name of the region to call.
 * @params_reply: Messages to be filled with parameters by region op.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_get_region_parameters(struct ieee802154_hw *hw, const char *scheduler_name,
				     uint32_t region_id, const char *region_name,
				     struct sk_buff *params_reply);

/**
 * mcps_call_region() - Call region specific procedure.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @scheduler_name: Scheduler name.
 * @region_id: Identifier of the region, scheduler specific.
 * @region_name: Name of the region to call.
 * @call_id: Identifier of the procedure, region specific.
 * @params_attr: Call procedure parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
int mcps_call_region(struct ieee802154_hw *hw, const char *scheduler_name, uint32_t region_id,
		     const char *region_name, uint32_t call_id, const struct nlattr *params_attr,
		     const struct genl_info *info);

/**
 * mcps_set_calibration() - Set calibration value.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @key: Calibration key to set.
 * @value: Value buffer.
 * @value_size: Value size in bytes.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_set_calibration(struct ieee802154_hw *hw, const char *key, void *value,
			       size_t value_size);

/**
 * mcps_get_calibration() - Get calibration value.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @key: Calibration key to get.
 * @value: Value buffer.
 * @value_size: Value size in bytes.
 *
 * Return: size of parameter written in buffer or error.
 */
int mcps_get_calibration(struct ieee802154_hw *hw, const char *key, void *value, size_t value_size);

/**
 * mcps_get_calibration_key_name() - Get calibration key name for a specific
 * key index.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @key_idx: Calibration key index to get name of.
 * @key: Calibration key name to fill-in.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_get_calibration_key_name(struct ieee802154_hw *hw, uint16_t key_idx, char *key);

/**
 * mcps_reset_calibration() - Reset values of all calibration keys.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_reset_calibration(struct ieee802154_hw *hw);

/**
 * mcps_call_testmode() - Call a test mode function.
 * @hw: UWB MAC context.
 * @data: Test data.
 * @length: Size of test data.
 *
 * Test mode allows to directly call the driver. This is expected to be called
 * for tests. Test mode may be disabled in a device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_call_testmode(struct ieee802154_hw *hw, void *data, int length);

/**
 * mcps_close_scheduler() - Close current scheduler and regions.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 */
void mcps_close_scheduler(struct ieee802154_hw *hw);

/**
 * mcps_get_pwr_stats() - Get power stats.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @pwr_stats: Global power statistics struct.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_get_pwr_stats(struct ieee802154_hw *hw, struct mcps802154_power_stats *pwr_stats);

/**
 * mcps_query_gpio_timestamp() - Dequeue and return timestamp and sequence
 * number.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @timestamp_us: Pointer to the timestamp in microseconds.
 * @sequence_number: Pointer to the sequence number.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_query_gpio_timestamp(struct ieee802154_hw *hw, int64_t *timestamp_us,
				    uint8_t *sequence_number);

/**
 * mcps_get_uwb_device_stats() - Get UWBS stats.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @uwb_device_stats: Global UWBS statistics struct.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_get_uwb_device_stats(struct ieee802154_hw *hw,
				    struct mcps802154_uwb_device_stats *uwb_device_stats);

/**
 * mcps_get_device_info() - Get device information.
 * @hw: Pointer as obtained from ieee802154_alloc_hw().
 * @device_info: Device information.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_get_device_info(struct ieee802154_hw *hw,
			       struct mcps802154_device_info *device_info);
