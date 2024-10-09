/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    priv_task.c

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

#include "priv_task.h"
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
    This structure should be initialized by the PRIV_TASK_Initialize function.

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

typedef enum
{
    INIT = 0,
    MENU,
    INPUT,
}PRIV_TASKS;
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void PRIV_TASK_Initialize ( void )

  Remarks:
    See prototype in priv_task.h.
 */

void PRIV_TASK_Initialize ( void )
{
    
}


/******************************************************************************
  Function:
    void PRIV_TASK_Tasks ( void )

  Remarks:
    See prototype in priv_task.h.
 */

void PRIV_TASK_Tasks ( void )
{
    static PRIV_TASKS tsk = INIT;
    char input_val;
    
    switch(tsk)
    {        
        case INIT:
            printf("Address of ucSharedMemory[1] is 0x%x\r\n\n", (int)&ucSharedMemory[1]);
            tsk = MENU;
            break;
        case MENU:
            printf("Press 1 to force the rdonly_task to write to ucSharedMemory to which it has read-only access. \r\n\n");
            printf("Press 2 to raise the privilege level of rdonly_task to write to ucSharedMemory. \r\n\n");
            printf("Press 3 to print counter value incremented by rw_task. \r\n\n");
            printf("Press ESC to print the menu again. \r\n\n");            
            tsk = INPUT;
            break;
        case INPUT:
            SERCOM2_USART_Read((void*)&input_val, 1);
            if (input_val == '1')
            {
                fault_testing[0] = 1;
            }
            else if (input_val == '2')
            {
                fault_testing[0] = 2;
            }
            else if (input_val == '3')
            {
                printf("ucSharedMemory[0] = %d\r\n\n", ucSharedMemory[0]);
            }
            else if (input_val == 27)   //ESC ASCII value
            {
                tsk = MENU;
            }
            break;
        default:
            break;
    }
}


/*******************************************************************************
 End of File
 */
