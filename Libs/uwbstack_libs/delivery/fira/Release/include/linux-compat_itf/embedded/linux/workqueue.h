/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef COMPAT_LINUX_WORKQUEUE_H
#define COMPAT_LINUX_WORKQUEUE_H

#include "qassert.h"

#include <qworkqueue.h>

struct work_struct;

typedef void (*work_func_t)(struct work_struct *work);

struct work_struct {
	struct qworkqueue *item;
	work_func_t func;
};

static void work_handler(void *work)
{
	struct work_struct *ws = (struct work_struct *)work;
	ws->func(ws);
}

static inline void INIT_WORK(struct work_struct *ws, work_func_t handler)
{
	QASSERT(ws != NULL);
	ws->func = handler;
	ws->item = qworkqueue_init(work_handler, ws);
	QASSERT(ws->item != NULL);
}

static inline void schedule_work(struct work_struct *ws)
{
	QASSERT(ws != NULL && ws->func != NULL && ws->item != NULL);
	qworkqueue_schedule_work(ws->item);
}

static inline bool cancel_work_sync(struct work_struct *ws)
{
	int r;

	QASSERT(ws != NULL);
	r = qworkqueue_cancel_work(ws->item);

	if (r)
		return true;
	else
		return false;
}

#endif /* COMPAT_LINUX_WORKQUEUE_H */
