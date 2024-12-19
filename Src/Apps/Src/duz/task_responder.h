#ifndef __responder_TASK__H__
#define __responder_TASK__H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void responder_task_notify(void);
bool responder_task_started(void);


#ifdef __cplusplus
}
#endif

#endif /* __responder_TASK__H__ */
