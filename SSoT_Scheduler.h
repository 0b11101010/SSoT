// author : erhan akagunduz
#ifndef SSOT_SCHEDULER_H
#define SSOT_SCHEDULER_H

#include <stdint.h>
#include "SSoT_ControlBlock.h"

#define MAX_LOW_PRIORITY_THREAD_COUNT       ((uint32_t)16U)
#define MAX_HIGH_PRIORITY_THREAD_COUNT      ((uint32_t)16U)

int32_t create_SSoT_thread(
    SSoT_PriorityType priority,
    SSoT_NameType* name,
    SSoT_ThreadMethodType method
    );

pthread_t run_SSoT_Scheduler(void);

#endif
