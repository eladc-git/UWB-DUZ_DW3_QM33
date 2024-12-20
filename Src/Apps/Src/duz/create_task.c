#include "create_task.h"

#define RESPONDER_TASK_ALL (STOP_TASK | RESPONDER_DATA)

error_e create_responder_task(void(*ResponderTask(void const *)), task_signal_t *responderTask, uint16_t stackSize)
{
    error_e ret = _ERR_Cannot_Alloc_Memory;
    responderTask->SignalMask = RESPONDER_TASK_ALL;
    osThreadDef(responderTask, (void *)ResponderTask, QTHREAD_PRIORITY_HIGH, 0, stackSize / 4);
    responderTask->Handle = osThreadCreate(osThread(responderTask), NULL);
    if (responderTask->Handle)
    {
        ret = _NO_ERR;
    }
    return (ret);
}


#define INVESTIGATOR_TASK_ALL (STOP_TASK | INVESTIGATOR_DATA)

error_e create_investigator_task(void(*InvestigatorTask(void const *)), task_signal_t *investigatorTask, uint16_t stackSize)
{
    error_e ret = _ERR_Cannot_Alloc_Memory;
    investigatorTask->SignalMask = INVESTIGATOR_TASK_ALL;
    osThreadDef(investigatorTask, (void *)InvestigatorTask, QTHREAD_PRIORITY_HIGH, 0, stackSize / 4);
    investigatorTask->Handle = osThreadCreate(osThread(investigatorTask), NULL);
    if (investigatorTask->Handle)
    {
        ret = _NO_ERR;
    }
    return (ret);
}