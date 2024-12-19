/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "llhw.h"
#include "llhw_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IEEE802154_FCS_LEN 2

#define TARGET_XTAL_OFFSET_VALUE_PPHM_MAX (700)
#define TARGET_XTAL_OFFSET_VALUE_PPHM_MIN (500)
/* Trimming per 1 pphm */
#define AVG_TRIM_PER_PPHM ((XTAL_TRIM_BIT_MASK + 1) / 48.0f / 100)
/* This define needs to be removed when PEG is fixed */
#define OTP_QM35B0_SIP_DEVICE 0x80000000UL

/**
 * llhw_drv_is_wifi_coex_enabled() - Check if WiFi COEX is enabled.
 *
 * Return: true if enabled, otherwise false.
 */
#ifdef CONFIG_LLHW_ENABLE_WIFI_COEX
#define llhw_drv_is_wifi_coex_enabled() 1
#else
#define llhw_drv_is_wifi_coex_enabled() 0
#endif

/**
 * llhw_drv_is_ext_sw_enabled() - Check if External Switch is enabled.
 *
 * Return: true if enabled, otherwise false.
 */
#ifdef CONFIG_LLHW_ENABLE_EXT_SW
#define llhw_drv_is_ext_sw_enabled() 1
#else
#define llhw_drv_is_ext_sw_enabled() 0
#endif

/**
 * llhw_drv_is_wifi_sw_enabled() - Check if WiFi switch is enabled.
 *
 * Return: true if enabled, otherwise false.
 */
#ifdef CONFIG_LLHW_ENABLE_WIFI_SW
#define llhw_drv_is_wifi_sw_enabled() 1
#else
#define llhw_drv_is_wifi_sw_enabled() 0
#endif

/**
 * llhw_drv_init() - Initialize LLHW driver.
 * @uwb: the UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_init(struct uwb_chip *uwb);

/**
 * llhw_drv_reinit() - Reinitialize UWB chip.
 * @uwb: the UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_reinit(struct uwb_chip *uwb);

/**
 * llhw_drv_setup_coex_timer() - Configure timer used for WiFi Coex.
 * @uwb: the UWB device.
 * @delay_us: expiration delay in microseconds.
 */
void llhw_drv_setup_coex_timer(const struct uwb_chip *uwb, uint32_t delay_us);

/**
 * llhw_drv_setup_wifi_sw_timer() - Configure timer used for WiFi switch.
 * @uwb: the UWB device.
 * @delay_us: expiration delay in microseconds.
 */
void llhw_drv_setup_wifi_sw_timer(const struct uwb_chip *uwb, uint32_t delay_us);

/**
 * llhw_drv_read_rx_timestamp() - Read RX frame timestamp and
 * antenna delay.
 * @uwb: the UWB device.
 * @ts: the returned timestamp.
 * @ant_delay: the returned antenna delay, can be NULL.
 * @sts_quality: the returned sts quality information, can be NULL.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_read_rx_timestamp(struct uwb_chip *uwb, uint64_t *ts, uint32_t *ant_delay,
				     uint8_t *sts_quality);

/**
 * llhw_drv_set_channel() - Set UWB channel.
 * @uwb: the UWB device.
 * @channel: channel.
 * @preamble_code: preamble code.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_set_channel(struct uwb_chip *uwb, uint8_t channel, uint8_t preamble_code);

/**
 * llhw_drv_idle() - Start a timer to wake-up from idle state.
 * @idle_duration_us: timer delay is microseconds.
 * If delay is 0, timer will be stopped.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_idle(uint32_t idle_duration_us);

/**
 * llhw_drv_set_deep_sleep() - Put UWB transceiver into deep-sleep mode.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_set_deep_sleep(void);

/**
 * llhw_drv_wakeup() - Wake-up UWB transceiver from deep-sleep mode.
 * @uwb: the UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_wakeup(struct uwb_chip *uwb);

/**
 * llhw_drv_hrp_params_to_dwt() - Convert HRP UWB params to DWT params..
 * @params: the HRP UWB params.
 * @psr: converted PSR value.
 * @sfd_type: converted SFD Type value.
 * @sfd_timeout: converted SFD Timeout value.
 * @data_rate: converted Data Rate value.
 * @phr_rate: converted PHR Rate value.
 * @phr_mode: converted PHR mode value.
 * @rx_pac: converted RX PAC value.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_hrp_params_to_dwt(const struct mcps802154_hrp_uwb_params *params, uint16_t *psr,
				     dwt_sfd_type_e *sfd_type, uint16_t *sfd_timeout,
				     dwt_uwb_bit_rate_e *data_rate, dwt_phr_rate_e *phr_rate,
				     dwt_phr_mode_e *phr_mode, dwt_pac_size_e *rx_pac);

/**
 * llhw_drv_set_hrp_params() - Set HRP UWB params.
 * @uwb: the UWB device.
 * @params: the HRP MCPS params.
 *
 * Return: zero or positive value on success, else a negative error code.
 * The special value 2 if a parameter related to timing has changed.
 * The special value 1 if any other parameter changed.
 */
int llhw_drv_set_hrp_params(struct uwb_chip *uwb, const struct mcps802154_hrp_uwb_params *params);

/**
 * llhw_drv_adjust_tx_power() - Adjust TX Power.
 * @channel: channel for which the linear tx power setting must be calculated.
 * @pwr_idx: TX power index to apply in 0.25dB units.
 * @pa_gain_offset: PA compensation gain offset to apply.
 * @frame_adj_cfg: Returned TX Adjusted Config for frame.
 * @post_tx_adj_cfg: Returned TX Adjusted Config for Post TX tone.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_adjust_tx_power(uint8_t channel, struct tx_power_idx pwr_idx,
				   int8_t pa_gain_offset, struct uwb_tx_adj_cfg *frame_adj_cfg,
				   struct uwb_tx_adj_cfg *post_tx_adj_cfg);

/**
 * llhw_drv_set_pll_config() - Set PLL common config, containing Bias Trim.
 * @pll_cfg: PLL Common config.
 *
 * Return: QERR_SUCCESS or error.
 */
void llhw_drv_set_pll_config(uint32_t pll_cfg);

/**
 * llhw_drv_cb_data_to_uwb() - Convert callback data to `struct uwb_chip`.
 * @cb_data: callback data.
 *
 * Return: structure uwb_chip representing the UWB device.
 */
struct uwb_chip *llhw_drv_cb_data_to_uwb(const dwt_cb_data_t *cb_data);

/**
 * llhw_drv_compute_rssi() - Get rssi power computed by the driver.
 * @diag: CIR diagnostics structure.
 * @acc_idx: Accumulator (see dwt_acc_idx_e).
 * @rssi_q8: Rssi in q8.8 format.
 */
void llhw_drv_compute_rssi(const dwt_cirdiags_t *diag, dwt_acc_idx_e acc_idx, int16_t *rssi_q8);

/**
 * llhw_drv_compute_fp_power() - Get first power computed by the driver.
 * @diag: CIR diagnostics structure.
 * @acc_idx: Accumulator (see dwt_acc_idx_e).
 * @fp_rsl_q8: First path rsl in q8.8 format.
 */
void llhw_drv_compute_fp_power(const dwt_cirdiags_t *diag, dwt_acc_idx_e acc_idx,
			       uint16_t *fp_rsl_q8);

/**
 * llhw_drv_is_sip() - Return if device is a SiP (System In Package) or not.
 * @uwb: the UWB device.
 *
 * Return: True if device is a SiP, false otherwise.
 */
bool llhw_drv_is_sip(const struct uwb_chip *uwb);

/**
 * llhw_drv_get_soi_variant() - Get the SOI variant.
 * @uwb: the UWB device.
 *
 * Return: the SOI variant.
 */
uint32_t llhw_drv_get_soi_variant(const struct uwb_chip *uwb);

/**
 * llhw_drv_irq_enable() - Enable UWB IRQ wrapper.
 *
 * Return: Nothing.
 */
void llhw_drv_irq_enable(void);

/**
 * llhw_drv_irq_disable() - Disable UWB IRQ wrapper.
 *
 * Return: Nothing.
 */
void llhw_drv_irq_disable(void);

/**
 * llhw_drv_get_temperature() - Get temperature in 24q8 format.
 * @temp_24q8: returned temperature in 24q8 format.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_get_temperature(int32_t *temp_24q8);

/**
 * llhw_drv_supports_aoa() - Tell if the chip supports AOA.
 * @uwb: the UWB device.
 *
 * Return: True if the chip supports AOA, regardless of configuration.
 */
bool llhw_drv_supports_aoa(const struct uwb_chip *uwb);

/**
 * llhw_drv_convert_dwt_sfd_to_ref_frame() - Convert SFD type parameter from dwt driver to L1
 * config.
 * @dwt_sfd: SFD type DWT value.
 * @ref_frame_sfd: Returned reference frame SFD type value.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_convert_dwt_sfd_to_ref_frame(dwt_sfd_type_e dwt_sfd,
						enum ref_frame_sfd *ref_frame_sfd);

/**
 * llhw_drv_convert_dwt_data_rate_to_ref_frame() - Convert data rate parameter from dwt driver to L1
 * config.
 * @dwt_data_rate: Data rate DWT value.
 * @ref_frame_data_rate: Returned reference frame data rate value.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr
llhw_drv_convert_dwt_data_rate_to_ref_frame(dwt_uwb_bit_rate_e dwt_data_rate,
					    enum ref_frame_data_rate *ref_frame_data_rate);

/**
 * llhw_drv_get_device_info() - Get device information.
 * @uwb: the UWB device.
 * @device_info: Device information.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_get_device_info(const struct uwb_chip *uwb,
				   struct mcps802154_device_info *device_info);

/**
 * llhw_drv_convert_tx_power_to_index() - Convert a TX power value into its corresponding TX Power
 * index.
 * @channel: the channel for which the TX Power index must be calculated.
 * @tx_power: The TX power to convert.
 * @tx_power_idx: the returned TX power index.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_drv_convert_tx_power_to_index(int channel, uint8_t tx_power, uint8_t *tx_power_idx);

#ifdef __cplusplus
}
#endif
