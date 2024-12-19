/**
 * @file      HAL_uwb.h
 *
 * @brief     Abstraction layer of uwb
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "deca_error.h"
#include "deca_device_api.h"
#include "HAL_SPI.h"

struct uwbs_port_config_s
{
    const uint16_t irqPin;
    const uint16_t irqN;
    const uint16_t rstPin;
    const uint16_t rstIrqN;
    const uint16_t wakeUpPin;
};

struct dw_s
{
    uint32_t devid;
    const struct spi_port_config_s *spi_io_cfg;
    const struct uwbs_port_config_s *ext_io_cfg;
    const void *void_cfg;
    struct spi_s *spi;   /* Spi instance to this UWB chip. */
    struct dwchip_s *dw; /* Driver instance to the UWB chip. */
};

typedef struct dw_s dw_t;


typedef enum
{
    UWB_CANNOT_SLEEP = 0x00,
    UWB_CAN_SLEEP_IN_IRQ = 0x01, /* UWB can be put to sleep immediately after IRQ. */
    UWB_CAN_SLEEP_IN_APP = 0x02  /* DW can be put to sleep by APP. */
} sleep_mode_t;

typedef enum
{
    UWB_IS_NOT_SLEEPING,
    UWB_IS_SLEEPING,
} sleep_status_t;


typedef enum
{
    AOA_ERROR,
    AOA_ENABLED,
    AOA_DISABLED
} aoa_enable_t;

/* Abstraction structure. */
struct hal_uwb_s
{
    void (*init)(void); /* init MCU I/O to the UWB chip. */
    int (*probe)(void); /* probe the driver. */

    void (*irq_init)(void);   /* init MCU IRQ line. */
    void (*enableIRQ)(void);  /* enable IRQ from UWBS. */
    void (*disableIRQ)(void); /* disable IRQ from UWBS. */

    void (*reset)(void); /* HW reset of UWBS, Note: this will update the sleep status. */

    /* UWB various wakeup mechanisms. */
    void (*wakeup_start)(void);   /* start waking up UWBS, Note: this will update the sleep status. */
    void (*wakeup_end)(void);     /* stop waking up UWBS. */
    void (*wakeup_fast)(void);    /* wakeup UWBS without final pause, Note: this will update the sleep status. */
    void (*wakeup_with_io)(void); /* wakeup UWBS with guarantee. Note: this will update the sleep status. */

    /* UWB sleeping. */
    sleep_status_t vsleep_status;
    void (*sleep_status_set)(sleep_status_t); /* set local sleep status to the new value. Avoid to use this fn() directly. */
    sleep_status_t (*sleep_status_get)(void); /* get local UWB sleeping status. */
    void (*sleep_config)(void);               /* configure UWB for sleeping. */
    void (*sleep_enter)(void);                /* enter deep sleep if not already. Note: this will update the status. */

    sleep_mode_t vsleep_mode;
    void (*sleep_mode_set)(sleep_mode_t); /* specify in which mode UWB can sleep NONE/IRQ/APP. */
    sleep_mode_t (*sleep_mode_get)(void); /* get the current UWB can sleep mode. */

    /* MCU sleeping. */
    void (*mcu_sleep_config)(void); /* Enable mcu-specific init features. */
    void (*mcu_suspend_clk)(void);  /* Turn off MCU clock XTAL. */
    void (*mcu_resume_clk)(void);   /* Turn on MCU clock XTAL. */

    aoa_enable_t (*is_aoa)(void); /* Chip + PCB design both support AOA. */

    struct dw_s *uwbs; /* UWB Subsystem connection: SPI, I/O and Driver. */

    void (*stop_all_uwb)(void);
    error_e (*disable_irq_and_reset)(int);
    void (*deinit_callback)(void);
};

extern struct hal_uwb_s hal_uwb;
