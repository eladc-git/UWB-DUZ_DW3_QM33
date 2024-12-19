/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM mcps802154_region_ccc

#if !defined(CCC_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define CCC_TRACE_H

#include "ccc_session.h"
#include "linux/tracepoint.h"
#include "net/ccc_region_nl.h"
#include "trace_common.h"

/* clang-format off */

#define CCC_SESSION_ENTRY __field(uint32_t, session_id)
#define CCC_SESSION_ASSIGN __entry->session_id = session->base.session_tmp.id
#define CCC_SESSION_PR_FMT "session_id=%u"
#define CCC_SESSION_PR_ARG __entry->session_id

/* clang-format on */

#endif /* !CCC_TRACE_H || TRACE_HEADER_MULTI_READ */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH ccc
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE ccc_trace
#include "trace/define_trace.h"
