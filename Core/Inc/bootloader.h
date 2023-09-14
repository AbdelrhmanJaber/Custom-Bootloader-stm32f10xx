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

#define CRC_ENGINE_OBJ               &hcrc

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

#define BL_VENDOR_ID                100
#define BL_MAJOR_VERSION         1
#define BL_MINOR_VERSION         1
#define BL_PATCH_VERSION         0


#define BL_hostCommunicationUART    &huart1


void BL_Print_Message(char *format, ...);

void BL_fetchHostCommand(void);


typedef enum{
	BL_NACK = 0,
	BL_OK
}BL_Status;

#define CRC_VERIFICATION_FAILED      0x00
#define CRC_VERIFICATION_PASSED      0x01

#define CBL_SEND_NACK                0xAB
#define CBL_SEND_ACK                 0xCD
#define CRC_TYPE_SIZE_BYTE           4



#endif /* SRC_BOOTLOADER_H_ */
