/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM mcps802154_region_lld_common

#if !defined(LLD_COMMON_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define LLD_COMMON_TRACE_H

#include "linux/tracepoint.h"
#include "lld/lld_common.h"
#include "lld/lld_mfs.h"

/* clang-format off */

TRACE_EVENT(
	region_lld_network_params,
	TP_PROTO(const struct lld_network_params *params), TP_ARGS(params),
	TP_STRUCT__entry(
		__field(int, block_dtu)
		__field(int, burst_ifs)
		__field(bool, fast_ds_twr)
		__field(int, sb_cap_slot_dtu)
		__field(int, sb_dl_slot_dtu)
		__field(int, sb_min_dtu)
		__field(int, sb_aux_max)
		__field(int, sb_ul_slot_last_dtu)
		__field(int, sb_ul_slot_dtu)
		__field(int, cap_max_be)
		__field(int, cap_min_be)
		__field(bool, ranging_aoa_azimuth)
		__field(bool, ranging_aoa_elevation)
		__field(int, ranging_exponent)
		__field(int, ranging_multi_node_mode)
		__field(bool, ranging_report_immediate)
		__field(int, ranging_round_usage)
		__field(int, ranging_sts_packet_config)
		__field(int, resync_threshold_exponent)
		),
	TP_fast_assign(
		__entry->block_dtu = params->block_dtu;
		__entry->burst_ifs = LLD_IFS_TO_INT(params->burst_ifs);
		__entry->fast_ds_twr = params->fast_ds_twr;
		__entry->sb_cap_slot_dtu = params->sb_cap_slot_dtu;
		__entry->sb_dl_slot_dtu = params->sb_dl_slot_dtu;
		__entry->sb_min_dtu = params->sb_min_dtu;
		__entry->sb_aux_max = params->sb_aux_max;
		__entry->sb_ul_slot_last_dtu = params->sb_ul_slot_last_dtu;
		__entry->sb_ul_slot_dtu = params->sb_ul_slot_dtu;
		__entry->cap_max_be = params->cap_max_be;
		__entry->cap_min_be = params->cap_min_be;
		__entry->ranging_aoa_azimuth = params->ranging_aoa_azimuth;
		__entry->ranging_aoa_elevation = params->ranging_aoa_elevation;
		__entry->ranging_exponent = params->ranging_exponent;
		__entry->ranging_multi_node_mode = params->ranging_multi_node_mode;
		__entry->ranging_report_immediate = params->ranging_report_immediate;
		__entry->ranging_round_usage = params->ranging_round_usage;
		__entry->ranging_sts_packet_config = params->ranging_sts_packet_config;
		__entry->resync_threshold_exponent =
			params->resync_threshold_exponent;
		),
	TP_printk("block_dtu=%d burst_ifs=%d fast_ds_twr=%d sb_cap_slot_dtu=%d "
		  "sb_dl_slot_dtu=%d sb_min_dtu=%d sb_aux_max=%d "
		  "sb_ul_slot_last_dtu=%d sb_ul_slot_dtu=%d cap_max_be=%d "
		  "cap_min_be=%d "
		  "ranging_aoa_azimuth=%d ranging_aoa_elevation=%d "
		  "ranging_exponent=%d ranging_multi_node_mode=%d "
		  "ranging_report_immediate=%d ranging_round_usage=%d "
		  "ranging_sts_packet_config=%d resync_threshold_exponent=%d",
		  __entry->block_dtu,
		  __entry->burst_ifs,
		  __entry->fast_ds_twr,
		  __entry->sb_cap_slot_dtu,
		  __entry->sb_dl_slot_dtu,
		  __entry->sb_min_dtu,
		  __entry->sb_aux_max,
		  __entry->sb_ul_slot_last_dtu,
		  __entry->sb_ul_slot_dtu,
		  __entry->cap_max_be,
		  __entry->cap_min_be,
		  __entry->ranging_aoa_azimuth,
		  __entry->ranging_aoa_elevation,
		  __entry->ranging_exponent,
		  __entry->ranging_multi_node_mode,
		  __entry->ranging_report_immediate,
		  __entry->ranging_round_usage,
		  __entry->ranging_sts_packet_config,
		  __entry->resync_threshold_exponent)
	);

TRACE_EVENT(
	region_lld_mfs_params,
	TP_PROTO(int mfs_id, const struct lld_mfs_params *params),
	TP_ARGS(mfs_id, params),
	TP_STRUCT__entry(
		__field(int, mfs_id)
		__field(int, mfs_expiration_ms)
		__field(int, mfs_window_msdu)
		__field(int, mfs_window_bytes)
		),
	TP_fast_assign(
		__entry->mfs_id = mfs_id;
		__entry->mfs_expiration_ms = params->mfs_expiration_ms;
		__entry->mfs_window_msdu = params->mfs_window_msdu;
		__entry->mfs_window_bytes = params->mfs_window_bytes;
		),
	TP_printk("mfs_id=%d mfs_expiration_ms=%d mfs_window_msdu=%d "
		  "mfs_window_bytes=%d",
		  __entry->mfs_id,
		  __entry->mfs_expiration_ms,
		  __entry->mfs_window_msdu,
		  __entry->mfs_window_bytes));

TRACE_EVENT(
	region_lld_tx_msdu,
	TP_PROTO(int mfs_id, int len, int seq),
	TP_ARGS(mfs_id, len, seq),
	TP_STRUCT__entry(
		__field(int, mfs_id)
		__field(int, len)
		__field(int, seq)
		),
	TP_fast_assign(
		__entry->mfs_id = mfs_id;
		__entry->len = len;
		__entry->seq = seq;
		),
	TP_printk("mfs_id=%d len=%d seq=%d",
		__entry->mfs_id,
		__entry->len,
		__entry->seq
		));

TRACE_EVENT(
	region_lld_rx_msdu,
	TP_PROTO(int mfs_id, int seq),
	TP_ARGS(mfs_id, seq),
	TP_STRUCT__entry(
		__field(int, mfs_id)
		__field(int, seq)
		),
	TP_fast_assign(
		__entry->mfs_id = mfs_id;
		__entry->seq = seq;
		),
	TP_printk("mfs_id=%d seq=%d",
		__entry->mfs_id,
		__entry->seq
		));

TRACE_EVENT(
	region_lld_enqueue_msdu,
	TP_PROTO(int mfs_id, int len, int seq),
	TP_ARGS(mfs_id, len, seq),
	TP_STRUCT__entry(
		__field(int, mfs_id)
		__field(int, len)
		__field(int, seq)
		),
	TP_fast_assign(
		__entry->mfs_id = mfs_id;
		__entry->len = len;
		__entry->seq = seq;
		),
	TP_printk("mfs_id=%d len=%d seq=%d",
		__entry->mfs_id,
		__entry->len,
		__entry->seq
		));

TRACE_EVENT(
	region_lld_mfs_tx_reset,
	TP_PROTO(int mfs_id, int n_queued),
	TP_ARGS(mfs_id, n_queued),
	TP_STRUCT__entry(
		__field(int, mfs_id)
		__field(int, n_queued)
		),
	TP_fast_assign(
		__entry->mfs_id = mfs_id;
		__entry->n_queued = n_queued;
		),
	TP_printk("mfs_id=%d n_queued=%d",
		__entry->mfs_id,
		__entry->n_queued
		)
	);

TRACE_EVENT(
	region_lld_mfs_dequeue_expired,
	TP_PROTO(int seq),
	TP_ARGS(seq),
	TP_STRUCT__entry(
		__field(int, seq)
		),
	TP_fast_assign(
		__entry->seq = seq;
		),
	TP_printk("seq=%d",
		__entry->seq
		)
	);

/* clang-format on */

#endif /* !LLD_COMMON_TRACE_H || TRACE_HEADER_MULTI_READ */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH lld
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE lld_common_trace
#include "trace/define_trace.h"
