/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef UWBMAC_REPORT_H
#define UWBMAC_REPORT_H

#include <linux/skbuff.h>
#include <net/mac802154.h>
#include <uwbmac/uwbmac.h>
#include <uwbmac/uwbmac_buf.h>

/**
 * uwbmac_device_state_report() - Report a device state change.
 * @hw: Pointer to MCPS hw instance.
 * @state: New device state.
 */
void uwbmac_device_state_report(struct ieee802154_hw *hw, enum uwbmac_device_state state);

/**
 * uwbmac_region_call_reply() - Reply to a region call.
 * @hw: Pointer to MCPS hw instance.
 * @reply: Reply message.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr uwbmac_region_call_reply(struct ieee802154_hw *hw, struct sk_buff *reply);

/**
 * uwbmac_event_report() - Report an event.
 * @hw: Pointer to MCPS hw instance.
 * @port_id: Port id to use to notify upper layer.
 * @report: Event report.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr uwbmac_event_report(struct ieee802154_hw *hw, uint32_t port_id, struct sk_buff *report);

/**
 * uwbmac_testmode_reply() - Reply to a testmode call.
 * @hw: Pointer to MCPS hw instance.
 * @reply: Reply message.
 *
 * NOTE: This method is only used by embedded flavor.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr uwbmac_testmode_reply(struct ieee802154_hw *hw, struct uwbmac_buf *reply);

#endif /* UWBMAC_REPORT_H */
