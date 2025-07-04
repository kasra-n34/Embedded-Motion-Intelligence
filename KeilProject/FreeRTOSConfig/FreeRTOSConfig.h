#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>
extern uint32_t gSysClock; // Your global clock set in main.c

#define configCPU_CLOCK_HZ              (gSysClock)
#define configTICK_RATE_HZ              ((TickType_t)1000) // 1 ms tick
#define configMAX_PRIORITIES            (5)
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  (5 << 5)  // Priority level 5, shifted for CMSIS


#define configKERNEL_INTERRUPT_PRIORITY         (7 << 5)  // Lowest priority
#define configMAX_API_CALL_INTERRUPT_PRIORITY   configMAX_SYSCALL_INTERRUPT_PRIORITY

#define configMINIMAL_STACK_SIZE        ((uint16_t)128)
#define configTOTAL_HEAP_SIZE           ((size_t)(10 * 1024)) // 10 KB
#define configMAX_TASK_NAME_LEN         (16)

#define configUSE_PREEMPTION            1
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0
#define configUSE_16_BIT_TICKS          0
#define configUSE_MUTEXES               1
#define configUSE_COUNTING_SEMAPHORES   1

#define configUSE_TIME_SLICING          1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

#define configUSE_TRACE_FACILITY        1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1

#define configUSE_CO_ROUTINES           0
#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       (2)
#define configTIMER_QUEUE_LENGTH        10
#define configTIMER_TASK_STACK_DEPTH    256

#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskDelete             1
#define INCLUDE_vTaskDelay              1
#define INCLUDE_vTaskDelayUntil         1
#define INCLUDE_vTaskSuspend            1

#endif /* FREERTOS_CONFIG_H */
