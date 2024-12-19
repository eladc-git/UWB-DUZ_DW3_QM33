/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM mcps802154_region_radar

#if !defined(RADAR_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define RADAR_TRACE_H

#include "linux/tracepoint.h"
#include "mcps802154_i.h"
#include "net/radar_region_nl.h"
#include "net/radar_region_params.h"
#include "radar/radar_region.h"
#include "radar/radar_session.h"
#include "trace_common.h"

/* clang-format off */
TRACE_EVENT(
	radar_session_report,
	TP_PROTO(const struct radar_session *session),
	TP_ARGS(session),
	TP_STRUCT__entry(
		__field(uint16_t, measurement_nb)
		),
	TP_fast_assign(
		__entry->measurement_nb = session->measurement_nb;
		),
	TP_printk("measurement_nb=%u", __entry->measurement_nb)
);

/* clang-format on */

#endif /* !RADAR_TRACE_H || TRACE_HEADER_MULTI_READ */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH radar
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE radar_trace
#include "trace/define_trace.h"
