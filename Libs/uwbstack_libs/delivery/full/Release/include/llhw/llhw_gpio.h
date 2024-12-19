/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"
#include "qgpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * wifi_coex_uwb_wlan_gpio_configure() - Configure UWB_WLAN_GPIO.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr wifi_coex_uwb_wlan_gpio_configure(void);

/**
 * wifi_coex_uwb_wlan_gpio_write() - Set level of UWB_WLAN_GPIO.
 *
 * @value: Value to write to GPIO output.
 */
void wifi_coex_uwb_wlan_gpio_write(bool value);

/**
 * wifi_coex_wlan_uwb_gpio_configure() - Configure WLAN_UWB_GPIO.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr wifi_coex_wlan_uwb_gpio_configure(void);

/**
 * wifi_coex_wlan_uwb_gpio_read_state() - Read level of WLAN_UWB_GPIO.
 *
 * Return: true if WLAN_UWB_GPIO is HIGH.
 */
bool wifi_coex_wlan_uwb_gpio_read_state(void);

/**
 * wifi_coex_wlan_uwb_gpio_read_interrupts() - Read interrupt status that
 * occurred on WLAN_UWB_GPIO.
 *
 * Return: true if an interrupt (Rising or Falling edge) has occurred
 * since the last call to wifi_coex_wlan_uwb_gpio_clear_interrupts().
 */
bool wifi_coex_wlan_uwb_gpio_read_interrupts(void);

/**
 * wifi_coex_wlan_uwb_gpio_clear_interrupts() - Clear interrupts that occurred
 * on WLAN_UWB_GPIO.
 *
 */
void wifi_coex_wlan_uwb_gpio_clear_interrupts(void);

/**
 * ext_sw_config_gpio_configure() - Configure EXT_SW_CONFIG_GPIO used for
 * External Switch configuration.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr ext_sw_config_gpio_configure(void);

/**
 * ext_sw_config_gpio_write() - Set level of EXT_SW_CONFIG_GPIO.
 *
 * @value: Value to write to GPIO output.
 */
void ext_sw_config_gpio_write(bool value);

/**
 * wifi_sw_gpio_configure() - Configure WiFi switch GPIO used for WiFi switch
 * configuration.
 *
 * Return: 0 in case of success, a negative error code otherwise.
 */
enum qerr wifi_sw_gpio_configure(void);

/**
 * wifi_sw_gpio_write() - Set level of WiFi switch GPIO.
 *
 * @value: Value to write to GPIO output.
 */
void wifi_sw_gpio_write(bool value);

/**
 * gpio_timestamp_configure() - Configure GPIO timestamp.
 *
 * @cb: Callback function pointer to be called in case of interrupt.
 * @cb_data: Private data for callback.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr gpio_timestamp_configure(qgpio_irq_cb cb, void *cb_data);

/**
 * llhw_gpio_retention_sleep_enter() - Save state of llhw GPIOs
 * and set them to LOW. No need to save the state for wifi_coex GPIOs
 * since it's already consider the S3 transition.
 */
void llhw_gpio_retention_sleep_enter(void);

/**
 * llhw_gpio_retention_sleep_restore() - restore the level of llhw GPIOs
 * after waking-up from S3.
 */
void llhw_gpio_retention_sleep_restore(void);

#ifdef __cplusplus
}
#endif
