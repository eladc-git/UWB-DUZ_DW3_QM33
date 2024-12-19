/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM mcps802154_region_fbs

#if !defined(FBS_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define FBS_TRACE_H

#include "fbs/fbs_session.h"
#include "fbs/fbs_session_runtime.h"
#include "linux/tracepoint.h"
#include "net/fbs_region_nl.h"
#include "trace_common.h"

/* clang-format off */

#define FBS_SESSION_ENTRY __field(uint32_t, handle)
#define FBS_SESSION_ASSIGN __entry->handle = fbs_session->handle
#define FBS_SESSION_PR_FMT "handle=%u"
#define FBS_SESSION_PR_ARG __entry->handle

#define fbs_call_name(name)                                                    \
	{ FBS_CALL_##name, #name }
#define FBS_CALL_NAMES                                                         \
	fbs_call_name(GET_CAPABILITIES),                                       \
	fbs_call_name(SESSION_INIT),                                           \
	fbs_call_name(SESSION_START),                                          \
	fbs_call_name(SESSION_STOP),                                           \
	fbs_call_name(SESSION_DEINIT),                                         \
	fbs_call_name(SESSION_SET_PARAMS),                                     \
	fbs_call_name(NEW_CONTROLEE),                                          \
	fbs_call_name(DEL_CONTROLEE),                                          \
	fbs_call_name(SESSION_NOTIFICATION),                                   \
	fbs_call_name(SESSION_GET_PARAMS),                                     \
	fbs_call_name(SESSION_GET_STATE),                                      \
	fbs_call_name(SESSION_GET_COUNT),                                      \
	fbs_call_name(GET_CONTROLEES),                                         \
	fbs_call_name(UPDATE_DT_ANCHOR_RANGING_ROUNDS),                        \
	fbs_call_name(UPDATE_DT_TAG_RANGING_ROUNDS),                           \
	fbs_call_name(DATA_MESSAGE_SND),                                       \
	fbs_call_name(DATA_MESSAGE_RCV),                                       \
	fbs_call_name(DATA_SIZE_IN_RANGING),                                   \
	fbs_call_name(SESSION_STATUS_NOTIFICATION),                            \
	fbs_call_name(SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NOTIFICATION),   \
	fbs_call_name(GET_RANGING_COUNT),   \
	fbs_call_name(SESSION_SET_HUS_CONTROLLER_CONFIG),  \
	fbs_call_name(SESSION_SET_HUS_CONTROLEE_CONFIG)
TRACE_DEFINE_ENUM(FBS_CALL_GET_CAPABILITIES);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_INIT);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_START);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_STOP);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_DEINIT);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_SET_PARAMS);
TRACE_DEFINE_ENUM(FBS_CALL_NEW_CONTROLEE);
TRACE_DEFINE_ENUM(FBS_CALL_DEL_CONTROLEE);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_NOTIFICATION);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_GET_PARAMS);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_GET_STATE);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_GET_COUNT);
TRACE_DEFINE_ENUM(FBS_CALL_GET_CONTROLEES);
TRACE_DEFINE_ENUM(FBS_CALL_UPDATE_DT_ANCHOR_RANGING_ROUNDS);
TRACE_DEFINE_ENUM(FBS_CALL_UPDATE_DT_TAG_RANGING_ROUNDS);
TRACE_DEFINE_ENUM(FBS_CALL_DATA_MESSAGE_SND);
TRACE_DEFINE_ENUM(FBS_CALL_DATA_MESSAGE_RCV);
TRACE_DEFINE_ENUM(FBS_CALL_DATA_SIZE_IN_RANGING);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_STATUS_NOTIFICATION);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NOTIFICATION);
TRACE_DEFINE_ENUM(FBS_CALL_GET_RANGING_COUNT);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_SET_HUS_CONTROLLER_CONFIG);
TRACE_DEFINE_ENUM(FBS_CALL_SESSION_SET_HUS_CONTROLEE_CONFIG);

#define fbs_session_state_name(name)                                           \
	{ QUWBS_FBS_SESSION_STATE_##name, #name }
#define QUWBS_FBS_SESSION_STATE_NAMES                                                  \
	fbs_session_state_name(INIT),                                          \
	fbs_session_state_name(DEINIT),                                        \
	fbs_session_state_name(ACTIVE),                                        \
	fbs_session_state_name(IDLE)

#define fbs_reason_code_name(name)                                             \
	{ QUWBS_FBS_REASON_CODE_##name, #name }
#define QUWBS_FBS_REASON_CODE_NAMES                                                  \
	fbs_reason_code_name(STATE_CHANGE_WITH_SESSION_MANAGEMENT_COMMANDS),   \
	fbs_reason_code_name(MAX_RANGING_ROUND_RETRY_COUNT_REACHED),           \
	fbs_reason_code_name(MAX_NUMBER_OF_MEASUREMENTS_REACHED),              \
	fbs_reason_code_name(SESSION_SUSPENDED_DUE_TO_INBAND_SIGNAL),          \
	fbs_reason_code_name(SESSION_RESUMED_DUE_TO_INBAND_SIGNAL),            \
	fbs_reason_code_name(SESSION_STOPPED_DUE_TO_INBAND_SIGNAL),            \
	fbs_reason_code_name(ERROR_INVALID_UL_TDOA_RANDOM_WINDOW),             \
	fbs_reason_code_name(ERROR_MIN_FRAMES_PER_RR_NOT_SUPPORTED),           \
	fbs_reason_code_name(ERROR_INTER_FRAME_INTERVAL_NOT_SUPPORTED),        \
	fbs_reason_code_name(ERROR_SLOT_LENGTH_NOT_SUPPORTED),                 \
	fbs_reason_code_name(ERROR_INSUFFICIENT_SLOTS_PER_RR),                 \
	fbs_reason_code_name(ERROR_MAC_ADDRESS_MODE_NOT_SUPPORTED),            \
	fbs_reason_code_name(ERROR_INVALID_RANGING_DURATION),                  \
	fbs_reason_code_name(ERROR_INVALID_STS_CONFIG),                        \
	fbs_reason_code_name(ERROR_INVALID_RFRAME_CONFIG),                     \
	fbs_reason_code_name(ERROR_HUS_NOT_ENOUGH_SLOTS),                      \
	fbs_reason_code_name(ERROR_HUS_CFP_PHASE_TOO_SHORT),                   \
	fbs_reason_code_name(ERROR_HUS_CAP_PHASE_TOO_SHORT),                   \
	fbs_reason_code_name(ERROR_HUS_OTHERS),                                \
	fbs_reason_code_name(ERROR_STATUS_SESSION_KEY_NOT_FOUND),              \
	fbs_reason_code_name(ERROR_STATUS_SUB_SESSION_KEY_NOT_FOUND),          \
	fbs_reason_code_name(ERROR_INVALID_PREAMBLE_CODE_INDEX),               \
	fbs_reason_code_name(ERROR_INVALID_SFD_ID),                            \
	fbs_reason_code_name(ERROR_INVALID_PSDU_DATA_RATE),                    \
	fbs_reason_code_name(ERROR_INVALID_PHR_DATA_RATE),                     \
	fbs_reason_code_name(ERROR_INVALID_PREAMBLE_DURATION),                 \
	fbs_reason_code_name(ERROR_INVALID_STS_LENGTH),                        \
	fbs_reason_code_name(ERROR_INVALID_NUM_OF_STS_SEGMENTS),               \
	fbs_reason_code_name(ERROR_INVALID_NUM_OF_CONTROLEES),                 \
	fbs_reason_code_name(ERROR_MAX_RANGING_REPLY_TIME_EXCEEDED),           \
	fbs_reason_code_name(ERROR_INVALID_DST_ADDRESS_LIST),                  \
	fbs_reason_code_name(ERROR_INVALID_OR_NOT_FOUND_SUB_SESSION_ID),       \
	fbs_reason_code_name(ERROR_INVALID_RESULT_REPORT_CONFIG),              \
	fbs_reason_code_name(ERROR_INVALID_RANGING_ROUND_CONTROL_CONFIG),      \
	fbs_reason_code_name(ERROR_INVALID_RANGING_ROUND_USAGE),               \
	fbs_reason_code_name(ERROR_INVALID_MULTI_NODE_MODE),                   \
	fbs_reason_code_name(ERROR_RDS_FETCH_FAILURE),                         \
	fbs_reason_code_name(ERROR_REF_UWB_SESSION_DOES_NOT_EXIST),            \
	fbs_reason_code_name(ERROR_REF_UWB_SESSION_RANGING_DURATION_MISMATCH), \
	fbs_reason_code_name(ERROR_REF_UWB_SESSION_INVALID_OFFSET_TIME),       \
	fbs_reason_code_name(ERROR_REF_UWB_SESSION_LOST),                      \
	fbs_reason_code_name(ERROR_DT_ANCHOR_RANGING_ROUNDS_NOT_CONFIGURED),   \
	fbs_reason_code_name(ERROR_DT_TAG_RANGING_ROUNDS_NOT_CONFIGURED),      \
	fbs_reason_code_name(ERROR_UWB_INITIATION_TIME_EXPIRED),               \
	fbs_reason_code_name(AOSP_ERROR_INVALID_CHANNEL_WITH_AOA),             \
	fbs_reason_code_name(AOSP_ERROR_STOPPED_DUE_TO_OTHER_SESSION_CONFLICT),\
	fbs_reason_code_name(AOSP_REGULATION_UWB_OFF),                         \
	fbs_reason_code_name(ERROR_RADAR_MEASUREMENT_TIME_REACHED),            \
	fbs_reason_code_name(ERROR_INVALID_DEVICE_ROLE),                       \
	fbs_reason_code_name(ERROR_NOMEM),                                     \
	fbs_reason_code_name(ERROR_DRIVER_DOWN),                               \
	fbs_reason_code_name(ERROR_INVALID_PROXIMITY_RANGE),                   \
	fbs_reason_code_name(ERROR_INVALID_FRAME_INTERVAL),                    \
	fbs_reason_code_name(ERROR_INVALID_CAP_SIZE_RANGE),                    \
	fbs_reason_code_name(ERROR_INVALID_SCHEDULE_MODE),                     \
	fbs_reason_code_name(ERROR_INVALID_PRF_MODE),                          \
	fbs_reason_code_name(ERROR_START_CONFIG),                              \
	fbs_reason_code_name(ERROR_RDS_BUSY)


TRACE_EVENT(
	region_fbs_build_block_start_dtu_return,
	TP_PROTO(const struct fbs_session *fbs_session,
		 const struct fbs_session_runtime *runtime,
		 int32_t diff_dtu),
	TP_ARGS(fbs_session, runtime, diff_dtu),
	TP_STRUCT__entry(
		FBS_SESSION_ENTRY
		__field(bool, block_start_valid)
		__field(uint32_t, block_start_dtu)
		__field(int32_t, diff_dtu)
		),
	TP_fast_assign(
		FBS_SESSION_ASSIGN;
		__entry->block_start_valid = runtime->block_start_valid;
		__entry->block_start_dtu = runtime->block_start_dtu;
		__entry->diff_dtu = diff_dtu;
		),
	TP_printk(
		FBS_SESSION_PR_FMT " block_start_valid=%s block_start_dtu=%#x diff_dtu=%d",
		FBS_SESSION_PR_ARG,
		print_boolean(__entry->block_start_valid),
		__entry->block_start_dtu,
		__entry->diff_dtu
		)
	);

TRACE_EVENT(region_fbs_call,
	TP_PROTO(enum fbs_call id),
	TP_ARGS(id),
	TP_STRUCT__entry(
		__field(enum fbs_call, id)
		),
	TP_fast_assign(
		__entry->id = id;
		),
	TP_printk(
		"id=%s",
		__print_symbolic(__entry->id, FBS_CALL_NAMES)
		)
	);

TRACE_EVENT(region_fbs_call_return,
	TP_PROTO(int r),
	TP_ARGS(r),
	TP_STRUCT__entry(
		__field(int, r)
		),
	TP_fast_assign(
		__entry->r = r;
		),
	TP_printk(
		"r=%d",
		__entry->r
		)
	);

TRACE_EVENT(region_fbs_get_demand,
	TP_PROTO(uint32_t next_timestamp_dtu, int max_duration_dtu),
	TP_ARGS(next_timestamp_dtu, max_duration_dtu),
	TP_STRUCT__entry(
		__field(uint32_t, next_timestamp_dtu)
		__field(int, max_duration_dtu)
		),
	TP_fast_assign(
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		__entry->max_duration_dtu = max_duration_dtu;
		),
	TP_printk(
		"next_timestamp_dtu=%#x max_duration_dtu=%d",
		__entry->next_timestamp_dtu,
		__entry->max_duration_dtu
		)
	);

TRACE_EVENT(region_fbs_get_access,
	TP_PROTO(uint32_t next_timestamp_dtu, int next_in_region_dtu, int region_duration_dtu),
	TP_ARGS(next_timestamp_dtu, next_in_region_dtu, region_duration_dtu),
	TP_STRUCT__entry(
		__field(uint32_t, next_timestamp_dtu)
		__field(int, next_in_region_dtu)
		__field(int, region_duration_dtu)
		),
	TP_fast_assign(
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		__entry->next_in_region_dtu = next_in_region_dtu;
		__entry->region_duration_dtu = region_duration_dtu;
		),
	TP_printk(
		"next_timestamp_dtu=%#x next_in_region_dtu=%d region_duration_dtu=%d",
		__entry->next_timestamp_dtu,
		__entry->next_in_region_dtu,
		__entry->region_duration_dtu
		)
	);

TRACE_EVENT(region_fbs_access_get_access,
	TP_PROTO(const struct fbs_session *fbs_session,
		 const struct fbs_session_demand *fsd),
	TP_ARGS(fbs_session, fsd),
	TP_STRUCT__entry(
		FBS_SESSION_ENTRY
		__field(uint32_t, block_index)
		__field(uint32_t, block_start_dtu)
		__field(uint32_t, timestamp_dtu)
		__field(int, max_duration_dtu)
		),
	TP_fast_assign(
		FBS_SESSION_ASSIGN;
		__entry->block_index = fbs_session->runtime->block_index;
		__entry->block_start_dtu = fsd->block_start_dtu;
		__entry->timestamp_dtu = fsd->timestamp_dtu;
		__entry->max_duration_dtu = fsd->max_duration_dtu;
		),
	TP_printk(
		FBS_SESSION_PR_FMT " "
		" block_index=%u block_start_dtu=%#x"
		" timestamp_dtu=%#x max_duration_dtu=%d",
		FBS_SESSION_PR_ARG,
		__entry->block_index,
		__entry->block_start_dtu,
		__entry->timestamp_dtu,
		__entry->max_duration_dtu
		)
	);

TRACE_EVENT(
	region_fbs_access_done_return,
	TP_PROTO(const struct fbs_session *fbs_session,
		 const struct mcps802154_access *access,
		 enum mcps802154_access_done_reason reason),
	TP_ARGS(fbs_session, access, reason),
	TP_STRUCT__entry(
		FBS_SESSION_ENTRY
		__field(uint32_t, at)
		__field(int, ad)
		__field(enum mcps802154_access_done_reason, reason)
		),
	TP_fast_assign(
		FBS_SESSION_ASSIGN;
		__entry->at = access->timestamp_dtu;
		__entry->ad = access->duration_dtu;
		__entry->reason = reason;
		),
	TP_printk(
		FBS_SESSION_PR_FMT " at=%#x ad=%d reason=%s",
		FBS_SESSION_PR_ARG,
		__entry->at,
		__entry->ad,
		__print_symbolic(__entry->reason,
				 MCPS802154_ACCESS_DONE_REASON_NAMES)
		)
	);

TRACE_EVENT(
	region_fbs_session_fsd_rejected,
	TP_PROTO(const struct fbs_session *fbs_session,
		 uint32_t next_timestamp_dtu, int max_duration_dtu,
		 const struct fbs_session_demand *fsd),
	TP_ARGS(fbs_session, next_timestamp_dtu, max_duration_dtu, fsd),
	TP_STRUCT__entry(
		FBS_SESSION_ENTRY
		__field(uint32_t, block_index)
		__field(uint32_t, next_timestamp_dtu)
		__field(uint32_t, max_duration_dtu)
		__field(uint32_t, fsd_block_start_dtu)
		__field(uint32_t, fsd_timestamp_dtu)
		__field(int, fsd_max_duration_dtu)
		__field(int, fsd_add_blocks)
		__field(int, fsd_round_index)
		__field(int, fsd_rx_timeout_dtu)
		),
	TP_fast_assign(
		FBS_SESSION_ASSIGN;
		__entry->block_index = fbs_session->runtime->block_index;
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		__entry->max_duration_dtu = max_duration_dtu;
		__entry->fsd_block_start_dtu = fsd->block_start_dtu;
		__entry->fsd_timestamp_dtu = fsd->timestamp_dtu;
		__entry->fsd_max_duration_dtu = fsd->max_duration_dtu;
		__entry->fsd_add_blocks = fsd->add_blocks;
		__entry->fsd_round_index = fsd->round_index;
		__entry->fsd_rx_timeout_dtu = fsd->rx_timeout_dtu;
		),
	TP_printk(
		FBS_SESSION_PR_FMT " block_index=%u"
		" next_timestamp_dtu=%#x max_duration_dtu=%d"
		" fsd_block_start_dtu=%#x fsd_timestamp_dtu=%#x"
		" fsd_max_duration_dtu=%d fsd_add_blocks=%d"
		" fsd_round_index=%d fsd_rx_timeout_dtu=%d",
		FBS_SESSION_PR_ARG,
		__entry->block_index,
		__entry->next_timestamp_dtu,
		__entry->max_duration_dtu,
		__entry->fsd_block_start_dtu,
		__entry->fsd_timestamp_dtu,
		__entry->fsd_max_duration_dtu,
		__entry->fsd_add_blocks,
		__entry->fsd_round_index,
		__entry->fsd_rx_timeout_dtu
		)
	);

TRACE_EVENT(
	region_fbs_session_status_ntf,
	TP_PROTO(const struct fbs_session *fbs_session,
		 enum quwbs_fbs_reason_code reason_code),
	TP_ARGS(fbs_session, reason_code),
	TP_STRUCT__entry(
		FBS_SESSION_ENTRY
		__field(enum quwbs_fbs_session_state, state)
		__field(enum quwbs_fbs_reason_code, reason_code)
		),
	TP_fast_assign(
		FBS_SESSION_ASSIGN;
		__entry->state = fbs_session->state;
		__entry->reason_code = reason_code;
		),
	TP_printk(
		FBS_SESSION_PR_FMT " state=%s reason_code=%s",
		FBS_SESSION_PR_ARG,
		__print_symbolic(__entry->state,
				 QUWBS_FBS_SESSION_STATE_NAMES),
		__print_symbolic(__entry->reason_code,
				 QUWBS_FBS_REASON_CODE_NAMES)
		)
	);

/* clang-format on */

#endif /* !FBS_TRACE_H || TRACE_HEADER_MULTI_READ */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH fbs
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE fbs_trace
#include "trace/define_trace.h"
