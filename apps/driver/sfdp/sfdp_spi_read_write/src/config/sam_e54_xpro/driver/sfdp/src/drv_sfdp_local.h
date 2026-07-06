/*******************************************************************************
  SFDP Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_sfdp_local.h

  Summary:
    SFDP driver local declarations and definitions

  Description:
    This file contains the SFDP driver's local declarations and definitions.
    This driver supports JEDEC JESD216 compliant Serial Flash Discoverable
    Parameters (SFDP) to automatically discover flash device characteristics
    at runtime.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef DRV_SFDP_LOCAL_H
#define DRV_SFDP_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "configuration.h"
#include "driver/sfdp/drv_sfdp.h"

// *****************************************************************************
// *****************************************************************************
// Section: SFDP Definitions
// *****************************************************************************
// *****************************************************************************

/* SFDP Signature value "SFDP" in ASCII */
#define SFDP_SIGNATURE              0x50444653U

/* SFDP Header size in bytes */
#define SFDP_HEADER_SIZE            8U

/* SFDP Parameter Header size in bytes */
#define SFDP_PARAM_HEADER_SIZE      8U

/* Basic Flash Parameter Table JEDEC ID */
#define SFDP_BASIC_PARAM_TABLE_ID   0xFF00U

/* Maximum number of parameter headers to parse */
#define SFDP_MAX_PARAM_HEADERS      16U

// *****************************************************************************
// *****************************************************************************
// Section: Local Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* SFDP and Flash Command set

  Summary:
    Enumeration listing the SFDP and standard Flash commands.

  Description:
    This enumeration defines the commands used to interact with JEDEC-compliant
    flash devices including SFDP discovery commands.

  Remarks:
    None
*/

typedef enum
{
    /* Command to read SFDP parameter tables */
    SFDP_CMD_READ_SFDP           = 0x5A,

    /* Reset enable command. */
    SFDP_CMD_FLASH_RESET_ENABLE  = 0x66,

    /* Command to reset the flash. */
    SFDP_CMD_FLASH_RESET         = 0x99,

    /* Command to read JEDEC-ID of the flash device. */
    SFDP_CMD_JEDEC_ID_READ       = 0x9F,

    /* QUAD Command to read JEDEC-ID of the flash device. */
    SFDP_CMD_QUAD_JEDEC_ID_READ  = 0xAF,

    /* Command to perform High Speed Read (1-1-1 mode) */
    SFDP_CMD_HIGH_SPEED_READ     = 0x0B,

    /* Write enable command. */
    SFDP_CMD_WRITE_ENABLE        = 0x06,

    /* Page Program command. */
    SFDP_CMD_PAGE_PROGRAM        = 0x02,

    /* Command to read the Flash status register. */
    SFDP_CMD_READ_STATUS_REG     = 0x05,

    /* Command to perform 4KB sector erase */
    SFDP_CMD_SECTOR_ERASE_4K     = 0x20,

    /* Command to perform 64KB block erase */
    SFDP_CMD_BLOCK_ERASE_64K     = 0xD8,

    /* Command to perform Chip erase */
    SFDP_CMD_CHIP_ERASE          = 0xC7,

    /* Command to unlock the flash device (SST26 specific). */
    SFDP_CMD_UNPROTECT_GLOBAL    = 0x98,

    /* Command to write the Flash status register. */
    SFDP_CMD_WRITE_STATUS_REG    = 0x01,

    /* N25Q specific commands */
    SFDP_CMD_WRITE_ENHANCED_VOLATILE_CONFIG_REG = 0x61,
    SFDP_CMD_READ_ENHANCED_VOLATILE_CONFIG_REG = 0x65,
    SFDP_CMD_ENTER_QUAD_N25Q = 0x35,
    SFDP_CMD_EXIT_QUAD_N25Q = 0xF5,

    /* W25 specific commands */
    SFDP_CMD_QUAD_INPUT_PAGE_PROGRAM = 0x32,
    SFDP_CMD_FAST_READ_QUAD_IO_W25 = 0xEB,

    /* MX25L/MX66 specific commands */
    SFDP_CMD_ENABLE_QUAD_IO_MX25L = 0x35,
    SFDP_CMD_RESET_QUAD_IO_MX25L = 0xF5,
    SFDP_CMD_ENTER_4_BYTE_ADDR_MODE = 0xB7,
    SFDP_CMD_HIGH_SPEED_QREAD_MX25L = 0xEB,

    /* S25FL/IS25 specific commands */
    SFDP_CMD_READ_STATUS_REG2 = 0x35,
    SFDP_CMD_WRITE_STATUS_REG2 = 0x31,
    SFDP_CMD_READ_CONFIG_REG = 0x15

} SFDP_CMD;

// *****************************************************************************
/* SFDP Device Type Enumeration

  Summary:
    Enumeration for different flash device types.

  Description:
    This enumeration defines device types for applying device-specific
    optimizations and quad enable methods.

  Remarks:
    Device type is detected at runtime via JEDEC ID.
*/

typedef enum
{
    /* Generic JEDEC-compliant device using SFDP discovery */
    SFDP_DEVICE_TYPE_GENERIC = 0,

    /* SST26 family devices */
    SFDP_DEVICE_TYPE_SST26,

    /* W25 family devices */
    SFDP_DEVICE_TYPE_W25,

    /* N25Q family devices */
    SFDP_DEVICE_TYPE_N25Q,

    /* MX25L/MX66 family devices */
    SFDP_DEVICE_TYPE_MX25L,

    /* S25FL family devices */
    SFDP_DEVICE_TYPE_S25FL,

    /* IS25 family devices */
    SFDP_DEVICE_TYPE_IS25

} SFDP_DEVICE_TYPE;

// *****************************************************************************
/* SFDP Header Structure

  Summary:
    Structure representing the SFDP header.

  Description:
    This structure contains the SFDP header information as per JESD216 standard.

  Remarks:
    This header is located at address 0x000000 in SFDP space.
*/

typedef struct
{
    /* SFDP Signature (0x50444653 - "SFDP" in ASCII) */
    uint32_t signature;

    /* Minor revision number */
    uint8_t minorRev;

    /* Major revision number */
    uint8_t majorRev;

    /* Number of Parameter Headers (NPH) - actual count is NPH + 1 */
    uint8_t numParamHeaders;

    /* Access Protocol (0xFF for SPI) */
    uint8_t accessProtocol;

} DRV_SFDP_HEADER;

// *****************************************************************************
/* SFDP Parameter Header Structure

  Summary:
    Structure representing an SFDP parameter header.

  Description:
    This structure contains parameter header information as per JESD216 standard.

  Remarks:
    Parameter headers follow immediately after the SFDP header.
*/

typedef struct
{
    /* Parameter ID LSB */
    uint8_t paramIdLsb;

    /* Parameter table minor revision */
    uint8_t minorRev;

    /* Parameter table major revision */
    uint8_t majorRev;

    /* Parameter table length in DWORDs */
    uint8_t lengthDwords;

    /* Parameter table pointer (byte address in SFDP space, bits 23:0) */
    uint8_t tablePointer[3];

    /* Parameter ID MSB */
    uint8_t paramIdMsb;

} DRV_SFDP_PARAM_HEADER;

// *****************************************************************************
/* Basic Flash Parameters Structure

  Summary:
    Structure representing parsed Basic Flash Parameters.

  Description:
    This structure contains key flash parameters extracted from the
    Basic Flash Parameter Table (JEDEC JESD216).

  Remarks:
    These parameters are discovered at runtime via SFDP.
*/

typedef struct
{
    /* Flash memory density in bytes */
    uint32_t flashSize;

    /* Page size in bytes (typically 256) */
    uint32_t pageSize;

    /* Sector erase size in bytes (typically 4KB) */
    uint32_t sectorSize;

    /* Block erase size in bytes (typically 64KB) */
    uint32_t blockSize;

    /* 4KB Sector erase opcode */
    uint8_t eraseOpcode4K;

    /* 64KB Block erase opcode */
    uint8_t eraseOpcode64K;

    /* Fast read opcode for 1-1-1 mode */
    uint8_t fastReadOpcode_1_1_1;
    uint8_t fastReadDummyCycles_1_1_1;

    /* Optimal read width mode to use */
    uint32_t optimalReadWidth;

    /* Optimal read opcode and dummy cycles based on best mode */
    uint8_t optimalReadOpcode;
    uint8_t optimalReadDummyCycles;

    /* Optimal width for write/erase operations */
    uint32_t optimalWriteWidth;

    /* Address bytes (3 or 4) */
    uint8_t addressBytes;

    /* Quad command */
    uint8_t quadCommandEnable;
    uint8_t quadCommandDisable;

    /* Device type detected from JEDEC ID */
    SFDP_DEVICE_TYPE deviceType;

    /* Vendor ID from JEDEC (first byte) */
    uint8_t vendorId;

    /* Device ID from JEDEC (bytes 1-2) */
    uint16_t deviceId;

} DRV_SFDP_FLASH_PARAMS;

typedef enum
{
    DRV_SFDP_STATE_READ_DATA,
    DRV_SFDP_STATE_WAIT_READ_COMPLETE,
    DRV_SFDP_STATE_WRITE_CMD_ADDR,
    DRV_SFDP_STATE_WRITE_DATA,
    DRV_SFDP_STATE_CHECK_ERASE_WRITE_STATUS,
    DRV_SFDP_STATE_WAIT_ERASE_WRITE_COMPLETE,
    DRV_SFDP_STATE_ERASE,
    DRV_SFDP_STATE_UNLOCK_FLASH,
    DRV_SFDP_STATE_WAIT_UNLOCK_FLASH_COMPLETE,
    DRV_SFDP_STATE_WAIT_RESET_FLASH_COMPLETE,
    DRV_SFDP_STATE_WAIT_JEDEC_ID_READ_COMPLETE,
    DRV_SFDP_STATE_DISCOVER_SFDP
} DRV_SFDP_STATE;

typedef struct
{
    /* Pointer to the receive data */
    void*   pReceiveData;

    /* Pointer to the transmit data */
    void*   pTransmitData;

    /* Number of bytes to be written */
    size_t  txSize;

    /* Number of bytes to be read */
    size_t  rxSize;

}DRV_SFDP_TRANSFER_OBJ;

/**************************************
 * SFDP Driver Hardware Instance Object
 **************************************/
typedef struct
{
    /* Flag to indicate in use  */
    bool inUse;

    /* Flag to indicate state  */
    DRV_SFDP_STATE state;

    /* Flag to indicate status of transfer */
    volatile DRV_SFDP_TRANSFER_STATUS transferStatus;

    /* The status of the driver */
    SYS_STATUS status;

    /* Intent of opening the driver */
    DRV_IO_INTENT ioIntent;

    /* Indicates the number of clients that have opened this driver */
    size_t nClients;

    /* Points to the FLASH memory address */
    uint32_t memoryAddr;

    /* Pointer to the buffer */
    uint8_t* bufferAddr;

    /* Number of bytes pending to read/write */
    uint32_t nPendingBytes;

    /* Stores the command to be sent */
    uint8_t currentCommand;

    /* Chip Select pin used */
    SYS_PORT_PIN chipSelectPin;

    /* Application event handler */
    DRV_SFDP_EVENT_HANDLER eventHandler;

    /* Application context */
    uintptr_t context;

    /* PLIB API list that will be used by the driver to access the hardware */
    const DRV_SFDP_PLIB_INTERFACE *sfdpPlib;

    DRV_SFDP_TRANSFER_OBJ          transferDataObj;

    /* Discovered flash parameters from SFDP */
    DRV_SFDP_FLASH_PARAMS flashParams;

    /* SFDP discovery completed flag */
    bool sfdpDiscovered;

} DRV_SFDP_OBJECT;




#endif //#ifndef DRV_SFDP_LOCAL_H

/*******************************************************************************
 End of File
*/
