/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

enum qerr mcps802154_endless_scheduler_init(void);
void mcps802154_endless_scheduler_exit(void);

enum qerr mcps802154_default_scheduler_init(void);
void mcps802154_default_scheduler_exit(void);
