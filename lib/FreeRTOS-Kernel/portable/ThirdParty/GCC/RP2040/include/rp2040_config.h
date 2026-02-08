/*
 * FreeRTOS Kernel V10.4.3
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: MIT AND BSD-3-Clause
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 */

#ifndef RP2040_CONFIG_H
#define RP2040_CONFIG_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/* FreeRTOS Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Hardware specifics. */
#include "hardware/exception.h"
#include "hardware/irq.h"  /* ESSENCIAL para Pico SDK v1.5.1 */
#include "hardware/regs/m0plus.h"
#include "hardware/structs/sio.h"
#include "hardware/sync.h"

/*-----------------------------------------------------------*/
/* Definições de IRQ para compatibilidade com Pico SDK v1.5.1 */
/*-----------------------------------------------------------*/

#ifndef SVC_IRQ
    #define SVC_IRQ 11
#endif

#ifndef PENDSV_IRQ
    #define PENDSV_IRQ 14
#endif

#ifndef SYSTICK_IRQ
    #define SYSTICK_IRQ 15
#endif

/* Mapeamento para compatibilidade com código existente */
#ifndef SVCall_IRQn
    #define SVCall_IRQn SVC_IRQ
#endif

#ifndef PendSV_IRQn
    #define PendSV_IRQn PENDSV_IRQ
#endif

#ifndef SysTick_IRQn
    #define SysTick_IRQn SYSTICK_IRQ
#endif

/*-----------------------------------------------------------*/
/* configUSE_DYNAMIC_EXCEPTION_HANDLERS == 1 means set the exception handlers dynamically on cores
 * that need them in case the user has set up distinct vector table offsets per core
 */
#ifndef configUSE_DYNAMIC_EXCEPTION_HANDLERS
    #if defined( PICO_NO_RAM_VECTOR_TABLE ) && ( PICO_NO_RAM_VECTOR_TABLE == 1 )
        #define configUSE_DYNAMIC_EXCEPTION_HANDLERS 0
    #else
        #define configUSE_DYNAMIC_EXCEPTION_HANDLERS 0  /* MUDADO para 0 para evitar problemas */
    #endif
#endif

/*-----------------------------------------------------------*/
/* configSUPPORT_PICO_SYNC_INTEROP == 1 means that SDK pico_sync
 * sem/mutex/queue etc. will work correctly when called from FreeRTOS tasks
 */
#ifndef configSUPPORT_PICO_SYNC_INTEROP
    #if LIB_PICO_SYNC
        #define configSUPPORT_PICO_SYNC_INTEROP 1
    #else
        #define configSUPPORT_PICO_SYNC_INTEROP 0
    #endif
#endif

/*-----------------------------------------------------------*/
/* configSUPPORT_PICO_SYNC_INTEROP == 1 means that SDK pico_time
 * sleep_ms/sleep_us/sleep_until will work correctly when called from FreeRTOS
 * tasks, and will actually block at the FreeRTOS level
 */
#ifndef configSUPPORT_PICO_TIME_INTEROP
    #if LIB_PICO_TIME
        #define configSUPPORT_PICO_TIME_INTEROP 1
    #else
        #define configSUPPORT_PICO_TIME_INTEROP 0
    #endif
#endif

/*-----------------------------------------------------------*/
/* SMP configuration */
#if ( configNUMBER_OF_CORES > 1 )
    /* configTICK_CORE indicates which core should handle the SysTick
     * interrupts */
    #ifndef configTICK_CORE
        #define configTICK_CORE 0
    #endif
    
    #ifndef portMAX_CORE_COUNT
        #define portMAX_CORE_COUNT 2
    #endif
#endif

/*-----------------------------------------------------------*/
/* This SMP port requires two spin locks, which are claimed from the SDK.
 * the spin lock numbers to be used are defined statically and defaulted here
 * to the values nominally set aside for RTOS by the SDK */
#ifndef configSMP_SPINLOCK_0
    #define configSMP_SPINLOCK_0 PICO_SPINLOCK_ID_OS1
#endif

#ifndef configSMP_SPINLOCK_1
    #define configSMP_SPINLOCK_1 PICO_SPINLOCK_ID_OS2
#endif

/*-----------------------------------------------------------*/
/* Other essential configurations */

#ifndef portHAS_SYSTICK
    #define portHAS_SYSTICK 1
#endif

#ifndef portUSE_DIVIDER_SAVE_RESTORE
    #define portUSE_DIVIDER_SAVE_RESTORE 1
#endif

#ifndef portEXTRA_STACK_SIZE
    #define portEXTRA_STACK_SIZE 16
#endif

/*-----------------------------------------------------------*/
/* Port specific definitions */

#define portGET_CORE_ID()            get_core_num()
#define portGET_CORE_COUNT()         (configNUMBER_OF_CORES)

/*-----------------------------------------------------------*/
/* Memory barrier */
#define portMEMORY_BARRIER()        __asm volatile("" ::: "memory")

/*-----------------------------------------------------------*/
/* Critical section management */
#define portDISABLE_INTERRUPTS()    __asm volatile("cpsid i" ::: "memory")
#define portENABLE_INTERRUPTS()     __asm volatile("cpsie i" ::: "memory")

/*-----------------------------------------------------------*/
/* Yield macros */
#define portYIELD()                 vPortYield()
#define portYIELD_FROM_ISR(x)       if(x != pdFALSE) { portYIELD(); }
#define portEND_SWITCHING_ISR(x)    portYIELD_FROM_ISR(x)

/*-----------------------------------------------------------*/
/* NOP instruction */
#define portNOP()                   __asm volatile("nop")

/*-----------------------------------------------------------*/
/* Stack growth direction */
#define portSTACK_GROWTH            (-1)

/*-----------------------------------------------------------*/
/* Byte alignment */
#define portBYTE_ALIGNMENT          8

/*-----------------------------------------------------------*/
/* Task function macros */
#define portTASK_FUNCTION_PROTO(vFunction, pvParameters) void vFunction(void *pvParameters)
#define portTASK_FUNCTION(vFunction, pvParameters) void vFunction(void *pvParameters)

/*-----------------------------------------------------------*/
/* Tickless idle support */
#if (configUSE_TICKLESS_IDLE == 1)
    #ifndef portSUPPRESS_TICKS_AND_SLEEP
        extern void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime);
        #define portSUPPRESS_TICKS_AND_SLEEP(xExpectedIdleTime) vPortSuppressTicksAndSleep(xExpectedIdleTime)
    #endif
#endif

/*-----------------------------------------------------------*/
/* Interrupt priority configuration */
#define configKERNEL_INTERRUPT_PRIORITY         255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    191

/*-----------------------------------------------------------*/
/* Cortex-M0+ specific */
#define configPRIO_BITS                         2
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 0x3
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 0x1

/*-----------------------------------------------------------*/
/* Pico SDK integration */
#ifndef LIB_PICO_SYNC
    #define LIB_PICO_SYNC 1
#endif

#ifndef LIB_PICO_TIME
    #define LIB_PICO_TIME 1
#endif

#ifndef LIB_PICO_MULTICORE
    #define LIB_PICO_MULTICORE 1
#endif

/*-----------------------------------------------------------*/
/* Function prototypes for port functions */
void vPortYield(void);
void vPortEnterCritical(void);
void vPortExitCritical(void);
void vPortEnableInterrupts(void);
uint32_t ulSetInterruptMaskFromISR(void);
void vClearInterruptMaskFromISR(uint32_t ulMask);
void vYieldCore(int xCoreID);

/*-----------------------------------------------------------*/
/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* RP2040_CONFIG_H */