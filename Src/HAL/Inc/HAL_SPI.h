/**
 * @file      HAL_SPI.h
 *
 * @brief     Interface for HAL_SPI
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdint.h>

struct spi_s
{
    void (*cs_low)(void *handler);
    void (*cs_high)(void *handler);
    void (*slow_rate)(void *handler);
    void (*fast_rate)(void *handler);
    int (*read)(void *handler, uint16_t headerLength, const uint8_t *headerBuffer, uint16_t readlength, uint8_t *readBuffer);
    int (*write)(void *handler, uint16_t headerLength, const uint8_t *headerBuffer, uint16_t readlength, const uint8_t *readBuffer);
    int (*read_write)(void *handler, uint8_t *readBuffer, uint16_t readlength, uint8_t *writebuffer, uint16_t writeLength);
    int (*write_with_crc)(void *handler, uint16_t headerLength, const uint8_t *headerBuffer, uint16_t bodyLength, const uint8_t *bodyBuffer, uint8_t crc8);
    void *handler;
};
typedef struct spi_s spi_t;

struct spi_port_config_s
{
    uint32_t idx;
    uint32_t cs;
    uint32_t clk;
    uint32_t mosi;
    uint32_t miso;
    uint32_t min_freq;
    uint32_t max_freq;
};
typedef struct spi_port_config_s spi_port_config_t;

/**
 * @brief initialisation of SPI connected to DW3000.
 * Init the SDK HAL SPI driver and configure the SPI pins.
 * @param spiIdx spi index from 0 to n depending on target.
 *
 * @return pointer on SPI driver *
 */
const struct spi_s *init_spi(const spi_port_config_t *spi_port_cfg);
