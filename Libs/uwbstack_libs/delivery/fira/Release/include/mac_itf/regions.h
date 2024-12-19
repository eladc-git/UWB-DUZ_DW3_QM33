/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "mac_config_types.h"

struct mcps802154_local;
struct mcps802154_llhw;

/**
 * mcps802154_region_restricted_channel_updated() - Handle restricted channel updated.
 * @local: MCPS private data.
 */
void mcps802154_region_restricted_channel_updated(struct mcps802154_local *local);

/**
 * mcps802154_region_get_mac_session_scheduler_id() - Retrieve the session scheduler identifier.
 * @llhw: Low layer context.
 *
 * Return: Value of the session scheduler identifier.
 */
enum mac_session_scheduler_id
mcps802154_region_get_mac_session_scheduler_id(struct mcps802154_llhw *llhw);
