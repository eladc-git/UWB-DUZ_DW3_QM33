#include "utils.h"


void waitforsysstatus(uint32_t *lo_result, uint32_t *hi_result, uint32_t lo_mask, uint32_t hi_mask)
{
    uint32_t lo_result_tmp = 0;
    uint32_t hi_result_tmp = 0;

    // If a mask has been passed into the function for the system status register (lower 32-bits)
    if (lo_mask)
    {
        while (!((lo_result_tmp = dwt_readsysstatuslo()) & (lo_mask)))
        {
            // If a mask value is set for the system status register (higher 32-bits)
            if (hi_mask)
            {
                // If mask value for the system status register (higher 32-bits) is found
                if ((hi_result_tmp = dwt_readsysstatushi()) & hi_mask)
                {
                    break;
                }
            }
        }
    }
    // if only a mask value for the system status register (higher 32-bits) is set
    else if (hi_mask)
    {
        while (!((hi_result_tmp = dwt_readsysstatushi()) & (hi_mask))) { };
    }

    if (lo_result != NULL)
    {
        *lo_result = lo_result_tmp;
    }

    if (hi_result != NULL)
    {
        *hi_result = hi_result_tmp;
    }
}
