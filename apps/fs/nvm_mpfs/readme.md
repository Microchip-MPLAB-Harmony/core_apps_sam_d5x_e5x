[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# MPFS filesystem using NVM Media

This application shows an example of implementing a MPFS disk in device Internal Flash memory.

## Description

### File System Operations on NVM:

- The application contains a MPFS disk image in the internal Flash memory. The disk image contains two files named:
  - **FILE.txt**, Size = **11 Bytes**. The content of the file is: **"Hello World"**
  - **TEST.txt**, Size = **10 Bytes**. The content of the file is: **"1234567890"**

- The application performs following file system related operations:
  - **SYS_FS_FileRead**
  - **SYS_FS_FileStat**
  - **SYS_FS_FileSize**
  - **SYS_FS_FileSeek**
  - **SYS_FS_FileEOF**

- The contents of both the files are read and compared with the expected strings as mentioned above. If the string compare is successful, An LED indication is provided.

### File system layer uses:

- Memory driver to communicate with underlying NVM media

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/fs/nvm_mpfs/firmware** .

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

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application using its IDE
2. The LED is turned ON when the following criteria is satisfied
    - File **"FILE.txt"** has the string **"Hello World"** in it
    - File **"TEST.txt"** has the string **"1234567890"** in it

Refer to the following table for LED name:

| Board | LED Name |
| ----- | -------- |
|  [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) | LED0 |
|||
