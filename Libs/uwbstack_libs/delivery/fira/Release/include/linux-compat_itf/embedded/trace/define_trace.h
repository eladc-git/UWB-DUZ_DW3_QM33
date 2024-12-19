/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#include "qmalloc.h"

#include <linux/tracepoint.h>
#include <qtracing.h>
#include <uwbmac/uwbmac.h>

/*
 * First Step of TRACE_EVENT macro generation:
 *
 * Allows to create a new string format which adds trace name as prefix to the
 * print string format defined by "TP_printk" macro.
 */

#ifndef DEFINE_TRACE_ENTRY_STEP0

#define DEFINE_TRACE_ENTRY_STEP0

/* Redefine all TRACE_SYSTEM to trace module unique ID.
 * Used to dynamically filter traces.
 */
#define mcps802154 UWBMAC_TRACE_MODULE_ID_MAIN
#define mcps802154_region_fbs UWBMAC_TRACE_MODULE_ID_FBS
#define mcps802154_region_fira UWBMAC_TRACE_MODULE_ID_FIRA
#define mcps802154_region_lld_common UWBMAC_TRACE_MODULE_ID_LLD_COMMON
#define mcps802154_region_lldd UWBMAC_TRACE_MODULE_ID_LLDD
#define mcps802154_region_lldc UWBMAC_TRACE_MODULE_ID_LLDC
#define mcps802154_region_pctt UWBMAC_TRACE_MODULE_ID_PCTT
#define mcps802154_region_radar UWBMAC_TRACE_MODULE_ID_RADAR
#define mcps802154_region_ccc UWBMAC_TRACE_MODULE_ID_CCC

#undef TP_printk
#define TP_printk(fmt, args...) fmt

#undef DECLARE_EVENT_CLASS
#define DECLARE_EVENT_CLASS(name, proto, args, tstruct, assign, print) \
	static const char tp_print_fmt_##name[] = "%s: " print;

#undef DEFINE_EVENT
#define DEFINE_EVENT(class, name, proto, args)

#undef TRACE_EVENT
#define TRACE_EVENT(name, proto, args, tstruct, assign, print)                                  \
	DECLARE_EVENT_CLASS(name, PARAMS(proto), PARAMS(args), PARAMS(tstruct), PARAMS(assign), \
			    PARAMS(print));                                                     \
	DEFINE_EVENT(name, name, PARAMS(proto), PARAMS(args));

/* clang-format off */
#define TRACE_INCLUDE(system) __stringify(TRACE_INCLUDE_PATH/system.h)
/* clang-format on */

/* Let the trace headers be reread */
#define TRACE_HEADER_MULTI_READ

#include TRACE_INCLUDE(TRACE_INCLUDE_FILE)

/*
 * Second Step of TRACE_EVENT macro generation:
 *
 * Allows to declare almost everything needed for TRACE_EVENT support:
 * "trace_xxx" function and other definitions of structs and functions.
 *
 * Functions "tp_alloc_xxx" and "tp_free_xxx" cannot be defined in that step,
 * because they need to use other definitions of macro "__dynamic_array".
 */

#elif !defined(DEFINE_TRACE_ENTRY_STEP1)
#define DEFINE_TRACE_ENTRY_STEP1

#undef __field
#undef __array
#undef __dynamic_array
#undef __string

#define __field(type, item) type item;
#define __array(type, item, size) type item[size];
#define __dynamic_array(type, item, len) \
	type *item;                      \
	size_t tp_dyn_len_##item;
#define __string(item, src) const char *item;

#undef TP_printk
#define TP_printk(fmt, args...) args

#undef DECLARE_EVENT_CLASS
#define DECLARE_EVENT_CLASS(name, proto, args, tstruct, assign, print)                      \
	struct tp_struct_##name {                                                           \
		tstruct                                                                     \
	};                                                                                  \
                                                                                            \
	static inline void tp_assign_##name(struct tp_struct_##name *__entry, proto)        \
	{                                                                                   \
		assign;                                                                     \
	}                                                                                   \
                                                                                            \
	static inline void tp_print_##name(const char *n, struct tp_struct_##name *__entry) \
	{                                                                                   \
		struct trace_seq seq;                                                       \
		trace_seq_init(&seq);                                                       \
		QOSAL_PRINT_TRACE(tp_print_fmt_##name, n, print);                           \
	}                                                                                   \
                                                                                            \
	static void tp_alloc_##name(struct tp_struct_##name *__entry, proto);               \
	static void tp_free_##name(struct tp_struct_##name *__entry);                       \
                                                                                            \
	static void tp_trace_##name(const char *n, proto)                                   \
	{                                                                                   \
		if (uwbmac_is_trace_module_enabled(TRACE_SYSTEM)) {                         \
			struct tp_struct_##name entry;                                      \
			tp_alloc_##name(&entry, args);                                      \
			tp_assign_##name(&entry, args);                                     \
			tp_print_##name(n, &entry);                                         \
			tp_free_##name(&entry);                                             \
		}                                                                           \
	}

#undef DEFINE_EVENT
#define DEFINE_EVENT(class, name, proto, args)             \
	static inline void trace_##name(proto)             \
	{                                                  \
		tp_trace_##class(__stringify(name), args); \
	}

#include TRACE_INCLUDE(TRACE_INCLUDE_FILE)

/*
 * Third Step: allows to allocate dynamic arrays.
 * For all other types, that step does nothing.
 */
#elif !defined(DEFINE_TRACE_ENTRY_STEP2)

#define DEFINE_TRACE_ENTRY_STEP2

#undef __field
#undef __array
#undef __dynamic_array
#undef __string

#define __field(type, item)
#define __array(type, item, size)
#define __string(item, src)

#define __dynamic_array(type, item, len)                     \
	__entry->item = (type *)qmalloc(sizeof(type) * len); \
	__entry->tp_dyn_len_##item = sizeof(type) * len;

#undef DECLARE_EVENT_CLASS
#define DECLARE_EVENT_CLASS(name, proto, args, tstruct, assign, print)              \
	static inline void tp_alloc_##name(struct tp_struct_##name *__entry, proto) \
	{                                                                           \
		tstruct                                                             \
	}

#undef DEFINE_EVENT
#define DEFINE_EVENT(class, name, proto, args)

#include TRACE_INCLUDE(TRACE_INCLUDE_FILE)

/*
 * Fourth Step: allows to free dynamic arrays.
 * For all other types, that step does nothing.
 */
#elif !defined(DEFINE_TRACE_ENTRY_STEP3)

#define DEFINE_TRACE_ENTRY_STEP3

#undef __field
#undef __array
#undef __dynamic_array
#undef __string

#define __field(type, item)
#define __array(type, item, size)
#define __dynamic_array(type, item, len) qfree(__entry->item);
#define __string(item, src)

#undef DECLARE_EVENT_CLASS
#define DECLARE_EVENT_CLASS(name, proto, args, tstruct, assign, print)      \
	static inline void tp_free_##name(struct tp_struct_##name *__entry) \
	{                                                                   \
		tstruct                                                     \
	}

#include TRACE_INCLUDE(TRACE_INCLUDE_FILE)

#undef TRACE_EVENT
#undef DECLARE_EVENT_CLASS
#undef DEFINE_EVENT
#undef DEFINE_TRACE_ENTRY_STEP0
#undef DEFINE_TRACE_ENTRY_STEP1
#undef DEFINE_TRACE_ENTRY_STEP2
#undef DEFINE_TRACE_ENTRY_STEP3
#undef TRACE_HEADER_MULTI_READ

#define TRACE_EVENT(name, proto, args, tstruct, assign, print)
#define DECLARE_EVENT_CLASS TRACE_EVENT
#define DEFINE_EVENT(class, name, proto, args)

#endif /* DEFINE_TRACE_ENTRY_STEP1 */
