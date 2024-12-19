/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Define all Linux kernel trace macros for embedded context.
 * Some of them will then be redefined in file trace/define_trace.h.
 */

#define __field(type, item)
#define __array(type, item, size)
#define __dynamic_array(type, item, len)
#define __get_dynamic_array(field)
#define __get_dynamic_array_len(field)
#define __get_str(field)
#define __assign_str(dst, src)

#define TP_STRUCT__entry(args...)
#define TP_PROTO(args...) args
#define TP_fast_assign(args...)
#define TP_printk(args...)
#define TP_ARGS(args...) args

#define __print_hex(buf, buf_len)
#define __print_flags(flag, delim, flag_array...)
#define __print_symbolic(value, symbol_array...)

/*
 * TRACE_EVENT, DECLARE_EVENT_CLASS and DEFINE_EVENT macros
 */

#define TRACE_DEFINE_ENUM(arg...)

#define TRACE_EVENT(name, proto, args, tstruct, assign, print) \
	static inline void trace_##name(proto)                 \
	{                                                      \
	}

#define DECLARE_EVENT_CLASS TRACE_EVENT

#define DEFINE_EVENT(class, name, proto, args) \
	static inline void trace_##name(proto) \
	{                                      \
		trace_##class(args);           \
	}

#ifdef __cplusplus
}
#endif
