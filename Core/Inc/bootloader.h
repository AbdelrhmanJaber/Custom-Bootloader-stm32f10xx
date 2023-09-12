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



#define BL_hostCommunicationUART    &huart1


void BL_Print_Message(char *format, ...);

void BL_fetchHostCommand(void);


#endif /* SRC_BOOTLOADER_H_ */
