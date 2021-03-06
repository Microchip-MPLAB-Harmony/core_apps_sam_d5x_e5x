---
parent: Harmony 3 driver and system service application examples for SAM D5X/E5X family
title: SST26 SQI flash driver - Flash read write in Quad IO mode 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# SST26 SQI flash driver - Flash read write in Quad IO mode

This example application shows how to use the SST26 SQI flash driver to perform block operations on the On-Board SST26 Flash memory in Quad IO mode.

## Description

This application uses the SST26 driver to Erase/Write/Read on the On-Board SST26 Flash memory using the QSPI or SQI peripheral library in Quad IO mode.

The application consists of APP_SST26_Tasks() which is called through SYS_Tasks() routine.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/driver/sqi_flash/sst26/sst26_sqi_read_write/firmware** .

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

**Note: This demo requires newer revision of [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) that replaced N25Q256A QSPI Flash with SST26 QSPI Flash memory**

## Running the Application

1. Build and program the application using its IDE
2. Reset the device by power cycle
3. LED is turned ON when the data read from SST26 flash matches with the data written in it

Refer to the following table for LED name:

| Board | LED Name |
| ----- | -------- |
|  [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsame54-xpro) | LED0 |
|||
