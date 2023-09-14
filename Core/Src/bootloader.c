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
	uint8_t dataLenght = 0 , test;
	memset(hostBuffer , 0 , MAX_HOST_COMMAND_LENGHT);
	HAL_UART_Receive(BL_hostCommunicationUART, hostBuffer, 1, HAL_MAX_DELAY);
	/*get length of the command*/
	dataLenght = hostBuffer[0];
	/*fetch the command from the user*/
	HAL_UART_Receive(BL_hostCommunicationUART, &hostBuffer[1], dataLenght, HAL_MAX_DELAY);
	//HAL_UART_Receive(BL_hostCommunicationUART, &hostBuffer[1], dataLenght, HAL_MAX_DELAY);
	switch(hostBuffer[1]){
		case BL_GET_VER_CMD:{
			Bootloader_Get_Version(hostBuffer);
			break;
		}
		case BL_GET_HELP_CMD:{
			BL_Print_Message("CBL_GET_HELP_CMD \r\n");
			break;
		}
		case BL_GET_CID_CMD:{
			BL_Print_Message("CBL_GET_CID_CMD \r\n");
			break;
		}
		case BL_GET_RDP_STATUS_CMD:{
			BL_Print_Message("CBL_GET_RDP_STATUS_CMD \r\n");
			break;
		}
		case BL_GO_TO_ADDR_CMD:{
			BL_Print_Message("CBL_GO_TO_ADDR_CMD \r\n");
			break;
		}
		case BL_FLASH_ERASE_CMD:{
			BL_Print_Message("CBL_FLASH_ERASE_CMD \r\n");
			break;
		}
		case BL_MEM_WRITE_CMD:{
			BL_Print_Message("CBL_MEM_WRITE_CMD \r\n");
			break;
		}
		case BL_ED_W_PROTECT_CMD:{
			BL_Print_Message("CBL_ED_W_PROTECT_CMD \r\n");
			break;
		}
		case BL_MEM_READ_CMD:{
			BL_Print_Message("CBL_MEM_READ_CMD \r\n");
			break;
		}
		case BL_READ_SECTOR_STATUS_CMD:{
			BL_Print_Message("CBL_READ_SECTOR_STATUS_CMD \r\n");
			break;
				}
		case BL_OTP_READ_CMD:{
			BL_Print_Message("CBL_OTP_READ_CMD \r\n");
			break;
				}
		case BL_CHANGE_ROP_Level_CMD:{
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



static uint8_t Bootloader_CRC_Verify(uint8_t *copy_Puint8_Data, uint32_t copy_uint32_dataLenght,
		uint32_t copy_uint32_hostCRC){
	uint8_t CRC_Status = CRC_VERIFICATION_FAILED;
		uint32_t MCU_CRC_Calculated = 0;
		uint8_t Data_Counter = 0;
		uint32_t Data_Buffer = 0;
		/* Calculate CRC32 */
		for(Data_Counter = 0; Data_Counter < copy_uint32_dataLenght; Data_Counter++){
			Data_Buffer = (uint32_t)copy_Puint8_Data[Data_Counter];
			MCU_CRC_Calculated = HAL_CRC_Accumulate(CRC_ENGINE_OBJ, &Data_Buffer, 1);
		}
		/* Reset the CRC Calculation Unit */
	  __HAL_CRC_DR_RESET(CRC_ENGINE_OBJ);
		/* Compare the Host CRC and Calculated CRC */
		if(MCU_CRC_Calculated == copy_uint32_hostCRC){
			CRC_Status = CRC_VERIFICATION_PASSED;
		}
		else{
			CRC_Status = CRC_VERIFICATION_FAILED;
		}

		return CRC_Status;
}

static void Bootloader_Send_ACK(uint8_t copy_uint8_replayLenght){
	uint8_t Ack_Value[2] = {0};
		Ack_Value[0] = CBL_SEND_ACK;
		Ack_Value[1] = copy_uint8_replayLenght;
		HAL_UART_Transmit(BL_hostCommunicationUART, (uint8_t *)Ack_Value, 2, HAL_MAX_DELAY);
}

static void Bootloader_Send_NACK(void){
	uint8_t Ack_Value = CBL_SEND_NACK;
    HAL_UART_Transmit(BL_hostCommunicationUART, (uint8_t *)Ack_Value, 1, HAL_MAX_DELAY);
}

static void Bootloader_Get_Version(uint8_t *copy_Puint8_hostBuffer){

	uint8_t BL_Version[4] = { CBL_VENDOR_ID, CBL_SW_MAJOR_VERSION, CBL_SW_MINOR_VERSION, CBL_SW_PATCH_VERSION };
	uint16_t Host_CMD_Packet_Len = 0;
  uint32_t Host_CRC32 = 0;

	/* Extract the CRC32 and packet length sent by the HOST */
	Host_CMD_Packet_Len = hostBuffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((hostBuffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));
	/* CRC Verification */
	if(CRC_VERIFICATION_PASSED == Bootloader_CRC_Verify((uint8_t *)&hostBuffer[0] , Host_CMD_Packet_Len - 4, Host_CRC32)){

		Bootloader_Send_ACK(4);
		Bootloader_Send_Data_To_Host((uint8_t *)(&BL_Version[0]), 4);
	}
	else{
		Bootloader_Send_NACK();
	}
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


static void Bootloader_Send_Data_To_Host(uint8_t *copy_Puint8_hostBuffer, uint32_t copy_uint32_dataLenght){
	HAL_UART_Transmit(BL_hostCommunicationUART, copy_Puint8_hostBuffer, copy_uint32_dataLenght, HAL_MAX_DELAY);
}

static uint8_t Host_Address_Verification(uint32_t copy_uint32_jumpAddress){

}

static uint8_t Perform_Flash_Erase(uint8_t copy_uint8_sectorNumber, uint8_t copy_uint8_numberOfSectors){

}





