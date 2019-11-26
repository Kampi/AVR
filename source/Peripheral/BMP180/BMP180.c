/*
 * BMP180.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for BMP180 I2C pressure sensor.

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

/** @file Peripheral/BMP180/BMP180.c
 *  @brief Driver for the Bosch BMP180 I2C pressure sensor.
 *
 *  This file contains the implementation of the pressure sensor driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/BMP180/BMP180.h"

/** @defgroup BMP180
 *  @{
 */
 	/** @defgroup BMP180-Register
	 *  BMP180 register addresses.
	 *  @{
	 */
		#define BMP180_REGISTER_AC1			0xAA
		#define BMP180_REGISTER_AC2			0xAC
		#define BMP180_REGISTER_AC3			0xAE
		#define BMP180_REGISTER_AC4			0xB0
		#define BMP180_REGISTER_AC5			0xB2
		#define BMP180_REGISTER_AC6			0xB4
		#define BMP180_REGISTER_B1			0xB6
		#define BMP180_REGISTER_B2			0xB8
		#define BMP180_REGISTER_MB			0xBA
		#define BMP180_REGISTER_MC			0xBC
		#define BMP180_REGISTER_MD			0xBE
		#define BMP180_REGISTER_ID			0xD0
		#define BMP180_REGISTER_RESET		0xE0
		#define BMP180_REGISTER_CTRL		0xF4
		#define BMP180_REGISTER_OUT_MSB		0xF6
		#define BMP180_REGISTER_OUT_LSB		0xF7
		#define BMP180_REGISTER_OUT_XLSB	0xF8
	/** @} */ // end of BMP180-Register

 	/** @defgroup BMP180-Commands
	 *  BMP180 commands.
	 *  @{
	 */
		#define BMP180_CMD_RESET			0xB6
		#define BMP180_CMD_TEMP_MEAS		0x2E
		#define BMP180_CMD_PRES_MEAS		0x34
	/** @} */ // end of BMP180-Commands

 	/** @defgroup BMP180-Control
	 *  BMP180 control bits.
	 *  @{
	 */
		#define BMP180_START_CONV			5
	/** @} */ // end of BMP180-Control
/** @} */ // end of BMP180

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#define BMP180_I2CM_INIT(Config)														I2CM_Init(Config)
	#define BMP180_I2CM_WRITEBYTE(Data, Stop)												I2CM_WriteByte(&BMP180_INTERFACE, BMP180_ADDRESS, Data, Stop)
	#define BMP180_I2CM_READBYTE(Data, Stop)												I2CM_ReadByte(&BMP180_INTERFACE, BMP180_ADDRESS, Data, Stop)
	#define BMP180_I2CM_WRITEBYTES(Bytes, Data, Stop)										I2CM_WriteBytes(&BMP180_INTERFACE, BMP180_ADDRESS, Bytes, Data, Stop)
	#define BMP180_I2CM_READBYTES(Bytes, Data, Stop)										I2CM_ReadBytes(&BMP180_INTERFACE, BMP180_ADDRESS, Bytes, Data, Stop)
#elif(MCU_ARCH == MCU_ARCH_AVR8)	 
	 #define BMP180_I2CM_INIT(Config)														I2CM_Init(Config)
	 #define BMP180_I2CM_WRITEBYTE(Data, Stop)												I2CM_WriteByte(BMP180_ADDRESS, Data, Stop)
	 #define BMP180_I2CM_READBYTE(Data, Stop)												I2CM_ReadByte(BMP180_ADDRESS, Data, Stop)
	 #define BMP180_I2CM_WRITEBYTES(Bytes, Data, Stop)										I2CM_WriteBytes(BMP180_ADDRESS, Bytes, Data, Stop)
	 #define BMP180_I2CM_READBYTES(Bytes, Data, Stop)										I2CM_ReadBytes(BMP180_ADDRESS, Bytes, Data, Stop)
#else
	 #error "Architecture not supported for BMP180!"
#endif

const I2C_Error_t BMP180_Init(I2CM_Config_t* Config)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {BMP180_REGISTER_ID, 0x00};

	// Initialize the i2c interface
	if(Config != NULL)
	{
		BMP180_I2CM_INIT(Config);
	}

	// Read the ID register
	ErrorCode = BMP180_I2CM_WRITEBYTE(Data[0], FALSE) | BMP180_I2CM_READBYTE(&Data[1], TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	if(Data[1] != BMP180_ID)
	{
		return I2C_NO_DEVICE;
	}

	// Reset the device
	ErrorCode = BMP180_Reset();
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	return ErrorCode;
}

const I2C_Error_t BMP180_Reset(void)
{
	uint8_t Data[2] = {BMP180_REGISTER_RESET, BMP180_CMD_RESET};
	I2C_Error_t ErrorCode = BMP180_I2CM_WRITEBYTES(sizeof(Data), Data, TRUE);

	// Wait some time
	for(uint8_t i = 0x00; i < 0xFF; i++);

	return ErrorCode;
}

const I2C_Error_t BMP180_ReadCalibration(BMP180_CalibCoef_t* CalibCoef)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(CalibCoef == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	ErrorCode = BMP180_I2CM_WRITEBYTE(BMP180_REGISTER_AC1, FALSE) | BMP180_I2CM_READBYTES(22, (uint8_t*)CalibCoef, TRUE);

	#if(defined(MCU_LITTLE_ENDIAN))
		// Flip the bytes, because the sensor stores the data in big endian format
		uint16_t* Address = (uint16_t*)CalibCoef;
		for(uint8_t i = 0; i < 22; i = i + 2)
		{
			*Address = (*((uint8_t*)CalibCoef + i) << 0x08) + *((uint8_t*)CalibCoef + i + 1);
			Address++;
		}
	#endif

	return ErrorCode;
}

const I2C_Error_t BMP180_MeasureTemperature(BMP180_DataPoint_t* DataPoint)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {BMP180_REGISTER_CTRL, 0x00};

	if(DataPoint == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Start the temperature measurement
	Data[1] = BMP180_CMD_TEMP_MEAS;
	ErrorCode = BMP180_I2CM_WRITEBYTES(2, Data, TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Wait for measurement end
	do 
	{
		ErrorCode = BMP180_I2CM_WRITEBYTE(Data[0], FALSE) | BMP180_I2CM_READBYTE(&Data[1], TRUE);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	} while(Data[1] & (0x01 << BMP180_START_CONV));
	
	// Read the temperature
	ErrorCode = BMP180_I2CM_WRITEBYTE(BMP180_REGISTER_OUT_MSB, FALSE) | BMP180_I2CM_READBYTES(sizeof(Data), Data, TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	DataPoint->Temperature = (Data[0] << 0x08) + Data[1];

	return ErrorCode;
}

const I2C_Error_t BMP180_MeasurePressure(const BMP180_OSS_t OSS, BMP180_DataPoint_t* DataPoint)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[3] = {BMP180_REGISTER_CTRL, 0x00, 0x00};

	if(DataPoint == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Start the temperature measurement
	Data[1] = BMP180_CMD_PRES_MEAS + (OSS << 0x06);
	ErrorCode = BMP180_I2CM_WRITEBYTES(2, Data, TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Wait for measurement end
	do 
	{
		ErrorCode = BMP180_I2CM_WRITEBYTE(Data[0], FALSE) | BMP180_I2CM_READBYTE(&Data[1], TRUE);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	} while(Data[1] & (0x01 << BMP180_START_CONV));
	
	// Read the temperature
	ErrorCode = BMP180_I2CM_WRITEBYTE(BMP180_REGISTER_OUT_MSB, FALSE) | BMP180_I2CM_READBYTES(sizeof(Data), Data, TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	DataPoint->Pressure = ((((uint32_t)Data[0]) << 0x10) + (((uint32_t)Data[1]) << 0x08) + ((uint32_t)Data[2])) >> (0x08 - OSS);

	return ErrorCode;
}

const I2C_Error_t BMP180_SingleMeasurement(const BMP180_OSS_t OSS, const BMP180_CalibCoef_t* CalibCoef, BMP180_DataPoint_t* DataPoint)
{
	uint32_t B4, B7;
	int32_t X1, X2, X3, B3, B5, B6, p;

	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if((CalibCoef == NULL) || (DataPoint == NULL))
	{
		return I2C_INVALID_PARAM;
	}

	ErrorCode = BMP180_MeasureTemperature(DataPoint) | BMP180_MeasurePressure(OSS, DataPoint);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Calibrate the temperature (see the official datasheet for more information)
	X1 = (((int32_t)DataPoint->Temperature - CalibCoef->AC6) * CalibCoef->AC5) >> 15;
	X2 = ((int32_t)CalibCoef->MC << 11) / (X1 + CalibCoef->MD);
	B5 = X1 + X2;
	DataPoint->Temperature = ((float)B5 + 8) / 160;

	// Calibrate the pressure (see the official datasheet for more information)
	B6 = B5 - 4000;
	X1 = (CalibCoef->B2 * ((B6 * B6) >> 12)) >> 11;
	X2 = (CalibCoef->AC2 * B6) >> 11;
	X3 = X1 + X2;
	B3 = (((((int32_t)CalibCoef->AC1 << 2) + X3) << OSS) + 2) >> 2;
	X1 = (CalibCoef->AC3 * B6) >> 13;
	X2 = CalibCoef->B1 * ((B6 * B6) >> 12) >> 16;
	X3 = (X1 + X2 + 2) >> 2;
	B4 = (CalibCoef->AC4 * (uint32_t)(X3 + 32768)) >> 15;
	B7 = (DataPoint->Pressure - B3) * (50000 >> OSS);

	if(B7 < 0x80000000)
	{
		p = (B7 << 1) / B4;	
	}
	else
	{
		p = (B7 / B4) << 1;
	}

	X1 = p >> 8;
	X1 *= X1;
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p = p + ((X1 + X2 + 3791) >> 4);

	DataPoint->Pressure = p;

	return ErrorCode;
}