/*
 * bootloader_config.h
 *
 *  Created on: Sep 13, 2023
 *      Author: Abdelrahman Ibrahim
 */

#ifndef INC_BOOTLOADER_CONFIG_H_
#define INC_BOOTLOADER_CONFIG_H_


/**** define number of host command bytes *****/


#define MAX_HOST_COMMAND_LENGHT              100


#define CRC_SIZE_BYTES					     4

#define BL_FLASH_MAX_PAGES					 16


#define BL_VENDOR_ID                100
#define BL_MAJOR_VERSION         1
#define BL_MINOR_VERSION         0
#define BL_PATCH_VERSION         0

#endif /* INC_BOOTLOADER_CONFIG_H_ */
