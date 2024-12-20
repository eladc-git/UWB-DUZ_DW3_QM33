#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "cmd.h"
#include "cmd_fn.h"
#include "EventManager.h"
#include "responder.h"
#include "reporter.h"
#include "qmalloc.h"
#include "qirq.h"

const char COMMENT_RSTAT[] = {"Displays the statistics inside the Duz Responder application."};
const char COMMENT_RESPONDER[] = {"Duz Responder."};

extern const app_definition_t helpers_app_responder[];

/**
 * @brief   defaultTask will start responder user application
 *
 * */
REG_FN(f_responder)
{
    app_definition_t *app_ptr = (app_definition_t *)&helpers_app_responder[0];
    EventManagerRegisterApp((void *)&app_ptr);
    return (CMD_FN_RET_OK);
}

REG_FN(f_responder_stat)
{
    char *str = qmalloc(MAX_STR_SIZE);

    if (str)
    {
        unsigned int lock = qirq_lock();
        int hlen, str_len;
        /* Listener RX Event Counts object. */
        responder_info_t *info = getResponderInfoPtr();
        /* Reserve space for length of JS object. */
        str_len = sprintf(str, "JS%04X", 0x5A5A);
        hlen = str_len;
        str_len += sprintf(&str[str_len], "{\"RX Events\":{\r\n");
        str_len += sprintf(&str[str_len], "\"CRCG\":%d,\r\n", (int)info->event_counts.CRCG);
        str_len += sprintf(&str[str_len], "\"CRCB\":%d,\r\n", (int)info->event_counts.CRCB);
        str_len += sprintf(&str[str_len], "\"ARFE\":%d,\r\n", (int)info->event_counts.ARFE);
        str_len += sprintf(&str[str_len], "\"PHE\":%d,\r\n", (int)info->event_counts.PHE);
        str_len += sprintf(&str[str_len], "\"RSL\":%d,\r\n", (int)info->event_counts.RSL);
        str_len += sprintf(&str[str_len], "\"SFDTO\":%d,\r\n", (int)info->event_counts.SFDTO);
        str_len += sprintf(&str[str_len], "\"PTO\":%d,\r\n", (int)info->event_counts.PTO);
        str_len += sprintf(&str[str_len], "\"FTO\":%d,\r\n", (int)info->event_counts.RTO);
        str_len += sprintf(&str[str_len], "\"SFDD\":%d}}", (int)info->event_counts_sfd_detect);
        /* Add formatted 4X of length, this will erase first '{'. */
        sprintf(&str[2], "%04X", str_len - hlen);
        /* Restore the start bracket. */
        str[hlen] = '{';
        str_len += sprintf(&str[strlen(str)], "\r\n");
        reporter_instance.print((char *)str, str_len);

        assert(str_len <= MAX_STR_SIZE);

        qfree(str);

        qirq_unlock(lock);
    }
    return (CMD_FN_RET_OK);
}


const struct command_s known_app_responder[] __attribute__((section(".known_commands_app"))) = {
    {"R", mIDLE, f_responder, COMMENT_RESPONDER},
};

const struct command_s known_subcommands_responder[] __attribute__((section(".known_app_subcommands"))) = {
    {"RSTAT", mAPP, f_responder_stat, COMMENT_RSTAT},
};
