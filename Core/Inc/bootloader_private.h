/*
 * bootloader_private.h
 *
 *  Created on: Sep 13, 2023
 *      Author: Kimo Store
 */

#ifndef INC_BOOTLOADER_PRIVATE_H_
#define INC_BOOTLOADER_PRIVATE_H_

#define FLASH_BASE_ADDRESS      0x08000000UL
#define RAM_BASE_ADDRESS        0x20000000UL

#define FLASH_SIZE                (128*1024)
#define RAM_SIZE                  (20*1024)

#define FLASH_END_ADDRESS       (FLASH_BASE_ADDRESS + FLASH_SIZE)
#define RAM_END_ADDRESS         (RAM_BASE_ADDRESS + RAM_SIZE)

#define BL_hostCommunicationUART    &huart1
#define CRC_REG                     &hcrc

#endif /* INC_BOOTLOADER_PRIVATE_H_ */
