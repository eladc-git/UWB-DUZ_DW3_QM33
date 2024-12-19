/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM mcps802154_region_fira

#if !defined(FIRA_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define FIRA_TRACE_H

#include "fbs/fbs_session_runtime.h"
#include "fira/fira_session.h"
#include "fira/twr/fira_twr_session.h"
#include "linux/tracepoint.h"
#include "net/fira_region_nl.h"
#include "trace_common.h"

/* clang-format off */

#define FIRA_SESSION_ENTRY __field(uint32_t, session_id)
#define FIRA_SESSION_ASSIGN __entry->session_id = session->base.id
#define FIRA_SESSION_PR_FMT "session_id=%u"
#define FIRA_SESSION_PR_ARG __entry->session_id

TRACE_EVENT(region_fira_rx_frame,
	TP_PROTO(const struct fira_session *session,
		 int id,
		 enum mcps802154_rx_error_type error),
	TP_ARGS(session, id, error),
	TP_STRUCT__entry(
		FIRA_SESSION_ENTRY
		__field(int, id)
		__field(enum mcps802154_rx_error_type, error)
		),
	TP_fast_assign(
		FIRA_SESSION_ASSIGN;
		__entry->id = id;
		__entry->error = error;
		),
	TP_printk(FIRA_SESSION_PR_FMT " id=%d error=%s",
		FIRA_SESSION_PR_ARG,
		__entry->id,
		__print_symbolic(__entry->error, MCPS802154_RX_ERROR_NAMES)
		)
	);

TRACE_EVENT(region_fira_rx_frame_control,
	TP_PROTO(const struct fira_session *session, const struct fira_twr_runtime *twr_runtime,
		 uint32_t rx_timestamp_dtu, int n_slots),
	TP_ARGS(session, twr_runtime, rx_timestamp_dtu, n_slots),
	TP_STRUCT__entry(
		FIRA_SESSION_ENTRY
		__field(uint32_t, block_start_dtu)
		__field(int, block_index)
		__field(int, round_index)
		__field(bool, stop_inband)
		__field(uint32_t, rx_timestamp_dtu)
		__field(int, n_slots)
		),
	TP_fast_assign(
		FIRA_SESSION_ASSIGN;
		__entry->block_start_dtu = session->base.runtime->block_start_dtu;
		__entry->block_index = session->base.runtime->block_index;
		__entry->round_index = session->round_index;
		__entry->stop_inband = twr_runtime->stop_inband;
		__entry->rx_timestamp_dtu = rx_timestamp_dtu;
		__entry->n_slots = n_slots;
		),
	TP_printk(FIRA_SESSION_PR_FMT " block_start_dtu=%#x block_index=%u "
		  "round_index=%d stop_inband=%s rx_timestamp_dtu=%#x n_slots=%d",
		FIRA_SESSION_PR_ARG,
		__entry->block_start_dtu,
		__entry->block_index,
		__entry->round_index,
		print_boolean(__entry->stop_inband),
		__entry->rx_timestamp_dtu,
		__entry->n_slots
		)
	);

TRACE_EVENT(region_fira_tx_return,
	TP_PROTO(const struct fira_session *session, int id,
		 enum mcps802154_access_tx_return_reason reason),
	TP_ARGS(session, id, reason),
	TP_STRUCT__entry(
		FIRA_SESSION_ENTRY
		__field(int, id)
		__field(enum mcps802154_access_tx_return_reason, reason)
		),
	TP_fast_assign(
		FIRA_SESSION_ASSIGN;
		__entry->id = id;
		__entry->reason = reason;
		),
	TP_printk(FIRA_SESSION_PR_FMT " id=%d reason=%s",
		FIRA_SESSION_PR_ARG,
		__entry->id,
		__print_symbolic(__entry->reason,
				 MCPS802154_TX_REASON_NAMES)
		)
	);

TRACE_EVENT(
	region_fira_session_report,
	TP_PROTO(const struct fira_session *session,
		 const struct fira_report_info *report_info),
	TP_ARGS(session, report_info),
	TP_STRUCT__entry(
		FIRA_SESSION_ENTRY
		__field(uint32_t, sequence_number)
		__field(uint32_t, block_index)
		__field(int, n_ranging_data)
		__field(int, n_diagnostics)
		),
	TP_fast_assign(
		FIRA_SESSION_ASSIGN;
		__entry->sequence_number = session->sequence_number;
		__entry->block_index = session->base.runtime->block_index;
		__entry->n_ranging_data = report_info->n_ranging_data;
		__entry->n_diagnostics = report_info->n_diagnostics;
		),
	TP_printk(FIRA_SESSION_PR_FMT " sequence_number=%u block_index=%u "
		  "n_ranging_data=%d n_diagnostics=%d",
		  FIRA_SESSION_PR_ARG,
		  __entry->sequence_number,
		  __entry->block_index,
		  __entry->n_ranging_data,
		  __entry->n_diagnostics
		)
	);

/* clang-format on */

#endif /* !FIRA_TRACE_H || TRACE_HEADER_MULTI_READ */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH fira
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE fira_trace
#include "trace/define_trace.h"
