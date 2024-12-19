/**
 *  @file     common_fira.c
 *
 *  @brief    Fira params control
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "common_fira.h"

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

#include "fira_app_config.h"
#include "reporter.h"
#include "uwb_translate.h"
#include "qtime.h"
#include "fira_default_params.h"
#include "driver_app_config.h"
#include "uwb_utils.h"

static char *fira_device_role_param_to_string(enum quwbs_fbs_device_role device_role)
{
    switch (device_role)
    {
        case QUWBS_FBS_DEVICE_ROLE_RESPONDER:
            return "RESPONDER";
            break;
        case QUWBS_FBS_DEVICE_ROLE_INITIATOR:
            return "INITIATOR";
            break;
        default:
            return "Unknown";
            break;
    }
}

static char *fira_ranging_round_usage_param_to_string(enum fira_ranging_round_usage ranging_round_usage)
{
    switch (ranging_round_usage)
    {
        case FIRA_RANGING_ROUND_USAGE_SSTWR_DEFERRED:
            return "SS_TWR_DEFERRED";
            break;
        case FIRA_RANGING_ROUND_USAGE_DSTWR_DEFERRED:
            return "DS_TWR_DEFERRED";
            break;
        case FIRA_RANGING_ROUND_USAGE_SSTWR_NON_DEFERRED:
            return "SS_TWR_NON_DEFERRED";
            break;
        case FIRA_RANGING_ROUND_USAGE_DSTWR_NON_DEFERRED:
            return "DS_TWR_NON_DEFERRED";
            break;
        default:
            return "Unknown";
            break;
    }
}

static char *fira_multi_node_mode_param_to_string(enum fira_multi_node_mode multi_node_mode)
{
    switch (multi_node_mode)
    {
        case FIRA_MULTI_NODE_MODE_UNICAST:
            return "UNICAST";
            break;
        case FIRA_MULTI_NODE_MODE_ONE_TO_MANY:
            return "ONE_TO_MANY";
            break;
        default:
            return "Unknown";
            break;
    }
}

static char *fira_rframe_config_param_to_string(enum fira_rframe_config rframe_config)
{
    switch (rframe_config)
    {
        case FIRA_RFRAME_CONFIG_SP1:
            return "SP1";
            break;
        case FIRA_RFRAME_CONFIG_SP3:
            return "SP3";
            break;
        default:
            return "Unknown";
            break;
    }
}

void fira_show_params()
{
#define REMAINING (2048 - strlen(str))
    /* Display the Fira session parameters. */
    char *str = qmalloc(2048);
    int hlen;

    if (!str)
    {
        char *err = "Not enough memory.";
        reporter_instance.print((char *)err, strlen(err));
        return;
    }
    fira_param_t *fira_param = get_fira_config();
    hlen = sprintf(str, "FiRa Session Parameters: {\r\n");

    snprintf(&str[strlen(str)], REMAINING, "SESSION_ID: %" PRIu32 ",\r\n", fira_param->session_id);
    snprintf(&str[strlen(str)], REMAINING, "CHANNEL_NUMBER: %d,\r\n", fira_param->session.channel_number);
    snprintf(&str[strlen(str)], REMAINING, "DEVICE_ROLE: %s,\r\n", fira_device_role_param_to_string(fira_param->session.device_role));
    snprintf(&str[strlen(str)], REMAINING, "RANGING_ROUND_USAGE: %s,\r\n", fira_ranging_round_usage_param_to_string(fira_param->session.ranging_round_usage));
    snprintf(&str[strlen(str)], REMAINING, "SLOT_DURATION [rstu]: %" PRIu32 ",\r\n", fira_param->session.slot_duration_rstu);
    snprintf(&str[strlen(str)], REMAINING, "RANGING_DURATION [ms]: %" PRIu32 ",\r\n", fira_param->session.block_duration_ms);
    snprintf(&str[strlen(str)], REMAINING, "SLOTS_PER_RR: %" PRIu32 ",\r\n", fira_param->session.round_duration_slots);
    snprintf(&str[strlen(str)], REMAINING, "MULTI_NODE_MODE: %s,\r\n", fira_multi_node_mode_param_to_string(fira_param->session.multi_node_mode));
    snprintf(&str[strlen(str)], REMAINING, "HOPPING_MODE: %s,\r\n", fira_param->session.round_hopping ? "Enabled" : "Disabled");
    snprintf(&str[strlen(str)], REMAINING, "RFRAME_CONFIG: %s,\r\n", fira_rframe_config_param_to_string(fira_param->session.rframe_config));
    snprintf(&str[strlen(str)], REMAINING, "SFD_ID: %d,\r\n", fira_param->session.sfd_id);
    snprintf(&str[strlen(str)], REMAINING, "PREAMBLE_CODE_INDEX: %d,\r\n", fira_param->session.preamble_code_index);
    uint8_t *v = fira_param->session.vupper64;
    snprintf(&str[strlen(str)], REMAINING, "STATIC_STS_IV: \"%02x:%02x:%02x:%02x:%02x:%02x\",\r\n", v[0], v[1], v[2], v[3], v[4], v[5]);
    snprintf(&str[strlen(str)], REMAINING, "VENDOR_ID: \"%02x:%02x\",\r\n", v[6], v[7]);
    snprintf(&str[strlen(str)], REMAINING, "DEVICE_MAC_ADDRESS: 0x%04X,\r\n", fira_param->session.short_addr);

    if (fira_param->session.device_type == QUWBS_FBS_DEVICE_TYPE_CONTROLLER)
    {
        for (int i = 0; i < fira_param->controlees_params.n_controlees; i++)
        {
            snprintf(&str[strlen(str)], REMAINING, "DST_MAC_ADDRESS[%d]: 0x%04X", i, fira_param->controlees_params.controlees[i].address);
            if (i < fira_param->controlees_params.n_controlees - 1)
                snprintf(&str[strlen(str)], REMAINING, ",");
            snprintf(&str[strlen(str)], REMAINING, "\r\n");
        }
    }
    else
    {
        snprintf(&str[strlen(str)], REMAINING, "DST_MAC_ADDRESS: 0x%04X\r\n", fira_param->session.destination_short_address[0]);
    }
    snprintf(&str[strlen(str)], REMAINING, "}");

    reporter_instance.print((char *)str, strlen(str));
    qfree(str);
    /* Wait a little bit for printing. */
    qtime_msleep(100);
#undef REMAINING
}

static bool fira_scan_params(char *text, bool controller)
{
    fira_param_t *fira_param = get_fira_config();
    char *pch = strtok(text, " -");
    unsigned int tmp_val = 0;
    char tmp_str[30] = "";
    int sz;
    char err_msg[128];
    uint8_t cpt_arg = 0;
    int param[8];

    while (pch != NULL)
    {
        if (sscanf(pch, "CHAN=%d", &tmp_val) == 1)
        {
            if (chan_to_deca(tmp_val) != -1)
            {
                fira_param->session.channel_number = chan_to_deca(tmp_val);
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect Channel: %d\r\n", tmp_val);
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "PRFSET=%cPRF%d", &tmp_str[0], &tmp_val) == 2)
        {
            /* Only BPRF supporetd. */
            if (tmp_str[0] == 'B')
            {
                fira_param->session.prf_mode = FIRA_PRF_MODE_BPRF;
                switch (tmp_val)
                {
                    case 3: /* SP1, SFD 2. */
                        fira_param->session.rframe_config = FIRA_RFRAME_CONFIG_SP1;
                        fira_param->session.sfd_id = FIRA_SFD_ID_2;
                        break;
                    case 4: /* SP3, SFD 2. */
                        fira_param->session.rframe_config = FIRA_RFRAME_CONFIG_SP3;
                        fira_param->session.sfd_id = FIRA_SFD_ID_2;
                        break;
                    case 5: /* SP1, SFD 0. */
                        fira_param->session.rframe_config = FIRA_RFRAME_CONFIG_SP1;
                        fira_param->session.sfd_id = FIRA_SFD_ID_0;
                        break;
                    case 6: /* SP3, SFD 0. */
                        fira_param->session.rframe_config = FIRA_RFRAME_CONFIG_SP3;
                        fira_param->session.sfd_id = FIRA_SFD_ID_0;
                        break;
                    default:
                        sz = sprintf(err_msg, "Incorrect PRF Set\r\n");
                        reporter_instance.print(err_msg, sz);
                        return false;
                }
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect PRF\r\n");
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "SLOT=%d", &tmp_val) == 1)
        {
            /* Slot duration (RSTU). */
            if (tmp_val >= 1200 && tmp_val <= 12000)
            {
                fira_param->session.slot_duration_rstu = tmp_val;
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect Slot Duration (RSTU)\r\n");
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "BLOCK=%d", &tmp_val) == 1)
        {
            /* Block duration (ms). */
            if (tmp_val >= 100 && tmp_val <= 500)
            {
                fira_param->session.block_duration_ms = tmp_val;
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect Block Duration (ms)\r\n");
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "ROUND=%d", &tmp_val) == 1)
        {
            /* Round duration (slots). */
            if (tmp_val >= 2 && tmp_val <= 255)
            {
                fira_param->session.round_duration_slots = tmp_val;
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect Round Duration (slots)\r\n");
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "RRU=%s", tmp_str) == 1)
        {
            /* Ranging Round Usage. */
            if (!strcmp(tmp_str, "SSTWR"))
            {
                fira_param->session.ranging_round_usage = FIRA_RANGING_ROUND_USAGE_SSTWR_DEFERRED;
            }
            else if (!strcmp(tmp_str, "DSTWR"))
            {
                fira_param->session.ranging_round_usage = FIRA_RANGING_ROUND_USAGE_DSTWR_DEFERRED;
            }
            else if (!strcmp(tmp_str, "SSTWRNDEF"))
            {
                fira_param->session.ranging_round_usage = FIRA_RANGING_ROUND_USAGE_SSTWR_NON_DEFERRED;
            }
            else if (!strcmp(tmp_str, "DSTWRNDEF"))
            {
                fira_param->session.ranging_round_usage = FIRA_RANGING_ROUND_USAGE_DSTWR_NON_DEFERRED;
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect Ranging Round Usage\r\n");
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "ID=%d", &tmp_val) == 1)
        {
            /* Session ID. */
            if (tmp_val != 0)
            {
                fira_param->session_id = tmp_val;
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect Session ID\r\n");
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "VUPPER=%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", &param[0], &param[1], &param[2], &param[3], &param[4], &param[5], &param[6], &param[7]) == 8)
        {
            /* vUpper64. */
            uint8_t *v = fira_param->session.vupper64;
            for (int i = 0; i < FIRA_VUPPER64_SIZE; i++)
                v[i] = param[i];
        }
        else if (!strcmp(pch, "MULTI"))
        {
            /* Multi Node Mode. */
            fira_param->session.multi_node_mode = FIRA_MULTI_NODE_MODE_ONE_TO_MANY;
        }
        else if (!strcmp(pch, "HOP"))
        {
            /* Round hopping. */
            fira_param->session.round_hopping = true;
        }
        else if (sscanf(pch, "ADDR=%d", &tmp_val) == 1)
        {
            /* Own device address. */
            fira_param->session.short_addr = tmp_val;
        }
        else if (sscanf(pch, "PADDR=%s", tmp_str) == 1)
        {
            /* Peer address. */

            fira_param->controlees_params.n_controlees = 0;
            /* Single address. */
            if (tmp_str[0] >= '0' && tmp_str[0] <= '9')
            {
                if (controller)
                    fira_param->controlees_params.controlees[fira_param->controlees_params.n_controlees++].address = atoi(tmp_str);
                else
                    fira_param->session.destination_short_address[0] = atoi(tmp_str);
            }
            /* Address list, only applicable to controller. */
            else if (controller)
            {
                const char filter[4] = "[],";
                char *peer = strtok(tmp_str, filter);

                fira_param->controlees_params.n_controlees = 0;
                while (peer != NULL)
                {
                    if (fira_param->controlees_params.n_controlees < FIRA_RESPONDERS_MAX)
                    {
                        fira_param->controlees_params.controlees[fira_param->controlees_params.n_controlees++].address = atoi(peer);
                        peer = strtok(NULL, filter);
                    }
                    else
                    {
                        sz = sprintf(err_msg, "Number of peers exceeds the max: (%d) \r\n", FIRA_RESPONDERS_MAX);
                        reporter_instance.print(err_msg, sz);
                        return false;
                    }
                }
            }
            else
            {
                sz = sprintf(err_msg, "Not a valid address given: %s \r\n", tmp_str);
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "PCODE=%d", &tmp_val) == 1)
        {
            if (preamble_code_to_deca(tmp_val) != -1)
            {
                fira_param->session.preamble_code_index = preamble_code_to_deca(tmp_val);
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect Preamble Code Index\r\n");
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else if (sscanf(pch, "DRATE=%d", &tmp_val) == 1)
        {
            if (bitrate_to_deca(tmp_val) != -1)
            {
                fira_param->session.phr_data_rate = tmp_val;
            }
            else
            {
                sz = sprintf(err_msg, "Incorrect PHR Data Rate\r\n");
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        else
        {
            if (cpt_arg != 0)
            {
                sz = sprintf(err_msg, "unknown argument %s\r\n", pch);
                reporter_instance.print(err_msg, sz);
                return false;
            }
        }
        pch = strtok(NULL, " -");
        cpt_arg++;
    }

    return true;
}

bool fira_set_user_params(char *text, bool controller)
{
    const fira_app_type_t new_app_type = controller ? FIRA_APP_INITF : FIRA_APP_RESPF;
    fira_param_t *fira_param = get_fira_config();

    /* User provided parameters. */
    if (text && strchr(text, '-') != NULL)
    {
        fira_set_default_params(controller);
        if (!fira_scan_params(text, controller))
            return false;
        fira_param->config_state = FIRA_APP_CONFIG_USER;
    }
    /* User not provided parameters but config was not saved or application has changed. */
    else if ((fira_param->config_state != FIRA_APP_CONFIG_SAVED) || (fira_param->app_type != new_app_type))
    {
        fira_set_default_params(controller);
    }
    fira_param->app_type = new_app_type;

    return true;
}


void fira_set_default_params(bool controller)
{
    fira_param_t *fira_params = get_fira_config();
    struct session_parameters *session_params = &fira_params->session;

    if (controller)
    {
        session_params->n_destination_short_address = 0;
        session_params->short_addr = FIRA_DEFAULT_CONTROLLER_SHORT_ADDR;
        session_params->device_type = QUWBS_FBS_DEVICE_TYPE_CONTROLLER;
        session_params->device_role = QUWBS_FBS_DEVICE_ROLE_INITIATOR;
        fira_params->controlees_params.n_controlees = FIRA_DEFAULT_NUM_OF_CONTROLEES;
        fira_params->controlees_params.controlees[0].address = FIRA_DEFAULT_CONTROLLEE_SHORT_ADDR;
    }
    else
    {
        session_params->n_destination_short_address = 1;
        session_params->destination_short_address[0] = FIRA_DEFAULT_CONTROLLER_SHORT_ADDR;
        session_params->short_addr = FIRA_DEFAULT_CONTROLLEE_SHORT_ADDR;
        session_params->device_type = QUWBS_FBS_DEVICE_TYPE_CONTROLEE;
        session_params->device_role = QUWBS_FBS_DEVICE_ROLE_RESPONDER;
        fira_params->controlees_params.n_controlees = 0;
    }

    fira_params->session_id = FIRA_DEFAULT_SESSION_ID;
    fira_params->config_state = FIRA_APP_CONFIG_DEFAULT;
    session_params->rframe_config = FIRA_DEFAULT_RFRAME_CONFIG;
    session_params->sfd_id = FIRA_DEFAULT_SFD_ID;
    session_params->slot_duration_rstu = FIRA_DEFAULT_SLOT_DURATION_RSTU;
    session_params->block_duration_ms = FIRA_DEFAULT_BLOCK_DURATION_MS;
    session_params->round_duration_slots = FIRA_DEFAULT_ROUND_DURATION_SLOTS;
    session_params->ranging_round_usage = FIRA_DEFAULT_RANGING_ROUND_USAGE;
    session_params->multi_node_mode = FIRA_DEFAULT_MULTI_NODE_MODE;
    session_params->round_hopping = FIRA_DEFAULT_ROUND_HOPPING;
    session_params->schedule_mode = FIRA_SCHEDULE_MODE_TIME_SCHEDULED;

    /* FiRa PHY default config. */
    session_params->preamble_code_index = FIRA_DEFAULT_PREAMBLE_CODE_INDEX;
    /* Default: standard PHR rate (0). */
    session_params->phr_data_rate = FIRA_PRF_MODE_BPRF;
    /* Default: 9. */
    session_params->channel_number = FIRA_DEFAULT_CHANNEL_NUMBER;
    /* Bit 0 is for setting tof report. */
    session_params->result_report_config |= fira_helper_bool_to_result_report_config(true, false, false, false);
    /* Request measurement report phase. */
    session_params->ranging_round_control |= fira_helper_bool_to_ranging_round_control(true, false);

    uint8_t *v = session_params->vupper64;
    for (int i = 0; i < FIRA_VUPPER64_SIZE; i++)
        v[i] = i + 1;
}

enum qerr fira_prepare_measurement_sequence(struct uwbmac_context *uwbmac_ctx, struct session_parameters *session)
{
    enum qerr r = QERR_EFAULT;
    struct uwbmac_device_info device_info;
    if (uwbmac_get_device_info(uwbmac_ctx, &device_info))
        return r;

    if (uwb_device_is_aoa(device_info.dev_id))
    {
        session->meas_seq.n_steps = 1;
        session->meas_seq.steps[0].type = FIRA_MEASUREMENT_TYPE_AOA_AZIMUTH;
        session->meas_seq.steps[0].n_measurements = 1;
        session->meas_seq.steps[0].rx_ant_set_nonranging = 0xff;
        session->meas_seq.steps[0].rx_ant_sets_ranging[0] = 0xff;
        session->meas_seq.steps[0].rx_ant_sets_ranging[1] = 0xff;
        session->meas_seq.steps[0].tx_ant_set_nonranging = 0xff;
        session->meas_seq.steps[0].tx_ant_set_ranging = 0xff;
        session->result_report_config |= fira_helper_bool_to_result_report_config(false, true, false, true);
        r = QERR_SUCCESS;
    }
    else
    {
        session->meas_seq.n_steps = 1;
        session->meas_seq.steps[0].type = FIRA_MEASUREMENT_TYPE_RANGE;
        session->meas_seq.steps[0].n_measurements = 1;
        session->meas_seq.steps[0].rx_ant_set_nonranging = 0xff;
        session->meas_seq.steps[0].rx_ant_sets_ranging[0] = 0xff;
        session->meas_seq.steps[0].rx_ant_sets_ranging[1] = 0xff;
        session->meas_seq.steps[0].tx_ant_set_nonranging = 0xff;
        session->meas_seq.steps[0].tx_ant_set_ranging = 0xff;
        session->result_report_config &= ~fira_helper_bool_to_result_report_config(false, true, true, true);
        r = QERR_SUCCESS;
    }

    return r;
}

enum qerr fira_set_session_parameters(struct fira_context *fira_context, uint32_t session_handle, struct session_parameters *session)
{
    enum qerr r = QERR_SUCCESS;

#define SET_AND_RETURN_IF_FAILED(param)                                                    \
    do                                                                                     \
    {                                                                                      \
        r = fira_helper_set_session_##param(fira_context, session_handle, session->param); \
        if (r)                                                                             \
            return r;                                                                      \
    } while (0)

    SET_AND_RETURN_IF_FAILED(channel_number);
    SET_AND_RETURN_IF_FAILED(preamble_code_index);
    SET_AND_RETURN_IF_FAILED(sfd_id);
    SET_AND_RETURN_IF_FAILED(phr_data_rate);
    SET_AND_RETURN_IF_FAILED(prf_mode);
    SET_AND_RETURN_IF_FAILED(device_type);
    SET_AND_RETURN_IF_FAILED(device_role);
    SET_AND_RETURN_IF_FAILED(multi_node_mode);
    SET_AND_RETURN_IF_FAILED(rframe_config);
    SET_AND_RETURN_IF_FAILED(slot_duration_rstu);
    SET_AND_RETURN_IF_FAILED(block_duration_ms);
    SET_AND_RETURN_IF_FAILED(round_duration_slots);
    SET_AND_RETURN_IF_FAILED(ranging_round_usage);
    SET_AND_RETURN_IF_FAILED(round_hopping);
    SET_AND_RETURN_IF_FAILED(block_stride_length);
    SET_AND_RETURN_IF_FAILED(schedule_mode);
    SET_AND_RETURN_IF_FAILED(vupper64);
    SET_AND_RETURN_IF_FAILED(result_report_config);
    SET_AND_RETURN_IF_FAILED(ranging_round_control);
    SET_AND_RETURN_IF_FAILED(enable_diagnostics);
    SET_AND_RETURN_IF_FAILED(report_rssi);

    r = fira_helper_set_session_short_address(fira_context, session_handle, session->short_addr);
    if (r)
        return r;

    r = fira_helper_set_session_destination_short_addresses(fira_context, session_handle, session->n_destination_short_address, session->destination_short_address);
    if (r)
        return r;

    r = fira_helper_set_session_measurement_sequence(fira_context, session_handle, &session->meas_seq);
    if (r)
        return r;

    r = fira_helper_set_session_diags_frame_reports_fields(fira_context, session_handle, FIRA_RANGING_DIAGNOSTICS_FRAME_REPORT_SEGMENT_METRICS | FIRA_RANGING_DIAGNOSTICS_FRAME_REPORT_CFO);
    if (r)
        return r;

    return r;
}
