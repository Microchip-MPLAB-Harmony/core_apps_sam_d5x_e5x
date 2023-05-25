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
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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
#include "app.h"
#include "user.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define SDCARD_MOUNT_NAME    SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0
#define SDCARD_DEV_NAME      SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0

#define APP_FILE_NAME        "newfile.txt"
const uint8_t fileWriteData[] = "FILEX filesystem demo using SD CARD Media";
static SYS_FS_FORMAT_PARAM opt = {0};

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

static APP_DATA appData;

/* Application data buffer */
static uint8_t BUFFER_ATTRIBUTES fileReadData[sizeof(fileWriteData)];

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

                appData.state = APP_MOUNT_WAIT;

                LED_OFF();
            }

            break;

        case SYS_FS_EVENT_ERROR:
            if(strcmp((const char *)eventData, SDCARD_MOUNT_NAME) == 0)
            {
                if (appData.state == APP_MOUNT_WAIT)
                {
                    appData.state = APP_FORMAT_DISK;
                }
            }
            break;
        default:
            break;
    }
}

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
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_MOUNT_WAIT;

    /* Register the File System Event handler */
    SYS_FS_EventHandlerSet((void const*)APP_SysFSEventHandler,(uintptr_t)NULL);
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appData.state )
    {
        case APP_MOUNT_WAIT:
            /* Wait for SDCARD to be Auto Mounted */
            if(appData.sdCardMountFlag == true)
            {
                appData.state = APP_SET_CURRENT_DRIVE;
            }
            break;

        case APP_FORMAT_DISK:
            if (SYS_FS_DriveFormat(SDCARD_MOUNT_NAME, &opt, NULL, 0) != SYS_FS_RES_SUCCESS)
            {
                appData.state = APP_ERROR;
            }
            else
            {
                appData.state = APP_MOUNT_DISK;
            }
            break;

        case APP_MOUNT_DISK:
            if (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FILEX, 0, NULL) != SYS_FS_RES_SUCCESS)
            {
                appData.state = APP_ERROR;
            }
            else
            {
                appData.state = APP_SET_CURRENT_DRIVE;
            }
            break;

        case APP_SET_CURRENT_DRIVE:
            if(SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE)
            {
                /* Error while setting current drive */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Open a file for write */
                appData.state = APP_OPEN_FILE;
            }
            break;

        case APP_OPEN_FILE:
            //Remove file if exist
            SYS_FS_FileDirectoryRemove(APP_FILE_NAME);
            
            appData.fileHandle = SYS_FS_FileOpen(APP_FILE_NAME, SYS_FS_FILE_OPEN_WRITE);
            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                /* Could not open the file. Error out*/
                appData.state = APP_ERROR;
            }
            else
            {
                /* Write data to file */
                appData.state = APP_WRITE_TO_FILE;
            }
            break;

        case APP_WRITE_TO_FILE:
            if(SYS_FS_FileWrite(appData.fileHandle, (const void *)fileWriteData, sizeof(fileWriteData)) == -1)
            {
                /* Write was not successful. Close the file
                 * and error out.*/
                SYS_FS_FileClose(appData.fileHandle);
                appData.state = APP_ERROR;
            }
            else
            {
                /* Set file read/write pointer */
                appData.state = APP_FILE_SEEK;
            }
            break;

        case APP_FILE_SEEK:
            if(SYS_FS_FileSeek(appData.fileHandle, 0, SYS_FS_SEEK_SET) == -1)
            {
                /* File seek caused an error */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Read data from file */
                appData.state = APP_READ_FROM_FILE;
            }
            break;

        case APP_READ_FROM_FILE:
            appData.nBytesRead = SYS_FS_FileRead(appData.fileHandle, (void *)fileReadData, sizeof(fileReadData));

            if (appData.nBytesRead == -1)
            {
                /* There was an error while reading the file.
                 * Close the file and error out. */

                SYS_FS_FileClose(appData.fileHandle);
                appData.state = APP_ERROR;
            }
            else if(SYS_FS_FileEOF(appData.fileHandle) == 1)    /* Test for end of file */
            {
                if (memcmp(fileWriteData, fileReadData, SYS_FS_FileSize(appData.fileHandle)) != 0)
                {
                    appData.state = APP_ERROR;
                }
                else
                {
                    appData.state = APP_CLOSE_FILE;
                }
            }
            break;

        case APP_CLOSE_FILE:
            /* Close file */
            SYS_FS_FileClose(appData.fileHandle);
            /* Unmount SD Card */
            SYS_FS_Unmount(SDCARD_MOUNT_NAME);
            /* The test was successful. Lets idle. */
            appData.state = APP_IDLE;
            break;

        case APP_IDLE:
            /* The application comes here when the demo has completed successfully.*/
            LED_ON();
            break;

        case APP_ERROR:
            /* The application comes here when the demo has failed. */
            break;

        default:
            break;
    }
}



/*******************************************************************************
 End of File
 */
