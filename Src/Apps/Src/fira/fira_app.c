/**
 * @file      fira_app.c
 *
 * @brief     Fira processes control
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "fira_app.h"

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

#include "fira_app_config.h"
#include "reporter.h"
#include "uwbmac/uwbmac.h"
#include "uwbmac/fira_helper.h"
#include "common_fira.h"
#include "task_signal.h"
#include "HAL_error.h"
#include "cmd.h"
#include "driver_app_config.h"
#include "qmalloc.h"

#define DATA_TASK_STACK_SIZE_BYTES 1400

static struct uwbmac_context *uwbmac_ctx = NULL;
static struct fira_context fira_ctx;

#define STR_SIZE (256)
#define TEN_TO_6 1000000

static uint32_t session_id = 42;
static uint32_t session_handle = 0;
static task_signal_t dataTransferTask;
static bool started = false;
static void fira_session_info_ntf_twr_cb(const struct fira_twr_ranging_results *results, void *user_data);
static void fira_range_diagnostics_ntf_cb(const struct fira_ranging_info *info, void *user_data);
static void fira_session_status_ntf_cb(const struct fira_session_status_ntf_content *status, void *user_data);
static void fira_helper_ntfs_cb(enum fira_helper_cb_type cb_type, const void *content, void *user_data);
static struct string_measurement output_result;

static error_e fira_app_process_init(bool controller, void const *arg)
{
    fira_param_t *fira_param = (fira_param_t *)arg;
    struct fbs_session_init_rsp rsp;

    session_id = fira_param->session_id;
    uint16_t string_len = STR_SIZE * (controller ? fira_param->controlees_params.n_controlees : 1);

    output_result.str = qmalloc(string_len);
    if (!(output_result.str))
    {
        char *err = "not enough memory";
        reporter_instance.print((char *)err, strlen(err));
        return _ERR_Cannot_Alloc_Memory;
    }
    output_result.len = string_len;

    fira_uwb_mcps_init(fira_param);

    int r = uwbmac_init(&uwbmac_ctx);
    assert(r == QERR_SUCCESS);

    /* Set local short address. */
    uwbmac_set_short_addr(uwbmac_ctx, fira_param->session.short_addr);
    /* Register report cb. */
    r = fira_helper_open(&fira_ctx, uwbmac_ctx, &fira_helper_ntfs_cb, "endless", 0, &output_result);
    assert(r == QERR_SUCCESS);
    /* Set fira scheduler. */
    r = fira_helper_set_scheduler(&fira_ctx);
    assert(r == QERR_SUCCESS);
    r = fira_prepare_measurement_sequence(uwbmac_ctx, &fira_param->session);
    assert(r == QERR_SUCCESS);
    /* Init session. */
    r = fira_helper_init_session(&fira_ctx, session_id, QUWBS_FBS_SESSION_TYPE_RANGING_NO_IN_BAND_DATA, &rsp);
    assert(r == QERR_SUCCESS);
    session_handle = rsp.session_handle;
    /* Set session parameters. */
    r = fira_set_session_parameters(&fira_ctx, session_handle, &fira_param->session);
    assert(r == QERR_SUCCESS);

    if (controller)
    {
        for (int i = 0; i < fira_param->controlees_params.n_controlees; i++)
        {
            /* Add controlee session parameters. */
            r = fira_helper_add_controlee(&fira_ctx, session_handle, (const struct controlee_parameters *)&fira_param->controlees_params.controlees[i]);
            assert(r == QERR_SUCCESS);
        }
    }
    return _NO_ERR;
}

static void fira_app_process_start(void)
{
    /* OK, let's start. */
    int r = uwbmac_start(uwbmac_ctx);
    assert(r == QERR_SUCCESS);
    /* Start session. */
    r = fira_helper_start_session(&fira_ctx, session_handle);
    assert(r == QERR_SUCCESS);
    started = true;
}

static error_e fira_app_process_terminate(void)
{
    if (started)
    {
        started = false; /* do not allow re-entrance. */

        /* Stop the MAC. */
        uwbmac_stop(uwbmac_ctx);

        /* Stop session. */
        int r = fira_helper_stop_session(&fira_ctx, session_handle);
        assert(!r);
        /* Uninit session. */
        r = fira_helper_deinit_session(&fira_ctx, session_handle);
        assert(!r);
        fira_helper_close(&fira_ctx);

        /* Unregister driver. */
        fira_uwb_mcps_deinit();

        qfree(output_result.str);
    }
    return _NO_ERR;
}

static float convert_aoa_2pi_q16_to_deg(int16_t aoa_2pi_q16)
{
    return (360.0 * aoa_2pi_q16 / (1 << 16));
}

static float convert_rssi_q7_to_dbm(uint8_t rssi_q7)
{
    return (-1.0 * rssi_q7 / (1 << 1));
}

static void fira_helper_ntfs_cb(enum fira_helper_cb_type cb_type, const void *content, void *user_data)
{
    switch (cb_type)
    {
        case FIRA_HELPER_CB_TYPE_TWR_RANGE_NTF:
            fira_session_info_ntf_twr_cb((const struct fira_twr_ranging_results *)content, user_data);
            fira_range_diagnostics_ntf_cb(((const struct fira_twr_ranging_results *)content)->info, user_data);
            break;
        case FIRA_HELPER_CB_TYPE_SESSION_STATUS_NTF:
            fira_session_status_ntf_cb((const struct fira_session_status_ntf_content *)content, user_data);
            break;
        default:
            break;
    }
}

static char *fira_session_info_ntf_twr_status_to_string(uint8_t status)
{
    switch (status)
    {
        case QUWBS_FBS_STATUS_RANGING_SUCCESS:
            return "SUCCESS";
            break;
        case QUWBS_FBS_STATUS_RANGING_TX_FAILED:
            return "TX_FAILED";
            break;
        case QUWBS_FBS_STATUS_RANGING_RX_TIMEOUT:
            return "RX_TIMEOUT";
            break;
        case QUWBS_FBS_STATUS_RANGING_RX_PHY_DEC_FAILED:
            return "RX_PHY_DEC_FAILED";
            break;
        case QUWBS_FBS_STATUS_RANGING_RX_PHY_TOA_FAILED:
            return "RX_PHY_TOA_FAILED";
            break;
        case QUWBS_FBS_STATUS_RANGING_RX_PHY_STS_FAILED:
            return "RX_PHY_STS_FAILED";
            break;
        case QUWBS_FBS_STATUS_RANGING_RX_MAC_DEC_FAILED:
            return "RX_MAC_DEC_FAILED";
            break;
        case QUWBS_FBS_STATUS_RANGING_RX_MAC_IE_DEC_FAILED:
            return "RX_MAC_IE_DEC_FAILED";
            break;
        case QUWBS_FBS_STATUS_RANGING_RX_MAC_IE_MISSING:
            return "RX_MAC_IE_MISSING";
            break;
        default:
            return "Unknown";
            break;
    }
}

static char *fira_session_status_ntf_state_to_string(enum quwbs_fbs_session_state state)
{
    switch (state)
    {
        case QUWBS_FBS_SESSION_STATE_INIT:
            return "INIT";
            break;
        case QUWBS_FBS_SESSION_STATE_DEINIT:
            return "DEINIT";
            break;
        case QUWBS_FBS_SESSION_STATE_ACTIVE:
            return "ACTIVE";
            break;
        case QUWBS_FBS_SESSION_STATE_IDLE:
            return "IDLE";
            break;
        default:
            return "Unknown";
            break;
    }
}

static char *fira_session_status_ntf_reason_code_to_string(enum quwbs_fbs_reason_code reason_code)
{
    switch (reason_code)
    {
        case QUWBS_FBS_REASON_CODE_STATE_CHANGE_WITH_SESSION_MANAGEMENT_COMMANDS:
            return "State change with session management commands";
        case QUWBS_FBS_REASON_CODE_MAX_RANGING_ROUND_RETRY_COUNT_REACHED:
            return "Max ranging round retry count reached";
        case QUWBS_FBS_REASON_CODE_MAX_NUMBER_OF_MEASUREMENTS_REACHED:
            return "Max number of measurements reached";
        case QUWBS_FBS_REASON_CODE_SESSION_SUSPENDED_DUE_TO_INBAND_SIGNAL:
            return "Session suspended due to inband signal";
        case QUWBS_FBS_REASON_CODE_SESSION_RESUMED_DUE_TO_INBAND_SIGNAL:
            return "Session resumed due to inband signal";
        case QUWBS_FBS_REASON_CODE_SESSION_STOPPED_DUE_TO_INBAND_SIGNAL:
            return "Session stopped due to inband signal";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_UL_TDOA_RANDOM_WINDOW:
            return "Invalid UL TDOA random window";
        case QUWBS_FBS_REASON_CODE_ERROR_MIN_FRAMES_PER_RR_NOT_SUPPORTED:
            return "Minimum frames per ranging round not supported";
        case QUWBS_FBS_REASON_CODE_ERROR_INTER_FRAME_INTERVAL_NOT_SUPPORTED:
            return "Inter frame interval not supported";
        case QUWBS_FBS_REASON_CODE_ERROR_SLOT_LENGTH_NOT_SUPPORTED:
            return "Slot length not supported";
        case QUWBS_FBS_REASON_CODE_ERROR_INSUFFICIENT_SLOTS_PER_RR:
            return "Insufficient slots per ranging round";
        case QUWBS_FBS_REASON_CODE_ERROR_MAC_ADDRESS_MODE_NOT_SUPPORTED:
            return "MAC address mode not supported";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_RANGING_DURATION:
            return "Invalid ranging duration";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_STS_CONFIG:
            return "Invalid STS configuration";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_RFRAME_CONFIG:
            return "Invalid RFrame configuration";
        case QUWBS_FBS_REASON_CODE_ERROR_HUS_NOT_ENOUGH_SLOTS:
            return "HUS not enough slots";
        case QUWBS_FBS_REASON_CODE_ERROR_HUS_CFP_PHASE_TOO_SHORT:
            return "HUS CFP phase too short";
        case QUWBS_FBS_REASON_CODE_ERROR_HUS_CAP_PHASE_TOO_SHORT:
            return "HUS CAP phase too short";
        case QUWBS_FBS_REASON_CODE_ERROR_HUS_OTHERS:
            return "HUS others";
        case QUWBS_FBS_REASON_CODE_ERROR_STATUS_SESSION_KEY_NOT_FOUND:
            return "Status session key not found";
        case QUWBS_FBS_REASON_CODE_ERROR_STATUS_SUB_SESSION_KEY_NOT_FOUND:
            return "Status sub session key not found";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_PREAMBLE_CODE_INDEX:
            return "Invalid preamble code index";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_SFD_ID:
            return "Invalid SFD ID";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_PSDU_DATA_RATE:
            return "Invalid PSDU data rate";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_PHR_DATA_RATE:
            return "Invalid PHR data rate";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_PREAMBLE_DURATION:
            return "Invalid preamble duration";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_STS_LENGTH:
            return "Invalid STS length";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_NUM_OF_STS_SEGMENTS:
            return "Invalid number of STS segments";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_NUM_OF_CONTROLEES:
            return "Invalid number of controlees";
        case QUWBS_FBS_REASON_CODE_ERROR_MAX_RANGING_REPLY_TIME_EXCEEDED:
            return "Max ranging reply time exceeded";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_DST_ADDRESS_LIST:
            return "Invalid destination address list";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_OR_NOT_FOUND_SUB_SESSION_ID:
            return "Invalid or not found sub session ID";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_RESULT_REPORT_CONFIG:
            return "Invalid result report configuration";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_RANGING_ROUND_CONTROL_CONFIG:
            return "Invalid ranging round control configuration";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_RANGING_ROUND_USAGE:
            return "Invalid ranging round usage";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_MULTI_NODE_MODE:
            return "Invalid multi node mode";
        case QUWBS_FBS_REASON_CODE_ERROR_RDS_FETCH_FAILURE:
            return "RDS fetch failure";
        case QUWBS_FBS_REASON_CODE_ERROR_REF_UWB_SESSION_DOES_NOT_EXIST:
            return "Ref UWB session does not exist";
        case QUWBS_FBS_REASON_CODE_ERROR_REF_UWB_SESSION_RANGING_DURATION_MISMATCH:
            return "Ref UWB session ranging duration mismatch";
        case QUWBS_FBS_REASON_CODE_ERROR_REF_UWB_SESSION_INVALID_OFFSET_TIME:
            return "Ref UWB session invalid offset time";
        case QUWBS_FBS_REASON_CODE_ERROR_REF_UWB_SESSION_LOST:
            return "Ref UWB session lost";
        case QUWBS_FBS_REASON_CODE_ERROR_DT_ANCHOR_RANGING_ROUNDS_NOT_CONFIGURED:
            return "DT anchor ranging rounds not configured";
        case QUWBS_FBS_REASON_CODE_ERROR_DT_TAG_RANGING_ROUNDS_NOT_CONFIGURED:
            return "DT tag ranging rounds not configured";
        case QUWBS_FBS_REASON_CODE_ERROR_UWB_INITIATION_TIME_EXPIRED:
            return "UWB initiation time expired";
        case QUWBS_FBS_REASON_CODE_AOSP_ERROR_INVALID_CHANNEL_WITH_AOA:
            return "AOSP error invalid channel with AOA";
        case QUWBS_FBS_REASON_CODE_AOSP_ERROR_STOPPED_DUE_TO_OTHER_SESSION_CONFLICT:
            return "AOSP error stopped due to other session conflict";
        case QUWBS_FBS_REASON_CODE_AOSP_REGULATION_UWB_OFF:
            return "AOSP regulation UWB off";
        case QUWBS_FBS_REASON_CODE_ERROR_MAX_STS_REACHED:
            return "Max STS reached";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_DEVICE_ROLE:
            return "Invalid device role";
        case QUWBS_FBS_REASON_CODE_ERROR_NOMEM:
            return "No memory";
        case QUWBS_FBS_REASON_CODE_ERROR_DRIVER_DOWN:
            return "Driver down";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_PROXIMITY_RANGE:
            return "Invalid proximity range";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_FRAME_INTERVAL:
            return "Invalid frame interval";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_CAP_SIZE_RANGE:
            return "Invalid CAP size range";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_SCHEDULE_MODE:
            return "Invalid schedule mode";
        case QUWBS_FBS_REASON_CODE_ERROR_INVALID_PRF_MODE:
            return "Invalid PRF mode";
        case QUWBS_FBS_REASON_CODE_ERROR_START_CONFIG:
            return "Start configuration";
        case QUWBS_FBS_REASON_CODE_ERROR_RDS_BUSY:
            return "RDS busy";
        default:
            return "Unknown";
    }
}

static char *fira_range_diagnostics_ntf_msg_id_to_string(uint8_t msg_id)
{
    switch (msg_id)
    {
        case FBS_MESSAGE_ID_RANGING_INITIATION:
            return "RANGING_INITIATION";
            break;
        case FBS_MESSAGE_ID_RANGING_RESPONSE:
            return "RANGING_RESPONSE";
            break;
        case FBS_MESSAGE_ID_RANGING_FINAL:
            return "RANGING_FINAL";
            break;
        case FBS_MESSAGE_ID_CONTROL:
            return "CONTROL";
            break;
        case FBS_MESSAGE_ID_MEASUREMENT_REPORT:
            return "MEASUREMENT_REPORT";
            break;
        case FBS_MESSAGE_ID_RESULT_REPORT:
            return "RESULT_REPORT";
            break;
        case FBS_MESSAGE_ID_CONTROL_UPDATE:
            return "CONTROL_UPDATE";
            break;
        default:
            return "Unknown";
            break;
    }
}

static char *fira_range_diagnostics_ntf_action_to_string(uint8_t action)
{
    switch (action)
    {
        case 0:
            return "RX";
            break;
        case 1:
            return "TX";
            break;
        default:
            return "RFU";
            break;
    }
}

static char *fira_range_diagnostics_ntf_frame_status_to_string(uint8_t frame_status)
{
    const char *str = "SUCCESS: %d, WIFI_COEX: %d, GRANT_DURATION_EXCEEDED: %d";
    static char result[60];

    sprintf(result, str, (frame_status & FIRA_RANGING_DIAGNOSTICS_FRAME_REPORTS_STATUS_FLAGS_SUCCESS) ? 1 : 0, (frame_status & FIRA_RANGING_DIAGNOSTICS_FRAME_REPORTS_STATUS_FLAGS_WIFI_COEX) ? 1 : 0, (frame_status & FIRA_RANGING_DIAGNOSTICS_FRAME_REPORTS_STATUS_FLAGS_GRANT_DURATION_EXCEEDED) ? 1 : 0);

    return result;
}

static void fira_session_info_ntf_twr_cb(const struct fira_twr_ranging_results *results, void *user_data)
{
    int len = 0;
    struct string_measurement *str_result = (struct string_measurement *)user_data;
    struct fira_twr_measurements *rm;

    len += snprintf(&str_result->str[len], str_result->len, "SESSION_INFO_NTF: ");
    len += snprintf(&str_result->str[len], str_result->len - len, "{session_handle=%" PRIu32 "", results->info->session_handle);
    len += snprintf(&str_result->str[len], str_result->len - len, ", sequence_number=%" PRIu32 "", results->info->sequence_number);
    len += snprintf(&str_result->str[len], str_result->len - len, ", block_index=%" PRIu32 "", results->info->block_index);
    len += snprintf(&str_result->str[len], str_result->len - len, ", n_measurements=%d", results->n_measurements);

    for (int i = 0; i < results->n_measurements; i++)
    {
        if (i > 0)
            len += snprintf(&str_result->str[len], str_result->len - len, ";");

        rm = (struct fira_twr_measurements *)(&results->measurements[i]);

        len += snprintf(&str_result->str[len], str_result->len - len, "\r\n\r [mac_address=0x%04x, status=\"%s\"", rm->short_addr, fira_session_info_ntf_twr_status_to_string(rm->status));

        if (rm->status == 0)
        {
            len += snprintf(&str_result->str[len], str_result->len - len, ", distance[cm]=%d", (int)rm->distance_cm);

            if (rm->local_aoa_measurements[0].aoa_fom_100 > 0)
                len += snprintf(&str_result->str[len], str_result->len - len, ", loc_az_pdoa=%0.2f, loc_az=%0.2f", convert_aoa_2pi_q16_to_deg(rm->local_aoa_measurements[0].pdoa_2pi), convert_aoa_2pi_q16_to_deg(rm->local_aoa_measurements[0].aoa_2pi));

            if (rm->local_aoa_measurements[1].aoa_fom_100 > 0)
                len += snprintf(&str_result->str[len], str_result->len - len, ", loc_el_pdoa=%0.2f, loc_el=%0.2f", convert_aoa_2pi_q16_to_deg(rm->local_aoa_measurements[1].pdoa_2pi), convert_aoa_2pi_q16_to_deg(rm->local_aoa_measurements[1].aoa_2pi));

            if (rm->remote_aoa_azimuth_fom_100 > 0)
                len += snprintf(&str_result->str[len], str_result->len - len, ", rmt_az=%0.2f", convert_aoa_2pi_q16_to_deg(rm->remote_aoa_azimuth_2pi));

            if (rm->remote_aoa_elevation_fom_100 > 0)
                len += snprintf(&str_result->str[len], str_result->len - len, ", rmt_el=%0.2f", convert_aoa_2pi_q16_to_deg(rm->remote_aoa_elevation_pi));

            if (rm->rssi)
                len += snprintf(&str_result->str[len], str_result->len - len, ", RSSI[dBm]=%0.1f", convert_rssi_q7_to_dbm(rm->rssi));
        }
        len += snprintf(&str_result->str[len], str_result->len - len, "]");
    }
    len += snprintf(&str_result->str[len], str_result->len - len, "}\r\n");
    reporter_instance.print((char *)str_result->str, len);
}

static void fira_range_diagnostics_ntf_cb(const struct fira_ranging_info *info, void *user_data)
{
    struct diagnostic_info *diagnostics_info = info->diagnostic;

    if (diagnostics_info == NULL)
        return;

    int len = 0;
    struct string_measurement *str_result = (struct string_measurement *)user_data;

    len += snprintf(&str_result->str[len], str_result->len, "RANGE_DIAGNOSTICS_NTF: ");
    len += snprintf(&str_result->str[len], str_result->len - len, "{n_reports=%d", (int)diagnostics_info->nb_reports);
    reporter_instance.print((char *)str_result->str, len);

    struct frame_report *current_report = diagnostics_info->reports;

    for (int i = 0; i < diagnostics_info->nb_reports; i++)
    {
        if (current_report == NULL)
            return;

        len = 0;
        str_result = (struct string_measurement *)user_data;

        len += snprintf(&str_result->str[len], str_result->len, "\r\n\r [msg_id=%s", fira_range_diagnostics_ntf_msg_id_to_string(current_report->msg_id));
        len += snprintf(&str_result->str[len], str_result->len - len, ", action=%s", fira_range_diagnostics_ntf_action_to_string(current_report->action));
        len += snprintf(&str_result->str[len], str_result->len - len, ", antenna_set=%u", current_report->antenna_set);
        len += snprintf(&str_result->str[len], str_result->len - len, ", frame_status={%s}", fira_range_diagnostics_ntf_frame_status_to_string(current_report->extra_status));
        len += snprintf(&str_result->str[len], str_result->len - len, ", cfo_present=%d", current_report->cfo_present);

        if (current_report->cfo_present)
        {
            /* Convert Q26 to hundredths of ppm: cfo_ppm = cfo_q26 * 100 * 2^10 / 2^26 */
            int32_t cfo_100ppm = (int32_t)((((int64_t)current_report->cfo_q26) * 100 * TEN_TO_6) >> 26);
            len += snprintf(&str_result->str[len], str_result->len - len, ", cfo_ppm=%" PRIi32 ".%u", cfo_100ppm / 100, abs(cfo_100ppm) % 100);
        }
        len += snprintf(&str_result->str[len], str_result->len - len, ", nb_aoa=%u", current_report->nb_aoa);

        struct aoa_measurement *current_aoa_meas = current_report->aoas;

        for (int i = 0; i < current_report->nb_aoa; i++)
        {
            if (current_aoa_meas == NULL)
                return;

            len += snprintf(&str_result->str[len], str_result->len - len, "\r\n\r tdoa=%i", current_aoa_meas->tdoa);
            len += snprintf(&str_result->str[len], str_result->len - len, ", pdoa=%i", current_aoa_meas->pdoa);
            len += snprintf(&str_result->str[len], str_result->len - len, ", aoa=%i", current_aoa_meas->aoa);
            len += snprintf(&str_result->str[len], str_result->len - len, ", fom=%u", current_aoa_meas->fom);
            len += snprintf(&str_result->str[len], str_result->len - len, ", type=%u", current_aoa_meas->type);

            current_aoa_meas = current_aoa_meas->next;
        }

        len += snprintf(&str_result->str[len], str_result->len - len, "]");
        if (i < (diagnostics_info->nb_reports - 1))
            len += snprintf(&str_result->str[len], str_result->len - len, ";");

        reporter_instance.print((char *)str_result->str, len);

        current_report = current_report->next;
    }

    len = 0;
    str_result = (struct string_measurement *)user_data;

    len += snprintf(&str_result->str[len], str_result->len, "}\r\n");
    reporter_instance.print((char *)str_result->str, len);
}

static void fira_session_status_ntf_cb(const struct fira_session_status_ntf_content *status, void *user_data)
{
    struct string_measurement *str_result = (struct string_measurement *)user_data;
    const char *fmt = "SESSION_STATUS_NTF: {state=\"%s\", reason=\"%s\"}\r\n";
    int len = 0;

    len = sprintf(str_result->str, fmt, fira_session_status_ntf_state_to_string(status->state), fira_session_status_ntf_reason_code_to_string(status->reason_code));

    reporter_instance.print(str_result->str, len);
}

/** @brief is a service function which starts the FiRa TWR top-level  application. */
static void fira_app(bool controller, void *arg)
{
    error_e err;

    err = fira_app_process_init(controller, arg);
    if (err != _NO_ERR)
    {
        error_handler(1, err);
    }
    fira_app_process_start();
}

/**
 * @brief Kills all task and timers related to FiRa.
 * DW3000's RX and IRQ shall be switched off before task termination,
 * that IRQ will not produce unexpected Signal.
 */
void fira_terminate(void)
{
    fira_app_process_terminate();
    terminate_task(&dataTransferTask);

    uwbmac_exit(uwbmac_ctx);
}

void fira_helper_controller(const void *arg)
{
    /* Not used. */
    (void)arg;

    fira_app(true, (void *)get_fira_config());
}

/**
 * @brief Start FiRa Controller+Responder.
 * @param arg Pointer to fira_param_t or NULL if use global config.
 */
void fira_helper_controlee(const void *arg)
{
    /* Not used. */
    (void)arg;

    fira_app(false, (void *)get_fira_config());
}

const app_definition_t helpers_app_fira[] __attribute__((
    section(".known_apps")
))
= {
    {"INITF", mAPP, fira_helper_controller, fira_terminate, waitForCommand, command_parser, NULL},
    {"RESPF", mAPP, fira_helper_controlee, fira_terminate, waitForCommand, command_parser, NULL},
};

/**
 * @brief Action to be taken before saving params to NVM.
 * Ensures initialization if user didn't provide configuration before.
 */
void fira_save_params(void)
{
    fira_param_t *fira_params = get_fira_config();

    /* Reinit when config is default (user set application without providing params). */
    if (fira_params->config_state == FIRA_APP_CONFIG_DEFAULT)
    {
        fira_set_default_params(fira_params->app_type == FIRA_APP_INITF);
    }
    fira_params->config_state = FIRA_APP_CONFIG_SAVED;
}
