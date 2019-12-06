/*
 * EEPROM_24CXXX.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel 24Cxxx I2C EEPROMs

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file EEPROM_24CXXX.c
 *  @brief Driver for Atmel 24Cxxx I2C EEPROMs.
 *
 *  This file contains the implementation of the 24er series EEPROM driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/EEPROM_24CXXX/EEPROM_24CXXX.h"

void EEPROM_24CXXX_Init(I2C_Config_t* Config)
{
	if(Config != NULL)
	{
		I2C_Init(Config);
	}
}

I2C_Error_t EEPROM_24CXXX_WriteByte(const uint16_t Address, const uint8_t Data)
{
	uint8_t Address_Low = Address & 0xFF;
	uint8_t Address_High = (Address & 0xFF) >> 0x08;
	
	if(I2C_SendStart() != I2C_START_SEND)
	{
		return I2C_START_ERROR;
	}

	if(I2C_SendByte(I2C_WRITE(EEPROM_24CXXX_ADDRESS)) != I2C_ADDR_ACK_W_RECEIVED)
	{
		return I2C_WRITE_ADDR_ERROR;
	}

	if(I2C_SendByte(Address_High) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}

	if(I2C_SendByte(Address_Low) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}

	if(I2C_SendByte(Data) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}

	I2C_SendStop();
	
	return I2C_NO_ERROR;
}

I2C_Error_t EEPROM_24CXXX_ReadByte(const uint16_t Address, uint8_t* Data)
{
	uint8_t Address_Low = Address & 0xFF;
	uint8_t Address_High = (Address & 0xFF) >> 0x08;
	
	if(I2C_SendStart() != I2C_START_SEND)
	{
		return I2C_START_ERROR;
	}

	if(I2C_SendByte(I2C_WRITE(EEPROM_24CXXX_ADDRESS)) != I2C_ADDR_ACK_W_RECEIVED)
	{
		return I2C_WRITE_ADDR_ERROR;
	}

	if(I2C_SendByte(Address_High) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}

	if(I2C_SendByte(Address_Low) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}

	if(I2C_SendStart() != I2C_REP_START_SEND)
	{
		return I2C_START_ERROR;
	}
	
	if(I2C_SendByte(I2C_READ(EEPROM_24CXXX_ADDRESS)) != I2C_ADDR_ACK_R_RECEIVED)
	{
		return I2C_READ_ADDR_ERROR;
	}

	*Data = I2C_ReadByte(FALSE);

	I2C_SendStop();
	
	return I2C_NO_ERROR;
}

I2C_Error_t EEPROM_24CXXX_WriteBytes(const uint16_t Address, const uint16_t Length, const uint8_t* Data)
{
	uint8_t Address_Low = Address & 0xFF;
	uint8_t Address_High = (Address & 0xFF) >> 0x08;
	
	// Write the address to the EEPROM
	if(I2C_SendStart() != I2C_START_SEND)
	{
		return I2C_START_ERROR;
	}

	if(I2C_SendByte(I2C_WRITE(EEPROM_24CXXX_ADDRESS)) != I2C_ADDR_ACK_W_RECEIVED)
	{
		return I2C_WRITE_ADDR_ERROR;
	}

	if(I2C_SendByte(Address_High) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}

	if(I2C_SendByte(Address_Low) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}

	// Write n bytes to the EEPROM
	for(uint8_t ByteCounter = 0x00; ByteCounter < Length; ByteCounter++)
	{
		if(I2C_SendByte(*(Data++)) != I2C_DATA_ACK_W_RECEIVED)
		{
			return I2C_DATA_ERROR;
		}		
	}
	
	I2C_SendStop();
	
	return I2C_NO_ERROR;
}

I2C_Error_t EEPROM_24CXXX_ReadBytes(const uint16_t Address, const uint16_t Length, uint8_t* Data)
{
	uint8_t Address_Low = Address & 0xFF;
	uint8_t Address_High = (Address & 0xFF) >> 0x08;
	
	if(I2C_SendStart() != I2C_START_SEND)
	{
		return I2C_START_ERROR;
	}

	if(I2C_SendByte(I2C_WRITE(EEPROM_24CXXX_ADDRESS)) != I2C_ADDR_ACK_W_RECEIVED)
	{
		return I2C_WRITE_ADDR_ERROR;
	}

	if(I2C_SendByte(Address_High) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}

	if(I2C_SendByte(Address_Low) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_DATA_ERROR;
	}
	
	if(I2C_SendStart() != I2C_REP_START_SEND)
	{
		return I2C_START_ERROR;
	}
	
	if(I2C_SendByte(I2C_READ(EEPROM_24CXXX_ADDRESS)) != I2C_ADDR_ACK_R_RECEIVED)
	{
		return I2C_READ_ADDR_ERROR;
	}

	for(uint8_t ByteCounter = 0x00; ByteCounter < (Length - 0x01); ByteCounter++)
	{
		*(Data++) = I2C_ReadByte(TRUE);
	}

	*Data = I2C_ReadByte(FALSE);

	I2C_SendStop();
	
	return I2C_NO_ERROR;
}