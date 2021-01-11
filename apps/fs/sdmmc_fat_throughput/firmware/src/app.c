/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

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

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "system/time/sys_time.h"
#include "app.h"
#include "user.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define SDCARD_MOUNT_NAME    SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0
#define SDCARD_DEV_NAME      SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0
#define SDCARD_FILE_NAME     "throughput.txt"

#define BYTES_TO_KB(num)    (num / 1024)
#define BYTES_TO_MB(num)    ((num / 1024) / 1024)

/* File size of 10 MegaBytes */
#define FILE_LEN            (10485760UL)

#define BUFFER_SIZE         (65536UL)

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

/* Application data buffer.*/
uint8_t CACHE_ALIGN dataBuffer[BUFFER_SIZE];

static const char messageBuffer[] =
"\n\r-------------------------------------------------------------------"
"\n\r\t         SD-Card Throughput Demo Application \t\t"
"\n\r-------------------------------------------------------------------"
"\n\r Creates a File throughput.txt on SD-Card and calculates throughput"
"\n\n\r        by Writing and Reading data into it at High Speed"
"\n\r-------------------------------------------------------------------\n\r";

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void APP_SysFSEventHandler(SYS_FS_EVENT event,void* eventData,uintptr_t context)
{
    switch(event)
    {
        /* If the event is mount then check if SDCARD media has been mounted */
        case SYS_FS_EVENT_MOUNT:
            if(strcmp((const char *)eventData, SDCARD_MOUNT_NAME) == 0)
            {
                appData.sdCardMountFlag = true;
            }
            break;

        /* If the event is unmount then check if SDCARD media has been unmount */
        case SYS_FS_EVENT_UNMOUNT:
            if(strcmp((const char *)eventData, SDCARD_MOUNT_NAME) == 0)
            {
                appData.sdCardMountFlag = false;
                appData.rwCounter = 0;

                appData.state = APP_MOUNT_WAIT;

                LED_OFF();
            }

            break;

        case SYS_FS_EVENT_ERROR:
        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

float APP_TIME_CountToSec ( uint64_t count )
{
    uint64_t timeMsec = 0;

    timeMsec = ((count * 1000) / SYS_TIME_FrequencyGet());

    return((float)((float)timeMsec / 1000));
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Initialize ( void )
{
    uint32_t i;

    /* Place the App state machine in its initial state. */
    appData.state = APP_MOUNT_WAIT;

    /* Register the File System Event handler */
    SYS_FS_EventHandlerSet((void const*)APP_SysFSEventHandler,(uintptr_t)NULL);
    
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        dataBuffer[i] = i;
    }

    printf("%s", messageBuffer);
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    uint64_t diffCount = 0;
    size_t rwBytes = -1;

    /* Check the application's current state. */
    switch ( appData.state )
    {
        case APP_MOUNT_WAIT:
        {
            /* Wait for SDCARD to be Auto Mounted */
            if(appData.sdCardMountFlag == true)
            {
                appData.state = APP_SET_CURRENT_DRIVE;
            }
            break;
        }

        case APP_SET_CURRENT_DRIVE:
        {
            if(SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE)
            {
                /* Error while setting current drive */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Open a file for reading. */
                appData.state = APP_OPEN_FILE;
            }

            break;
        }

        case APP_OPEN_FILE:
        {
            appData.fileHandle = SYS_FS_FileOpen(SDCARD_FILE_NAME, (SYS_FS_FILE_OPEN_WRITE_PLUS));

            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                /* Could not open the file. Error out*/
                appData.state = APP_ERROR;
            }
            else
            {
                appData.state = APP_WRITE_TO_FILE;

                printf("\n\r Writing %ld Megabytes of data to file in chunks of %ld Kilobytes\r\n",
                            BYTES_TO_MB(FILE_LEN), BYTES_TO_KB(BUFFER_SIZE));

                appData.prevCounterVal = SYS_TIME_Counter64Get();
            }
            break;
        }

        case APP_WRITE_TO_FILE:
        {
            if (appData.rwCounter == FILE_LEN)
            {
                appData.state = APP_FLUSH_FILE;
                appData.rwCounter = 0;
            }
            else
            {
                rwBytes = SYS_FS_FileWrite(appData.fileHandle, (void *)dataBuffer, BUFFER_SIZE);
                
                if(rwBytes == -1)
                {
                    /* Write was not successful. Close the file
                     * and error out.*/
                    SYS_FS_FileClose(appData.fileHandle);
                    appData.state = APP_ERROR;
                }
                else
                {
                    appData.rwCounter += BUFFER_SIZE;
                }
            }
            break;
        }

        case APP_FLUSH_FILE:
        {
            if (SYS_FS_FileSync(appData.fileHandle) != 0)
            {
                /* Could not flush the contents of the file. Error out. */
                appData.state = APP_ERROR;
            }
            else
            {
                diffCount = (SYS_TIME_Counter64Get() - appData.prevCounterVal);
                
                appData.writeTimeSec = APP_TIME_CountToSec(diffCount);

                appData.writeThroughput = ((float)BYTES_TO_MB(FILE_LEN) / appData.writeTimeSec);

                printf("\n\r\tTime Taken = %0.3f seconds\r\n", appData.writeTimeSec);

                printf("\n\r\tThroughput = %0.3f Megabytes/s\r\n", appData.writeThroughput);

                /* Check the file status */
                appData.state = APP_FILE_SEEK;
            }
            break;
        }

        case APP_FILE_SEEK:
        {
            /* Move file pointer to beginning of file */
            if(SYS_FS_FileSeek( appData.fileHandle, 0, SYS_FS_SEEK_SET ) == -1)
            {
                /* File seek caused an error */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Check for original file content */
                appData.state = APP_READ_FILE;

                printf("\n\r Reading %ld Megabytes of data from file in chunks of %ld Kilobytes\r\n",
                            BYTES_TO_MB(FILE_LEN), BYTES_TO_KB(BUFFER_SIZE));

                appData.prevCounterVal = 0;
                appData.prevCounterVal = SYS_TIME_Counter64Get();
            }
            break;
        }

        case APP_READ_FILE:
        {
            rwBytes = SYS_FS_FileRead(appData.fileHandle, (void *)dataBuffer, BUFFER_SIZE);

            if (rwBytes == -1)
            {
                /* There was an error while reading the file.
                 * Close the file and error out. */

                SYS_FS_FileClose(appData.fileHandle);
                appData.state = APP_ERROR;
            }
            else
            {
                appData.rwCounter += rwBytes;

                if (appData.rwCounter == FILE_LEN)
                {
                    diffCount = (SYS_TIME_Counter64Get() - appData.prevCounterVal);

                    appData.readTimeSec = APP_TIME_CountToSec(diffCount);

                    appData.readThroughput = ((float)BYTES_TO_MB(FILE_LEN) / appData.readTimeSec);

                    printf("\n\r\tTime Taken = %0.3f seconds\r\n", appData.readTimeSec);

                    printf("\n\r\tThroughput = %0.3f Megabytes/s\r\n", appData.readThroughput);

                    appData.state = APP_CLOSE_FILE;
                }
            }
            break;
        }

        case APP_CLOSE_FILE:
        {
            appData.rwCounter = 0;

            SYS_FS_FileClose(appData.fileHandle);

            /* Go to idle once throughput calculation is complete */
            appData.state = APP_IDLE;
            break;
        }

        case APP_IDLE:
        {
            /* The application comes here when the demo has completed successfully.*/
            LED_ON();
            break;
        }

        case APP_ERROR:
        default:
        {
            /* The application comes here when the demo has failed. */
            break;
        }
    }
}

/*******************************************************************************
 End of File
 */
