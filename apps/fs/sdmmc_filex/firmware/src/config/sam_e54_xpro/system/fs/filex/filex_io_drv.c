/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#include <string.h>
#include "filex_io_drv.h"        /* FileX low level media I/O header */
#include "system/fs/sys_fs_media_manager.h"

typedef struct
{
    SYS_FS_MEDIA_COMMAND_STATUS commandStatus;
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle;
} SYS_FS_FILEX_IO_DATA;

static SYS_FS_FILEX_IO_DATA CACHE_ALIGN gSysFsDiskData[SYS_FS_MEDIA_NUMBER];

void filexIoEventHandler
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

static FILEX_IO_RESULT filexIoCheckCommandStatus(uint8_t pdrv)
{
    FILEX_IO_RESULT result = RES_ERROR;

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

static FILEX_IO_RESULT disk_read_aligned
(
    uint8_t pdrv,   /* Physical drive nmuber (0..) */
    uint8_t *buff,  /* Data buffer to store read data */
    uint32_t sector,/* Sector address (LBA) */
    uint32_t sector_count   /* Number of sectors to read (1..128) */
)
{
    FILEX_IO_RESULT result = RES_ERROR;

    gSysFsDiskData[pdrv].commandStatus = SYS_FS_MEDIA_COMMAND_IN_PROGRESS;

    gSysFsDiskData[pdrv].commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;

    /* Submit the read request to media */
    gSysFsDiskData[pdrv].commandHandle = SYS_FS_MEDIA_MANAGER_SectorRead(pdrv /* DISK Number */ ,
            buff /* Destination Buffer*/,
            sector /* Source Sector */,
            sector_count /* Number of Sectors */);

    result = filexIoCheckCommandStatus(pdrv);

    return result;
}

void filex_io_drv_entry(FX_MEDIA *media_ptr)
{
    FILEX_IO_RESULT result = RES_ERROR;
    uint8_t *ptrdrv = (uint8_t *)media_ptr->fx_media_driver_info;
    uint8_t pdrv = *ptrdrv;

    /* Process the driver request specified in the media control block.  */
    switch (media_ptr -> fx_media_driver_request)
    {
        case FX_DRIVER_INIT:
        {
            SYS_FS_MEDIA_MANAGER_RegisterTransferHandler((void *)filexIoEventHandler);

            media_ptr->fx_media_driver_free_sector_update = FX_TRUE;
            if (SYS_FS_FILEX_READONLY)
            {
                media_ptr -> fx_media_driver_write_protect = FX_TRUE;
            }
            /* Success */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_READ:
        {
            {
                result = disk_read_aligned(pdrv, (uint8_t *)media_ptr->fx_media_driver_buffer, (uint32_t)(media_ptr -> fx_media_driver_logical_sector + media_ptr -> fx_media_hidden_sectors), (uint32_t)media_ptr->fx_media_driver_sectors);
            }

            if (result == RES_OK)
            {
                /* Success  */
                media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            }
            else
            {
                /* Error */
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
            }
            break;
        }

        case FX_DRIVER_WRITE:
        {
            {
                gSysFsDiskData[pdrv].commandStatus = SYS_FS_MEDIA_COMMAND_IN_PROGRESS;

                gSysFsDiskData[pdrv].commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;

                /* Submit the write request to media */
                gSysFsDiskData[pdrv].commandHandle = SYS_FS_MEDIA_MANAGER_SectorWrite(pdrv /* DISK Number */ ,
                        (uint32_t)(media_ptr -> fx_media_driver_logical_sector + media_ptr -> fx_media_hidden_sectors) /* Destination Sector*/,
                        (uint8_t *)media_ptr->fx_media_driver_buffer /* Source Buffer */,
                        (uint32_t)media_ptr->fx_media_driver_sectors /* Number of Sectors */);

                result = filexIoCheckCommandStatus(pdrv);
            }

            if (result == RES_OK)
            {
                /* Success  */
                media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            }
            else
            {
                /* Error */
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
            }
            break;
        }

        case FX_DRIVER_BOOT_READ:
        {
            {
                result = disk_read_aligned(pdrv, (uint8_t *)media_ptr->fx_media_driver_buffer, 0, 1);
            }

            if (result == RES_OK)
            {
                /* Success  */
                media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            }
            else
            {
                /* Error */
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
            }
            break;
        }

        case FX_DRIVER_BOOT_WRITE:
        {
            {
                gSysFsDiskData[pdrv].commandStatus = SYS_FS_MEDIA_COMMAND_IN_PROGRESS;

                gSysFsDiskData[pdrv].commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;

                /* Submit the write request to media */
                gSysFsDiskData[pdrv].commandHandle = SYS_FS_MEDIA_MANAGER_SectorWrite(pdrv /* DISK Number */ ,
                        0 /* Boot Sector*/,
                        (uint8_t *)media_ptr->fx_media_driver_buffer /* Source Buffer */,
                        1 /* Number of Sectors */);

                result = filexIoCheckCommandStatus(pdrv);
            }

            if (result == RES_OK)
            {
                /* Success  */
                media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            }
            else
            {
                /* Error */
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
            }
            break;
        }

        case FX_DRIVER_FLUSH:
        {

            /* Success */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_ABORT:
        {

            /* Success */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_UNINIT:
        {
            /* Success */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        default:
        {
            /* Invalid request */
            media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
            break;
        }
    }
}

uint32_t filex_io_disk_get_sector_count(uint8_t pdrv)
{
    uint32_t mediaBlockSize = 0;
    uint32_t numBlocksPerSector = 1;
    SYS_FS_MEDIA_GEOMETRY *mediaGeometry = NULL;
    uint32_t sector_count = 64;

    mediaGeometry = SYS_FS_MEDIA_MANAGER_GetMediaGeometry (pdrv);
    if (mediaGeometry != NULL)
    {
        mediaBlockSize = mediaGeometry->geometryTable[0].blockSize;

        if (mediaBlockSize < SYS_FS_FILEX_MAX_SS)
        {
            /* Perform block to sector translation */
            numBlocksPerSector = (SYS_FS_FILEX_MAX_SS / mediaBlockSize);
        }
        sector_count = mediaGeometry->geometryTable[0].numBlocks / numBlocksPerSector;
    }

    return sector_count;
}
