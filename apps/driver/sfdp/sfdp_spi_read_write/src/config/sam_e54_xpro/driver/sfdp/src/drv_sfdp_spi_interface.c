/******************************************************************************
  SFDP Driver SPI Interface Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_sfdp_spi_interface.c

  Summary:
    SFDP Driver Interface implementation

  Description:
    This interface file segregates the SFDP protocol from the underlying
    hardware layer implementation for SPI PLIB and SPI driver
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "drv_sfdp_spi_interface.h"


void DRV_SFDP_SPIPlibCallbackHandler(uintptr_t context )
{
    DRV_SFDP_OBJECT* dObj = (DRV_SFDP_OBJECT*)context;

    dObj->transferDataObj.txSize = dObj->transferDataObj.rxSize = 0;
    dObj->transferDataObj.pTransmitData = dObj->transferDataObj.pReceiveData = NULL;

    DRV_SFDP_Handler();
}


void DRV_SFDP_InterfaceInit(DRV_SFDP_OBJECT* dObj, DRV_SFDP_INIT* sfdpInit)
{

    /* Initialize the attached memory device functions */
    dObj->sfdpPlib = sfdpInit->sfdpPlib;
    dObj->sfdpPlib->callbackRegister(DRV_SFDP_SPIPlibCallbackHandler, (uintptr_t)dObj);
}

bool DRV_SFDP_SPIWriteRead(
    DRV_SFDP_OBJECT* dObj,
    DRV_SFDP_TRANSFER_OBJ* transferObj
)
{
    bool isSuccess = true;

    SYS_PORT_PinClear(dObj->chipSelectPin);

    dObj->transferStatus    = DRV_SFDP_TRANSFER_BUSY;
    (void) dObj->sfdpPlib->writeRead (transferObj->pTransmitData, transferObj->txSize, transferObj->pReceiveData, transferObj->rxSize);
    return isSuccess;
}
