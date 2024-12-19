/**
 * @file      HAL_sleep.c
 *
 * @brief     platform dependent sleep implementation
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include <stdbool.h>

#include "HAL_timer.h"
#include "nrf_delay.h"

/**
 * @brief Sleep.
 * -DDEBUG defined in Makefile prevents __WFI
 */
void Sleep(volatile uint32_t dwMs)
{
    uint32_t dwStart;
    Timer.start(&dwStart);
    while (Timer.check(dwStart, dwMs) == false)
    {
#ifndef DEBUG
/*  __WFI(); */
#endif
    }
}

/**
 * @brief Wrapper function to be used by decadriver. Declared in deca_device_api.h
 * with parameter = 1; precision is 2-5 %. measured on Saleae.
 */
void deca_sleep(unsigned int time_ms)
{
    nrf_delay_ms(time_ms);
}

/**
 * @brief precise usleep() delay
 */
void usleep(unsigned long time_us)
{
    nrf_delay_us(time_us);
}

/**
 * @brief Wrapper function to be used by decadriver. Declared in deca_device_api.h.
 */
void deca_usleep(unsigned long time_us)
{
    usleep(time_us);
}
