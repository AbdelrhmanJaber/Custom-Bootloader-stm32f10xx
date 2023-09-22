/*
 * bootloader.c
 *
 *  Created on: Sep 12, 2023
 *      Author: Abdelrahman Ibrahim
 */


#include"STD_Types.h"

#include"bootloader_private.h"

#include"bootloader_config.h"

#include"bootloader.h"

/****************** static function declaration ******************/

static BL_Status Bootloader_Get_Version(uint8_t *copy_Puint8_hostBuffer);
static BL_Status Bootloader_Get_Help(uint8_t *copy_Puint8_hostBuffer);
static BL_Status Bootloader_Get_Chip_Identification_Number(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Read_Protection_Level(uint8_t *copy_Puint8_hostBuffer);
static BL_Status Bootloader_Jump_To_Address(uint8_t *copy_Puint8_hostBuffer);
static BL_Status Bootloader_Erase_Flash(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Memory_Write(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Enable_RW_Protection(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Memory_Read(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Get_Sector_Protection_Status(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Read_OTP(uint8_t *copy_Puint8_hostBuffer);
static void Bootloader_Change_Read_Protection_Level(uint8_t *copy_Puint8_hostBuffer);
static BL_Status Bootloader_CRC_Verify(uint8_t *copy_Puint8_Data, uint32_t copy_uint32_dataLenght,
		uint32_t copy_uint32_hostCRC);
static void Bootloader_Send_ACK(uint8_t copy_uint8_replayLenght);
static void Bootloader_Send_NACK(void);
static void Bootloader_Send_Data_To_Host(uint8_t *copy_Puint8_hostBuffer, uint32_t copy_uint32_dataLenght);
static uint8_t Host_Address_Verification(uint32_t copy_uint32_jumpAddress);
static uint8_t Perform_Flash_Erase(uint8_t copy_uint8_pageNumber, uint8_t copy_uint8_numberOfPages);

/************************** variables and structures definition***************************************/

static uint8_t Bootloader_Supported_CMDs[12] = {
    BL_GET_VER_CMD,
    BL_GET_HELP_CMD,
    BL_GET_CID_CMD,
    BL_GET_RDP_STATUS_CMD,
    BL_GO_TO_ADDR_CMD,
    BL_FLASH_ERASE_CMD,
    BL_MEM_WRITE_CMD,
    BL_ED_W_PROTECT_CMD,
    BL_MEM_READ_CMD,
    BL_READ_SECTOR_STATUS_CMD,
    BL_OTP_READ_CMD,
    BL_CHANGE_ROP_Level_CMD
};


static uint8_t hostBuffer[MAX_HOST_COMMAND_LENGHT];

/****************** Macros  ******************/

#define  TEST_BOOTLOADER     0x10


static void test(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
}

void BL_fetchHostCommand(void){
	uint8_t dataLenght = 0 ;
	BL_Status status = BL_STATUS_NOK;
	memset(hostBuffer , 0 , MAX_HOST_COMMAND_LENGHT);
	HAL_UART_Receive(BL_hostCommunicationUART, hostBuffer, 1, HAL_MAX_DELAY);
	/*get length of the command*/
	dataLenght = hostBuffer[0];
	/*fetch the command from the user*/
	HAL_UART_Receive(BL_hostCommunicationUART, &hostBuffer[1], dataLenght, HAL_MAX_DELAY);
	//HAL_UART_Receive(BL_hostCommunicationUART, &hostBuffer[1], dataLenght, HAL_MAX_DELAY);
	switch(hostBuffer[1]){
		case BL_GET_VER_CMD:{
			status = Bootloader_Get_Version(hostBuffer);
			if(status == BL_STATUS_NOK) BL_Print_Message("Error GET Version\r\n");
			break;
		}
		case BL_GET_HELP_CMD:{
			status = Bootloader_Get_Help(hostBuffer);
			if(status == BL_STATUS_NOK) BL_Print_Message("Error GET HELP CMD \r\n");
			break;
		}
		case BL_GET_CID_CMD:{
			status = Bootloader_Get_Chip_Identification_Number(hostBuffer);
			if(status == BL_STATUS_NOK) BL_Print_Message("Error GET HELP CMD \r\n");
			break;
		}
		case BL_GET_RDP_STATUS_CMD:{
			BL_Print_Message("CBL_GET_RDP_STATUS_CMD \r\n");
			break;
		}
		case BL_GO_TO_ADDR_CMD:{
			status = Bootloader_Jump_To_Address(hostBuffer);
			if(status == BL_STATUS_NOK) BL_Print_Message("Error GET HELP CMD \r\n");
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
	test();
	char Messsage[100] = {0};
	va_list args;
	va_start(args, format);
	vsprintf(Messsage, format, args);
	HAL_UART_Transmit(BL_hostCommunicationUART, (uint8_t *)Messsage, sizeof(Messsage), HAL_MAX_DELAY);
	va_end(args);
}


/************************************ Implement static functions*********************************/



static void Bootloader_Send_Data_To_Host(uint8_t *copy_Puint8_hostBuffer, uint32_t copy_uint32_dataLenght){
	HAL_UART_Transmit(BL_hostCommunicationUART, copy_Puint8_hostBuffer, copy_uint32_dataLenght, HAL_MAX_DELAY);
}

static void Bootloader_Send_ACK(uint8_t copy_uint8_replayLenght){
	uint8_t sendACK[2];
	sendACK[0] = BL_ACK;
	sendACK[1] = copy_uint8_replayLenght;
	/*send ACK to the host then the number of bytes that host wait*/
	Bootloader_Send_Data_To_Host((uint8_t*)sendACK , 2);
}

static void Bootloader_Send_NACK(void){
	uint8_t sendNack = BL_NACK;
	Bootloader_Send_Data_To_Host(&sendNack , 1);
}


static BL_Status Bootloader_CRC_Verify(uint8_t *copy_Puint8_Data, uint32_t copy_uint32_dataLenght,
		uint32_t copy_uint32_hostCRC){
	BL_Status status = BL_CRC_NOK;
	uint32_t CRC_value = 0;
	uint32_t bufferData;
	/*calculate CRC*/
	for(uint8_t counter = 0 ; counter < copy_uint32_dataLenght ; counter++){
		bufferData = (uint32_t)copy_Puint8_Data[counter];
		CRC_value = HAL_CRC_Accumulate(CRC_REG , &bufferData , 1);
	}
	__HAL_CRC_DR_RESET(CRC_REG);

	/*compare CRC calculated by CRC is send*/
	if(CRC_value == copy_uint32_hostCRC) status = BL_CRC_OK;
	return status;
}


static BL_Status Bootloader_Get_Version(uint8_t *copy_Puint8_hostBuffer){
	BL_Status status = BL_STATUS_NOK;
	/*define array that include version data*/
	uint8_t BL_version[4] = {BL_VENDOR_ID , BL_MAJOR_VERSION , BL_MINOR_VERSION , BL_PATCH_VERSION};
	uint8_t commandLenght;
	uint32_t CRC_hostValue = 0;
	commandLenght = copy_Puint8_hostBuffer[0] + 1;
	CRC_hostValue = *(uint32_t*)(copy_Puint8_hostBuffer + commandLenght - CRC_SIZE_BYTES);
	status = Bootloader_CRC_Verify(&copy_Puint8_hostBuffer[0], commandLenght - CRC_SIZE_BYTES
			, CRC_hostValue);
	if(status == BL_CRC_OK){
		status = BL_STATUS_OK;
		/*send ACK to the host*/
		Bootloader_Send_ACK(4);
		/*send the data to the host*/
		Bootloader_Send_Data_To_Host((uint8_t *)&BL_version[0] , 4);
	}else{
		status = BL_STATUS_NOK;
		Bootloader_Send_NACK();
	}
	return status;
}

static BL_Status Bootloader_Get_Help(uint8_t *copy_Puint8_hostBuffer){
	BL_Status status = BL_STATUS_NOK;
	uint8_t commandLenght;
	uint32_t CRC_hostValue = 0;
	commandLenght = copy_Puint8_hostBuffer[0] + 1;
	CRC_hostValue = *(uint32_t*)(copy_Puint8_hostBuffer + commandLenght - CRC_SIZE_BYTES);
	status = Bootloader_CRC_Verify(&copy_Puint8_hostBuffer[0], commandLenght - CRC_SIZE_BYTES
			, CRC_hostValue);
		if(status == BL_CRC_OK){
			status = BL_STATUS_OK;
			/*send ACK to the host*/
			Bootloader_Send_ACK(12);
			Bootloader_Send_Data_To_Host((uint8_t *)&Bootloader_Supported_CMDs[0] , 12);
		}else{
			status = BL_STATUS_NOK;
			Bootloader_Send_NACK();
		}
	return status;
}


static BL_Status Bootloader_Get_Chip_Identification_Number(uint8_t *copy_Puint8_hostBuffer){
	BL_Status status = BL_STATUS_NOK;
		uint8_t commandLenght;
		uint32_t CRC_hostValue = 0;
		uint16_t MCU_ID_Number = 0;
		commandLenght = copy_Puint8_hostBuffer[0] + 1;
		CRC_hostValue = *(uint32_t*)(copy_Puint8_hostBuffer + commandLenght - CRC_SIZE_BYTES);
		status = Bootloader_CRC_Verify(&copy_Puint8_hostBuffer[0], commandLenght - CRC_SIZE_BYTES
				, CRC_hostValue);
		MCU_ID_Number = (uint16_t)((DBGMCU->IDCODE) & 0x00000FFF);
			if(status == BL_CRC_OK){
				status = BL_STATUS_OK;
				/*send ACK to the host*/
				Bootloader_Send_ACK(2);
				Bootloader_Send_Data_To_Host((uint8_t *)&MCU_ID_Number, 2);
			}else{
				status = BL_STATUS_NOK;
				Bootloader_Send_NACK();
			}
		return status;
}


static uint8_t Host_Address_Verification(uint32_t copy_uint32_jumpAddress){
	uint8_t address_verification = ADDRESS_IS_INVALID;
	if(copy_uint32_jumpAddress >= FLASH_LOWER && copy_uint32_jumpAddress <= FLASH_UPPER){
		address_verification = ADDRESS_IS_VALID;
	}
	else if(copy_uint32_jumpAddress >= RAM_LOWER && copy_uint32_jumpAddress <= RAM_UPPER){
		address_verification = address_verification;
	}
	return address_verification;
}


static BL_Status Bootloader_Jump_To_Address(uint8_t *copy_Puint8_hostBuffer){
	BL_Status status = BL_STATUS_NOK;
	uint8_t address_verification = ADDRESS_IS_INVALID;
	uint8_t commandLenght;
	uint32_t CRC_hostValue = 0;
	uint32_t jumpAddress = 0;
	commandLenght = copy_Puint8_hostBuffer[0] + 1;
	CRC_hostValue = *(uint32_t*)(copy_Puint8_hostBuffer + commandLenght - CRC_SIZE_BYTES);
	status = Bootloader_CRC_Verify(&copy_Puint8_hostBuffer[0], commandLenght - CRC_SIZE_BYTES
				, CRC_hostValue);
	if(status == BL_CRC_OK){
		Bootloader_Send_ACK(1);
		jumpAddress = *((uint32_t *)&hostBuffer[2]);
		address_verification = Host_Address_Verification(jumpAddress);
		if(address_verification == ADDRESS_IS_VALID){
			Bootloader_Send_Data_To_Host( (uint8_t *)&address_verification , 1);
			/*JUMP TO THE ADDRESS*/
			/*CASTING To pointer to function*/
			jumpAdressPtr addresToJumpPF = (jumpAdressPtr)(jumpAddress + 1);
			addresToJumpPF();
			status = BL_STATUS_OK;
		}
		else{
			Bootloader_Send_Data_To_Host( (uint8_t *)&address_verification , 1);
		}
	}
	else{
		status = BL_STATUS_NOK;
		Bootloader_Send_NACK();
	}
	return status;
}


static uint8_t Perform_Flash_Erase(uint8_t copy_uint8_pageNumber, uint8_t copy_uint8_numberOfPages){
	/*check if valid page number*/
	/*check if user want to erase all the flash or pages*/
	/*init flash erasing*/
	/*unlock flash*/
	/*erasing flash*/
	/*lock flash*/
}

static BL_Status Bootloader_Erase_Flash(uint8_t *copy_Puint8_hostBuffer){
	BL_Status status = BL_STATUS_NOK ;
	uint8_t commandLenght ;
	uint32_t CRC_hostValue = 0 ;
	uint8_t eraseStatus = UNSUCCESSFUL_ERASE ;
	commandLenght = copy_Puint8_hostBuffer[0] + 1 ;
	CRC_hostValue = *(uint32_t*)(copy_Puint8_hostBuffer + commandLenght - CRC_SIZE_BYTES) ;
	status = Bootloader_CRC_Verify(&copy_Puint8_hostBuffer[0], commandLenght - CRC_SIZE_BYTES
				, CRC_hostValue) ;
	if(status == BL_CRC_OK){
		Bootloader_Send_ACK(1);
		/*erase flash*/
		eraseStatus = Perform_Flash_Erase(hostBuffer[2] , hostBuffer[3]);
		if(eraseStatus == SUCCESSFUL_ERASE)   status = BL_STATUS_OK;
	    else   status = BL_STATUS_NOK;
		 Bootloader_Send_Data_To_Host((uint8_t *)&eraseStatus , 1);
	}
	else{
		status = BL_STATUS_NOK;
		Bootloader_Send_NACK();
	}
	return status;
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



static void Bootloader_Read_Protection_Level(uint8_t *copy_Puint8_hostBuffer){

}


