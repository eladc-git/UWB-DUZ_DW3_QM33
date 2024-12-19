#include "task_signal.h"
#include "int_priority.h"
#include "HAL_error.h"
#include "responder.h"
#include "investigator.h"

#define RESPONDER_DATA  2
#define INVESTIGATOR_DATA  2

error_e create_responder_task(void(*ResponderTask(void const *)), task_signal_t *responderTask, uint16_t stackSize);
error_e create_investigator_task(void(*InvestigatorTask(void const *)), task_signal_t *investigatorTask, uint16_t stackSize);