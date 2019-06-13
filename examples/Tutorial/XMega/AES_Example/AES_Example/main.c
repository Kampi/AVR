/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AES example for XMega.

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
 *  @brief AES example for XMega. This example implement the EBC and the CBC mode
 *  with the XMega AES module.
 *
 *  Software for the XMega AES tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-aes/
 *
 *  @author Daniel Kampert
 */

#include "AES/AES.h"

#define BLOCK_COUNT							3

/*
	Keys
*/
uint8_t Subkey[AES_DATASIZE];
uint8_t Key[AES_KEYSIZE] = {
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
	Input for ECB mode
*/
uint8_t DataBlock_ECB[AES_DATASIZE] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
	Input for CBC mode
*/
uint8_t InitVector[AES_DATASIZE] = {
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t DataBlock_CBC[AES_DATASIZE * BLOCK_COUNT] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,	0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
	Output
*/
uint8_t EncryptedData_ECB[AES_DATASIZE];
uint8_t EncryptedData_CBC[AES_DATASIZE * BLOCK_COUNT];

int main(void)
{
	uint8_t Result = 0x00;
	
	Result = AES_GenerateLastSubkey(Key, Subkey);
	
	/*
		Run ECB-Mode
	*/
	Result = AES_Encrypt(DataBlock_ECB, EncryptedData_ECB, Key);
	Result = AES_Decrypt(EncryptedData_ECB, DataBlock_ECB, Subkey);
	
	/*
		Run CBC-Mode
	*/
	Result = AES_CBC_Encrypt(DataBlock_CBC, EncryptedData_CBC, Key, InitVector, BLOCK_COUNT);
	Result = AES_CBC_Decrypt(EncryptedData_CBC, DataBlock_CBC, Subkey, InitVector, BLOCK_COUNT);

    while(1) 
    {
    }
}