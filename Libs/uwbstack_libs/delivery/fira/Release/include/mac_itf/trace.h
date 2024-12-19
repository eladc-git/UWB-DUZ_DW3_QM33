/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM mcps802154

#if !defined(TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define TRACE_H

#include "linux/tracepoint.h"
#include "mcps802154_i.h"

/* clang-format off */

#define LOCAL_ENTRY __field(int, hw_idx)
#define LOCAL_ASSIGN __entry->hw_idx = local->mcps_nl.id
#define LOCAL_PR_FMT "hw%d"
#define LOCAL_PR_ARG __entry->hw_idx

#define mcps802154_device_state_name(name)            \
	{                                             \
		MCPS802154_DEVICE_STATE_##name, #name \
	}
#define MCPS802154_DEVICE_STATE_NAMES          \
	mcps802154_device_state_name(STOPPED), \
	mcps802154_device_state_name(STARTED), \
	mcps802154_device_state_name(BROKEN)
TRACE_DEFINE_ENUM(MCPS802154_DEVICE_STATE_STOPPED);
TRACE_DEFINE_ENUM(MCPS802154_DEVICE_STATE_STARTED);
TRACE_DEFINE_ENUM(MCPS802154_DEVICE_STATE_BROKEN);

#define mcps802154_tx_frame_config_name(name)                     \
	{                                                  \
		MCPS802154_TX_FRAME_CONFIG_##name, #name          \
	}
#define MCPS802154_TX_FRAME_CONFIG_NAMES                          \
	mcps802154_tx_frame_config_name(TIMESTAMP_DTU),           \
	mcps802154_tx_frame_config_name(CCA),                     \
	mcps802154_tx_frame_config_name(RANGING),                 \
	mcps802154_tx_frame_config_name(KEEP_RANGING_CLOCK),      \
	mcps802154_tx_frame_config_name(RANGING_PDOA),            \
	mcps802154_tx_frame_config_name(SP1),                     \
	mcps802154_tx_frame_config_name(SP2),                     \
	mcps802154_tx_frame_config_name(SP3)
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_CONFIG_TIMESTAMP_DTU);
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_CONFIG_CCA);
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_CONFIG_RANGING);
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_CONFIG_KEEP_RANGING_CLOCK);
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_CONFIG_RANGING_PDOA);
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_CONFIG_SP1);
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_CONFIG_SP2);
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_CONFIG_SP3);

#define mcps802154_tx_frame_info_name(name)                       \
	{                                                         \
		MCPS802154_TX_FRAME_INFO_##name, #name            \
	}
#define MCPS802154_TX_FRAME_INFO_NAMES                            \
	mcps802154_tx_frame_info_name(WIFI_ACTIVE)
TRACE_DEFINE_ENUM(MCPS802154_TX_FRAME_INFO_WIFI_ACTIVE);

#define mcps802154_rx_frame_config_name(name)                      \
	{                                                  \
		MCPS802154_RX_FRAME_CONFIG_##name, #name   \
	}
#define MCPS802154_RX_FRAME_CONFIG_NAMES                   \
	mcps802154_rx_frame_config_name(TIMESTAMP_DTU),            \
	mcps802154_rx_frame_config_name(AACK),                     \
	mcps802154_rx_frame_config_name(RANGING),                  \
	mcps802154_rx_frame_config_name(KEEP_RANGING_CLOCK),       \
	mcps802154_rx_frame_config_name(RANGING_PDOA),             \
	mcps802154_rx_frame_config_name(SP1),                      \
	mcps802154_rx_frame_config_name(SP2),                      \
	mcps802154_rx_frame_config_name(SP3)
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_CONFIG_TIMESTAMP_DTU);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_CONFIG_AACK);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_CONFIG_RANGING);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_CONFIG_KEEP_RANGING_CLOCK);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_CONFIG_RANGING_PDOA);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_CONFIG_SP1);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_CONFIG_SP2);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_CONFIG_SP3);

#define mcps802154_rx_error_name(name)                     \
	{                                                  \
		MCPS802154_RX_ERROR_##name, #name          \
	}
#define MCPS802154_RX_ERROR_NAMES                          \
	mcps802154_rx_error_name(NONE),            \
	mcps802154_rx_error_name(TIMEOUT),         \
	mcps802154_rx_error_name(BAD_CKSUM),       \
	mcps802154_rx_error_name(UNCORRECTABLE),   \
	mcps802154_rx_error_name(FILTERED),        \
	mcps802154_rx_error_name(SFD_TIMEOUT),     \
	mcps802154_rx_error_name(PHR_DECODE),      \
	mcps802154_rx_error_name(OTHER)
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_NONE);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_TIMEOUT);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_BAD_CKSUM);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_UNCORRECTABLE);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_FILTERED);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_SFD_TIMEOUT);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_PHR_DECODE);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_OTHER);

#define ieee802154_afilt_name(name)                        \
	{                                                  \
		IEEE802154_AFILT_##name, #name             \
	}
#define IEEE802154_AFILT_NAMES                             \
	ieee802154_afilt_name(SADDR_CHANGED),              \
	ieee802154_afilt_name(IEEEADDR_CHANGED),           \
	ieee802154_afilt_name(PANID_CHANGED),              \
	ieee802154_afilt_name(PANC_CHANGED)
TRACE_DEFINE_ENUM(IEEE802154_AFILT_SADDR_CHANGED);
TRACE_DEFINE_ENUM(IEEE802154_AFILT_IEEEADDR_CHANGED);
TRACE_DEFINE_ENUM(IEEE802154_AFILT_PANID_CHANGED);
TRACE_DEFINE_ENUM(IEEE802154_AFILT_PANC_CHANGED);

#define RX_FRAME_INFO_FLAGS_ENTRY __field(uint16_t, flags)
#define RX_FRAME_INFO_FLAGS_ASSIGN __entry->flags = info->flags
#define RX_FRAME_INFO_FLAGS_PR_FMT "flags=%s"
#define rx_frame_info_name(name)                           \
	{                                                  \
		MCPS802154_RX_FRAME_INFO_##name, #name     \
	}
#define RX_FRAME_INFO_FLAGS_NAMES                          \
	rx_frame_info_name(TIMESTAMP_DTU),                 \
	rx_frame_info_name(TIMESTAMP_RCTU),                \
	rx_frame_info_name(LQI),                           \
	rx_frame_info_name(RANGING_FOM),                   \
	rx_frame_info_name(RANGING_OFFSET),                \
	rx_frame_info_name(RANGING_PDOA),                  \
	rx_frame_info_name(RANGING_PDOA_FOM),              \
	rx_frame_info_name(RANGING_STS_TIMESTAMP_RCTU),    \
	rx_frame_info_name(RANGING_STS_FOM),               \
	rx_frame_info_name(AACK),                          \
	rx_frame_info_name(WIFI_ACTIVE)
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_TIMESTAMP_DTU);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_TIMESTAMP_RCTU);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_LQI);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_RANGING_FOM);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_RANGING_OFFSET);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_RANGING_PDOA);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_RANGING_PDOA_FOM);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_RANGING_STS_TIMESTAMP_RCTU);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_RANGING_STS_FOM);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_AACK);
TRACE_DEFINE_ENUM(MCPS802154_RX_FRAME_INFO_WIFI_ACTIVE);
#define RX_FRAME_INFO_FLAGS_PR_ARG \
	__print_flags(__entry->flags, "|", RX_FRAME_INFO_FLAGS_NAMES)

#define RX_FRAME_INFO_ENTRY              \
	__field(uint32_t, timestamp_dtu)      \
	__field(uint64_t, timestamp_rctu)     \
	__field(int, frame_duration_dtu) \
	__field(uint8_t, ranging_sts_fom0)    \
	RX_FRAME_INFO_FLAGS_ENTRY
#define RX_FRAME_INFO_ASSIGN                                    \
	__entry->timestamp_dtu = info->timestamp_dtu;           \
	__entry->timestamp_rctu = info->timestamp_rctu;         \
	__entry->frame_duration_dtu = info->frame_duration_dtu; \
	__entry->ranging_sts_fom0 = info->ranging_sts_fom[0];   \
	RX_FRAME_INFO_FLAGS_ASSIGN
#define RX_FRAME_INFO_PR_FMT                                                \
	"timestamp_dtu=%#x timestamp_rctu=0x%llx frame_duration_dtu=%d "    \
	"ranging_sts_fom[0]=%#x"                                            \
	RX_FRAME_INFO_FLAGS_PR_FMT
#define RX_FRAME_INFO_PR_ARG         \
	__entry->timestamp_dtu,      \
	__entry->timestamp_rctu,     \
	__entry->frame_duration_dtu, \
	__entry->ranging_sts_fom0,   \
	RX_FRAME_INFO_FLAGS_PR_ARG

#define KEY_ENTRY	__string(key, key)
#define KEY_ASSIGN	__assign_str(key, key)
#define KEY_PR_FMT	"key=%s"
#define KEY_PR_ARG	__get_str(key)

DECLARE_EVENT_CLASS(local_only_evt,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		),
	TP_printk(LOCAL_PR_FMT, LOCAL_PR_ARG)
	);

DEFINE_EVENT(local_only_evt, llhw_return_void,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

TRACE_EVENT(llhw_return_int,
	TP_PROTO(const struct mcps802154_local *local, int ret),
	TP_ARGS(local, ret),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, ret)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->ret = ret;
		),
	TP_printk(LOCAL_PR_FMT " returned=%d", LOCAL_PR_ARG, __entry->ret)
	);

TRACE_EVENT(llhw_return_rx_frame,
	TP_PROTO(const struct mcps802154_local *local, int ret,
		 const struct mcps802154_rx_frame_info *info),
	TP_ARGS(local, ret, info),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, ret)
		RX_FRAME_INFO_ENTRY
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->ret = ret;
		RX_FRAME_INFO_ASSIGN;
		),
	TP_printk(LOCAL_PR_FMT " returned=%d " RX_FRAME_INFO_PR_FMT,
		  LOCAL_PR_ARG, __entry->ret, RX_FRAME_INFO_PR_ARG)
	);

TRACE_EVENT(llhw_return_timestamp_dtu,
	TP_PROTO(const struct mcps802154_local *local, int ret,
		 uint32_t timestamp_dtu),
	TP_ARGS(local, ret, timestamp_dtu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, ret)
		__field(uint32_t, timestamp_dtu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->ret = ret;
		__entry->timestamp_dtu = timestamp_dtu;
		),
	TP_printk(LOCAL_PR_FMT " returned=%d timestamp_dtu=%#x",
		  LOCAL_PR_ARG, __entry->ret, __entry->timestamp_dtu)
	);

TRACE_EVENT(llhw_return_timestamp_rctu,
	TP_PROTO(const struct mcps802154_local *local, int ret,
		 uint64_t timestamp_rctu),
	TP_ARGS(local, ret, timestamp_rctu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, ret)
		__field(uint64_t, timestamp_rctu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->ret = ret;
		__entry->timestamp_rctu = timestamp_rctu;
		),
	TP_printk(LOCAL_PR_FMT " returned=%d timestamp_rctu=0x%llx",
		  LOCAL_PR_ARG, __entry->ret, __entry->timestamp_rctu)
	);

DEFINE_EVENT(local_only_evt, llhw_start,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DEFINE_EVENT(local_only_evt, llhw_stop,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

TRACE_EVENT(llhw_tx_frame,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_tx_frame_config *config,
		 int frame_idx, int next_delay_dtu),
	TP_ARGS(local, config, frame_idx, next_delay_dtu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint32_t, timestamp_dtu)
		__field(int, rx_enable_after_tx_dtu)
		__field(int, rx_enable_after_tx_timeout_dtu)
		__field(uint8_t, flags)
		__field(int8_t, ant_set_id)
		__field(int, frame_idx)
		__field(int, next_delay_dtu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->timestamp_dtu = config->timestamp_dtu;
		__entry->rx_enable_after_tx_dtu = config->rx_enable_after_tx_dtu;
		__entry->rx_enable_after_tx_timeout_dtu = config->rx_enable_after_tx_timeout_dtu;
		__entry->ant_set_id = config->ant_set_id;
		__entry->flags = config->flags;
		__entry->frame_idx = frame_idx;
		__entry->next_delay_dtu = next_delay_dtu;
		),
	TP_printk(LOCAL_PR_FMT " timestamp_dtu=%#x"
		  " rx_enable_after_tx_dtu=%d rx_enable_after_tx_timeout_dtu=%d"
		  " ant_set_id=%d flags=%s frame_idx=%d next_delay_dtu=%d",
		  LOCAL_PR_ARG,
		  __entry->timestamp_dtu, __entry->rx_enable_after_tx_dtu,
		  __entry->rx_enable_after_tx_timeout_dtu, __entry->ant_set_id,
		  __print_flags(__entry->flags, "|", MCPS802154_TX_FRAME_CONFIG_NAMES),
		  __entry->frame_idx,
		  __entry->next_delay_dtu
		  )
	);

DEFINE_EVENT(local_only_evt, llhw_tx_get_frame_info,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

TRACE_EVENT(llhw_return_tx_frame_info,
	TP_PROTO(const struct mcps802154_local *local,
		 int r,
		 const struct mcps802154_tx_frame_info *info),
	TP_ARGS(local, r, info),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, r)
		__field(enum mcps802154_tx_frame_info_flags, flags)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->r = r;
		__entry->flags = info->flags;
		),
	TP_printk(LOCAL_PR_FMT " r=%d flags=%s",
		  LOCAL_PR_ARG, __entry->r,
		  __print_flags(__entry->flags, "|",
				 MCPS802154_TX_FRAME_INFO_NAMES)
		  )
);

DEFINE_EVENT(local_only_evt, llhw_tx_disable,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

TRACE_EVENT(llhw_rx_enable,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_rx_frame_config *config,
		 int frame_idx, int next_delay_dtu),
	TP_ARGS(local, config, frame_idx, next_delay_dtu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint32_t, timestamp_dtu)
		__field(int, timeout_dtu)
		__field(int, frame_timeout_dtu)
		__field(uint8_t, flags)
		__field(int8_t, ant_set_id)
		__field(int, frame_idx)
		__field(int, next_delay_dtu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->timestamp_dtu = config->timestamp_dtu;
		__entry->timeout_dtu = config->timeout_dtu;
		__entry->frame_timeout_dtu = config->frame_timeout_dtu;
		__entry->flags = config->flags;
		__entry->ant_set_id = config->ant_set_id;
		__entry->frame_idx = frame_idx;
		__entry->next_delay_dtu = next_delay_dtu;
		),
	TP_printk(LOCAL_PR_FMT " timestamp_dtu=%#x timeout_dtu=%d"
		  " frame_timeout_dtu=%d ant_set_id=%d flags=%s frame_idx=%d"
		  " next_delay_dtu=%d",
		  LOCAL_PR_ARG,
		  __entry->timestamp_dtu, __entry->timeout_dtu,
		  __entry->frame_timeout_dtu, __entry->ant_set_id,
		  __print_flags(__entry->flags, "|", MCPS802154_RX_FRAME_CONFIG_NAMES),
		  __entry->frame_idx,
		  __entry->next_delay_dtu
		  )
	);

DEFINE_EVENT(local_only_evt, llhw_rx_disable,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DECLARE_EVENT_CLASS(rx_frame_info_evt,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_rx_frame_info *info),
	TP_ARGS(local, info),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		RX_FRAME_INFO_FLAGS_ENTRY
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		RX_FRAME_INFO_FLAGS_ASSIGN;
		),
	TP_printk(LOCAL_PR_FMT " " RX_FRAME_INFO_FLAGS_PR_FMT, LOCAL_PR_ARG,
		  RX_FRAME_INFO_FLAGS_PR_ARG)
	);

DEFINE_EVENT(rx_frame_info_evt, llhw_rx_get_frame,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_rx_frame_info *info),
	TP_ARGS(local, info)
	);

DEFINE_EVENT(rx_frame_info_evt, llhw_rx_get_error_frame,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_rx_frame_info *info),
	TP_ARGS(local, info)
	);

DEFINE_EVENT(local_only_evt, llhw_idle,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

TRACE_EVENT(llhw_idle_timestamp,
	TP_PROTO(const struct mcps802154_local *local,
		 uint32_t timestamp_dtu),
	TP_ARGS(local, timestamp_dtu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint32_t, timestamp_dtu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->timestamp_dtu = timestamp_dtu;
		),
	TP_printk(LOCAL_PR_FMT " timestamp_dtu=%#x",
		  LOCAL_PR_ARG,
		  __entry->timestamp_dtu
		  )
	);

DEFINE_EVENT(local_only_evt, llhw_reset,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DEFINE_EVENT(local_only_evt, llhw_get_current_timestamp_dtu,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

TRACE_EVENT(llhw_set_channel,
	TP_PROTO(const struct mcps802154_local *local, uint8_t page, uint8_t channel,
		 uint8_t preamble_code),
	TP_ARGS(local, page, channel, preamble_code),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint8_t, page)
		__field(uint8_t, channel)
		__field(uint8_t, preamble_code)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->page = page;
		__entry->channel = channel;
		__entry->preamble_code = preamble_code;
		),
	TP_printk(LOCAL_PR_FMT " page=%d channel=%d preamble_code=%d",
		  LOCAL_PR_ARG, __entry->page, __entry->channel,
		  __entry->preamble_code)
	);

TRACE_EVENT(llhw_set_hrp_uwb_params,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_hrp_uwb_params *params),
	TP_ARGS(local, params),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, prf)
		__field(int, psr)
		__field(int, sfd_selector)
		__field(int, phr_hi_rate)
		__field(int, data_rate)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->prf = params->prf;
		__entry->psr = params->psr;
		__entry->sfd_selector = params->sfd_selector;
		__entry->phr_hi_rate = params->phr_hi_rate;
		__entry->data_rate = params->data_rate;
		),
	TP_printk(LOCAL_PR_FMT " prf=%d psr=%d sfd_selector=%d phr_hi_rate=%d data_rate=%d",
		  LOCAL_PR_ARG, __entry->prf, __entry->psr,
		  __entry->sfd_selector, __entry->phr_hi_rate, __entry->data_rate)
	);

TRACE_EVENT(llhw_check_hrp_uwb_params,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_hrp_uwb_params *params),
	TP_ARGS(local, params),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, prf)
		__field(int, psr)
		__field(int, sfd_selector)
		__field(int, phr_hi_rate)
		__field(int, data_rate)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->prf = params->prf;
		__entry->psr = params->psr;
		__entry->sfd_selector = params->sfd_selector;
		__entry->phr_hi_rate = params->phr_hi_rate;
		__entry->data_rate = params->data_rate;
		),
	TP_printk(LOCAL_PR_FMT " prf=%d psr=%d sfd_selector=%d phr_hi_rate=%d data_rate=%d",
		  LOCAL_PR_ARG, __entry->prf, __entry->psr,
		  __entry->sfd_selector, __entry->phr_hi_rate, __entry->data_rate)
	);

TRACE_EVENT(llhw_set_sts_params,
	TP_PROTO(const struct mcps802154_local *local, const struct
		 mcps802154_sts_params *params),
	TP_ARGS(local, params),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint16_t, seg_len)
		__field(uint8_t, n_segs)
		__array(uint8_t, key, AES_BLOCK_SIZE)
		__array(uint8_t, v, AES_BLOCK_SIZE)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->n_segs = params->n_segs;
		__entry->seg_len = params->seg_len;
		memcpy(__entry->key, params->key, sizeof(params->key));
		memcpy(__entry->v, params->v, sizeof(params->v));
		),
	TP_printk(LOCAL_PR_FMT " n_segs=%u seg_len=%u"
		  " sts_key=%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x"
		  " sts_v=%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
		  LOCAL_PR_ARG, __entry->n_segs, __entry->seg_len,
		  __entry->key[0], __entry->key[1], __entry->key[2],
		  __entry->key[3], __entry->key[4], __entry->key[5],
		  __entry->key[6], __entry->key[7], __entry->key[8],
		  __entry->key[9], __entry->key[10], __entry->key[11],
		  __entry->key[12], __entry->key[13], __entry->key[14],
		  __entry->key[15], __entry->v[0], __entry->v[1],
		  __entry->v[2], __entry->v[3], __entry->v[4], __entry->v[5],
		  __entry->v[6], __entry->v[7], __entry->v[8], __entry->v[9],
		  __entry->v[10], __entry->v[11], __entry->v[12], __entry->v[13],
		  __entry->v[14], __entry->v[15])
	);

TRACE_EVENT(llhw_rx_get_measurement,
	TP_PROTO(const struct mcps802154_local *local, const void *rx_ctx),
	TP_ARGS(local, rx_ctx),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(const void *, rx_ctx)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->rx_ctx = rx_ctx;
		),
	TP_printk(LOCAL_PR_FMT " rx_ctx=%p",
		  LOCAL_PR_ARG,
		  __entry->rx_ctx)
	);

TRACE_EVENT(llhw_return_measurement,
	TP_PROTO(const struct mcps802154_local *local, int r,
		 const struct mcps802154_rx_measurement_info *info),
	TP_ARGS(local, r, info),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, r)
		__field(int, n_aoas)
		__field(int, n_segment_meas)
		__field(uint16_t, pdoa_0)
		__field(uint16_t, pdoa_1)
		__field(uint16_t, pdoa_2)
		__field(uint16_t, aoa_0)
		__field(uint16_t, aoa_1)
		__field(uint16_t, aoa_2)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->r = r;
		__entry->n_aoas = info->n_aoas;
		__entry->n_segment_meas = info->n_segment_meas;
		__entry->pdoa_0 = info->aoas[0].pdoa_rad_q11;
		__entry->pdoa_1 = info->aoas[1].pdoa_rad_q11;
		__entry->pdoa_2 = info->aoas[2].pdoa_rad_q11;
		__entry->aoa_0 = info->aoas[0].aoa_rad_q11;
		__entry->aoa_1 = info->aoas[1].aoa_rad_q11;
		__entry->aoa_2 = info->aoas[2].aoa_rad_q11;
		),
	TP_printk(LOCAL_PR_FMT " r=%d n_aoas=%d n_segment_meas=%d pdoa_0=0x%04x "
			"pdoa_1=0x%04x pdoa_2=0x%04x aoa_0=0x%04x aoa_1=0x%04x aoa_2=0x%04x",
		  LOCAL_PR_ARG,
		  __entry->r,
		  __entry->n_aoas,
		  __entry->n_segment_meas,
		  __entry->pdoa_0,
		  __entry->pdoa_1,
		  __entry->pdoa_2,
		  __entry->aoa_0,
		  __entry->aoa_1,
		  __entry->aoa_2)
	);

TRACE_EVENT(llhw_set_hw_addr_filt,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct ieee802154_hw_addr_filt *filt,
		 unsigned long changed),
	TP_ARGS(local, filt, changed),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint16_t, pan_id)
		__field(uint16_t, short_addr)
		__field(uint64_t, extended_addr)
		__field(bool, pan_coord)
		__field(unsigned long, changed)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->pan_id = filt->pan_id;
		__entry->short_addr = filt->short_addr;
		__entry->extended_addr = filt->ieee_addr;
		__entry->pan_coord = filt->pan_coord;
		__entry->changed = changed;
		),
	TP_printk(LOCAL_PR_FMT " pan_id=0x%04x short_addr=0x%04x"
		  " extended_addr=0x%016llx pan_coord=%s changed=%s",
		  LOCAL_PR_ARG, __entry->pan_id, __entry->short_addr,
		  __entry->extended_addr,
		  __entry->pan_coord ? "true" : "false",
		  __print_flags(__entry->changed, "|", IEEE802154_AFILT_NAMES))
	);

TRACE_EVENT(llhw_set_txpower,
	TP_PROTO(const struct mcps802154_local *local, int32_t mbm),
	TP_ARGS(local, mbm),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int32_t, mbm)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->mbm = mbm;
		),
	TP_printk(LOCAL_PR_FMT " mbm=%d", LOCAL_PR_ARG, __entry->mbm)
	);

TRACE_EVENT(llhw_set_cca_ed_level,
	TP_PROTO(const struct mcps802154_local *local, int32_t mbm),
	TP_ARGS(local, mbm),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int32_t, mbm)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->mbm = mbm;
		),
	TP_printk(LOCAL_PR_FMT " mbm=%d", LOCAL_PR_ARG, __entry->mbm)
	);

DECLARE_EVENT_CLASS(bool_on_evt,
	TP_PROTO(const struct mcps802154_local *local, bool on),
	TP_ARGS(local, on),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(bool, on)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->on = on;
		),
	TP_printk(LOCAL_PR_FMT " %s", LOCAL_PR_ARG, __entry->on ? "on" : "off")
	);

DEFINE_EVENT(bool_on_evt, llhw_set_promiscuous_mode,
	TP_PROTO(const struct mcps802154_local *local, bool on),
	TP_ARGS(local, on)
	);

DEFINE_EVENT(bool_on_evt, llhw_set_scanning_mode,
	TP_PROTO(const struct mcps802154_local *local, bool on),
	TP_ARGS(local, on)
	);

DEFINE_EVENT(local_only_evt, llhw_testmode_cmd,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DEFINE_EVENT(local_only_evt, llhw_event_rx_frame,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DEFINE_EVENT(local_only_evt, llhw_event_rx_timeout,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DECLARE_EVENT_CLASS(str_key_evt,
	TP_PROTO(const struct mcps802154_local *local, const char *const key),
	TP_ARGS(local, key),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		KEY_ENTRY
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		KEY_ASSIGN;
		),
	TP_printk(LOCAL_PR_FMT " " KEY_PR_FMT, LOCAL_PR_ARG, KEY_PR_ARG)
	);

DEFINE_EVENT(str_key_evt, llhw_set_calibration,
	TP_PROTO(const struct mcps802154_local *local, const char *const key),
	TP_ARGS(local, key)
	);

DEFINE_EVENT(str_key_evt, llhw_get_calibration,
	TP_PROTO(const struct mcps802154_local *local, const char *const key),
	TP_ARGS(local, key)
	);

TRACE_EVENT(llhw_get_calibration_key_name,
	TP_PROTO(const struct mcps802154_local *local, uint16_t key_idx),
	TP_ARGS(local, key_idx),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint16_t, key_idx)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->key_idx = key_idx;
		),
	TP_printk(LOCAL_PR_FMT " key_idx=%d",
		  LOCAL_PR_ARG, __entry->key_idx)
	);

DEFINE_EVENT(local_only_evt, llhw_reset_calibration,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

TRACE_EVENT(llhw_vendor_cmd,
	TP_PROTO(const struct mcps802154_local *local, uint32_t vendor_id,
		 uint32_t subcmd, size_t data_len),
	TP_ARGS(local, vendor_id, subcmd, data_len),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint32_t, vendor_id)
		__field(uint32_t, subcmd)
		__field(uint32_t, data_len)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->vendor_id = vendor_id;
		__entry->subcmd = subcmd;
		__entry->data_len = data_len;
		),
	TP_printk(LOCAL_PR_FMT " vendor_id=0x%06x subcmd=0x%x data_len=%d",
		  LOCAL_PR_ARG, __entry->vendor_id, __entry->subcmd,
		  __entry->data_len)
	);

TRACE_EVENT(llhw_event_rx_error,
	TP_PROTO(const struct mcps802154_local *local,
		 enum mcps802154_rx_error_type error),
	TP_ARGS(local, error),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(enum mcps802154_rx_error_type, error)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->error = error;
		),
	TP_printk(LOCAL_PR_FMT " error=%s", LOCAL_PR_ARG,
		  __print_symbolic(__entry->error, MCPS802154_RX_ERROR_NAMES))
	);

DEFINE_EVENT(local_only_evt, llhw_event_tx_done,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DEFINE_EVENT(local_only_evt, llhw_event_broken,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DEFINE_EVENT(local_only_evt, llhw_event_timer_expired,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DEFINE_EVENT(local_only_evt, llhw_event_done,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

DEFINE_EVENT(local_only_evt, llhw_event_se_done,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local)
	);

TRACE_EVENT(ca_set_scheduler,
	TP_PROTO(const struct mcps802154_local *local, const char *name),
	TP_ARGS(local, name),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__string(name, name)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__assign_str(name, name);
		),
	TP_printk(LOCAL_PR_FMT " name=%s", LOCAL_PR_ARG, __get_str(name))
	);

TRACE_EVENT(ca_set_scheduler_parameters,
	TP_PROTO(const struct mcps802154_local *local, const char *name),
	TP_ARGS(local, name),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__string(name, name)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__assign_str(name, name);
		),
	TP_printk(LOCAL_PR_FMT " name=%s", LOCAL_PR_ARG, __get_str(name))
	);

TRACE_EVENT(ca_set_region,
	TP_PROTO(const struct mcps802154_local *local,
		 const char *scheduler_name, uint32_t region_id,
		 const char *region_name),
	TP_ARGS(local, scheduler_name, region_id, region_name),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__string(scheduler_name, scheduler_name)
		__field(uint32_t, region_id)
		__string(region_name, region_name)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__assign_str(scheduler_name, scheduler_name);
		__entry->region_id = region_id;
		__assign_str(region_name, region_name);
		),
	TP_printk(LOCAL_PR_FMT " scheduler=%s region_id=%u region_name=%s",
		  LOCAL_PR_ARG, __get_str(scheduler_name), __entry->region_id,
		  __get_str(region_name))
	);

TRACE_EVENT(ca_scheduler_call,
	TP_PROTO(const struct mcps802154_local *local,
		const char *scheduler_name, uint32_t call_id),
	TP_ARGS(local, scheduler_name, call_id),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__string(scheduler_name, scheduler_name)
		__field(uint32_t, call_id)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__assign_str(scheduler_name, scheduler_name);
		__entry->call_id = call_id;
		),
	TP_printk(LOCAL_PR_FMT" scheduler=%s call_id=0x%x",
		  LOCAL_PR_ARG, __get_str(scheduler_name), __entry->call_id)
	);

TRACE_EVENT(ca_set_region_params,
	    TP_PROTO(const struct mcps802154_local *local,
		     const char *scheduler_name, uint32_t region_id,
		     const char *region_name),
	    TP_ARGS(local, scheduler_name, region_id, region_name),
	    TP_STRUCT__entry(
		    LOCAL_ENTRY
		    __string(scheduler_name, scheduler_name)
		    __field(uint32_t, region_id)
		    __string(region_name, region_name)
		    ),
	    TP_fast_assign(
		    LOCAL_ASSIGN;
		    __assign_str(scheduler_name, scheduler_name);
		    __entry->region_id = region_id;
		    __assign_str(region_name, region_name);
		    ),
	    TP_printk(LOCAL_PR_FMT " scheduler=%s region_id=%u region_name=%s",
		      LOCAL_PR_ARG, __get_str(scheduler_name), __entry->region_id,
		      __get_str(region_name))
	);

TRACE_EVENT(ca_get_region_parameters,
	TP_PROTO(const struct mcps802154_local *local,
		const char *scheduler_name, uint32_t region_id,
		const char *region_name),
	TP_ARGS(local, scheduler_name, region_id, region_name),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__string(scheduler_name, scheduler_name)
		__field(uint32_t, region_id)
		__string(region_name, region_name)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__assign_str(scheduler_name, scheduler_name);
		__entry->region_id = region_id;
		__assign_str(region_name, region_name);
		),
	TP_printk(LOCAL_PR_FMT " scheduler=%s region_id=%u region_name=%s",
		  LOCAL_PR_ARG, __get_str(scheduler_name), __entry->region_id,
		  __get_str(region_name))
	);

TRACE_EVENT(ca_call_region,
	TP_PROTO(const struct mcps802154_local *local,
		const char *scheduler_name, uint32_t region_id,
		const char *region_name, uint32_t call_id),
	TP_ARGS(local, scheduler_name, region_id, region_name, call_id),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__string(scheduler_name, scheduler_name)
		__field(uint32_t, region_id)
		__string(region_name, region_name)
		__field(uint32_t, call_id)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__assign_str(scheduler_name, scheduler_name);
		__entry->region_id = region_id;
		__assign_str(region_name, region_name);
		__entry->call_id = call_id;
		),
	TP_printk(LOCAL_PR_FMT " scheduler=%s region_id=%u region_name=%s call_id=0x%x",
		  LOCAL_PR_ARG, __get_str(scheduler_name), __entry->region_id,
		  __get_str(region_name), __entry->call_id)
	);

TRACE_EVENT(ca_get_access,
	TP_PROTO(const struct mcps802154_local *local, uint32_t next_timestamp_dtu),
	TP_ARGS(local, next_timestamp_dtu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint32_t, next_timestamp_dtu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		),
	TP_printk(LOCAL_PR_FMT " next_timestamp_dtu=%#x", LOCAL_PR_ARG,
		  __entry->next_timestamp_dtu)
	);

TRACE_EVENT(ca_return_int,
	TP_PROTO(const struct mcps802154_local *local, int r),
	TP_ARGS(local, r),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int, r)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->r = r;
		),
	TP_printk(LOCAL_PR_FMT " r=%d", LOCAL_PR_ARG, __entry->r)
	);

TRACE_EVENT(fproc_broken_enter,
	TP_PROTO(const struct mcps802154_local *local),
	TP_ARGS(local),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		),
	TP_printk(LOCAL_PR_FMT, LOCAL_PR_ARG)
	);

TRACE_EVENT(schedule_update,
	TP_PROTO(const struct mcps802154_local *local, uint32_t next_timestamp_dtu),
	TP_ARGS(local, next_timestamp_dtu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint32_t, next_timestamp_dtu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		),
	TP_printk(LOCAL_PR_FMT " next_timestamp_dtu=%#x", LOCAL_PR_ARG,
		  __entry->next_timestamp_dtu)
	);

TRACE_EVENT(update_schedule_error,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_schedule_update *su,
		 uint32_t next_timestamp_dtu),
	TP_ARGS(local, su, next_timestamp_dtu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint32_t, expected_start_timestamp_dtu)
		__field(uint32_t, start_timestamp_dtu)
		__field(int, duration_dtu)
		__field(size_t, n_regions)
		__field(uint32_t, next_timestamp_dtu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->expected_start_timestamp_dtu = su->expected_start_timestamp_dtu;
		__entry->start_timestamp_dtu = su->start_timestamp_dtu;
		__entry->duration_dtu = su->duration_dtu;
		__entry->n_regions = su->n_regions;
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		),
	TP_printk(LOCAL_PR_FMT " expected_start_timestamp_dtu=%#x "
		  "start_timestamp_dtu=%#x duration_dtu=%d "
		  "n_regions=%lu next_timestamp_dtu=%#x",
		  LOCAL_PR_ARG,
		  __entry->expected_start_timestamp_dtu,
		  __entry->start_timestamp_dtu,
		  __entry->duration_dtu,
		  __entry->n_regions,
		  __entry->next_timestamp_dtu)
	);

TRACE_EVENT(schedule_update_done,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_schedule *sched),
	TP_ARGS(local, sched),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(uint32_t, start_timestamp_dtu)
		__field(int, duration_dtu)
		__field(size_t, n_regions)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->start_timestamp_dtu = sched->start_timestamp_dtu;
		__entry->duration_dtu = sched->duration_dtu;
		__entry->n_regions = sched->n_regions;
		),
	TP_printk(LOCAL_PR_FMT " start_timestamp_dtu=%#x duration_dtu=%d n_regions=%lu",
		  LOCAL_PR_ARG, __entry->start_timestamp_dtu,
		  __entry->duration_dtu, __entry->n_regions)
	);

TRACE_EVENT(
	region_notify_stop,
	TP_PROTO(const struct mcps802154_region *region),
	TP_ARGS(region),
	TP_STRUCT__entry(
		__string(region_name, region->ops->name)
		),
	TP_fast_assign(
		__assign_str(region_name, region->ops->name);
		),
	TP_printk("region=%s",
		  __get_str(region_name)
		)
	);

TRACE_EVENT(
	region_get_demand,
	TP_PROTO(const struct mcps802154_region *region,
		 uint32_t next_timestamp_dtu),
	TP_ARGS(region, next_timestamp_dtu),
	TP_STRUCT__entry(
		__string(region_name, region->ops->name)
		__field(uint32_t, next_timestamp_dtu)
		),
	TP_fast_assign(
		__assign_str(region_name, region->ops->name);
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		),
	TP_printk("region=%s next_timestamp_dtu=%#x",
		  __get_str(region_name),
		  __entry->next_timestamp_dtu
		)
	);

TRACE_EVENT(
	region_get_demand_return,
	TP_PROTO(const struct mcps802154_region *region,
		 const struct mcps802154_region_demand *demand,
		 int r),
	TP_ARGS(region, demand, r),
	TP_STRUCT__entry(
		__field(int, r)
		__field(uint32_t, timestamp_dtu)
		__field(uint32_t, max_duration_dtu)
		),
	TP_fast_assign(
		__entry->r = r;
		__entry->timestamp_dtu = demand->timestamp_dtu;
		__entry->max_duration_dtu = demand->max_duration_dtu;
		),
	TP_printk("r=%d timestamp_dtu=%#x max_duration_dtu=%d",
		  __entry->r,
		  __entry->timestamp_dtu,
		  __entry->max_duration_dtu
		)
	);

TRACE_EVENT(region_get_access,
	TP_PROTO(const struct mcps802154_local *local,
		 const struct mcps802154_region *region,
		 uint32_t next_timestamp_dtu, int next_in_region_dtu,
		 int region_duration_dtu),
	TP_ARGS(local, region, next_timestamp_dtu, next_in_region_dtu,
		region_duration_dtu),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__string(region_name, region->ops->name)
		__field(uint32_t, next_timestamp_dtu)
		__field(int, next_in_region_dtu)
		__field(int, region_duration_dtu)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__assign_str(region_name, region->ops->name);
		__entry->next_timestamp_dtu = next_timestamp_dtu;
		__entry->next_in_region_dtu = next_in_region_dtu;
		__entry->region_duration_dtu = region_duration_dtu;
		),
	TP_printk(LOCAL_PR_FMT " region=%s next_timestamp_dtu=%#x next_in_region_dtu=%d region_duration_dtu=%d",
		  LOCAL_PR_ARG,
		  __get_str(region_name), __entry->next_timestamp_dtu,
		  __entry->next_in_region_dtu, __entry->region_duration_dtu)
	);

TRACE_EVENT(device_state,
	TP_PROTO(const struct mcps802154_local *local,
		 enum mcps802154_device_state state),
	TP_ARGS(local, state),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(enum mcps802154_device_state, state)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->state = state;
		),
	TP_printk(LOCAL_PR_FMT " state=%s", LOCAL_PR_ARG,
		  __print_symbolic(__entry->state, MCPS802154_DEVICE_STATE_NAMES))
	);

TRACE_EVENT(llhw_get_pwr_stats,
	TP_PROTO(const struct mcps802154_local *local,
		const struct mcps802154_power_stats *pwr_stats),
	TP_ARGS(local, pwr_stats),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(const struct mcps802154_power_stats *, pwr_stats)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->pwr_stats = pwr_stats;
		),
	TP_printk(LOCAL_PR_FMT " pwr_stats=%p",
		  LOCAL_PR_ARG,
		  __entry->pwr_stats)
	);

TRACE_EVENT(llhw_query_gpio_timestamp,
	TP_PROTO(const struct mcps802154_local *local,
			int64_t *timestamp_us,
			uint8_t *sequence_number),
	TP_ARGS(local, timestamp_us, sequence_number),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(int64_t*, timestamp_us)
		__field(uint8_t*, sequence_number)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->timestamp_us = timestamp_us;
		__entry->sequence_number = sequence_number;
		),
	TP_printk(LOCAL_PR_FMT "timestamp_us=%d, sequence_number=%d",
		  LOCAL_PR_ARG,
		   __entry->timestamp_us,
		  __entry->sequence_number)
	);

TRACE_EVENT(llhw_get_uwb_device_stats,
	TP_PROTO(const struct mcps802154_local *local,
			const struct mcps802154_uwb_device_stats *uwb_device_stats),
	TP_ARGS(local, uwb_device_stats),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(const struct mcps802154_uwb_device_stats*, uwb_device_stats)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->uwb_device_stats = uwb_device_stats;
		),
	TP_printk(LOCAL_PR_FMT "uwb_device_stats=%p",
		  LOCAL_PR_ARG,
		   __entry->uwb_device_stats)
	);

TRACE_EVENT(llhw_get_device_info,
	TP_PROTO(const struct mcps802154_local *local,
			const struct mcps802154_device_info *device_info),
	TP_ARGS(local, device_info),
	TP_STRUCT__entry(
		LOCAL_ENTRY
		__field(const struct mcps802154_device_info*, device_info)
		),
	TP_fast_assign(
		LOCAL_ASSIGN;
		__entry->device_info = device_info;
		),
	TP_printk(LOCAL_PR_FMT "device_info=%p",
		  LOCAL_PR_ARG,
		   __entry->device_info)
	);

/* clang-format on */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE trace
#include "trace/define_trace.h"

#endif /* !TRACE_H || TRACE_HEADER_MULTI_READ */
