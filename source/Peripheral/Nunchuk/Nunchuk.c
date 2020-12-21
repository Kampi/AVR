/*
 * Nunchuk.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the a Nintendo Nunchuk.

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

/** @file Peripheral/Nunchuk/Nunchuk.c
 *  @brief Driver for NXP Nunchuk I2C PWM controller.
 *
 *  This file contains the implementation of the Nunchuk driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/Nunchuk/Nunchuk.h"

/** @defgroup Nunchuk
 *  @{
 */
	/** @defgroup Nunchuk-Register
	 *  Nunchuk register addresses.
	 *  @{
	 */
		#define NUNCHUK_REGISTER_JOYSTICK_X			0x00
		#define NUNCHUK_REGISTER_JOYSTICK_Y			0x01
		#define NUNCHUK_REGISTER_ACC_X				0x02
		#define NUNCHUK_REGISTER_ACC_Y				0x03
		#define NUNCHUK_REGISTER_ACC_Z				0x04
		#define NUNCHUK_REGISTER_ACC_BUTTON			0x05
	/** @} */ // end of Nunchuk-Register
/** @} */ // end of Nunchuk

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#define NUNCHUK_I2CM_INIT(Config)														I2CM_Init(Config)
	#define NUNCHUK_I2CM_WRITEBYTE(Data, Stop)												I2CM_WriteByte(&NUNCHUK_INTERFACE, NUNCHUK_ADDRESS, Data, Stop)
	#define NUNCHUK_I2CM_READBYTE(Data, Stop)												I2CM_ReadByte(&NUNCHUK_INTERFACE, NUNCHUK_ADDRESS, Data, Stop)
	#define NUNCHUK_I2CM_WRITEBYTES(Length, Data, Stop)										I2CM_WriteBytes(&NUNCHUK_INTERFACE, NUNCHUK_ADDRESS, Length, Data, Stop)
	#define NUNCHUK_I2CM_READBYTES(Length, Data, Stop)										I2CM_ReadBytes(&NUNCHUK_INTERFACE, NUNCHUK_ADDRESS, Length, Data, Stop)
#elif(MCU_ARCH == MCU_ARCH_AVR8)
	#define NUNCHUK_I2CM_INIT(Config)														I2CM_Init(Config)
	#define NUNCHUK_I2CM_WRITEBYTE(Data, Stop)												I2CM_WriteByte(NUNCHUK_ADDRESS, Data, Stop)
	#define NUNCHUK_I2CM_READBYTE(Data, Stop)												I2CM_ReadByte(NUNCHUK_ADDRESS, Data, Stop)
	#define NUNCHUK_I2CM_WRITEBYTES(Length, Data, Stop)										I2CM_WriteBytes(NUNCHUK_ADDRESS, Length, Data, Stop)
	#define NUNCHUK_I2CM_READBYTES(Length, Data, Stop)										I2CM_ReadBytes(NUNCHUK_ADDRESS, Length, Data, Stop)
#else
	#error "Architecture not supported for Nunchuk!"
#endif

const I2C_Error_t Nunchuk_Init(I2CM_Config_t* Config)
{
	uint8_t Temp[2];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(Config != NULL)
	{
		NUNCHUK_I2CM_INIT(Config);
	}

	// Send first initialization bytes
	Temp[0] = 0xF0;
	Temp[1] = 0x55;
	ErrorCode = NUNCHUK_I2CM_WRITEBYTES(sizeof(Temp), Temp, true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Send second initialization bytes
	Temp[0] = 0xFB;
	Temp[1] = 0x00;
	return NUNCHUK_I2CM_WRITEBYTES(sizeof(Temp), Temp, true);
}

const I2C_Error_t Nunchuk_Read(Nunchuk_Data_t* Data)
{
	uint8_t Temp[6];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(Data == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	ErrorCode = NUNCHUK_I2CM_WRITEBYTE(0x00, true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = NUNCHUK_I2CM_READBYTES(sizeof(Temp), Temp, true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Data->Button_Z = !((Temp[5] & (0x01 << 0x00)) >> 0x00);
	Data->Button_C = !((Temp[5] & (0x01 << 0x01)) >> 0x01);
	Data->Joy_X = (Temp[0] ^ 0x17) + 0x17;
	Data->Joy_Y = (Temp[1] ^ 0x17) + 0x17;
	Data->Acc_X = (((((int16_t)Temp[2]) << 0x02) | ((Temp[5] & (0x03 << 0x02)) >> 0x02)) ^ 0x17) + 0x17;
	Data->Acc_Y = (((((int16_t)Temp[3]) << 0x02) | ((Temp[5] & (0x03 << 0x04)) >> 0x04)) ^ 0x17) + 0x17;
	Data->Acc_Z = (((((int16_t)Temp[4]) << 0x02) | ((Temp[5] & (0x03 << 0x06)) >> 0x06)) ^ 0x17) + 0x17;

	return I2C_NO_ERROR;
}