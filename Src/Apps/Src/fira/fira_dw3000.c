/**
 * @file      fira_dw3000.c
 *
 * @brief     FiRa for DW3000
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include <stdio.h>

#include "common_fira.h"
#include "llhw.h"
#include "qplatform.h"

extern struct l1_config_platform_ops l1_config_platform_ops;

void fira_uwb_mcps_init(fira_param_t *fira_param)
{
    int r;

    r = qplatform_init();
    assert(!r);
    r = l1_config_init(&l1_config_platform_ops);
    assert(!r);
    r = llhw_init();

    assert(!r);
}

void fira_uwb_mcps_deinit(void)
{
    llhw_deinit();
    l1_config_deinit();
    qplatform_deinit();
}
