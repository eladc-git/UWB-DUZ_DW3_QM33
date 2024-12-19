/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct work_struct;

typedef void (*work_func_t)(struct work_struct *work);

struct work_struct {
	int *init;
	work_func_t func;
};

void INIT_WORK(struct work_struct *ws, work_func_t handler);
void schedule_work(struct work_struct *ws);
bool cancel_work_sync(struct work_struct *ws);

#ifdef __cplusplus
}
#endif
