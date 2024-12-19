/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_session.h"
#include "fbs/fbs_session_runtime.h"
#include "fira/fira_session.h"
#include "fira/fira_session_app.h"
#include "qtils.h"

extern const struct fira_session_app fira_owr_ut_anchor_session_app;

struct fira_owr_ut_anchor_runtime {
	/**
	 * @runtime: inherit from base runtime struct.
	 */
	struct fbs_session_runtime runtime;
};

/**
 * fira_owr_ut_anchor_session_get_runtime() - Retrieve session UT-Anchor runtime context.
 * @session: Session context.
 *
 * Return: OWR UT-Anchor runtime context.
 */
static inline struct fira_owr_ut_anchor_runtime *
fira_owr_ut_anchor_session_get_runtime(const struct fira_session *session)
{
	struct fbs_session_runtime *runtime = session->base.runtime;
	return qparent_of(runtime, struct fira_owr_ut_anchor_runtime, runtime);
}
