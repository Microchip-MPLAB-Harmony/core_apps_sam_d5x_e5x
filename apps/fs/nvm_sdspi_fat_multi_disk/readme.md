[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# FAT file system using NVM and SDSPI media

This application shows an example of using the FAT File System to access files across multiple media (NVM, SDSPI).

## Description

### File System Operations on NVM and SD Card:

- The application contains a FAT disk image consisting of a Master Boot Record (MBR) sector, Logical Boot Sector, File Allocation Table, and Root Directory Area, placed in the internal Flash memory (NVM)
- A SD card is used as another disk, which might have FAT16 or FAT32 implemented on it (dependent on the formatting of SD card)
- The application searches the NVM media for a named **FILE.TXT**, opens and reads the contents of the file in NVM and copies the contents to the file, **FILE.TXT**, in the SD card
- Once the copy is successful, an addition string **"Test is successful"** is added to the file. If the write operation is successful, LED indication is provided

### File system layer uses:

- Memory driver to communicate with underlying NVM media
- SDSPI Driver to communicate to SD Card over SPI

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/fs/nvm_sdspi_fat_multi_disk/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e54_xpro_freertos.X | MPLABX project for [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| sam_e54_xpro_freertos.X | [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) |
|||

### Setting up [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro)

- To run this demo, following additional hardware is required:
  - [I/O1 Xplained Pro Extension Kit](https://www.microchip.com/developmenttools/ProductDetails/ATIO1-XPRO)
  - micro-SD card formatted to FAT filesystem

- Connect [I/O1 Xplained Pro Extension Kit](https://www.microchip.com/developmenttools/ProductDetails/ATIO1-XPRO) to the EXT1 connector
- Insert the micro-SD Card in the micro-SD Card slot of the [I/O1 Xplained Pro Extension Kit](https://www.microchip.com/developmenttools/ProductDetails/ATIO1-XPRO)
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application using its IDE
2. The LED is turned ON when the content from the NVM media is copied to SD-Card successfully

    Refer to the following table for LED name:

    | Board | LED Name |
    | ----- | -------- |
    |  [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) | LED0 |
    |||

3. If Success, Insert the SD Card on to your host PC
4. FILE.txt should have the content "This data from NVM Disk Test is successful"
