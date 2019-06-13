/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: NVM controller example for XMega.

  GNU GENERAL PUBLIC LICENSE:
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file main.c
 *  @brief NVM controller example for XMega.
 *			NOTE: This example is for XMega384C3. Check the begin of the bootloader section in the datasheet
 *				  if you use other devices and modify the linker flag (byte address!)
 *					-Wl,--section-start=.boot=0x060000
 *				  or change the memory settings in the project properties (word address!)
 *					.boot=0x030000
 *				  with the correct address.
 *
 *  Software for the XMega NVM controller tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-nvm/
 *
 *	This file contains several examples:
 *		1) Use the NVM controller to write some data to the internal EEPROM and read them back. Set symbol EXAMPLE = 1.
 *		2) Use the NVM controller to write some data to the user signature row and read them back. Set symbol EXAMPLE = 2.
 *		3) Use the NVM controller to read and write the application flash. Set symbol EXAMPLE = 3.
 *
 *  @author Daniel Kampert
 */

#include "NVM/NVM.h"

int main(void)
{
	#if(EXAMPLE == 1)
		#define EEPROM_PAGE_0				0x00
		#define EEPROM_PAGE_1				0x01
		#define EEPROM_OFFSET_0				0x00
		#define EEPROM_OFFSET_1				0x1F
		#define EEPROM_OFFSET_2				0x02

		/*
			Write some test data to the EEPROM
		*/
		NVM_EEPROMWriteByte(EEPROM_PAGE_0, EEPROM_OFFSET_0, 0x01);
		NVM_EEPROMWriteByte(EEPROM_PAGE_0, EEPROM_OFFSET_1, 0x02);
		NVM_EEPROMWriteByte(EEPROM_PAGE_1, EEPROM_OFFSET_2, 0x03);

		/*
			Read the test data from the EEPROM
		*/
		uint8_t Data0 = NVM_EEPROMReadByte(EEPROM_PAGE_0, EEPROM_OFFSET_0);
		uint8_t Data1 = NVM_EEPROMReadByte(EEPROM_PAGE_0, EEPROM_OFFSET_1);
		uint8_t Data2 = NVM_EEPROMReadByte(EEPROM_PAGE_1, EEPROM_OFFSET_2);

		// Create a new page buffer for the EEPROM
		uint8_t EEPROM_PageBufferOut[EEPROM_PAGE_SIZE];
		uint8_t EEPROM_PageBufferIn[EEPROM_PAGE_SIZE];
		for(uint8_t i = 0x00; i < EEPROM_PAGE_SIZE; i++)
		{
			EEPROM_PageBufferOut[i] = i;
		}

		NVM_EEPROMWritePage(1, EEPROM_PageBufferOut);
		NVM_EEPROReadPage(1, EEPROM_PageBufferIn);

	#elif(EXAMPLE == 2)
		#define USER_SIG_ROW_WORD_0			0x00
		#define USER_SIG_ROW_WORD_1			0x0A

		/*
			Clear the User Signature Row and write some new data 
		*/
		NVM_EraseUserSignature();
		NVM_FlashWriteWord(USER_SIG_ROW_WORD_0, 0x1001);
		NVM_FlashWriteWord(USER_SIG_ROW_WORD_1, 0x0110);
		NVM_FlushUserSignature();
		NVM_WaitBusy();

		/*
			Read the User Signature Row
		*/
		uint16_t Data0 = NVM_UserSignatureReadWord(USER_SIG_ROW_WORD_0);
		uint16_t Data1 = NVM_UserSignatureReadWord(USER_SIG_ROW_WORD_1);

		/*
			Clear the User Signature Row and write a new page
		*/
		// Create a new field for the user signature
		uint16_t UserSignature_Read[APP_SECTION_PAGE_SIZE / 2];
		uint16_t UserSignature_Write[APP_SECTION_PAGE_SIZE / 2];

		// Fill the write array
		for(uint16_t i = 0x00; i < (APP_SECTION_PAGE_SIZE / 2); i++)
		{
			UserSignature_Write[i] = i;
		}

		NVM_EraseUserSignature();
		NVM_FlashWritePage(UserSignature_Write);
		NVM_FlushUserSignature();
		NVM_WaitBusy();
		NVM_UserSignatureReadPage(UserSignature_Read);
		
	#elif(EXAMPLE == 3)

	#endif
	

	
    while(1) 
    {
    }
}