/*******************************************************************************
 System Tasks File

  File Name:
    tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled tasks.

  Description:
    This file contains source code necessary to maintain system's polled tasks.
    It implements the "SYS_Tasks" function that calls the individual "Tasks"
    functions for all polled MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "sys_tasks.h"


// *****************************************************************************
// *****************************************************************************
// Section: RTOS "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/* Handle for the RDONLY_TASK_Tasks. */
TaskHandle_t xRDONLY_TASK_Tasks;



/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
static StackType_t xTask0Stack[ 4096 / sizeof(StackType_t) ] __attribute__((aligned(4096)));
uint8_t ucSharedMemory[32] __attribute__((aligned(32)));
uint8_t fault_testing[32] __attribute__((aligned(32)));

static void lRDONLY_TASK_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        RDONLY_TASK_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}

/* Handle for the RW_TASK_Tasks. */
TaskHandle_t xRW_TASK_Tasks;



/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
static StackType_t xTask1Stack[ 4096 / sizeof(StackType_t) ] __attribute__((aligned(4096)));

static void lRW_TASK_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        RW_TASK_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}

/* Handle for the PRIV_TASK_Tasks. */
TaskHandle_t xPRIV_TASK_Tasks;



static void lPRIV_TASK_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        PRIV_TASK_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}




// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
void SYS_Tasks ( void )
{
    /* Maintain system services */
    

    /* Maintain Device Drivers */
    

    /* Maintain Middleware & Other Libraries */
    

    /* Maintain the application's state machine. */
    
   static const xTaskParameters xTask0Parameters =
   {
       (TaskFunction_t) lRDONLY_TASK_Tasks, /* pvTaskCode - the function that implements the task. */
       "RDONLY_TASK_Tasks", /* pcName */
       4096U / sizeof(StackType_t), /* usStackDepth - defined in words, not bytes. */
       NULL, /* pvParameters - not being used in this case. */
       1U , /* uxPriority*/
       xTask0Stack, /* puxStackBuffer - the array to use as the task stack. */
       /* xRegions - MPU regions*/
       {
           /* Base - address Length - Parameters */
           { (void*)ucSharedMemory , 32 , portMPU_REGION_PRIVILEGED_READ_WRITE_UNPRIV_READ_ONLY | portMPU_REGION_EXECUTE_NEVER },
           { (void*)fault_testing , 32 , portMPU_REGION_READ_WRITE | portMPU_REGION_EXECUTE_NEVER },
           { 0, 0, 0 }
       }
       
   };

   (void) xTaskCreateRestricted(
       &xTask0Parameters,
       &xRDONLY_TASK_Tasks
   );

   static const xTaskParameters xTask1Parameters =
   {
       (TaskFunction_t) lRW_TASK_Tasks, /* pvTaskCode - the function that implements the task. */
       "RW_TASK_Tasks", /* pcName */
       4096U / sizeof(StackType_t), /* usStackDepth - defined in words, not bytes. */
       NULL, /* pvParameters - not being used in this case. */
       1U , /* uxPriority*/
       xTask1Stack, /* puxStackBuffer - the array to use as the task stack. */
       /* xRegions - MPU regions*/
       {
           /* Base - address Length - Parameters */
           { (void*)ucSharedMemory , 32 , portMPU_REGION_READ_WRITE | portMPU_REGION_EXECUTE_NEVER },
           { 0, 0, 0 },
           { 0, 0, 0 }
       }
       
   };

   (void) xTaskCreateRestricted(
       &xTask1Parameters,
       &xRW_TASK_Tasks
   );

    /* Create OS Thread for PRIV_TASK_Tasks. */
    (void) xTaskCreate(
           (TaskFunction_t) lPRIV_TASK_Tasks,
           "PRIV_TASK_Tasks",
           1024,
           NULL,
           1U  | portPRIVILEGE_BIT ,
           &xPRIV_TASK_Tasks);



    /* Start RTOS Scheduler. */
    
     /**********************************************************************
     * Create all Threads for APP Tasks before starting FreeRTOS Scheduler *
     ***********************************************************************/
    vTaskStartScheduler(); /* This function never returns. */

}

/*******************************************************************************
 End of File
 */

