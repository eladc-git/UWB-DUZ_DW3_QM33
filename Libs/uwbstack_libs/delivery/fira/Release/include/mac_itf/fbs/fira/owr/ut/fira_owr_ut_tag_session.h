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

#include <stdint.h>

extern const struct fira_session_app fira_owr_ut_tag_session_app;

struct fira_owr_ut_tag_runtime {
	/**
	 * @runtime: inherit from base runtime struct.
	 */
	struct fbs_session_runtime runtime;
};

/**
 * struct fira_session_owr_ut_tag: Group of persistent variables used when
 * the device is an UT-Tag.
 */
struct fira_session_owr_ut_tag {
	/**
	 * @frame_number: UTM frame number.
	 */
	uint32_t frame_number;
};

/**
 * fira_owr_ut_tag_session_get_runtime() - Retrieve session OWR UT tag runtime
 * Context.
 * @session: Session context.
 *
 * Return: OWR UT tag runtime context.
 */
static inline struct fira_owr_ut_tag_runtime *
fira_owr_ut_tag_session_get_runtime(const struct fira_session *session)
{
	struct fbs_session_runtime *runtime = session->base.runtime;
	return qparent_of(runtime, struct fira_owr_ut_tag_runtime, runtime);
}
