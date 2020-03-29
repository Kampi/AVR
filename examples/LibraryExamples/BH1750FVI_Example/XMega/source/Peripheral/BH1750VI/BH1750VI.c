/*
 * BH1750VI.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the BH1750FVI I2C ambient light sensor.

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

/** @file Peripheral/BH1750VI/BH1750VI.c
 *  @brief Driver for the Rohm BH1750FVI I2C ambient light sensor.
 *
 *  This file contains the implementation of the ambient light sensor driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/BH1750FVI/BH1750FVI.h"

/** @defgroup BH1750VI
 *  @{
 */
	/** @defgroup BH1750VI-Commands
	 *  BH1750VI commands.
	 *  @{
	 */
		#define BH1750VI_CMD_PWR_DOWN				0x00
		#define BH1750VI_CMD_PWR_ON					0x01
		#define BH1750VI_CMD_RESET					0x07
		#define BH1750VI_CMD_CONT_MEASURE(Res)		(0x10 | (Res & 0x03))
		#define BH1750VI_CMD_MEASURE(Res)			(0x20 | (Res & 0x03))
		#define BH1750VI_CMD_MEAS_TIME_H(Time)		(0x40 | (Time & 0x07))
		#define BH1750VI_CMD_MEAS_TIME_L(Time)		(0x60 | (Time & 0x1F))
	/** @} */ // end of BH1750VI-Commands
/** @} */ // end of BH1750VI

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#define BH1750VI_I2CM_INIT(Config)														I2CM_Init(Config)
	#define BH1750VI_I2CM_WRITEBYTE(Data, Stop)												I2CM_WriteByte(&BH1750VI_INTERFACE, BH1750VI_ADDRESS, Data, Stop)
	#define BH1750VI_I2CM_READBYTE(Data, Stop)												I2CM_ReadByte(&BH1750VI_INTERFACE, BH1750VI_ADDRESS, Data, Stop)
	#define BH1750VI_I2CM_WRITEBYTES(Length, Data, Stop)									I2CM_WriteBytes(&BH1750VI_INTERFACE, BH1750VI_ADDRESS, Length, Data, Stop)
	#define BH1750VI_I2CM_READBYTES(Length, Data, Stop)										I2CM_ReadBytes(&BH1750VI_INTERFACE, BH1750VI_ADDRESS, Length, Data, Stop)
	#define BH1750VI_RESET()																I2CM_WriteByte(&BH1750VI_INTERFACE, 0x00, 0x03, true)
#elif(MCU_ARCH == MCU_ARCH_AVR8)
	#define BH1750VI_I2CM_INIT(Config)														I2CM_Init(Config)
	#define BH1750VI_I2CM_WRITEBYTE(Data, Stop)												I2CM_WriteByte(BH1750VI_ADDRESS, Data, Stop)
	#define BH1750VI_I2CM_READBYTE(Data, Stop)												I2CM_ReadByte(BH1750VI_ADDRESS, Data, Stop)
	#define BH1750VI_I2CM_WRITEBYTES(Length, Data, Stop)									I2CM_WriteBytes(BH1750VI_ADDRESS, Length, Data, Stop)
	#define BH1750VI_I2CM_READBYTES(Length, Data, Stop)										I2CM_ReadBytes(BH1750VI_ADDRESS, Length, Data, Stop)
	#define BH1750VI_RESET()																I2CM_WriteByte(0x00, 0x06, true)
#else
	#error "Architecture not supported for BH1750VI!"
#endif

static BH1750VI_DeviceMode_t __BH1750Mode;

const I2C_Error_t BH1750VI_Init(I2CM_Config_t* Config)
{	
	GPIO_SetDirection(GET_PERIPHERAL(BH1750VI_ADDR), GET_INDEX(BH1750VI_ADDR), GPIO_DIRECTION_OUT);

	// Set the address pin according to the address settings
	#if(defined(BH1750VI_USE_ADDR0))
		GPIO_Set(GET_PERIPHERAL(BH1750VI_ADDR), GET_INDEX(BH1750VI_ADDR));
	#elif(defined(BH1750VI_USE_ADDR1))
		GPIO_Clear(GET_PERIPHERAL(BH1750VI_ADDR), GET_INDEX(BH1750VI_ADDR));
	#endif

	if(Config != NULL)
	{
		BH1750VI_I2CM_INIT(Config);
	}
	
	return BH1750VI_SetMode(BH1750VI_MODE_NORMAL) | BH1750VI_Reset();
}

const I2C_Error_t BH1750VI_Reset(void)
{
	return BH1750VI_I2CM_WRITEBYTE(BH1750VI_CMD_RESET, true);
	for(uint16_t i = 0x00; i < 0xFFFF; i++);
}

const I2C_Error_t BH1750VI_SetMode(const BH1750VI_DeviceMode_t Mode)
{
	I2C_Error_t ErrorCode = BH1750VI_I2CM_WRITEBYTE(Mode & 0x01, true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
		
	__BH1750Mode = Mode;

	return I2C_NO_ERROR;
}

const I2C_Error_t BH1750VI_Adjust(const uint8_t Mtreg)
{
	return 	BH1750VI_CMD_MEAS_TIME_H(Mtreg) | BH1750VI_CMD_MEAS_TIME_L(Mtreg);
}

const BH1750VI_DeviceMode_t BH1750VI_GetMode(void)
{
	return __BH1750Mode;
}

const I2C_Error_t BH1750VI_SingleMeasurement(const BH1750VI_Resolution_t Resolution, uint16_t* Result)
{
	uint8_t Data[2] = {0x00, 0x00};

	I2C_Error_t ErrorCode = BH1750VI_I2CM_WRITEBYTE(BH1750VI_CMD_MEASURE(Resolution), true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Wait for end of conversion
	_delay_ms(120);
	
	ErrorCode = BH1750VI_I2CM_READBYTES(sizeof(Data), &Data[0], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Result = (Data[0] << 0x08) | Data[1];
	 
	return ErrorCode;
}


const I2C_Error_t BH1750VI_StartMeasurement(const BH1750VI_Resolution_t Resolution)
{
	return BH1750VI_SetMode(BH1750VI_MODE_NORMAL) | BH1750VI_I2CM_WRITEBYTE(BH1750VI_CMD_CONT_MEASURE(Resolution), true);
}

const I2C_Error_t BH1750VI_GetResult(uint16_t* Result)
{
	uint8_t Data[2] = {0x00, 0x00};

	I2C_Error_t ErrorCode = BH1750VI_I2CM_READBYTES(sizeof(Data), &Data[0], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Result = (Data[0] << 0x08) | Data[1];
	
	return ErrorCode;
}

const I2C_Error_t BH1750VI_StopMeasurement(void)
{
	return BH1750VI_SetMode(BH1750VI_MODE_PWR_DOWN);
}