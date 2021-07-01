/*******************************************************************************
  Low Level Block Deivce Interface Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    lfs_bdio.c

  Summary:
    This file contains implementation of Low Level Block Deivce Interface functions 
    which hooking to LFS library.

  Description:
    This file contains implementation of Low Level Block Deivce Interface functions 
    which hooking to LFS library.
*******************************************************************************/

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
 
#include "lfs_bdio.h"

#include <stdlib.h>
#include "system/fs/sys_fs_media_manager.h"

typedef struct
{
    SYS_FS_MEDIA_COMMAND_STATUS commandStatus;
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle;
} SYS_FS_BD_DATA;

static SYS_FS_BD_DATA CACHE_ALIGN gSysFsDiskData[SYS_FS_MEDIA_NUMBER];

void bdEventHandler
(
    SYS_FS_MEDIA_BLOCK_EVENT event,
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle,
    uintptr_t context
)
{
    switch(event)
    {
        case SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_COMPLETE:
            gSysFsDiskData[context].commandStatus = SYS_FS_MEDIA_COMMAND_COMPLETED;
            break;
        case SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_ERROR:
            gSysFsDiskData[context].commandStatus= SYS_FS_MEDIA_COMMAND_UNKNOWN;
            break;
        default:
            break;
    }
}

static BDSTATUS bd_checkCommandStatus(uint8_t pdrv)
{
    BDSTATUS result = RES_ERROR;

    /* Buffer is invalid report error */
    if (gSysFsDiskData[pdrv].commandHandle == SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID)
    {
        result = RES_PARERR;
    }
    else
    {
        /* process the read request by blocking on the task routine that process the
         I/O request */
        while (gSysFsDiskData[pdrv].commandStatus == SYS_FS_MEDIA_COMMAND_IN_PROGRESS)
        {
            SYS_FS_MEDIA_MANAGER_TransferTask (pdrv);
        }


        if (gSysFsDiskData[pdrv].commandStatus == SYS_FS_MEDIA_COMMAND_COMPLETED)
        {
            /* Buffer processed successfully */
            result = RES_OK;
        }
    }
    
    return result;
}

static BDSTATUS disk_read_aligned
(
    uint8_t pdrv,   /* Physical drive nmuber (0..) */
    uint8_t *buff,  /* Data buffer to store read data */
    uint32_t sector,/* Sector address (LBA) */
    uint32_t sector_count   /* Number of sectors to read (1..128) */
)
{
    BDSTATUS result = RES_ERROR;

    gSysFsDiskData[pdrv].commandStatus = SYS_FS_MEDIA_COMMAND_IN_PROGRESS;

    gSysFsDiskData[pdrv].commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;

    /* Submit the read request to media */
    gSysFsDiskData[pdrv].commandHandle = SYS_FS_MEDIA_MANAGER_SectorRead(pdrv /* DISK Number */ ,
            buff /* Destination Buffer*/,
            sector /* Source Sector */,
            sector_count /* Number of Sectors */);

    result = bd_checkCommandStatus(pdrv);

    return result;
}

BDSTATUS lfs_bdio_initilize (
    uint8_t pdrv                /* Physical drive nmuber to identify the drive */
)
{
    switch( pdrv ) {
    case 0:
    default:
        break;
    }

    SYS_FS_MEDIA_MANAGER_RegisterTransferHandler( (void *) bdEventHandler );
    return 0;
}

/// block device API ///
int lfs_bdio_read(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size) {
    
    BDSTATUS result = RES_ERROR;
    BLOCK_DEV *bd = cfg->context;
    uint32_t sector = block * (cfg->block_size/ SYS_FS_LFS_MAX_SS);
    uint32_t count = size/ SYS_FS_LFS_MAX_SS;

    {
        result = disk_read_aligned(bd->disk_num, buffer, sector, count);
		
        if (result != RES_OK)
        {
            return LFS_ERR_IO;
        }
    }
    
    return LFS_ERR_OK;
}

int lfs_bdio_prog(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size) {
    
    BDSTATUS result = RES_ERROR;
    BLOCK_DEV *bd = cfg->context;
    
    {
        gSysFsDiskData[bd->disk_num].commandStatus = SYS_FS_MEDIA_COMMAND_IN_PROGRESS;
        gSysFsDiskData[bd->disk_num].commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;

       gSysFsDiskData[bd->disk_num].commandHandle =  SYS_FS_MEDIA_MANAGER_SectorWrite(bd->disk_num, block * cfg->block_size/SYS_FS_LFS_MAX_SS, (uint8_t*) buffer,  size/ SYS_FS_LFS_MAX_SS);      

       result = bd_checkCommandStatus(bd->disk_num);
        if (result != RES_OK)
        {
            return RES_ERROR;
        }
    }
    return 0;
}

int lfs_bdio_erase(const struct lfs_config *cfg, lfs_block_t block) {
    BDSTATUS result = RES_ERROR;
    BLOCK_DEV *bd = cfg->context;
    uint8_t buffer[SYS_FS_LFS_MAX_SS];
    memset(buffer, 0xff, sizeof(buffer));

    gSysFsDiskData[bd->disk_num].commandStatus = SYS_FS_MEDIA_COMMAND_IN_PROGRESS;
    gSysFsDiskData[bd->disk_num].commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
    
    gSysFsDiskData[bd->disk_num].commandHandle =  SYS_FS_MEDIA_MANAGER_SectorWrite(bd->disk_num, block * cfg->block_size/SYS_FS_LFS_MAX_SS, (uint8_t*) buffer,  1);
   
    result = bd_checkCommandStatus(bd->disk_num);

    if (result != RES_OK)
    {
        return RES_ERROR;
    }
    return result;
}

int lfs_bdio_sync(const struct lfs_config *cfg) {
    
    LFS_TRACE("[%s] In\r\n", __func__);
    return 0;
}


