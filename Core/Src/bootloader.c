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
		case TEST_BOOTLOADER:{
			BL_Print_Message("Bootloader is ok \r\n");
			break;
		}
		default:{
			BL_Print_Message("wrong\r\n");
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
