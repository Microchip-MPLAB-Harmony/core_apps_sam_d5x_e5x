[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# MPFS filesystem using NVM and FAT filessystem using SDMMC media

This application shows an example of using the MPFS filesystem on NVM and FAT filesystem on SDMMC media simultaneously.

## Description

### File System Operations on NVM and SD Card:

- The application contains a MPFS disk image placed in the internal Flash memory (NVM). The disk image contains below file:
  - **abc.txt**, Size = **13 Bytes**. The content of the file is: **"Hello World !"**
- A SD card is used as another disk, which might have FAT16 or FAT32 implemented on it (dependent on the formatting of SD card)
- On board RTC is used by file system to update the time stamps of the file. Initial value to RTC is the **app.c compile time**
- The application opens and reads the contents of the file **abc.txt** in NVM and copies the contents to the file to **FILE.TXT** in the SD card
- Once the copy is successful from NVM to SDCARD, the application writes the last modified timestamp of **FILE.txt** updated by file system to **FILE.txt** on SDCARD
- If the write operation is successful, LED indication is provided

### File system layer uses:

- Memory driver to communicate with underlying NVM media.
- SDMMC Driver to communicate to SD Card
- RTC to update file modification time

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/fs/nvm_sdmmc_fat_mpfs_multi_disk/firmware** .

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

- To run the demo, the following additional hardware are required:
  - SD card

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application using its IDE
2. Insert the SD Card in the SD Card slot of the Device.
3. The LED is turned ON when
    - The content from the NVM media (HELLO WORLD !) is copied to SD Card successfully
    - Application has successfully written the last modified time stamp to SD Card

    Refer to the following table for LED name:

    | Board | LED Name |
    | ----- | -------- |
    |  [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) | LED0 |
    |||

4. If Success, Insert the SD Card on to your host PC
5. FILE.txt should have the following content:

```
    HELLO WORLD !

    File Modified on [dd/mm/yy hh:mm:ss]
```

### Notes

- The initial time loaded into RTC is the compile time of app.c file
- The File Modified time stamp mentioned above is the RTC time retrieved after writing "HELLO WORLD !" to FILE.txt
- If step 3 is repeated then new timestamp along with "HELLO WORLD !" is appended to the existing FILE.txt
- If device is reset, RTC will be again loaded with compile time of app.c file
