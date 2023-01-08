---
parent: Harmony 3 driver and system service application examples for SAM D5X/E5X family
title: FILEX filesystem using SDMMC Media
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# FILEX filesystem using SDMMC Media

This application shows an example of using the FILEX File System to access and modify the contents of a SD card using the SDMMC driver

## Description

### File System Operations on the SD Card:

- Application creates a file named **newfile.txt** in the root directory of the SD card and writes **"FILEX filesystem demo using SD CARD Media"** string into file
- Application reads file named **newfile.txt** and stores its content into memory buffer
- Then read memory buffer is compared with the string that was written to the file. If the string compare is successful, An LED indication is provided
- During execution, the application performs following file system related operations:
  - **SYS_FS_DriveFormat**
  - **SYS_FS_Mount**
  - **SYS_FS_CurrentDriveSet**
  - **SYS_FS_FileDirectoryRemove**
  - **SYS_FS_FileOpen**
  - **SYS_FS_FileWrite**
  - **SYS_FS_FileSeek**
  - **SYS_FS_FileRead**
  - **SYS_FS_FileStat**
  - **SYS_FS_FileEOF**
  - **SYS_FS_FileSize**
  - **SYS_FS_FileClose**
  - **SYS_FS_Unmount**

### File system layer uses:

- FILEX Filesystem
- SDMMC Driver to communicate to SD Card

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/fs/sdmmc_filex/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e54_xpro.X | MPLABX project for [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| sam_e54_xpro.X | [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) |
|||

### Setting up [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro)

- To run this application, following additional hardware is required
  - One micro-sd card
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application using its IDE
2. Insert the SD Card in the SD Card slot of the Device
3. The LED is turned ON when the File **newfile.txt** has the app data **"FILEX filesystem demo using SD CARD Media"** written in it

    Refer to the following table for LED name:

    | Board | LED Name |
    | ----- | -------- |
    |  [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) | LED0 |
    |||

4. If LED is ON, Insert the SD Card on to your host PC
5. SD Card should have the file **newfile.txt** and should have **"FILEX filesystem demo using SD CARD Media"** string
