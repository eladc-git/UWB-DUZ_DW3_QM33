/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM mcps802154_region_lldd

#if !defined(LLDD_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define LLDD_TRACE_H

#include "linux/tracepoint.h"
#include "lldd_frame.h"
#include "lldd_region.h"
#include "trace_common.h"

/* clang-format off */
#define lldd_subaccess_type_name(name)                               \
	{                                                            \
		LLDD_SUBACCESS_TYPE_##name, #name                    \
	}
#define LLDD_SUBACCESS_TYPE_SYMBOLS                                  \
	lldd_subaccess_type_name(ADDITIONAL_CAP),                    \
	lldd_subaccess_type_name(AUX),                             \
	lldd_subaccess_type_name(NON_ASSOCIATED),                    \
	lldd_subaccess_type_name(NOTHING)
TRACE_DEFINE_ENUM(LLDD_SUBACCESS_TYPE_AUX);
TRACE_DEFINE_ENUM(LLDD_SUBACCESS_TYPE_NON_ASSOCIATED);
TRACE_DEFINE_ENUM(LLDD_SUBACCESS_TYPE_NOTHING);

#define lldd_state_id_name(name)                                     \
	{                                                            \
		LLDD_STATE_ID_##name, #name                          \
	}
#define LLDD_STATE_ID_SYMBOLS                                        \
	lldd_state_id_name(STANDBY),                                 \
	lldd_state_id_name(SCANNING),                                \
	lldd_state_id_name(TRACKING),                                \
	lldd_state_id_name(ASSOCIATED_ACTIVE),                       \
	lldd_state_id_name(ASSOCIATED_INACTIVE)
TRACE_DEFINE_ENUM(LLDD_STATE_ID_STANDBY);
TRACE_DEFINE_ENUM(LLDD_STATE_ID_SCANNING);
TRACE_DEFINE_ENUM(LLDD_STATE_ID_TRACKING);
TRACE_DEFINE_ENUM(LLDD_STATE_ID_ASSOCIATED_ACTIVE);
TRACE_DEFINE_ENUM(LLDD_STATE_ID_ASSOCIATED_INACTIVE);

#define lld_next_frame_type_name(name)                               \
	{                                                            \
		LLD_NEXT_FRAME_TYPE_##name, #name                    \
	}
#define LLD_NEXT_FRAME_TYPE_NAMES                                    \
	lld_next_frame_type_name(NOTHING),                           \
	lld_next_frame_type_name(AUX),                             \
	lld_next_frame_type_name(DATA_BURST),                        \
	lld_next_frame_type_name(NON_ASSOCIATED)
TRACE_DEFINE_ENUM(LLD_NEXT_FRAME_TYPE_NOTHING);
TRACE_DEFINE_ENUM(LLD_NEXT_FRAME_TYPE_AUX);
TRACE_DEFINE_ENUM(LLD_NEXT_FRAME_TYPE_DATA_BURST);
TRACE_DEFINE_ENUM(LLD_NEXT_FRAME_TYPE_NON_ASSOCIATED);

TRACE_EVENT(
	region_lldd_params,
	TP_PROTO(const struct lldd_params *params),
	TP_ARGS(params),
	TP_STRUCT__entry(
		__field(int, agg_limit_bytes)
		__field(int, inactive_exponent)
		__field(bool, smart_power)
		__field(int, tracking_loss_threshold)
		),
	TP_fast_assign(
		__entry->agg_limit_bytes = params->agg_limit_bytes;
		__entry->inactive_exponent = params->inactive_exponent;
		__entry->smart_power = params->smart_power;
		__entry->tracking_loss_threshold = params->tracking_loss_threshold;
		),
	TP_printk("agg_limit_bytes=%d inactive_exponent=%d smart_power=%d "
		  "tracking_loss_threshold=%d",
		  __entry->agg_limit_bytes,
		  __entry->inactive_exponent,
		  __entry->smart_power,
		  __entry->tracking_loss_threshold
		)
	);

DECLARE_EVENT_CLASS(
	region_lldd_rx_frame,
	TP_PROTO(enum mcps802154_rx_error_type error),
	TP_ARGS(error),
	TP_STRUCT__entry(
		__field(enum mcps802154_rx_error_type, error)
		),
	TP_fast_assign(
		__entry->error = error;
		),
	TP_printk("error=%s",
		  __print_symbolic(__entry->error,
				   MCPS802154_TX_REASON_NAMES)
		 )
	);

DEFINE_EVENT(
	region_lldd_rx_frame, region_lldd_rx_frame_subblock,
	TP_PROTO(enum mcps802154_rx_error_type error),
	TP_ARGS(error)
	);

DEFINE_EVENT(
	region_lldd_rx_frame, region_lldd_rx_frame_scanning,
	TP_PROTO(enum mcps802154_rx_error_type error),
	TP_ARGS(error)
	);

DEFINE_EVENT(
	region_lldd_rx_frame, region_lldd_rx_frame_non_associated,
	TP_PROTO(enum mcps802154_rx_error_type error),
	TP_ARGS(error)
	);

#define DL_INFO(member) \
	__entry->member = dl_info->member
TRACE_EVENT(
	region_lldd_rx_frame_dl,
	TP_PROTO(const struct lldd_frame_dl_info *dl_info),
	TP_ARGS(dl_info),
	TP_STRUCT__entry(
		__field(uint32_t, block_index)
		__field(uint32_t, timestamp_dtu)
		__field(int, n_ul_slots)
		__field(bool, ack)
		__field(bool, aux_subblock)
		__field(enum lld_next_frame_type, next_frame_type)
		__field(int, next_frame_repetition)
		__field(int, next_frame_ifs_dtu)
		__field(bool, next_sb_combined_with_cap)
		__field(bool, next_sb_ds_twr_final)
		),
	TP_fast_assign(
		DL_INFO(block_index);
		DL_INFO(timestamp_dtu);
		DL_INFO(n_ul_slots);
		DL_INFO(ack);
		DL_INFO(aux_subblock);
		DL_INFO(next_frame_type);
		DL_INFO(next_frame_repetition);
		DL_INFO(next_frame_ifs_dtu);
		DL_INFO(next_sb_combined_with_cap);
		DL_INFO(next_sb_ds_twr_final);
		),
	TP_printk("block_index=%u timestamp_dtu=%#x n_ul_slots=%d "
		  "ack=%s aux_subblock=%s next_frame_type=%s "
		  "next_frame_repetition=%d next_frame_ifs_dtu=%d "
		  "next_sb_combined_with_cap=%s next_sb_ds_twr_final=%s",
		  __entry->block_index, __entry->timestamp_dtu,
		  __entry->n_ul_slots, print_boolean(__entry->ack),
		  print_boolean(__entry->aux_subblock),
		  __print_symbolic(__entry->next_frame_type,
				   LLD_NEXT_FRAME_TYPE_NAMES),
		  __entry->next_frame_repetition, __entry->next_frame_ifs_dtu,
		  print_boolean(__entry->next_sb_combined_with_cap),
		  print_boolean(__entry->next_sb_ds_twr_final)
		)
	);
#undef DL_INFO

#define NETWORK_PARAMS(member) \
	__entry->member = beacon_info->network_params.member
TRACE_EVENT(
	region_lldd_fsm_scanning_track_found,
	TP_PROTO(const struct lldd_frame_beacon_info *beacon_info,
		 uint32_t beacon_timestamp_dtu),
	TP_ARGS(beacon_info, beacon_timestamp_dtu),
	TP_STRUCT__entry(
		__field(uint32_t, beacon_timestamp_dtu)
		__field(uint32_t, block_index)
		__field(uint32_t, distance_dtu)
		__field(int, block_dtu)
		__field(int, cap_max_be)
		__field(int, cap_min_be)
		__field(bool, fast_ds_twr)
		__field(int, sb_cap_slot_dtu)
		__field(int, sb_dl_slot_dtu)
		__field(int, sb_min_dtu)
		__field(int, sb_aux_max)
		__field(int, sb_ul_slot_last_dtu)
		__field(int, sb_ul_slot_dtu)
		),
	TP_fast_assign(
		__entry->beacon_timestamp_dtu = beacon_timestamp_dtu;
		__entry->block_index = beacon_info->block_index;
		__entry->distance_dtu = beacon_info->distance_dtu;
		NETWORK_PARAMS(block_dtu);
		NETWORK_PARAMS(cap_max_be);
		NETWORK_PARAMS(cap_min_be);
		NETWORK_PARAMS(fast_ds_twr);
		NETWORK_PARAMS(sb_cap_slot_dtu);
		NETWORK_PARAMS(sb_dl_slot_dtu);
		NETWORK_PARAMS(sb_min_dtu);
		NETWORK_PARAMS(sb_aux_max);
		NETWORK_PARAMS(sb_ul_slot_last_dtu);
		NETWORK_PARAMS(sb_ul_slot_dtu);
		),
	TP_printk("beacon_timestamp_dtu=%#x "
		  "block_index=%u distance_dtu=%d block_dtu=%d "
		  "cap_max_be=%#x cap_min_be=%#x fast_ds_twr=%s "
		  "sb_cap_slot_dtu=%#x sb_dl_slot_dtu=%#x sb_min_dtu=%#x "
		  "sb_aux_max=%#x sb_ul_slot_last_dtu=%#x "
		  "sb_ul_slot_dtu=%#x",
		  __entry->beacon_timestamp_dtu,
		  __entry->block_index, __entry->distance_dtu,
		  __entry->block_dtu, __entry->cap_max_be,
		  __entry->cap_min_be, print_boolean(__entry->fast_ds_twr),
		  __entry->sb_cap_slot_dtu, __entry->sb_dl_slot_dtu,
		  __entry->sb_min_dtu, __entry->sb_aux_max,
		  __entry->sb_ul_slot_last_dtu, __entry->sb_ul_slot_dtu)
	);
#undef NETWORK_PARAMS

TRACE_EVENT(
	region_lldd_tx_return_subblock,
	TP_PROTO(enum mcps802154_access_tx_return_reason reason),
	TP_ARGS(reason),
	TP_STRUCT__entry(
		__field(enum mcps802154_access_tx_return_reason, reason)
		),
	TP_fast_assign(
		__entry->reason = reason;
		),
	TP_printk("reason=%s",
		  __print_symbolic(__entry->reason,
				   MCPS802154_TX_REASON_NAMES)
		 )
	);

TRACE_EVENT(
	region_lldd_tx_return_additional_cap,
	TP_PROTO(enum mcps802154_access_tx_return_reason reason),
	TP_ARGS(reason),
	TP_STRUCT__entry(
		__field(enum mcps802154_access_tx_return_reason, reason)
		),
	TP_fast_assign(
		__entry->reason = reason;
		),
	TP_printk("reason=%s",
		  __print_symbolic(__entry->reason,
				   MCPS802154_TX_REASON_NAMES)
		 )
	);

TRACE_EVENT(
	region_lldd_fsm_change_state,
	TP_PROTO(enum lldd_state_id new_state_id),
	TP_ARGS(new_state_id),
	TP_STRUCT__entry(
		__field(enum lldd_state_id, new_state_id)
		),
	TP_fast_assign(
		__entry->new_state_id = new_state_id;
		),
	TP_printk("new_state_id=%s",
		  __print_symbolic(__entry->new_state_id, LLDD_STATE_ID_SYMBOLS)
		)
	);

TRACE_EVENT(
	region_lldd_fsm_common_get_access,
	TP_PROTO(enum lldd_state_id state_id, uint32_t block_index, uint32_t subaccess_timestamp_dtu),
	TP_ARGS(state_id, block_index, subaccess_timestamp_dtu),
	TP_STRUCT__entry(
		__field(enum lldd_state_id, state_id)
		__field(uint32_t, block_index)
		__field(uint32_t, subaccess_timestamp_dtu)
		),
	TP_fast_assign(
		__entry->state_id = state_id;
		__entry->block_index = block_index;
		__entry->subaccess_timestamp_dtu = subaccess_timestamp_dtu;
		),
	TP_printk("state_id=%s block_index=%u subaccess_timestamp_dtu=%#x",
		  __print_symbolic(__entry->state_id, LLDD_STATE_ID_SYMBOLS),
		  __entry->block_index,
		  __entry->subaccess_timestamp_dtu
		)
	);

TRACE_EVENT(
	region_lldd_get_access_scanning,
	TP_PROTO(uint32_t next_timestamp_dtu),
	TP_ARGS(next_timestamp_dtu),
	TP_STRUCT__entry(
		__field(uint32_t, next_timestamp_dtu)
		),
	TP_fast_assign(
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		),
	TP_printk("next_timestamp_dtu=%#x",
		  __entry->next_timestamp_dtu
		)
	);

TRACE_EVENT(
	region_lldd_access_extend_subblock,
	TP_PROTO(enum lldd_subaccess_type subaccess_type,
		 uint32_t subaccess_timestamp_dtu),
	TP_ARGS(subaccess_type, subaccess_timestamp_dtu),
	TP_STRUCT__entry(
		__field(enum lldd_subaccess_type, subaccess_type)
		__field(uint32_t, subaccess_timestamp_dtu)
		),
	TP_fast_assign(
		__entry->subaccess_type = subaccess_type;
		__entry->subaccess_timestamp_dtu = subaccess_timestamp_dtu;
		),
	TP_printk("subaccess_type=%s subaccess_timestamp_dtu=%#x",
		  __print_symbolic(__entry->subaccess_type,
				   LLDD_SUBACCESS_TYPE_SYMBOLS),
		  __entry->subaccess_timestamp_dtu
		 )
	);

TRACE_EVENT(
	region_lldd_access_done,
	TP_PROTO(uint32_t subaccess_timestamp_dtu, int access_duration_dtu,
		 enum mcps802154_access_done_reason reason),
	TP_ARGS(subaccess_timestamp_dtu, access_duration_dtu, reason),
	TP_STRUCT__entry(
		__field(uint32_t, subaccess_timestamp_dtu)
		__field(int, access_duration_dtu)
		__field(enum mcps802154_access_done_reason, reason)
		),
	TP_fast_assign(
		__entry->subaccess_timestamp_dtu = subaccess_timestamp_dtu;
		__entry->access_duration_dtu = access_duration_dtu;
		__entry->reason = reason;
		),
	TP_printk("subaccess_timestamp_dtu=%#x access_duration_dtu=%d error=%s",
		  __entry->subaccess_timestamp_dtu,
		  __entry->access_duration_dtu,
		  __print_symbolic(__entry->reason,
				   MCPS802154_ACCESS_DONE_REASON_NAMES)
		)
	);

TRACE_EVENT(
	region_lldd_received_block_struct_change,
	TP_PROTO(int index_offset, int n_ul_slots, int start_offset_dtu),
	TP_ARGS(index_offset, n_ul_slots, start_offset_dtu),
	TP_STRUCT__entry(
		__field(int, index_offset)
		__field(int, n_ul_slots)
		__field(int, start_offset_dtu)
		),
	TP_fast_assign(
		__entry->index_offset = index_offset;
		__entry->n_ul_slots = n_ul_slots;
		__entry->start_offset_dtu = start_offset_dtu;
		),
	TP_printk("index_offset=%d n_ul_slots=%d start_offset_dtu=%d",
		  __entry->index_offset,
		  __entry->n_ul_slots,
		  __entry->start_offset_dtu
		)
);

TRACE_EVENT(
	region_lldd_apply_block_struct_change,
	TP_PROTO(const struct lld_block_struct_change *info,
		 int ul_slot),
	TP_ARGS(info, ul_slot),
	TP_STRUCT__entry(
		__field(int, ul_slot)
		__field(int, n_ul_slots)
		__field(int, block_dtu)
		__field(int, sb_aux_max)
		__field(int, sb_ul_slot_dtu)
		__field(int, sb_ul_slot_last_dtu)
		__field(int, sb_dl_slot_dtu)
		__field(int, sb_min_dtu)
		__field(int, sb_cap_slot_dtu)
		),
	TP_fast_assign(
		__entry->ul_slot = ul_slot;
		__entry->n_ul_slots = info->n_ul_slots;
		__entry->block_dtu = info->network_params.block_dtu;
		__entry->sb_aux_max = info->network_params.sb_aux_max;
		__entry->sb_ul_slot_dtu = info->network_params.sb_ul_slot_dtu;
		__entry->sb_ul_slot_last_dtu = info->network_params.sb_ul_slot_last_dtu;
		__entry->sb_dl_slot_dtu = info->network_params.sb_dl_slot_dtu;
		__entry->sb_min_dtu = info->network_params.sb_min_dtu;
		__entry->sb_cap_slot_dtu = info->network_params.sb_cap_slot_dtu;
		),
	TP_printk("ul_slot=%d n_ul_slots=%d block_dtu=%d "
		  "sb_aux_max=%d sb_ul_slot_dtu=%d sb_ul_slot_last_dtu=%d "
		  "sb_dl_slot_dtu=%d sb_min_dtu=%d sb_cap_slot_dtu=%d ",
		  __entry->ul_slot,
		  __entry->n_ul_slots,
		  __entry->block_dtu,
		  __entry->sb_aux_max,
		  __entry->sb_ul_slot_dtu,
		  __entry->sb_ul_slot_last_dtu,
		  __entry->sb_dl_slot_dtu,
		  __entry->sb_min_dtu,
		  __entry->sb_cap_slot_dtu)
	);

/* clang-format on */

#endif /* !LLDD_TRACE_H || TRACE_HEADER_MULTI_READ */

#undef TRACE_INCLUDE_PATH
/* clang-format off */
#define TRACE_INCLUDE_PATH lld/device
/* clang-format on */
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE lldd_trace
#include "trace/define_trace.h"
