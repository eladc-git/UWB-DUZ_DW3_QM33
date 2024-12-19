/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef COMPAT__LINUX_TRACEPOINT_H
#define COMPAT__LINUX_TRACEPOINT_H

#include "uwbmac/uwbmac.h"

#include <stdarg.h>
#include <stdio.h>

#define __stringify_1(x...) #x
#define __stringify(x...) __stringify_1(x)

/*
 * Define all Linux kernel trace macros for embedded context.
 * Some of them will then be redefined in file trace/define_trace.h.
 */

#define __field(type, item)
#define __array(type, item, size)
#define __dynamic_array(type, item, len)

#define __get_dynamic_array(field) __entry->field
#define __get_dynamic_array_len(field) __entry->tp_dyn_len_##field
#define __get_str(field) (__entry->field)

#define __assign_str(dst, src) __entry->dst = (src) ? src : "(null)";

#define TP_STRUCT__entry(args...) args
#define TP_PROTO(args...) args
#define TP_fast_assign(args...) args
#define TP_printk(args...) args
#define TP_ARGS(args...) args

/*
 * Trace sequences are used to allow a function to call several other functions
 * to create a string of data to use (up to a max of SEQ_BUF_SIZE).
 */

#define SEQ_BUF_SIZE 256

struct trace_seq {
	char buffer[SEQ_BUF_SIZE];
	size_t len;
	int full;
};

struct trace_print_flags {
	unsigned long mask;
	const char *name;
};

void trace_seq_init(struct trace_seq *seq);
char *trace_print_hex_seq(struct trace_seq *seq, const unsigned char *buf, int buf_len);
char *trace_print_flags_seq(struct trace_seq *p, const char *delim, unsigned long flags,
			    const struct trace_print_flags *flag_array);
const char *trace_print_symbols(unsigned long val, const struct trace_print_flags *symbol_array);

#define __print_hex(buf, buf_len) trace_print_hex_seq(&seq, buf, buf_len)

#define __print_flags(flag, delim, flag_array...)                                               \
	({                                                                                      \
		static const struct trace_print_flags __flags[] = { flag_array, { -1, NULL } }; \
		trace_print_flags_seq(&seq, delim, flag, __flags);                              \
	})

#define __print_symbolic(value, symbol_array...)                                                  \
	({                                                                                        \
		static const struct trace_print_flags symbols[] = { symbol_array, { -1, NULL } }; \
		trace_print_symbols(value, symbols);                                              \
	})

/*
 * TRACE_EVENT, DECLARE_EVENT_CLASS and DEFINE_EVENT macros
 */

#define TRACE_DEFINE_ENUM(arg...)

#define TRACE_EVENT(name, proto, args, tstruct, assign, print)
#define DECLARE_EVENT_CLASS TRACE_EVENT
#define DEFINE_EVENT(class, name, proto, args)

#define PARAMS(args...) args

#endif /* COMPAT__LINUX_TRACEPOINT_H */
