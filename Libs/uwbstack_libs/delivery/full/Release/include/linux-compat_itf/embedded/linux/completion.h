/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef _COMPLETION_H_
#define _COMPLETION_H_

#include "qassert.h"

#include <qsemaphore.h>
#include <qtime.h>

#define HZ (qtime_get_sys_freq_hz())

struct completion {
	struct qsemaphore *sem;
	uint32_t hz;
};

/**
 * init_completion(): Initialize a completion.
 * @x: Completion structure that is to be initialized.
 */
static inline void init_completion(struct completion *x)
{
	x->sem = qsemaphore_init(0, 1);
	/* Save frequency to avoid repeated calls. */
	x->hz = HZ;
	QASSERT(x->sem);
}

/**
 * wait_for_completion_interruptible(): Waits for completion.
 * @x: Holds the state of this particular completion.
 *
 * Return: 0 when completed, -1 in case of error.
 */
static inline int wait_for_completion_interruptible(struct completion *x)
{
	int r = qsemaphore_take(x->sem, QOSAL_WAIT_FOREVER);

	if (r)
		return -1;

	return 0;
}

/**
 * wait_for_completion_interruptible_timeout(): Waits for completion.
 * @x: Holds the state of this particular completion.
 * @timeout: Timeout value in ticks.
 *
 * Return: 0 in case of error or timeout, 1 if completed.
 */
static inline long wait_for_completion_interruptible_timeout(struct completion *x,
							     unsigned long timeout)
{
	int r = qsemaphore_take(x->sem, (timeout / x->hz) * 1000);

	/* Error or timeout. */
	if (r)
		return 0;

	/* Completed in time. */
	return 1;
}

/**
 * complete(): Signals a thread waiting on this completion.
 * @x: Holds the state of this particular completion.
 */
static inline void complete(struct completion *x)
{
	qsemaphore_give(x->sem);
}

#endif /* _COMPLETION_H_ */
