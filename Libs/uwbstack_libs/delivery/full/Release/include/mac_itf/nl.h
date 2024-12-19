/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_nl_defs.h"

struct mcps802154_local;

/**
 * mcps802154_device_state_report() - Send a device state report.
 * @local: MCPS private data.
 * @state: New device state.
 */
void mcps802154_device_state_report(struct mcps802154_local *local,
				    enum mcps802154_device_state state);
