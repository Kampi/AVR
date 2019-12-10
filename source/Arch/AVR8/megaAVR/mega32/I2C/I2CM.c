/*
 * I2C.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 I2C module master mode.

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

/** @file Arch/AVR8/megaAVR/I2C/I2C.c
 *  @brief Driver for AVR8 I2C module in master mode.
 *
 *  This contains the implementation of the AVR8 I2C master mode driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/AVR8/megaAVR/I2C/I2C.h"

/** @brief	Send a start condition.
 *  @return	I2C error code
 */
static I2C_Status_t I2CM_SendStart(void)
{
	TWCR = (0x01 << TWINT) | (0x01 << TWEN) | (0x01 << TWSTA);

	while(!(TWCR & (0x01 << TWINT)));

	return I2C_ReadStatus();
}

/** @brief		Send a single byte.
 *  @param Data	Data byte
 *  @return		I2C error code
 */
static I2C_Status_t I2CM_Send(const uint8_t Data)
{
	TWDR = Data;
	TWCR = (0x01 << TWINT) | (0x01 << TWEN);

	while(!(TWCR & (0x01 << TWINT)));

	return I2C_ReadStatus();
}

/** @brief		Read a single byte.
 *  @param NACK	#TRUE if a NACK should be send instead of an ACK
 *  @return		Data byte
 */
static uint8_t I2CM_Read(const Bool_t NACK)
{
	if(NACK == TRUE)
	{
		TWCR = (0x01 << TWINT) | (0x01 << TWEN);
	}
	else
	{
		TWCR = (0x01 << TWINT) | (0x01 << TWEN) | (0x01 << TWEA);
	}

	while(!(TWCR & (0x01 << TWINT)));

	return TWDR;
}

/** @brief	Send a stop condition.
 */
static void I2CM_SendStop(void)
{
	TWCR = (0x01 << TWINT) | (0x01 << TWEN) | (0x01 << TWSTO);

	// Wait a bit to reduce errors when a Start-Condition follows a Stop-Condition
	for(uint8_t Delay = 0x00; Delay < 0xFF; Delay++);
}

void I2CM_Init(I2CM_Config_t* Config)
{
	// Reset status register
	TWSR = 0x00;

	I2C_Reset();
	I2CM_SetPrescaler(Config->Prescaler);
	I2CM_SetBaudrate(Config->Bitrate, Config->Prescaler, SysClock_GetCPUClock());

	I2C_Enable();
}

void I2CM_SetBaudrate(const uint32_t Bitrate, const uint8_t Prescaler, const uint32_t Clock)
{
	uint8_t Exp = 0x01;

	for(uint8_t i = 0x00; i < Prescaler; i++)
	{
		Exp = Exp << 0x02;
	}

	TWBR = ((Clock / Bitrate) - 0x10) / (Exp << 0x01);
}

void I2C_EnablePullUp(const Bool_t Enable)
{
	if(Enable == TRUE)
	{
		GPIO_Set(TWI_PORT, TWI_SCL);
		GPIO_Set(TWI_PORT, TWI_SDA);
	}
	else
	{
		GPIO_Clear(TWI_PORT, TWI_SCL);
		GPIO_Clear(TWI_PORT, TWI_SDA);
	}
}

void I2CM_EnableInterruptSupport(void)
{
	// ToDo
}

void I2CM_DisableInterruptSupport(void)
{
	// ToDo
}

void I2CS_EnableInterruptSupport(void)
{
	// ToDo
}

void I2CS_DisableInterruptSupport(void)
{
	// ToDo
}

I2C_Error_t I2CM_WriteByte(const uint8_t Address, const uint8_t Data, const Bool_t Stop)
{
	I2C_Status_t Status = 0x00;
	
	Status = I2CM_SendStart();
	if((Status != I2C_START_SEND) & (Status != I2C_REP_START_SEND))
	{
		return I2C_START_ERROR;
	}

	if(I2CM_Send(I2C_WRITE(Address)) != I2C_ADDR_ACK_W_RECEIVED)
	{
		return I2C_WRITE_ADDR_ERROR;
	}

	if(I2CM_Send(Data) != I2C_DATA_ACK_W_RECEIVED)
	{
		return I2C_BUS_ERROR;
	}

	if(Stop == TRUE)
	{
		I2CM_SendStop();
	}

	return I2C_NO_ERROR;
}

I2C_Error_t I2CM_ReadByte(const uint8_t Address, uint8_t* Data, const Bool_t Stop)
{
	I2C_Status_t Status = 0x00;
	
	Status = I2CM_SendStart();
	if((Status != I2C_START_SEND) & (Status != I2C_REP_START_SEND))
	{
		return I2C_START_ERROR;
	}

	if(I2CM_Send(I2C_READ(Address)) != I2C_ADDR_ACK_R_RECEIVED)
	{
		return I2C_READ_ADDR_ERROR;
	}

	*Data = I2CM_Read(TRUE);

	if(Stop == TRUE)
	{
		I2CM_SendStop();
	}

	return I2C_NO_ERROR;
}

I2C_Error_t I2CM_WriteBytes(const uint8_t Address, const uint8_t Length, const uint8_t* Data, const Bool_t Stop)
{	
	I2C_Status_t Status = 0x00;
	
	Status = I2CM_SendStart();
	if((Status != I2C_START_SEND) & (Status != I2C_REP_START_SEND))
	{
		return I2C_START_ERROR;
	}

	if(I2CM_Send(I2C_WRITE(Address)) != I2C_ADDR_ACK_W_RECEIVED)
	{
		return I2C_WRITE_ADDR_ERROR;
	}

	for(uint8_t i = 0x00; i < Length; i++)
	{
		if(I2CM_Send(*Data++) != I2C_DATA_ACK_W_RECEIVED)
		{
			return I2C_BUS_ERROR;
		}
	}

	if(Stop == TRUE)
	{
		I2CM_SendStop();
	}

	return I2C_NO_ERROR;
}

I2C_Error_t I2CM_ReadBytes(const uint8_t Address, const uint8_t Length, uint8_t* Data, const Bool_t Stop)
{
	I2C_Status_t Status = 0x00;
	
	Status = I2CM_SendStart();
	if((Status != I2C_START_SEND) & (Status != I2C_REP_START_SEND))
	{
		return I2C_START_ERROR;
	}

	if(I2CM_Send(I2C_READ(Address)) != I2C_ADDR_ACK_R_RECEIVED)
	{
		return I2C_READ_ADDR_ERROR;
	}

	for(uint8_t i = 0x00; i < (Length - 0x01); i++)
	{
		*Data++ = I2CM_Read(FALSE);
	}

	*Data = I2CM_Read(TRUE);

	if(Stop == TRUE)
	{
		I2CM_SendStop();
	}

	return I2C_NO_ERROR;
}