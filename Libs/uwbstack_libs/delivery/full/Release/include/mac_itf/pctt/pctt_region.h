/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154.h"
#include "net/mcps802154_schedule.h"
#include "net/pctt_region_params.h"
#include "pctt/pctt_session.h"
#include "qtils.h"

#include <stdbool.h>
#include <stdint.h>

#define PCTT_SESSION_ID 0
#define PCTT_BOOLEAN_MAX 1
#define PCTT_FRAMES_MAX 2

#define PCTT_TIMESTAMP_SHIFT 9

/* Size of the Histogram (100dBm - 40dBm) with a step of 0.5 dBm. */
#define HISTOGRAM_SIZE 121
/* Histogram step (0.5 dBm) */
#define HISTOGRAM_STEP_Q8 128
/* MIN_RSSI = 40 dBm. */
#define MIN_RSSI_Q8 10240
/* MAX_RSSI = 100 dBm. */
#define MAX_RSSI_Q8 25600

/**
 * struct pctt_aoa_info - Ranging AoA information.
 */
struct pctt_aoa_info {
	/**
	 * @pdoa_2pi: Phase Difference of Arrival.
	 */
	int16_t pdoa_2pi;
	/**
	 * @aoa_2pi: Angle of Arrival.
	 */
	int16_t aoa_2pi;
	/**
	 * @present: true if AoA information is present.
	 */
	bool present;
};

/**
 * struct pctt_test_rssi_results - RSSI information used for reports.
 */
struct pctt_test_rssi_results {
	/**
	 * @n_rssis: The number of RSSIs in the array below.
	 */
	uint8_t n_rssis;
	/**
	 * @rssis_q8: Received signal strength indication (RSSI), absolute value
	 * in Q8.8 fixed point format, unit is dBm.
	 */
	uint16_t rssis_q8[MCPS802154_RSSIS_N_MAX];
	/**
	 * @noise_value: General noise value in dB, allowing to compute SNR.
	 */
	int16_t noise_value;
};

/**
 * struct pctt_test_per_rx_results - PER_RX result for report.
 */
struct pctt_test_per_rx_results {
	/**
	 * @attempts: Number of RX attempts.
	 */
	uint32_t attempts;
	/**
	 * @acq_detect: Number of times signal was detected.
	 */
	uint32_t acq_detect;
	/**
	 * @acq_reject: Number of times signal was rejected.
	 */
	uint32_t acq_reject;
	/**
	 * @rx_fail: Number of times RX did not go beyond ACQ stage.
	 */
	uint32_t rx_fail;
	/**
	 * @sync_cir_ready: Number of times sync CIR ready event was received.
	 */
	uint32_t sync_cir_ready;
	/**
	 * @sfd_fail: Number of time RX was stuck at either ACQ detect or sync
	 * CIR ready.
	 */
	uint32_t sfd_fail;
	/**
	 * @sfd_found: Number of times SFD was found.
	 */
	uint32_t sfd_found;
	/**
	 * @phr_dec_error: Number of times PHR decode failed.
	 */
	uint32_t phr_dec_error;
	/**
	 * @phr_bit_error: Number of times PHR bits in error.
	 */
	uint32_t phr_bit_error;
	/**
	 * @psdu_dec_error: Number of times payload decode failed.
	 */
	uint32_t psdu_dec_error;
	/**
	 * @psdu_bit_error: Number of times payload bits in error.
	 */
	uint32_t psdu_bit_error;
	/**
	 * @sts_found: Number of times STS detection was successful.
	 */
	uint32_t sts_found;
	/**
	 * @eof: Number of times end of frame event was triggered.
	 */
	uint32_t eof;
	/**
	 * @rssi_acc_q8: Accumulated values of Received signal strength
	 * indication (RSSI).
	 */
	uint64_t rssi_acc_q8[MCPS802154_RSSIS_N_MAX];
	/**
	 * @rssi_histogram: sort RSSI values with a histogram.
	 */
	uint16_t *rssi_histogram[MCPS802154_RSSIS_N_MAX];
	/**
	 * @rssi_acc_cnt: Counter of accumulated RSSI measurements.
	 */
	uint32_t rssi_acc_cnt;
	/**
	 * @cfo_q26_sum: Clock Frequency Offset (CFO) sum.
	 */
	int cfo_q26_sum;
	/**
	 * @cfo_count: Number of CFOs aggregated.
	 */
	int cfo_count;
	/**
	 * @noise_value: General noise value in dB, allowing to compute SNR.
	 */
	int16_t noise_value;
	/**
	 * @n_rssis: The number of RSSIs in the array ``rssi_acc_q8``.
	 */
	uint8_t n_rssis;
};

/**
 * struct pctt_test_rx_results - RX result for report.
 */
struct pctt_test_rx_results {
	/**
	 * @rx_done_ts_int: Integer part of timestamp 1/124.8Mhz ticks.
	 */
	uint32_t rx_done_ts_int;
	/**
	 * @rx_done_ts_frac: Fractional part of timestamp in 1/(128 * 499.2Mhz)
	 * ticks.
	 */
	uint16_t rx_done_ts_frac;
	/**
	 * @aoa_azimuth: Azimuth ranging AoA information.
	 */
	struct pctt_aoa_info aoa_azimuth;
	/**
	 * @aoa_elevation: Elevation ranging AoA information.
	 */
	struct pctt_aoa_info aoa_elevation;
	/**
	 * @phr: Received PHR (bits 0-12 as per IEEE spec).
	 */
	uint16_t phr;
	/**
	 * @toa_gap: ToA of main path minus ToA of first path in nanosecond.
	 */
	uint8_t toa_gap;
	/**
	 * @rssis: Received signal strength indication (RSSI) information.
	 */
	struct pctt_test_rssi_results rssis;
	/**
	 * @cfo_q26: Clock Frequency Offset (CFO).
	 */
	int16_t cfo_q26;
	/**
	 * @psdu_data: Received PSDU Data.
	 */
	struct sk_buff *psdu_data;
};

/**
 * struct pctt_test_ss_twr_results - SS_TWR result for report.
 */
struct pctt_test_ss_twr_results {
	/**
	 * @tx_timestamps_rctu: Timestamps of transmitted frame.
	 */
	uint64_t tx_timestamps_rctu;
	/**
	 * @rx_timestamps_rctu: Timestamps of received frame.
	 */
	uint64_t rx_timestamps_rctu;
	/**
	 * @measurement_rctu: Contains Tround time of Initiator or
	 * Treply time of Responder depending on DEVICE_ROLE option.
	 */
	uint32_t measurement_rctu;
	/**
	 * @aoa_azimuth: Azimuth ranging AoA information.
	 */
	struct pctt_aoa_info aoa_azimuth;
	/**
	 * @aoa_elevation: Elevation ranging AoA information.
	 */
	struct pctt_aoa_info aoa_elevation;
	/**
	 * @rssis: Received signal strength indication (RSSI) information.
	 */
	struct pctt_test_rssi_results rssis;
	/**
	 * @cfo_q26: Clock Frequency Offset (CFO).
	 */
	int16_t cfo_q26;
};

/**
 * struct pctt_test_loopback_results - LOOPBACK result for report.
 */
struct pctt_test_loopback_results {
	/**
	 * @rssis: Received signal strength indication (RSSI) information.
	 */
	struct pctt_test_rssi_results rssis;
	/**
	 * @tx_ts_int: Integer part of TX timestamp in 1/124.8 us. resolution.
	 */
	uint32_t tx_ts_int;
	/**
	 * @tx_ts_frac: Fractional part of TX timestamp in 1/124.8/512 us.
	 * resolution.
	 */
	uint16_t tx_ts_frac;
	/**
	 * @rx_ts_int: Integer part of Rx timestamp in 1/124.8 us. resolution.
	 */
	uint32_t rx_ts_int;
	/**
	 * @rx_ts_frac: Fractional part of RX timestamp in 1/124.8/512 us.
	 * resolution.
	 */
	uint16_t rx_ts_frac;
	/**
	 * @cfo_q26: Clock Frequency Offset (CFO).
	 */
	int16_t cfo_q26;
};

/**
 * union pctt_tests_results - All commands notifications.
 */
union pctt_tests_results {
	/**
	 * @per_rx: Result of the PER_RX command.
	 */
	struct pctt_test_per_rx_results per_rx;
	/**
	 * @rx: Result of the RX command.
	 */
	struct pctt_test_rx_results rx;
	/**
	 * @ss_twr: Result of the SS_TWR command.
	 */
	struct pctt_test_ss_twr_results ss_twr;
	/**
	 * @loopback: Result of the LOOPBACK command.
	 */
	struct pctt_test_loopback_results loopback;
};

/**
 * union pctt_results - Main notification for all commands.
 */
struct pctt_results {
	/**
	 * @status: Result of the command done.
	 */
	enum pctt_status_ranging status;
	/**
	 * @tests: Result detail.
	 */
	union pctt_tests_results tests;
};

/**
 * struct pctt_frame - PCTT frame information used to build mcps802154 frame.
 */
struct pctt_frame {
	/**
	 * @is_tx: Is transmit frame?
	 */
	bool is_tx;
	/**
	 * @is_rframe: Is ranging frame?
	 */
	bool is_rframe;
	/**
	 * @is_immediate: True when the frame is immediate.
	 */
	bool is_immediate;
	/**
	 * @is_ranging_pdoa: Enable the PDOA in rx.
	 */
	bool is_ranging_pdoa;
	/**
	 * @timestamp_dtu: Date to enable the receiver, or transmission.
	 */
	uint32_t timestamp_dtu;
	/**
	 * @timeout_dtu: see (mcps802154_rx_frame_config).timeout_dtu.
	 */
	int32_t timeout_dtu;
	/**
	 * @need_ranging_clock: Requests that the ranging clock be kept valid
	 * until the next frame.
	 */
	bool need_ranging_clock;
};

struct pctt_local {
	/**
	 * @region: Region instance returned to MCPS.
	 */
	struct mcps802154_region region;
	/**
	 * @llhw: Low-level device pointer.
	 */
	struct mcps802154_llhw *llhw;
	/**
	 * @access: Access returned to MCPS.
	 */
	struct mcps802154_access access;
	/**
	 * @session: Unique session on the PCTT.
	 */
	struct pctt_session session;
	/**
	 * @frames: Access frames referenced from access.
	 */
	struct mcps802154_access_frame frame;
	/**
	 * @sts_param: STS parameters for access frames.
	 */
	struct mcps802154_sts_params sts_param;
	/**
	 * @hrp_uwb_params: HRP UWB parameters, read only while the session is
	 * active.
	 */
	struct mcps802154_hrp_uwb_params hrp_uwb_params;
	/**
	 * @results: Test result used for notification at the end of test.
	 */
	struct pctt_results results;
	/**
	 * @channel: Channel parameters for access.
	 */
	struct mcps802154_channel channel;
};

static inline struct pctt_local *region_to_local(struct mcps802154_region *region)
{
	return qparent_of(region, struct pctt_local, region);
}

static inline struct pctt_local *access_to_local(struct mcps802154_access *access)
{
	return qparent_of(access, struct pctt_local, access);
}

/**
 * pctt_report() - PCTT Report.
 * @local: pctt context.
 */
void pctt_report(struct pctt_local *local);

/**
 * pctt_session_notify_state_change() - Notify session state change to upper
 * layers.
 * @local: context.
 */
void pctt_session_notify_state_change(struct pctt_local *local);
