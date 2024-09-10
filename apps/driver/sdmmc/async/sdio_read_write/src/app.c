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

#include "app.h"
#include "user.h"
#include "definitions.h"
#include <string.h>

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
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
uint8_t writeBuffer[512];
uint8_t readBuffer[512];
uint32_t csa_addr = 0x3b0000;
uint32_t csa_data = 0;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void appTransferHandler
(
    DRV_SDMMC_EVENT event,
    DRV_SDMMC_COMMAND_HANDLE commandHandle,
    uintptr_t context
)
{
    APP_DATA* app_data = (APP_DATA *)context;

    switch(event)
    {
        case DRV_SDMMC_EVENT_COMMAND_COMPLETE:
            app_data->xfer_done = true;
            break;

        case DRV_SDMMC_EVENT_COMMAND_ERROR:
            app_data->state = APP_STATE_ERROR;
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
    appData.state = APP_STATE_OPEN_DRIVER;
    appData.xfer_done = false;
}

typedef struct
{
    uint8_t sdio_rev;
    uint8_t sdspec_rev;
    uint8_t io_en;
    uint8_t io_rdy;
    uint8_t int_en;
    uint8_t int_pending;
    uint8_t io_abort;
    uint8_t bus_interface_ctrl;    
    uint8_t card_capability;
    uint8_t cmn_cis_ptr_0;
    uint8_t cmn_cis_ptr_1;
    uint8_t cmn_cis_ptr_2;
    uint8_t bus_suspend;
    uint8_t func_sel;
    uint8_t exec_flags;
    uint8_t ready_flags;
    uint8_t fn0_block_sz0;
    uint8_t fn0_block_sz1;
    uint8_t power_ctrl;
    uint8_t bus_speed_sel;
    uint8_t uhs1_support;
    uint8_t driver_strength;
    uint8_t interrupt_extn;
    uint8_t reserved[233];
    
}CARD_DATA;

BUFFER_ATTRIBUTES CARD_DATA cardData;
/******************************************************************************
  Function:
    void APP_Tasks ( void )

 Description:
    Demonstrates Erase, Write and Read operation of DRV_SDMMC in Buffer Model.
    Each case is a fall through when the request is queued up successfully.

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        case APP_STATE_OPEN_DRIVER:
        
            appData.drvHandle = DRV_SDMMC_Open(DRV_SDMMC_INDEX_0, DRV_IO_INTENT_READWRITE);

            if (appData.drvHandle != DRV_HANDLE_INVALID)
            {
                DRV_SDMMC_EventHandlerSet(appData.drvHandle, (const void*)appTransferHandler, (uintptr_t)&appData);
                appData.state = APP_STATE_SDCARD_ATTACHED;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;

        case APP_STATE_SDCARD_ATTACHED:
        
            if (DRV_SDMMC_IsAttached(appData.drvHandle) == true)
            {
                appData.state = APP_STATE_SDIO_READ_CARD_CAPABILITY;               
            }
            break;
        
        case APP_STATE_SDIO_READ_CARD_CAPABILITY:
            DRV_SDMMC_Async_SDIO_Read(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x08, &cardData.card_capability, 1, false);
            appData.nextState = APP_STATE_SDIO_READ_FN0_BLK_SIZE;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_READ_FN0_BLK_SIZE:
            printf("CMD52 Byte Read: Card Capability = 0x%x\r\n", cardData.card_capability);
            if (cardData.card_capability != 0x9f)
            {
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                DRV_SDMMC_Async_SDIO_Read(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x10, &cardData.fn0_block_sz0, 2, true);
                appData.nextState = APP_STATE_SDIO_WRITE_FN0_BLK_SIZE_256;
                appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            }
            break;
            
        case APP_STATE_SDIO_WRITE_FN0_BLK_SIZE_256:
            printf("CMD53 2 Bytes Read: Function 0 Block Size = %d\r\n", *((uint16_t*)&cardData.fn0_block_sz0));
            *((uint16_t*)&cardData.fn0_block_sz0) = 256;
            printf("CMD53 2 Bytes Write: Updating Function 0 Block Size to %d\r\n", *((uint16_t*)&cardData.fn0_block_sz0));
            DRV_SDMMC_Async_SDIO_Write(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x10, &cardData.fn0_block_sz0, 2, true);
            appData.nextState = APP_STATE_SDIO_READ_FN0_BLK_SIZE_256;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_READ_FN0_BLK_SIZE_256:
            DRV_SDMMC_Async_SDIO_Read(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x10, &cardData.fn0_block_sz0, 2, true);
            appData.nextState = APP_STATE_SDIO_WRITE_FN0_BLK_SIZE_512_0;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_WRITE_FN0_BLK_SIZE_512_0:
            printf("CMD53 2 Bytes Read: Function 0 Block Size = %d\r\n", *((uint16_t*)&cardData.fn0_block_sz0));
            if (*((uint16_t*)&cardData.fn0_block_sz0) != 256)
            {
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                *((uint16_t*)&cardData.fn0_block_sz0) = 512;
                printf("Updating Function 0 Block Size back to %d using two CMD 52 Byte Writes\r\n", *((uint16_t*)&cardData.fn0_block_sz0));
                DRV_SDMMC_Async_SDIO_DirByteWrite(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x10, &cardData.fn0_block_sz0, true);                
                appData.nextState = APP_STATE_SDIO_WRITE_FN0_BLK_SIZE_512_1;
                appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            }
            
            break;
            
        case APP_STATE_SDIO_WRITE_FN0_BLK_SIZE_512_1:
            DRV_SDMMC_Async_SDIO_DirByteWrite(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x11, &cardData.fn0_block_sz1, true);                
            appData.nextState = APP_STATE_SDIO_READ_BLK;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            
            break;       
            
        case APP_STATE_SDIO_READ_BLK:
            printf("Function 0 Block Size = %d\r\n", *((uint16_t*)&cardData.fn0_block_sz0));
            printf("CMD53 Block Read: Card Common Control Register (CCCR)\r\n");
            DRV_SDMMC_Async_SDIO_Read(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0, &cardData, 512, true);
            appData.nextState = APP_STATE_SDIO_CCCR_PRINT;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_CCCR_PRINT:
            printf("00h CCCR/SDIO Revision          0x%x\r\n", cardData.sdio_rev);
            printf("01h SD Specification Revision   0x%x\r\n", cardData.sdspec_rev);
            printf("02h I/O Enable                  0x%x\r\n", cardData.io_en);
            printf("03h I/O Ready                   0x%x\r\n", cardData.io_rdy);
            printf("04h Int Enable                  0x%x\r\n", cardData.int_en);
            printf("05h Int Pending                 0x%x\r\n", cardData.int_pending);
            printf("06h I/O Abort                   0x%x\r\n", cardData.io_abort);
            printf("07h Bus Interface Control       0x%x\r\n", cardData.bus_interface_ctrl);
            printf("08h Card Capability             0x%x\r\n", cardData.card_capability);
            printf("09h Common CIS Pointer 0        0x%x\r\n", cardData.cmn_cis_ptr_0);
            printf("0Ah Common CIS Pointer 1        0x%x\r\n", cardData.cmn_cis_ptr_1);
            printf("0Bh Common CIS Pointer 2        0x%x\r\n", cardData.cmn_cis_ptr_2);
            printf("0Ch Bus Suspend                 0x%x\r\n", cardData.bus_suspend);
            printf("0Dh Function Select             0x%x\r\n", cardData.func_sel);
            printf("0Eh Exec Flags                  0x%x\r\n", cardData.exec_flags);
            printf("0Fh Ready Flags                 0x%x\r\n", cardData.ready_flags);
            printf("10h FN0 Block Size - 0          0x%x\r\n", cardData.fn0_block_sz0);
            printf("11h FN0 Block Size - 1          0x%x\r\n", cardData.fn0_block_sz1);
            printf("12h Power Control               0x%x\r\n", cardData.power_ctrl);
            printf("13h Bus Speed Select            0x%x\r\n", cardData.bus_speed_sel);
            printf("14h USH-1 Support               0x%x\r\n", cardData.uhs1_support);
            printf("15h Drive Strength              0x%x\r\n", cardData.driver_strength);
            printf("16h Interrupt Extension         0x%x\r\n", cardData.interrupt_extn);
            appData.state = APP_STATE_SDIO_ENABLE_CSA;
            break;
            
        case APP_STATE_SDIO_ENABLE_CSA:
            printf("Enabling Function 1 CSA \r\n");
            writeBuffer[0] = 0x80;
            DRV_SDMMC_Async_SDIO_DirByteWrite(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x100, (uint8_t*)writeBuffer, false);
            appData.nextState = APP_STATE_SDIO_READ_FBR_100;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_READ_FBR_100:            
            DRV_SDMMC_Async_SDIO_DirByteRead(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x100, (uint8_t*)readBuffer);
            appData.nextState = APP_STATE_SDIO_WRITE_CSA_ADDR_CHIP_ID_1;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_WRITE_CSA_ADDR_CHIP_ID_1:
            printf("Reading back the FBR value at address 0x100 is 0x%x\r\n", (unsigned char)readBuffer[0]);
            if ((readBuffer[0] & 0xC0) != 0xC0)
            {
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                csa_addr = 0x1000;
                printf("Writing CSA address to 0x%x\r\n", (unsigned int)csa_addr);
                DRV_SDMMC_Async_SDIO_DirByteWrite(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x10C, &(((uint8_t*)&csa_addr)[0]), false);
                appData.nextState = APP_STATE_SDIO_WRITE_CSA_ADDR_CHIP_ID_2;
                appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            }
            break;
            
        case APP_STATE_SDIO_WRITE_CSA_ADDR_CHIP_ID_2:           
            DRV_SDMMC_Async_SDIO_DirByteWrite(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x10D, &(((uint8_t*)&csa_addr)[1]), false);
            appData.nextState = APP_STATE_SDIO_WRITE_CSA_ADDR_CHIP_ID_3;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_WRITE_CSA_ADDR_CHIP_ID_3:            
            DRV_SDMMC_Async_SDIO_DirByteWrite(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x10E, &(((uint8_t*)&csa_addr)[2]), false);
            appData.nextState = APP_STATE_SDIO_READ_CSA_DATA;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_READ_CSA_DATA:
            printf("Reading Chip ID data from CSA address 0x%x\r\n", (unsigned int)csa_addr);
            DRV_SDMMC_Async_SDIO_Read(appData.drvHandle, &appData.cmdHandle, DRV_SDMMC_FN0, 0x10F, &csa_data, 4, true);
            appData.nextState = APP_STATE_SDIO_READ_CSA_DATA_PRINT;
            appData.state = APP_STATE_SDCARD_WAIT_XFER_DONE;
            break;
            
        case APP_STATE_SDIO_READ_CSA_DATA_PRINT:
            printf("CSA Data (Chip ID) = 0x%x\r\n", (unsigned int)csa_data);
            appData.state = APP_STATE_SUCCESS;
            break;
        
        case APP_STATE_SDCARD_WAIT_XFER_DONE:
            if (appData.xfer_done == true)
            {
                appData.xfer_done = false;
                appData.state = appData.nextState;
            }
            break;                
        
        case APP_STATE_SUCCESS:        
            DRV_SDMMC_Close(appData.drvHandle);
            LED_Clear();
            break;
        
        case APP_STATE_ERROR:
        default:
            DRV_SDMMC_Close(appData.drvHandle);
            LED_Set();
            break;   
            
        case APP_STATE_IDLE:
            break;
    }

}

/*******************************************************************************
 End of File
 */
