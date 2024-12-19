/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "quwbs/fbs/defs.h"

struct fira_session;
struct mcps802154_llhw;

/**
 * fira_config_init() - Initializes default configuration.
 * @llhw: Low-level device pointer.
 * @session: FiRa session context.
 * @stype: Session type.
 */
void fira_config_init(struct mcps802154_llhw *llhw, struct fira_session *session,
		      enum quwbs_fbs_session_type stype);
