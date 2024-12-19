#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "EventManager.h"
#include "investigator.h"
#include "reporter.h"
#include "qmalloc.h"
#include "cmd.h"
#include "cmd_fn.h"
#include "qirq.h"

;
const char COMMENT_ISTAT[] = {"Displays the statistics inside the Investigator application."};
const char COMMENT_INVESTIGATOR[] = {"Duz Investigator."};

extern const app_definition_t helpers_app_investigator[];

/**
 * @brief   defaultTask will start investigator user application
 *
 * */
REG_FN(f_investigator)
{
    app_definition_t *app_ptr = (app_definition_t *)&helpers_app_investigator[0];
    EventManagerRegisterApp((void *)&app_ptr);
    return (CMD_FN_RET_OK);
}

REG_FN(f_investigator_stat)
{
    char *str = qmalloc(MAX_STR_SIZE);

    if (str)
    {
        unsigned int lock = qirq_lock();
        int hlen, str_len;
        /* Listener RX Event Counts object. */
        investigator_info_t *info = getInvestigatorInfoPtr();
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


const struct command_s known_app_investigator[] __attribute__((section(".known_commands_app"))) = {
    {"I", mIDLE, f_investigator, COMMENT_INVESTIGATOR},
};

const struct command_s known_subcommands_investigator[] __attribute__((section(".known_app_subcommands"))) = {
    {"STAT", mAPP, f_investigator_stat, COMMENT_ISTAT},
};
