/*
 * MLX90393.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Melexis MLX90393 I2C magnetic sensor.

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

/** @file Peripheral/MLX90393/MLX90393.c
 *  @brief Driver for Melexis MLX90393 I2C magnetic sensor.
 *
 *  This file contains the implementation of the functions for the Melexis MLX90393 I2C driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/MLX90393/MLX90393.h"

/** @defgroup MLX90393
 *  @{
 */
 	/** @defgroup MLX90393-Register
	 *  AD5933 register addresses.
	 *  @{
	 */
		#define MLX90393_REGISTER_0						0x00
		#define MLX90393_REGISTER_1						0x01
		#define MLX90393_REGISTER_2						0x02
		#define MLX90393_REGISTER_3						0x03
		#define MLX90393_REGISTER_4						0x04
		#define MLX90393_REGISTER_5						0x05
		#define MLX90393_REGISTER_6						0x06
		#define MLX90393_REGISTER_7						0x07
		#define MLX90393_REGISTER_8						0x08
		#define MLX90393_REGISTER_9						0x09
	/** @} */ // end of MLX90393-Register
	 
 	/** @defgroup MLX90393-Commands
	 *  MLX90393 command codes without parameter.
	 *  @{
	 */
		#define MLX30393_CMD_START_BURST(Mask)			(0x10 | Mask)
		#define MLX30393_CMD_START_WOC(Mask)			(0x20 | Mask)
		#define MLX30393_CMD_START_SINGLE(Mask)			(0x30 | Mask)
		#define MLX30393_CMD_READ_MEAS(Mask)			(0x40 | Mask)
		#define MLX30393_CMD_READ_REG					0x50
		#define MLX30393_CMD_WRITE_REG					0x60
		#define MLX30393_CMD_EXIT						0x80
		#define MLX30393_CMD_MEM_RECALL					0xD0
		#define MLX30393_CMD_MEM_STORE					0x70
		#define MLX30393_CMD_RESET						0xF0
	/** @} */ // end of MLX90393-Commands
	
 	/** @defgroup MLX90393-Control
	 *  Position of MLX90393 control bits.
	 *  @{
	 */
		#define MLX90393_BIST							0x00
		#define MLX90393_Z_SERIES						0x07
		#define MLX90393_GAIN_2							0x06
		#define MLX90393_GAIN_1							0x05
		#define MLX90393_GAIN_0							0x04
		#define MLX90393_HALLCONF_0						0x00
		#define MLX90393_HALLCONF_1						0x01
		#define MLX90393_HALLCONF_2						0x02
		#define MLX90393_HALLCONF_3						0x03
		
		#define MLX90393_BURST_DATA_RATE_0				0x00
		#define MLX90393_BURST_DATA_RATE_1				0x01
		#define MLX90393_BURST_DATA_RATE_2				0x02
		#define MLX90393_BURST_DATA_RATE_3				0x03
		#define MLX90393_BURST_DATA_RATE_4				0x04
		#define MLX90393_BURST_DATA_RATE_5				0x05
		#define MLX90393_BURST_SEL_0					0x06
		#define MLX90393_BURST_SEL_1					0x07
		#define MLX90393_BURST_SEL_2					0x00
		#define MLX90393_BURST_SEL_3					0x01
		#define MLX90393_TCMP_EN						0x02
		#define MLX90393_EXT_TRIG						0x03
		#define MLX90393_WOC_DIFF						0x04
		#define MLX90393_COMM_MODE_0					0x05
		#define MLX90393_COMM_MODE_1					0x06
		#define MLX90393_TRIGG_INT						0x07
		
		#define MLX90393_OSR_3							0x04
		#define MLX90393_OSR_2							0x03
		#define MLX90393_RES_X_1						0x02
		#define MLX90393_RES_X_0						0x01
		#define MLX90393_RES_Y_1						0x00
		#define MLX90393_RES_Y_0						0x07
		#define MLX90393_RES_Z_1						0x06
		#define MLX90393_RES_Z_0						0x05
		#define MLX90393_DIGFILT_2						0x04
		#define MLX90393_DIGFILT_1						0x03
		#define MLX90393_DIGFILT_0						0x02
		#define MLX90393_OSR_1							0x01
		#define MLX90393_OSR_0							0x00
	/** @} */ // end of MLX90393-Control
/** @} */ // end of MLX90393

/** @brief			Write a register of the device.
 *  @param Address	Register address
 *  @param Data		Register data
 *  @param Status	Pointer to status answer
 *  @return			Error code
 */
static MLX90393_COMM_ERROR MLX90393_WriteRegister(const uint8_t Address, const uint16_t Data, uint8_t* Status)
{
	#if(MLX90393_INTERFACE_TYPE == INTERFACE_I2C)
		uint8_t Tx[] = {MLX30393_CMD_WRITE_REG, (Data >> 0x08), (Data & 0xFF), ((Address & 0x3F) << 0x02)};

		return (I2CM_WRITEBYTES(MLX90393_INTERFACE, MLX90393_ADDRESS, sizeof(Tx), Tx, FALSE) || I2CM_READBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, Status, TRUE));

	#endif
}

/** @brief			Read a register from the device.
 *  @param Address	Register address
 *  @param Data		Pointer to status and register data
 *  @return			Error code
 */
static MLX90393_COMM_ERROR MLX90393_ReadRegister(const uint8_t Address, uint8_t* Data)
{
	#if(MLX90393_INTERFACE_TYPE == INTERFACE_I2C)
		uint8_t Tx[] = {MLX30393_CMD_READ_REG, ((Address & 0x3F) << 0x02)};

		I2C_Error_t ErrorCode = I2CM_WRITEBYTES(MLX90393_INTERFACE, MLX90393_ADDRESS, sizeof(Tx), Tx, FALSE);
		
		// ToDo: Add timeout
		if(ErrorCode == I2C_BUS_ERROR)
		{
			//(I2CM_READBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, NULL, TRUE));
			//ErrorCode = I2CM_WRITEBYTES(MLX90393_INTERFACE, MLX90393_ADDRESS, sizeof(Tx), Tx, FALSE);
		}
		
		ErrorCode = I2CM_READBYTES(MLX90393_INTERFACE, MLX90393_ADDRESS, 3, Data, TRUE);

		return ErrorCode;
	#endif
}

/** @brief		Switch the device into measurement mode.
 *  @param Mode	Measurement mode
 *  @return		Error code
 */
static MLX90393_COMM_ERROR MLX90393_SetMeasurementMode(const uint8_t Mode)
{
	#if(MLX90393_INTERFACE_TYPE == INTERFACE_I2C)
		return (I2CM_WRITEBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, Mode, TRUE) || I2CM_READBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, NULL, TRUE));

	#elif((MLX90393_INTERFACE_TYPE == INTERFACE_USART_SPI) | (MLX90393_INTERFACE_TYPE == INTERFACE_SPI))
		// Todo
	#else
		#error "Invalid interface for MXL90393!"
	#endif
}

/** @brief		Set the device mode.
 *  @param Mode	Device mode
 *  @return		Error code
 */
static const MLX90393_COMM_ERROR MLX90393_SetMode(const MLX90393_Mode_t Mode)
{
	uint8_t Data[3];

	I2C_Error_t ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_1, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Clear old mode bits
	Data[1] &= ~((0x01 << MLX90393_COMM_MODE_1) | (0x01 << MLX90393_COMM_MODE_0));

	// Set new mode
	Data[1] |= (Mode << MLX90393_COMM_MODE_0);

	return MLX90393_WriteRegister(MLX90393_REGISTER_1, (Data[1] << 0x08) | Data[2], NULL);
}

const MLX90393_COMM_ERROR MLX90393_Init(I2CM_Config_t* Config, const MLX90393_Config_t* Device)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	GPIO_SetDirection(MLX90393_DRDY_PORT, MLX90393_DRDY_PIN, GPIO_DIRECTION_IN);

	if(Config != NULL)
	{
		I2CM_INIT(Config);
	}

	// Read old data
	if(GPIO_Read(MLX90393_DRDY_PORT, MLX90393_DRDY_PIN))
	{
		ErrorCode = MLX90393_ReadMeasurementData(MLX90393_AXIS_Z | MLX90393_AXIS_Y | MLX90393_AXIS_X | MLX90393_AXIS_T, 9, NULL);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}

	//ErrorCode = MLX90393_SetMode(MLX90393_MODE_I2C);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = (MLX90393_SwitchSelfTest(FALSE) | MLX90393_SetGain(Device->Gain) | MLX90393_SetHallConfig(Device->HallConfig) | MLX90393_SwitchZSeries(FALSE));
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	return (MLX90393_SetDigFilt(Device->DigFilt) | MLX90393_SetOSR(Device->Oversampling) | MLX90393_SetResolution(MLX90393_AXIS_X | MLX90393_AXIS_Y | MLX90393_AXIS_Z, Device->Resolution));
}

// ToDo: Funktioniert noch nicht richtig
const MLX90393_COMM_ERROR MLX90393_Reset(uint8_t* Status)
{
	#if(MLX90393_INTERFACE_TYPE == INTERFACE_I2C)
		return (I2CM_WRITEBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, MLX30393_CMD_RESET, FALSE) || I2CM_READBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, Status, TRUE));
	#endif
}

const MLX90393_COMM_ERROR MLX90393_SwitchZSeries(const Bool_t Enable)
{
	uint8_t Status;
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_0, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	if(Enable == TRUE)
	{
		Data[2] |= (0x01 << MLX90393_Z_SERIES);
	}
	else
	{
		Data[2] &= ~(0x01 << MLX90393_Z_SERIES);
	}

	return MLX90393_WriteRegister(MLX90393_REGISTER_0, (Data[1] << 0x08) | Data[2], &Status);
}

const MLX90393_COMM_ERROR MLX90393_SwitchSelfTest(const Bool_t Enable)
{
	uint8_t Status;
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_0, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	if(Enable == TRUE)
	{
		Data[1] |= (0x01 << MLX90393_BIST);
	}
	else
	{
		Data[1] &= ~(0x01 << MLX90393_BIST);
	}

	return MLX90393_WriteRegister(MLX90393_REGISTER_0, (Data[1] << 0x08) | Data[2], &Status);
}

const MLX90393_COMM_ERROR MLX90393_SetHallConfig(const MLX90393_HallConf_t Config)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode =	MLX90393_ReadRegister(MLX90393_REGISTER_0, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Clear hall config bits
	Data[2] &= ~((0x01 << MLX90393_HALLCONF_3) | (0x01 << MLX90393_HALLCONF_2) | (0x01 << MLX90393_HALLCONF_1) | (0x01 << MLX90393_HALLCONF_0));

	// Set new hall config
	Data[2] |= (Config.Duration << 0x02) | Config.Periods;

	return MLX90393_WriteRegister(MLX90393_REGISTER_0, (Data[1] << 0x08) | Data[2], NULL);
}

const MLX90393_COMM_ERROR MLX90393_GetHallConfig(MLX90393_HallConf_t* Config)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_0, Data);

	Config->Duration = (Data[2] >> 0x02);
	Config->Periods = Data[2];

	return ErrorCode;
}

const MLX90393_COMM_ERROR MLX90393_SetGain(const uint8_t Gain)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode =	MLX90393_ReadRegister(MLX90393_REGISTER_0, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Clear gain bits
	Data[2] &= ~((0x01 << MLX90393_GAIN_2) | (0x01 << MLX90393_GAIN_1) | (0x01 << MLX90393_GAIN_0));

	// Set new gain
	Data[2] |= (Gain & 0x07) << MLX90393_GAIN_0;
	
	return MLX90393_WriteRegister(MLX90393_REGISTER_0, (Data[1] << 0x08) | Data[2], NULL);
}

const MLX90393_COMM_ERROR MLX90393_GetGain(uint8_t* Gain)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_0, Data);
	
	// Get the gain
	*Gain = (Data[2] >> MLX90393_GAIN_0) & 0x07;
	
	return ErrorCode;
}

const MLX90393_COMM_ERROR MLX90393_SetDigFilt(const uint8_t Samples)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode =	MLX90393_ReadRegister(MLX90393_REGISTER_2, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Clear gain bits
	Data[2] &= ~((0x01 << MLX90393_DIGFILT_2) | (0x01 << MLX90393_DIGFILT_1) | (0x01 << MLX90393_DIGFILT_0));

	// Set new gain
	Data[2] |= (Samples & 0x07) << MLX90393_DIGFILT_0;
	
	return MLX90393_WriteRegister(MLX90393_REGISTER_2, (Data[1] << 0x08) | Data[2], NULL);
}

const MLX90393_COMM_ERROR MLX90393_GetDigFilt(uint8_t* Filter)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_2, Data);
	
	// Get the gain
	*Filter = (Data[2] >> MLX90393_DIGFILT_0) & 0x07;
	
	return ErrorCode;
}

const MLX90393_COMM_ERROR MLX90393_SetOSR(const uint8_t OSR)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode =	MLX90393_ReadRegister(MLX90393_REGISTER_2, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Clear OSR bits
	Data[2] &= ~((0x01 << MLX90393_OSR_1) | (0x01 << MLX90393_OSR_0));

	// Set new OSR bits
	Data[2] |= (OSR & 0x03);
	
	return MLX90393_WriteRegister(MLX90393_REGISTER_2, (Data[1] << 0x08) | Data[2], NULL);
}

const MLX90393_COMM_ERROR MLX90393_GetOSR(uint8_t* OSR)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_2, Data);
	
	// Get the gain
	*OSR = Data[2] & 0x03;
	
	return ErrorCode;
}

const MLX90393_COMM_ERROR MLX90393_SetOSR2(const uint8_t OSR)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode =	MLX90393_ReadRegister(MLX90393_REGISTER_2, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Clear OSR bits
	Data[1] &= ~((0x01 << MLX90393_OSR_3) | (0x01 << MLX90393_OSR_2));

	// Set new OSR bits
	Data[1] |= (OSR & 0x03) << MLX90393_OSR_2;
	
	return MLX90393_WriteRegister(MLX90393_REGISTER_2, (Data[1] << 0x08) | Data[2], NULL);
}

const MLX90393_COMM_ERROR MLX90393_GetOSR2(uint8_t* OSR)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_2, Data);
	
	// Get the gain
	*OSR = Data[2] & 0x03;
	
	return ErrorCode;
}

const MLX90393_COMM_ERROR MLX90393_SetResolution(const MLX90393_Axis_t Axis, const uint8_t Resolution)
{
	uint8_t Data[3];
	uint8_t Resolution_Temp = Resolution & 0x03;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode =	MLX90393_ReadRegister(MLX90393_REGISTER_2, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Clear resolution bits
	Data[2] &= ~((0x01 << MLX90393_RES_Y_0) | (0x01 << MLX90393_RES_Z_1) | (0x01 << MLX90393_RES_Z_0));
	Data[1] &= ~((0x01 << MLX90393_RES_X_1) | (0x01 << MLX90393_RES_X_0) | (0x01 << MLX90393_RES_Y_1));

	uint16_t Config = (Data[1] << 0x08) | Data[2];

	// Set new resolution
	if(Axis & MLX90393_AXIS_X)
	{
		Config |= Resolution_Temp << 0x09;
	}

	if(Axis & MLX90393_AXIS_Y)
	{
		Config |= Resolution_Temp << 0x07;
	}

	if(Axis & MLX90393_AXIS_Z)
	{
		Config |= Resolution_Temp << 0x05;
	}

	return MLX90393_WriteRegister(MLX90393_REGISTER_2, Config, NULL);
}

const MLX90393_COMM_ERROR MLX90393_GetResolution(const MLX90393_Axis_t Axis, uint8_t* Resolution)
{
	uint8_t Data[3];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	ErrorCode =	MLX90393_ReadRegister(MLX90393_REGISTER_2, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	uint16_t Config = (Data[1] << 0x08) | Data[1];
	Config++;
	
	// ToDo
	
	return ErrorCode;
}

const MLX90393_COMM_ERROR MLX90393_GetMode(MLX90393_Mode_t* Mode)
{
	uint8_t Data[3];

	MLX90393_COMM_ERROR ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_1, Data);

	// Set the mode to both if the second bit is cleared
	if(!(Data[1] & (0x01 << 0x06)))
	{
		*Mode = MLX90393_MODE_BOTH;
		return ErrorCode;
	}

	*Mode = (Data[1] >> 0x05);
	return ErrorCode;
}

const MLX90393_COMM_ERROR MLX90393_ReadMeasurementData(const uint8_t Mask, uint8_t Bytes, uint8_t* Data)
{
	#if(MLX90393_INTERFACE_TYPE == INTERFACE_I2C)
		return (I2CM_WRITEBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, MLX30393_CMD_READ_MEAS(Mask), FALSE) || I2CM_READBYTES(MLX90393_INTERFACE, MLX90393_ADDRESS, Bytes, Data, TRUE));

	#elif((MLX90393_INTERFACE_TYPE == INTERFACE_USART_SPI) | (MLX90393_INTERFACE_TYPE == INTERFACE_SPI))
		// Todo
	#else
		#error "Invalid interface for MXL90393!"
	#endif
}

// ToDo: Auslesen der Daten muss an die Maske angepasst werden
const MLX90393_COMM_ERROR MLX90393_Single(const MLX90393_Axis_t Mask, MLX90393_DataPoint_t* Data)
{
	uint8_t MeasurementData[3];
	
	// Put the device into single measurement mode
	I2C_Error_t ErrorCode = MLX90393_SetMeasurementMode(MLX30393_CMD_START_SINGLE(Mask));
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Wait for conversion end
	while(!(GPIO_Read(MLX90393_DRDY_PORT, MLX90393_DRDY_PIN)));

	ErrorCode = MLX90393_ReadMeasurementData(Mask, 3, MeasurementData);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	if(Mask == MLX90393_AXIS_X)
	{
		Data->x = (MeasurementData[1] << 0x08) | MeasurementData[2];
	}
	else if(Mask == MLX90393_AXIS_Y)
	{
		Data->y = (MeasurementData[1] << 0x08) | MeasurementData[2];
	}
	else if(Mask == MLX90393_AXIS_Z)
	{
		Data->z = (MeasurementData[1] << 0x08) | MeasurementData[2];
	}

	return ErrorCode;
}

const MLX90393_COMM_ERROR MLX90393_WOC(const MLX90393_WakeMode_t Mode, const MLX90393_Axis_t Mask, const MLX90393_Threshold_t* Threshold)
{
	uint8_t Data[3];
	MLX90393_COMM_ERROR ErrorCode;

	// Set the threshold
	ErrorCode = MLX90393_SetThreshold(Threshold);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_1, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set the Wake-up mode
	if(Mode == MLX90393_WAKE_DIFFERENTIAL)
	{
		Data[1] |= (0x01 << MLX90393_WOC_DIFF);
	}
	else
	{
		Data[1] &= ~(0x01 << MLX90393_WOC_DIFF);
	}

	ErrorCode = MLX90393_WriteRegister(MLX90393_REGISTER_1, (Data[1] << 0x08) | Data[2], NULL);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Put the device into Wake-up on Change mode
	return MLX90393_SetMeasurementMode(MLX30393_CMD_START_WOC(Mask));
}

const MLX90393_COMM_ERROR MLX90393_Burst(const uint8_t DataRate, const MLX90393_Axis_t Mask, GPIO_InterruptConfig_t* DataReady)
{
	uint8_t Data[3];
	MLX90393_COMM_ERROR ErrorCode;
	
	ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_1, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set the data rate
	Data[2] &= ((0x01 << MLX90393_BURST_DATA_RATE_5) | (0x01 << MLX90393_BURST_DATA_RATE_4) | (0x01 << MLX90393_BURST_DATA_RATE_3) | (0x01 << MLX90393_BURST_DATA_RATE_2) | (0x01 << MLX90393_BURST_DATA_RATE_1) | (0x01 << MLX90393_BURST_DATA_RATE_0));
	Data[2] |= DataRate & 0x3F;
	
	ErrorCode = MLX90393_WriteRegister(MLX90393_REGISTER_1, (Data[1] << 0x08) | Data[2], NULL);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Install the callback
	if(DataReady != NULL)
	{
		GPIO_InstallCallback(DataReady);
	}

	return MLX90393_SetMeasurementMode(MLX30393_CMD_START_BURST(Mask));
}

const MLX90393_COMM_ERROR MLX90393_Cancel(void)
{
	return (I2CM_WRITEBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, MLX30393_CMD_EXIT, FALSE) || I2CM_READBYTE(MLX90393_INTERFACE, MLX90393_ADDRESS, NULL, TRUE));
}

const MLX90393_COMM_ERROR MLX90393_SetThreshold(const MLX90393_Threshold_t* Threshold)
{
	return (MLX90393_WriteRegister(MLX90393_REGISTER_7, (Threshold->x << 0x08) | Threshold->y, NULL) || MLX90393_WriteRegister(MLX90393_REGISTER_8, Threshold->z, NULL));
}








 void Bla(void)
 {
	 uint8_t Data[3];
	 MLX90393_COMM_ERROR ErrorCode;
	 GPIO_SetDirection(MLX90393_TRG_PORT, MLX90393_TRG_PIN, GPIO_DIRECTION_OUT);
	 GPIO_Clear(MLX90393_TRG_PORT, MLX90393_TRG_PIN);
	 
	 ErrorCode = MLX90393_ReadRegister(MLX90393_REGISTER_1, Data);
	 if(ErrorCode != I2C_NO_ERROR)
	 {
		 return ErrorCode;
	 }
	 
	 // Clear TRIG_INT and BURST_SEL bits
	 Data[1] &= ~((0x01 << MLX90393_TRIGG_INT) | (0x01 << MLX90393_BURST_SEL_3) | (0x01 << MLX90393_BURST_SEL_2));
	 Data[2] &= ~((0x01 << MLX90393_BURST_SEL_1) | (0x01 << MLX90393_BURST_SEL_0));

	 Data[1] |= (0x01 << MLX90393_EXT_TRIG) | (0x01 << MLX90393_BURST_SEL_3);


	 ErrorCode = MLX90393_WriteRegister(MLX90393_REGISTER_1, (Data[1] << 0x08) | Data[2], NULL);
	 if(ErrorCode != I2C_NO_ERROR)
	 {
		 return ErrorCode;
	 }
	 
	 GPIO_Set(MLX90393_TRG_PORT, MLX90393_TRG_PIN);
	 
	 MLX90393_ReadRegister(1, Data);
	 
	 while(!(GPIO_Read(MLX90393_DRDY_PORT, MLX90393_DRDY_PIN)))
	 {
	 }
	 
 }