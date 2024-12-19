/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM mcps802154_region_lldc

#if !defined(LLDC_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define LLDC_TRACE_H

#include "linux/tracepoint.h"
#include "lldc_frame.h"
#include "lldc_region.h"
#include "trace_common.h"

/* clang-format off */

#define lldc_state_id_name(name)                                     \
	{                                                            \
		LLDC_STATE_ID_##name, #name                          \
	}
#define LLDC_STATE_ID_SYMBOLS                                        \
	lldc_state_id_name(STANDBY),                                 \
	lldc_state_id_name(SCANNING),                                \
	lldc_state_id_name(PROBING),                                 \
	lldc_state_id_name(ACTIVE)
TRACE_DEFINE_ENUM(LLDC_STATE_ID_STANDBY);
TRACE_DEFINE_ENUM(LLDC_STATE_ID_SCANNING);
TRACE_DEFINE_ENUM(LLDC_STATE_ID_PROBING);
TRACE_DEFINE_ENUM(LLDC_STATE_ID_ACTIVE);

#define lldc_subaccess_type_name(name)                               \
	{                                                            \
		LLDC_SUBACCESS_TYPE_##name, #name                    \
	}
#define LLDC_SUBACCESS_TYPE_SYMBOLS                                  \
	lldc_subaccess_type_name(MAIN),                              \
	lldc_subaccess_type_name(ADDITIONAL_CAP),                    \
	lldc_subaccess_type_name(AUX),                             \
	lldc_subaccess_type_name(DATA_BURST),                        \
	lldc_subaccess_type_name(NON_ASSOCIATED),                    \
	lldc_subaccess_type_name(BEACON),                            \
	lldc_subaccess_type_name(NOTHING)
TRACE_DEFINE_ENUM(LLDC_SUBACCESS_TYPE_MAIN);
TRACE_DEFINE_ENUM(LLDC_SUBACCESS_TYPE_ADDITIONAL_CAP);
TRACE_DEFINE_ENUM(LLDC_SUBACCESS_TYPE_AUX);
TRACE_DEFINE_ENUM(LLDC_SUBACCESS_TYPE_DATA_BURST);
TRACE_DEFINE_ENUM(LLDC_SUBACCESS_TYPE_NON_ASSOCIATED);
TRACE_DEFINE_ENUM(LLDC_SUBACCESS_TYPE_BEACON);
TRACE_DEFINE_ENUM(LLDC_SUBACCESS_TYPE_NOTHING);

#define ARG_ASSIGN(member) \
	__entry->member = member
#define P_ASSIGN(p, member) \
	__entry->member = p->member

DECLARE_EVENT_CLASS(
	region_lldc_rx_frame,
	TP_PROTO(enum mcps802154_rx_error_type error),
	TP_ARGS(error),
	TP_STRUCT__entry(
		__field(enum mcps802154_rx_error_type, error)
		),
	TP_fast_assign(
		ARG_ASSIGN(error);
		),
	TP_printk("error=%s",
		  __print_symbolic(__entry->error,
				   MCPS802154_RX_ERROR_NAMES)
		 )
	);

DECLARE_EVENT_CLASS(
	region_lldc_tx_return,
	TP_PROTO(int frame_idx,
		 enum mcps802154_access_tx_return_reason reason),
	TP_ARGS(frame_idx, reason),
	TP_STRUCT__entry(
		__field(int, frame_idx)
		__field(enum mcps802154_access_tx_return_reason, reason)
		),
	TP_fast_assign(
		ARG_ASSIGN(frame_idx);
		ARG_ASSIGN(reason);
		),
	TP_printk("frame_idx=%d reason=%s",
		  __entry->frame_idx,
		  __print_symbolic(__entry->reason,
				   MCPS802154_TX_REASON_NAMES)
		 )
	);

TRACE_EVENT(
	region_lldc_subaccess_subblock,
	TP_PROTO(uint32_t block_index, int subblock_index, int n_ul_slots),
	TP_ARGS(block_index, subblock_index, n_ul_slots),
	TP_STRUCT__entry(
		__field(uint32_t, block_index)
		__field(int, subblock_index)
		__field(int, n_ul_slots)
		),
	TP_fast_assign(
		ARG_ASSIGN(block_index);
		ARG_ASSIGN(subblock_index);
		ARG_ASSIGN(n_ul_slots);
		),
	TP_printk("block_index=%u subblock_index=%d n_ul_slots=%d",
		  __entry->block_index,
		  __entry->subblock_index,
		  __entry->n_ul_slots
		 )
	);

TRACE_EVENT(
	region_lldc_subaccess_non_associated,
	TP_PROTO(const struct lldc_association_response *association_response),
	TP_ARGS(association_response),
	TP_STRUCT__entry(
		__field(uint64_t, extended_addr)
		),
	TP_fast_assign(
		P_ASSIGN(association_response, extended_addr);
		),
	TP_printk("extended_addr=%#llx",
		  __entry->extended_addr
		 )
	);

TRACE_EVENT(
	region_lldc_subaccess_beacon,
	TP_PROTO(uint32_t beacon_next_dtu),
	TP_ARGS(beacon_next_dtu),
	TP_STRUCT__entry(
		__field(uint32_t, beacon_next_dtu)
		),
	TP_fast_assign(
		ARG_ASSIGN(beacon_next_dtu);
		),
	TP_printk("beacon_next_dtu=%#x",
		  __entry->beacon_next_dtu
		 )
	);

TRACE_EVENT(
	region_lldc_subaccess_additional_cap,
	TP_PROTO(int cap_slot_remaining),
	TP_ARGS(cap_slot_remaining),
	TP_STRUCT__entry(
		__field(int, cap_slot_remaining)
		),
	TP_fast_assign(
		ARG_ASSIGN(cap_slot_remaining);
		),
	TP_printk("cap_slot_remaining=%d",
		  __entry->cap_slot_remaining
		 )
	);

#define LLDC_PARAMS(member) P_ASSIGN(params, member)
#define LLDC_PARAMS_IFS(member) \
	__entry->member = LLD_IFS_TO_INT(params->member)
TRACE_EVENT(
	region_lldc_params,
	TP_PROTO(const struct lldc_params *params),
	TP_ARGS(params),
	TP_STRUCT__entry(
		__field(bool, allow_pairing)
		__field(int, agg_limit_bytes)
		__field(int, beacon_ifs)
		__field(int, beacon_interval_dtu)
		__field(int, sb_beacon_cap_slot_ifs)
		__field(int, sb_dl_cap_slot_ifs)
		__field(int, cap_interval_max_dtu)
		__field(int, cap_slots_per_second)
		__field(int, non_assoc_ifs)
		__field(bool, probing_beacon_as_dl)
		__field(int, probing_cap_slots)
		__field(int, probing_interval_dtu)
		__field(bool, smart_power)
		__field(size_t, beacon_payload_len)
		__dynamic_array(uint8_t, beacon_payload,
				params->beacon_payload_len)
		),
	TP_fast_assign(
		LLDC_PARAMS(allow_pairing);
		LLDC_PARAMS(agg_limit_bytes);
		LLDC_PARAMS_IFS(beacon_ifs);
		LLDC_PARAMS(beacon_interval_dtu);
		LLDC_PARAMS(cap_interval_max_dtu);
		LLDC_PARAMS_IFS(sb_beacon_cap_slot_ifs);
		LLDC_PARAMS_IFS(sb_dl_cap_slot_ifs);
		LLDC_PARAMS(cap_slots_per_second);
		LLDC_PARAMS_IFS(non_assoc_ifs);
		LLDC_PARAMS(probing_beacon_as_dl);
		LLDC_PARAMS(probing_cap_slots);
		LLDC_PARAMS(probing_interval_dtu);
		LLDC_PARAMS(smart_power);
		LLDC_PARAMS(beacon_payload_len);
		memcpy(__get_dynamic_array(beacon_payload),
		       params->beacon_payload,
		       __get_dynamic_array_len(beacon_payload));
		),
	TP_printk("allow_pairing=%d agg_limit_bytes=%d beacon_ifs=%d "
		  "beacon_interval_dtu=%d cap_interval_max_dtu=%d "
		  "sb_beacon_cap_slot_ifs=%d sb_dl_cap_slot_ifs=%d "
		  "cap_slots_per_second=%d non_assoc_ifs=%d "
		  "probing_beacon_as_dl=%d probing_cap_slots=%d "
		  "probing_interval_dtu=%d smart_power=%d "
		  "beacon_payload_len=%zu beacon_payload=%s",
		  __entry->allow_pairing,
		  __entry->agg_limit_bytes,
		  __entry->beacon_ifs,
		  __entry->beacon_interval_dtu,
		  __entry->cap_interval_max_dtu,
		  __entry->sb_beacon_cap_slot_ifs,
		  __entry->sb_dl_cap_slot_ifs,
		  __entry->cap_slots_per_second,
		  __entry->non_assoc_ifs,
		  __entry->probing_beacon_as_dl,
		  __entry->probing_cap_slots,
		  __entry->probing_interval_dtu,
		  __entry->smart_power,
		  __entry->beacon_payload_len,
		  __print_hex(__get_dynamic_array(beacon_payload),
			      __get_dynamic_array_len(beacon_payload))
		)
	);
#undef LLDC_PARAMS_IFS
#undef LLDC_PARAMS

DEFINE_EVENT(
	region_lldc_rx_frame,
	region_lldc_rx_frame_subblock,
	TP_PROTO(enum mcps802154_rx_error_type error),
	TP_ARGS(error)
	);

DEFINE_EVENT(
	region_lldc_rx_frame,
	region_lldc_rx_frame_additional_cap,
	TP_PROTO(enum mcps802154_rx_error_type error),
	TP_ARGS(error)
	);

TRACE_EVENT(
	region_lldc_rx_frame_ul,
	TP_PROTO(int device_index,
		 const struct lldc_frame_ul_info *ul_info),
	TP_ARGS(device_index, ul_info),
	TP_STRUCT__entry(
		__field(int, device_index)
		__array(uint8_t, mfs_init, LLD_MFS_ID_NB)
		__array(uint8_t, mfs_next_expected_seq, LLD_MFS_ID_NB)
		),
	TP_fast_assign(
		int i;

		__entry->device_index = device_index;
		for (i = 0; i < LLD_MFS_ID_NB; i++)
			__entry->mfs_init[i] = !!ul_info->mfs_init[i];
		*__entry->mfs_next_expected_seq =
		       *ul_info->mfs_next_expected_seq;
		),
	TP_printk("device_index=%d mfs_init=%s mfs_next_expected_seq=%s",
		  __entry->device_index,
		  __print_hex(__entry->mfs_init, LLD_MFS_ID_NB),
		  __print_hex(__entry->mfs_next_expected_seq, LLD_MFS_ID_NB)
		  )
	);

TRACE_EVENT(
	region_lldc_tx_get_frame_beacon,
	TP_PROTO(int distance_dtu),
	TP_ARGS(distance_dtu),
	TP_STRUCT__entry(
		__field(int, distance_dtu)
		),
	TP_fast_assign(
		ARG_ASSIGN(distance_dtu);
		),
	TP_printk("distance_dtu=%d", __entry->distance_dtu)
	);

TRACE_EVENT(
	region_lldc_tx_get_frame_non_associated,
	TP_PROTO(bool ok),
	TP_ARGS(ok),
	TP_STRUCT__entry(
		__field(bool, ok)
		),
	TP_fast_assign(
		ARG_ASSIGN(ok);
		),
	TP_printk("ok=%s",
		  __entry->ok ? "true": "false"
		 )
	);

DEFINE_EVENT(
	region_lldc_tx_return,
	region_lldc_tx_return_beacon,
	TP_PROTO(int frame_idx,
		 enum mcps802154_access_tx_return_reason reason),
	TP_ARGS(frame_idx, reason)
	);

DEFINE_EVENT(
	region_lldc_tx_return,
	region_lldc_tx_return_non_associated,
	TP_PROTO(int frame_idx,
		 enum mcps802154_access_tx_return_reason reason),
	TP_ARGS(frame_idx, reason)
	);

DEFINE_EVENT(
	region_lldc_tx_return,
	region_lldc_tx_return_subblock,
	TP_PROTO(int frame_idx,
		 enum mcps802154_access_tx_return_reason reason),
	TP_ARGS(frame_idx, reason)
	);

TRACE_EVENT(
	region_lldc_access_extend_subblock,
	TP_PROTO(enum lldc_subaccess_type subaccess_type,
		 uint32_t subaccess_timestamp_dtu),
	TP_ARGS(subaccess_type, subaccess_timestamp_dtu),
	TP_STRUCT__entry(
		__field(enum lldc_subaccess_type, subaccess_type)
		__field(uint32_t, subaccess_timestamp_dtu)
		),
	TP_fast_assign(
		ARG_ASSIGN(subaccess_type);
		ARG_ASSIGN(subaccess_timestamp_dtu);
		),
	TP_printk("subaccess_type=%s subaccess_timestamp_dtu=%#x",
		  __print_symbolic(__entry->subaccess_type,
				   LLDC_SUBACCESS_TYPE_SYMBOLS),
		  __entry->subaccess_timestamp_dtu
		 )
	);

TRACE_EVENT(
	region_lldc_fsm_change_state,
	TP_PROTO(enum lldc_state_id new_state_id),
	TP_ARGS(new_state_id),
	TP_STRUCT__entry(
		__field(enum lldc_state_id, new_state_id)
		),
	TP_fast_assign(
		ARG_ASSIGN(new_state_id);
		),
	TP_printk("new_state_id=%s",
		  __print_symbolic(__entry->new_state_id,
				   LLDC_STATE_ID_SYMBOLS)
		)
	);

TRACE_EVENT(
	region_lldc_fsm_active_get_access,
	TP_PROTO(uint32_t block_index, uint32_t subaccess_timestamp_dtu),
	TP_ARGS(block_index, subaccess_timestamp_dtu),
	TP_STRUCT__entry(
		__field(uint32_t, block_index)
		__field(uint32_t, subaccess_timestamp_dtu)
		),
	TP_fast_assign(
		ARG_ASSIGN(block_index);
		ARG_ASSIGN(subaccess_timestamp_dtu);
		),
	TP_printk("block_index=%u subaccess_timestamp_dtu=%#x",
		  __entry->block_index,
		  __entry->subaccess_timestamp_dtu
		)
	);

TRACE_EVENT(
	region_lldc_access_done,
	TP_PROTO(uint32_t subaccess_timestamp_dtu, int access_duration_dtu,
		 enum mcps802154_access_done_reason reason),
	TP_ARGS(subaccess_timestamp_dtu, access_duration_dtu, reason),
	TP_STRUCT__entry(
		__field(uint32_t, subaccess_timestamp_dtu)
		__field(int, access_duration_dtu)
		__field(enum mcps802154_access_done_reason, reason)
		),
	TP_fast_assign(
		ARG_ASSIGN(subaccess_timestamp_dtu);
		ARG_ASSIGN(access_duration_dtu);
		ARG_ASSIGN(reason);
		),
	TP_printk("subaccess_timestamp_dtu=%#x access_duration_dtu=%d reason=%s",
		  __entry->subaccess_timestamp_dtu,
		  __entry->access_duration_dtu,
		  __print_symbolic(__entry->reason,
				   MCPS802154_ACCESS_DONE_REASON_NAMES)
		)
	);

TRACE_EVENT(
	region_lldc_skip,
	TP_PROTO(uint32_t skip_timestamp_dtu, int skip_duration_dtu,
		 const struct lld_block_skip *block_skip),
	TP_ARGS(skip_timestamp_dtu, skip_duration_dtu, block_skip),
	TP_STRUCT__entry(
		__field(uint32_t, skip_timestamp_dtu)
		__field(int, skip_duration_dtu)
		__field(int, block_skip_index)
		__field(int, block_skip_n_blocks)
		),
	TP_fast_assign(
		ARG_ASSIGN(skip_timestamp_dtu);
		ARG_ASSIGN(skip_duration_dtu);
		__entry->block_skip_index = block_skip->index;
		__entry->block_skip_n_blocks = block_skip->n_blocks;
		),
	TP_printk("skip_timestamp_dtu=%#x skip_duration_dtu=%d "
		  "block_skip_index=%d block_skip_n_blocks=%d",
		  __entry->skip_timestamp_dtu,
		  __entry->skip_duration_dtu,
		  __entry->block_skip_index,
		  __entry->block_skip_n_blocks
		)
	);

TRACE_EVENT(
	region_lldc_get_next_cap_repetition,
	TP_PROTO(int cap_repetition, int cap_lost_duration_dtu),
	TP_ARGS(cap_repetition, cap_lost_duration_dtu),
	TP_STRUCT__entry(
		__field(int, cap_repetition)
		__field(int, cap_lost_duration_dtu)
		),
	TP_fast_assign(
		ARG_ASSIGN(cap_repetition);
		ARG_ASSIGN(cap_lost_duration_dtu);
		),
	TP_printk("cap_repetition=%d cap_lost_duration_dtu=%d",
		  __entry->cap_repetition,
		  __entry->cap_lost_duration_dtu
		)
);

TRACE_EVENT(
	region_lldc_block_struct_change,
	TP_PROTO(int index_offset, int n_ul_slots, int start_offset_dtu),
	TP_ARGS(index_offset, n_ul_slots, start_offset_dtu),
	TP_STRUCT__entry(
		__field(int, index_offset)
		__field(int, n_ul_slots)
		__field(int, start_offset_dtu)
		),
	TP_fast_assign(
		ARG_ASSIGN(index_offset);
		ARG_ASSIGN(n_ul_slots);
		ARG_ASSIGN(start_offset_dtu);
		),
	TP_printk("index_offset=%d n_ul_slots=%d start_offset_dtu=%d",
		  __entry->index_offset,
		  __entry->n_ul_slots,
		  __entry->start_offset_dtu
		)
);

#undef P_ASSIGN
#undef ARG_ASSIGN

/* clang-format on */

#endif /* !LLDC_TRACE_H || TRACE_HEADER_MULTI_READ */

#undef TRACE_INCLUDE_PATH
/* clang-format off */
#define TRACE_INCLUDE_PATH lld/coordinator
/* clang-format on */
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE lldc_trace
#include "trace/define_trace.h"
