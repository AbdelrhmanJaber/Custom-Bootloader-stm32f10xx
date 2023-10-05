/*
 * bootloader.h
 *
 *  Created on: Sep 12, 2023
 *      Author: Abdelrahman Ibrahim
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include"usart.h"
#include"crc.h"

#ifndef SRC_BOOTLOADER_H_
#define SRC_BOOTLOADER_H_


#define CBL_FLASH_MASS_ERASE                  0xff


#define ADDRESS_IS_INVALID           0x00
#define ADDRESS_IS_VALID             0x01

#define FLASH_PAYLOAD_WRITE_FAILED   0x00
#define FLASH_PAYLOAD_WRITE_PASSED   0x01

#define INVALID_PAGE_NUMBER          0x00
#define VALID_PAGE_NUMBER            0x01
#define UNSUCCESSFUL_ERASE           0x02
#define SUCCESSFUL_ERASE             0x03

#define 	HAL_SUCCESSFUL_ERASE         			0xFFFFFFFFU

#define CBL_FLASH_MAX_PAGE_NUMBER           12

#define FLASH_LOWER          FLASH_BASE_ADDRESS
#define FLASH_UPPER          FLASH_END_ADDRESS

#define RAM_LOWER            RAM_BASE_ADDRESS
#define RAM_UPPER            RAM_END_ADDRESS



#define BL_GET_VER_CMD              0x10
#define BL_GET_HELP_CMD             0x11
#define BL_GET_CID_CMD              0x12
/* Get Read Protection Status */
#define BL_GET_RDP_STATUS_CMD       0x13
#define BL_GO_TO_ADDR_CMD           0x14
#define BL_FLASH_ERASE_CMD          0x15
#define BL_MEM_WRITE_CMD            0x16
/* Enable/Disable Write Protection */
#define BL_ED_W_PROTECT_CMD         0x17
#define BL_MEM_READ_CMD             0x18
/* Get Sector Read/Write Protection Status */
#define BL_READ_SECTOR_STATUS_CMD   0x19
#define BL_OTP_READ_CMD             0x20
/* Change Read Out Protection Level */
#define BL_CHANGE_ROP_Level_CMD     0x21


typedef void (*jumpAdressPtr)(void);


void BL_Print_Message(char *format, ...);

void BL_fetchHostCommand(void);


typedef enum{
	BL_STATUS_OK = 0,
	BL_STATUS_NOK,
	BL_CRC_OK,
	BL_CRC_NOK,
	BL_ADDRESS_AVAILABLE,
	BL_ADDRESS_UNAVAILABLE
}BL_Status;


#define BL_ACK                       0xAB
#define BL_NACK                      0xCD

#endif /* SRC_BOOTLOADER_H_ */
