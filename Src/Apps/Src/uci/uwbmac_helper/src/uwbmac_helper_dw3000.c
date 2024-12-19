/**
 * @file      uwbmac_helper_dw3000.c
 *
 * @brief     UWBMAC helper init and configuration
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "uwbmac_helper.h"
#include "llhw.h"
#include "qplatform.h"

extern struct l1_config_platform_ops l1_config_platform_ops;

/** @brief Setup the UWB chip. */
int uwbmac_helper_init_fira(void)
{
    int ret;

    ret = qplatform_init();
    assert(!ret);
    ret = l1_config_init(&l1_config_platform_ops);
    assert(!ret);
    ret = llhw_init();

    assert(!ret);

    return 0;
}

void uwbmac_helper_deinit(void)
{
    llhw_deinit();
    l1_config_deinit();
    qplatform_deinit();
}
