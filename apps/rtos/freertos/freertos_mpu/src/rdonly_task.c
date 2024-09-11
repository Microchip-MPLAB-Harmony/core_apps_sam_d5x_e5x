/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    rdonly_task.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "rdonly_task.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the RDONLY_TASK_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void RDONLY_TASK_Initialize ( void )

  Remarks:
    See prototype in rdonly_task.h.
 */

void RDONLY_TASK_Initialize ( void )
{
   
}


/******************************************************************************
  Function:
    void RDONLY_TASK_Tasks ( void )

  Remarks:
    See prototype in rdonly_task.h.
 */
extern volatile uint8_t ucSharedMemory[32];
extern volatile uint8_t fault_testing[32];

void RDONLY_TASK_Tasks ( void )
{
    if (fault_testing[0] == 1)
    {
        ucSharedMemory[1]++;
        fault_testing[0] = 0;
    }
    else if (fault_testing[0] == 2)
    {
        portRAISE_PRIVILEGE();
        printf("\r\nAccessing ucSharedMemory[1] with privileged access\r\n");
        ucSharedMemory[1]++;
        fault_testing[0] = 0;
        printf("ucSharedMemory[1] = %d\r\n", ucSharedMemory[1]);
        printf("Switching back to User mode\r\n\n");
        portSWITCH_TO_USER_MODE();
    }
}


/*******************************************************************************
 End of File
 */
