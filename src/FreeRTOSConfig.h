#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// Incluir apenas headers genéricos que não dependem de configuração
#include <stdint.h>
#include <stddef.h>

/* ========================================================== */
/* CONFIGURAÇÕES DO KERNEL                    */
/* ========================================================== */
#define configUSE_PREEMPTION                    1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_MINIMAL_IDLE_HOOK             0 
#define configCPU_CLOCK_HZ                      ( 125000000 )
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                    ( 5 )
#define configMINIMAL_STACK_SIZE                ( ( unsigned short ) 256 )
#define configMAX_TASK_NAME_LEN                 ( 16 )

/* --- CORREÇÃO DO ERRO DE TICK (08/02) --- */
/* O RP2040 é 32-bit. Definimos explicitamente a largura do tick. */
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS

/* ========================================================== */
/* SMP (MULTICORE) & MEMÓRIA                   */
/* ========================================================== */
#define configNUMBER_OF_CORES                   2
#define configRUN_MULTIPLE_PRIORITIES           1
#define configUSE_CORE_AFFINITY                 1
#define configTICK_CORE                         0

/* --- SMP SPINLOCKS (SINCRONIZAÇÃO MULTICORE) --- */
#define configSMP_SPINLOCK_0                    PICO_SPINLOCK_ID_OS1
#define configSMP_SPINLOCK_1                    PICO_SPINLOCK_ID_OS2

#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 64 * 1024 ) )

/* ========================================================== */
/* FUNCIONALIDADES DO SISTEMA                 */
/* ========================================================== */
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            1024
#define configUSE_QUEUE_SETS                    1
#define configUSE_TASK_NOTIFICATIONS            1

/* ========================================================== */
/* INTEROPERABILIDADE E API                   */
/* ========================================================== */
#define configSUPPORT_PICO_SYNC_INTEROP         1
#define configSUPPORT_PICO_TIME_INTEROP         1

#define INCLUDE_vTaskDelay                      1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xSemaphoreGetMutexHolder        1

/* ========================================================== */
/* MAPEAMENTO DE INTERRUPÇÕES                   */
/* ========================================================== */
#define vPortSVCHandler                         isr_svc
#define xPortPendSVHandler                      isr_pendsv
#define xPortSysTickHandler                     isr_systick

/* --- CORREÇÃO DE ESCOPO (08/02) --- */
/* Usamos portDISABLE_INTERRUPTS (nível de hardware) em vez de task... */
#define configASSERT( x ) \
    if( ( x ) == 0 ) { \
        portDISABLE_INTERRUPTS(); \
        for( ;; ); \
    }

#endif /* FREERTOS_CONFIG_H */