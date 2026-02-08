#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "pico.h"

/* --- 1. NÚCLEO E ESTABILIDADE --- */
#define configNUMBER_OF_CORES                   1
#define configUSE_PREEMPTION                    1
#define configCPU_CLOCK_HZ                      ( 125000000 )
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                    ( 5 )
#define configMINIMAL_STACK_SIZE                ( ( unsigned short ) 256 )
#define configMAX_TASK_NAME_LEN                 ( 16 )
#define configIDLE_SHOULD_YIELD                 1

/* --- 2. SINCRONIZAÇÃO (RESOLVE O ERRO DE LINKAGEM) --- */
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_EVENT_GROUPS                  1 // Necessário para o port.c
#define configSUPPORT_PICO_SYNC_INTEROP         1 // Ativa a integração com o SDK
#define configUSE_16_BIT_TICKS                  0
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0

/* --- 3. TIMERS (ESSENCIAL PARA vEventGroupSetBitsCallback) --- */
#define configUSE_TIMERS                        1
#define INCLUDE_xTimerPendFunctionCall          1 // ESTA LINHA É A SOLUÇÃO DO ERRO
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            1024

/* --- 4. GESTÃO DE MEMÓRIA --- */
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 64 * 1024 ) )

/* --- 5. PERMISSÕES DE API --- */
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xSemaphoreGetMutexHolder        1

/* --- 6. MAPEAMENTO DE INTERRUPÇÕES RP2040 --- */
#define vPortSVCHandler    isr_svc
#define xPortPendSVHandler isr_pendsv
#define xPortSysTickHandler isr_systick

/* --- 7. ASSERT SEGURO (RESOLVE O ERRO 'taskDISABLE_INTERRUPTS') --- */
/* Usamos __breakpoint para Debug ou um loop infinito simples */
#define configASSERT( x ) if( ( x ) == 0 ) { portDISABLE_INTERRUPTS(); for( ;; ); }

#endif