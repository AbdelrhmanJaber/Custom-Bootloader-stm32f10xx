/*
 * bootloader.c
 *
 *  Created on: Sep 12, 2023
 *      Author: Abdelrahman Ibrahim
 */


#include"STD_Types.h"

#include"bootloader.h"

#include"bootloader_config.h"

/****************** static function declaration ******************/

static void Bootloader_Get_Version(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Get_Help(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Get_Chip_Identification_Number(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Read_Protection_Level(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Jump_To_Address(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Erase_Flash(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Memory_Write(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Enable_RW_Protection(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Memory_Read(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Get_Sector_Protection_Status(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Read_OTP(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Change_Read_Protection_Level(uint8_t *copy_Puint8_hostBuffer);
static uint8_t Bootloader_CRC_Verify(uint8_t *copy_Puint8_Data, uint32_t copy_uint32_dataLenght,
		uint32_t copy_uint32_hostCRC);
static void Bootloader_Send_ACK(uint8_t copy_uint8_replayLenght);
static void Bootloader_Send_NACK(void);
static void Bootloader_Send_Data_To_Host(uint8_t *copy_Puint8_hostBuffer, uint32_t copy_uint32_dataLenght);
static uint8_t Host_Address_Verification(uint32_t copy_uint32_jumpAddress);
static uint8_t Perform_Flash_Erase(uint8_t copy_uint8_sectorNumber, uint8_t copy_uint8_numberOfSectors);

/************************** variables and structures definition***************************************/

static uint8_t hostBuffer[MAX_HOST_COMMAND_LENGHT];

/****************** Macros  ******************/

#define  TEST_BOOTLOADER     0x10

void BL_fetchHostCommand(void){
	uint8_t dataLenght = 0;
	memset(hostBuffer , 0 , MAX_HOST_COMMAND_LENGHT);
	HAL_UART_Receive(BL_hostCommunicationUART, hostBuffer, 1, HAL_MAX_DELAY);
	/*get length of the command*/
	dataLenght = hostBuffer[0];
	/*fetch the command from the user*/
	HAL_UART_Receive(BL_hostCommunicationUART, &hostBuffer[1], dataLenght, HAL_MAX_DELAY);
	switch(hostBuffer[1]){
		case CBL_GET_VER_CMD:{
			BL_Print_Message("CBL_GET_VER_CMD \r\n");
			break;
		}
		case CBL_GET_HELP_CMD:{
			BL_Print_Message("CBL_GET_HELP_CMD \r\n");
			break;
		}
		case CBL_GET_CID_CMD:{
			BL_Print_Message("CBL_GET_CID_CMD \r\n");
			break;
		}
		case CBL_GET_RDP_STATUS_CMD:{
			BL_Print_Message("CBL_GET_RDP_STATUS_CMD \r\n");
			break;
		}
		case CBL_GO_TO_ADDR_CMD:{
			BL_Print_Message("CBL_GO_TO_ADDR_CMD \r\n");
			break;
		}
		case CBL_FLASH_ERASE_CMD:{
			BL_Print_Message("CBL_FLASH_ERASE_CMD \r\n");
			break;
		}
		case CBL_MEM_WRITE_CMD:{
			BL_Print_Message("CBL_MEM_WRITE_CMD \r\n");
			break;
		}
		case CBL_ED_W_PROTECT_CMD:{
			BL_Print_Message("CBL_ED_W_PROTECT_CMD \r\n");
			break;
		}
		case CBL_MEM_READ_CMD:{
			BL_Print_Message("CBL_MEM_READ_CMD \r\n");
			break;
		}
		case CBL_READ_SECTOR_STATUS_CMD:{
			BL_Print_Message("CBL_READ_SECTOR_STATUS_CMD \r\n");
			break;
				}
		case CBL_OTP_READ_CMD:{
			BL_Print_Message("CBL_OTP_READ_CMD \r\n");
			break;
				}
		case CBL_CHANGE_ROP_Level_CMD:{
			BL_Print_Message("CBL_CHANGE_ROP_Level_CMD \r\n");
			break;
		}
		default:{
			BL_Print_Message("Invalid Command\r\n");
			break;
		}
	}
}


void BL_Print_Message(char *format, ...){
	char Messsage[100] = {0};
	va_list args;
	va_start(args, format);
	vsprintf(Messsage, format, args);
	HAL_UART_Transmit(BL_hostCommunicationUART, (uint8_t *)Messsage, sizeof(Messsage), HAL_MAX_DELAY);
	va_end(args);
}


/************************************ Implement static functions*********************************/




static void Bootloader_Get_Version(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Get_Help(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Get_Chip_Identification_Number(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Read_Protection_Level(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Jump_To_Address(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Erase_Flash(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Memory_Write(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Enable_RW_Protection(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Memory_Read(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Get_Sector_Protection_Status(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Read_OTP(uint8_t *copy_Puint8_hostBuffer){

}

static void Bootloader_Change_Read_Protection_Level(uint8_t *copy_Puint8_hostBuffer){

}

static uint8_t Bootloader_CRC_Verify(uint8_t *copy_Puint8_Data, uint32_t copy_uint32_dataLenght,
		uint32_t copy_uint32_hostCRC){

}

static void Bootloader_Send_ACK(uint8_t copy_uint8_replayLenght){

}

static void Bootloader_Send_NACK(void){

}

static void Bootloader_Send_Data_To_Host(uint8_t *copy_Puint8_hostBuffer, uint32_t copy_uint32_dataLenght){

}

static uint8_t Host_Address_Verification(uint32_t copy_uint32_jumpAddress){

}

static uint8_t Perform_Flash_Erase(uint8_t copy_uint8_sectorNumber, uint8_t copy_uint8_numberOfSectors){

}





