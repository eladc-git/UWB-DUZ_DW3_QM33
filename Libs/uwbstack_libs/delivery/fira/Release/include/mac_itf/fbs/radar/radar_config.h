/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

struct radar_session;

/**
 * radar_config_init() - Initializes default configuration.
 * @session: Radar session context.
 */
void radar_config_init(struct radar_session *session);
