/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/ccc/ccc_session.h"
#include "fbs/fbs_session.h"

struct fbs_session_demand;
struct mcps802154_access;

static inline struct ccc_session *access_to_ccc(struct mcps802154_access *access)
{
	return fbs_to_ccc(access_to_session(access));
}

struct mcps802154_access *ccc_access_initiator(struct ccc_session *session,
					       const struct fbs_session_demand *fsd);
