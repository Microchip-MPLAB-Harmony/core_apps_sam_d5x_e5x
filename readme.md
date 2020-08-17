[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Harmony 3 Driver and System Service application examples for SAM E5x/D5x family

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices.  Refer to the following links for more information.

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 Driver and System Service application examples for SAM E5x/D5x family

- [Release Notes](release_notes.md)
- [MPLAB® Harmony License](mplab_harmony_license.md)

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

## Contents Summary

| Folder     | Description                             |
| ---        | ---                                     |
| apps       | Peripheral library example applications |
| docs       | Applications help documentation         |

## Code Examples

The following applications are provided to demonstrate the typical or interesting usage models of one or more driver and system services.

| Name | Description|
|:---------|:-----------|
| [I2C driver asynchronous - I2C EEPROM](apps/driver/i2c/async/i2c_eeprom/readme.md) | This example application shows how to use the I2C driver in asynchronous mode to perform operations on the EEPROM |
| [I2C driver asynchronous - multi client](apps/driver/i2c/async/i2c_multi_slave/readme.md) | This example application demonstrates the multi-client feature of the asynchronous mode of the I2C driver by communicating with an external EEPROM and a temperature sensor |
| [I2C driver synchronous - I2C EEPROM](apps/driver/i2c/sync/i2c_eeprom/readme.md) | This example application shows how to use the I2C driver in synchronous mode to perform operations on the EEPROM |
| [I2C driver synchronous - multi client](apps/driver/i2c/sync/i2c_multi_slave/readme.md) | This example application demonstrates the multi-client feature of the synchronous mode of the I2C driver by communicating with an external EEPROM and a temperature sensor |
| [I2C EEPROM AT24 driver - EEPROM Read Write](apps/driver/i2c_eeprom/at24/at24_eeprom_read_write/readme.md) | This example application shows how to use the AT24 driver to perform read and write operations on AT24 series of EEPROM |
| [SDMMC driver asynchronous - SDMMC Read Write](apps/driver/sdmmc/async/sdmmc_read_write/readme.md) | This example application shows how to use the SDMMC driver in asynchronous mode to perform block operations on the SD Card Media |
| [SDSPI driver synchronous - SDSPI Read Write](apps/driver/sdspi/sync/sdspi_read_write/readme.md) | This example application shows how to use the SDSPI driver in synchronous mode to perform block operations on the SD Card Media |
| [SPI driver asynchronous - multi instance](apps/driver/spi/async/spi_multi_instance/readme.md) | This example application demonstrates the multi instance feature of the asynchronous mode of the SPI driver by reading and writing to two EEPROMs connected to two different SPI bus |
| [SPI Driver asynchronous - multi client](apps/driver/spi/async/spi_multi_slave/readme.md) | This example demonstrates how to use single instance of the SPI driver in asynchronous mode to communicate with multiple EEPROMs |
| [SPI Driver asynchronous - Self loopback multi client](apps/driver/spi/async/spi_self_loopback_multi_client/readme.md) | This example demonstrates how to use the SPI driver in asynchronous mode to achieve self-loop back between multiple clients |
| [SPI driver synchronous - multi instance](apps/driver/spi/sync/spi_multi_instance/readme.md) | This example application demonstrates the multi instance feature of the synchronous mode of the SPI driver by reading and writing to two EEPROMs connected to two different SPI bus |
| [SPI Driver synchronous - multi client](apps/driver/spi/sync/spi_multi_slave/readme.md) | This example demonstrates how to use single instance of the SPI driver in synchronous mode to communicate with multiple EEPROMs |
| [SPI Driver synchronous - Self loopback multi client](apps/driver/spi/sync/spi_self_loopback_multi_client/readme.md) | This example demonstrates how to use the SPI driver in synchronous mode to achieve self-loop back between multiple clients in RTOS environment |
| [SPI Driver synchronous - Self loopback multi client](apps/driver/spi/sync/spi_self_loopback_multi_client/readme.md) | This example demonstrates how to use the SPI driver in synchronous mode to achieve self-loop back between multiple clients |
| [SPI EEPROM AT25 driver - EEPROM Read Write](apps/driver/spi_eeprom/at25/at25_eeprom_read_write/readme.md) | This example application shows how to use the AT25 driver to perform read and write operations on AT25 series of EEPROM |
| [SST26 SQI flash driver - Flash read write in SPI mode](apps/driver/sqi_flash/sst26/sst26_spi_read_write/readme.md) | This example application shows how to use the SST26 SQI flash driver to perform block operations on the On-Board SST26 Flash memory in SPI mode |
| [SST26 SQI flash driver - Flash read write in Quad IO mode](apps/driver/sqi_flash/sst26/sst26_sqi_read_write/readme.md) | This example application shows how to use the SST26 SQI flash driver to perform block operations on the On-Board SST26 Flash memory in Quad IO mode |
| [USART driver asynchronous - USART echo](apps/driver/usart/async/usart_echo/readme.md) | This example echoes the received characters over the console using the USART driver in asynchronous mode |
| [USART driver asynchronous - USART multi instance](apps/driver/usart/async/usart_multi_instance/readme.md) | This example echoes the received characters over the two consoles using the USART driver in asynchronous mode |
| [USART driver synchronous - USART echo](apps/driver/usart/sync/usart_echo/readme.md) | This example echoes the received characters over the console using the USART driver in synchronous mode |
| [USART driver synchronous - USART multi instance](apps/driver/usart/sync/usart_multi_instance/readme.md) | This example echoes the received characters over the two consoles using the USART driver in synchronous mode |
| [FAT filesystem using AT24 EEPROM media](apps/fs/i2c_eeprom_fat/readme.md) | This application shows an example of using the MPLAB Harmony File System to access I2C EEPROM based media |
| [FAT filesystem using NVM Media](apps/fs/nvm_fat/readme.md) | This application shows an example of implementing a FAT disk in the device internal Flash memory |
| [MPFS filesystem using NVM Media](apps/fs/nvm_mpfs/readme.md) | This application shows an example of implementing a MPFS disk in device Internal Flash memory |
| [MPFS filesystem using NVM and FAT filessystem using SDMMC media](apps/fs/nvm_sdmmc_fat_mpfs_multi_disk/readme.md) | This application shows an example of using the MPFS filesystem on NVM and FAT filesystem on SDMMC media simultaneously |
| [FAT file system using NVM and SDMMC media](apps/fs/nvm_sdmmc_fat_multi_disk/readme.md) | This application shows an example of using the FAT File System to access files across multiple media (NVM, SDMMC) |
| [FAT file system using NVM and SDSPI media](apps/fs/nvm_sdspi_fat_multi_disk/readme.md) | This application shows an example of using the FAT File System to access files across multiple media (NVM, SDSPI) |
| [FAT filesystem using SDMMC Media](apps/fs/sdmmc_fat/readme.md) | This application shows an example of using the MPLAB Harmony File System to access and modify the contents of a SD card using the SDMMC driver |
| [FAT filesystem using SDSPI media](apps/fs/sdspi_fat/readme.md) | This application shows an example of using the MPLAB Harmony File System to access and modify the contents of a SD card using the SDSPI driver and the SPI PLIB |
| [FreeRTOS basic](apps/rtos/freertos/basic_freertos/readme.md) | This example application blinks an LED to show the FreeRTOS threads that are running and to indicate status |
| [FreeRTOS Task Notification](apps/rtos/freertos/task_notification_freertos/readme.md) | This example application is to illustrate the FreeRTOS Task Notification feature which is used as a light weight binary semaphore |
| [Console Debug System Service using UART](apps/system/console_debug/sys_console_debug_uart_read_write/readme.md) | This example application demonstrates the UART based console and debug system service |
| [Console Debug System Service using USB](apps/system/console_debug/sys_console_debug_usb_read_write/readme.md) | This example application demonstrates the USB based console and debug system service |
| [Time System Service multi-client](apps/system/time/sys_time_multiclient/readme.md) | This example application demonstrates the multi-client system timer functionality |
|||

____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x/blob/master/mplab_harmony_license.md)
[![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x.svg)](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x/releases/latest)
[![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x.svg)](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x/releases/latest)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x.svg)](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x.svg)]()

____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/core_apps_sam_d5x_e5x.svg?style=social)]()
