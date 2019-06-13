/*
 * ADXL345.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Analog Devices ADXL345 Accelerometer.

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

/** @file Peripheral/ADXL345/ADXL345.c
 *  @brief Driver for Analog Devices ADXL345 Accelerometer.
 *
 *  This file contains the implementation of the functions for the ADXL345 driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/ADXL345/ADXL345.h"

/** @defgroup ADXL345
 *  @{
 */
 	/** @defgroup ADXL345-Register
	 *  ADXL345 register addresses.
	 *  @{
	 */
		#define ADXL345_REGISTER_DEVID			0x00
		#define ADXL345_REGISTER_THRESH_TAP		0x1D
		#define ADXL345_REGISTER_OFSX			0x1E
		#define ADXL345_REGISTER_OFSY			0x1F
		#define ADXL345_REGISTER_OFSZ			0x20
		#define ADXL345_REGISTER_DUR			0x21
		#define ADXL345_REGISTER_LATENT			0x22
		#define ADXL345_REGISTER_WINDOW			0x23
		#define ADXL345_REGISTER_THRESH_ACT		0x24
		#define ADXL345_REGISTER_THRESH_INACT	0x25
		#define ADXL345_REGISTER_TIME_INACT		0x26
		#define ADXL345_REGISTER_ACT_INACT_CTL	0x27
		#define ADXL345_REGISTER_THRESH_FF		0x28
		#define ADXL345_REGISTER_TIME_FF		0x29
		#define ADXL345_REGISTER_TAP_AXES		0x2A
		#define ADXL345_REGISTER_ACT_TAP_STATUS	0x2B
		#define ADXL345_REGISTER_BW_RATE		0x2C
		#define ADXL345_REGISTER_POWER_CTL		0x2D
		#define ADXL345_REGISTER_INT_ENABLE		0x2E
		#define ADXL345_REGISTER_INT_MAP		0x2F
		#define ADXL345_REGISTER_INT_SOURCE		0x30
		#define ADXL345_REGISTER_DATA_FORMAT	0x31
		#define ADXL345_REGISTER_DATAX0			0x32
		#define ADXL345_REGISTER_DATAX1			0x33
		#define ADXL345_REGISTER_DATAY0			0x34
		#define ADXL345_REGISTER_DATAY1			0x35
		#define ADXL345_REGISTER_DATAZ0			0x36
		#define ADXL345_REGISTER_DATAZ1			0x37
		#define ADXL345_REGISTER_FIFO_CTL		0x38
		#define ADXL345_REGISTER_FIFO_STATUS	0x39
	/** @} */ // end of ADXL345-Register
	 
 	/** @defgroup ADXL345-Status
	 *  ADXL345 status bits.
	 *  @{
	 */
		#define ADXL345_ASLEEP					0x03
		#define ADXL345_DATA_READY				0x07
	/** @} */ // end of ADXL345-Status
	 
 	/** @defgroup ADXL345-Control
	 *  ADXL345 control bits
	 *  @{
	 */
		#define ADXL345_SLEEP					0x02
		#define ADXL345_MEASURE					0x03
		#define ADXL345_SUPPRESS				0x03
		#define ADXL345_FULL_RES				0x03
		#define ADXL345_INACT_AC				0x03
		#define ADXL345_AUTO_SLEEP				0x04
		#define ADXL345_LOW_POWER				0x04
		#define ADXL345_LINK					0x05
		#define ADXL345_TRIGGER					0x05
		#define ADXL345_INT_INVERT				0x05
		#define ADXL345_SELFTEST				0x07
		#define ADXL345_ACT_AC					0x07
	/** @} */ // end of ADXL345-Control
/** @} */ // end of ADXL345

#ifndef DOXYGEN
	// Callbacks for each interrupt channel
	ADXL345_Callbacks_t __ADXL345_Callbacks[2];
	
	// Flag for enabling/disabling full resolution mode
	static Bool_t __ADXL345_FullRessolution;
	
	// Resolution of the device in mg/LSB
	static double __ADXL345_Resolution;
	
	// Current range settings
	static ADXL345_Range_t __ADXL345_Range;
	
	// Supply voltage of the device
	static ADXL345_Supply_t __ADXL345_SupplyVoltage;
	
	static ADXL345_DataPoint_t __ADXL345_AverageNoSelfTest;
	static ADXL345_DataPoint_t __ADXL345_AverageSelfTest;
	
	// Buffer for the save and restore functions
	static ADXL345_DataRate_t __OldRate;
	static ADXL345_Range_t __OldRange;
	static ADXL345_Alignment_t __OldAlignment;
	static Bool_t __OldIsFullResolution;
#endif

/** @brief	Common handler for all interrupts.
 */
static void __ADXL345_InterruptHandler(const uint8_t Channel)
{
	uint8_t Status = 0x00;

	if((ADXL345_READBYTE(ADXL345_REGISTER_INT_SOURCE, &Status) == I2C_NO_ERROR))
	{
		if(Status & ADXL345_DATARDY_INTERRUPT)
		{
			if(__ADXL345_Callbacks[Channel].DataRdyCallback != NULL)
			{
				__ADXL345_Callbacks[Channel].DataRdyCallback();
			}
		}
		
		if(Status & ADXL345_SINGLE_TAP_INTERRUPT)
		{			
			if(__ADXL345_Callbacks[Channel].SingleTapCallback != NULL)
			{
				__ADXL345_Callbacks[Channel].SingleTapCallback();
			}
		}
		
		if(Status & ADXL345_DOUBLE_TAP_INTERRUPT)
		{
			if(__ADXL345_Callbacks[Channel].DoubleTapCallback != NULL)
			{
				__ADXL345_Callbacks[Channel].DoubleTapCallback();
			}
		}
		
		if(Status & ADXL345_ACTIVITY_INTERRUPT)
		{
			if(__ADXL345_Callbacks[Channel].ActivityCallback != NULL)
			{
				__ADXL345_Callbacks[Channel].ActivityCallback();
			}
		}

		if(Status & ADXL345_INACTIVITY_INTERRUPT)
		{
			if(__ADXL345_Callbacks[Channel].InactivityCallback != NULL)
			{
				__ADXL345_Callbacks[Channel].InactivityCallback();
			}
		}
		
		if(Status & ADXL345_FREEFALL_INTERRUPT)
		{
			if(__ADXL345_Callbacks[Channel].FreeFallCallback != NULL)
			{
				__ADXL345_Callbacks[Channel].FreeFallCallback();
			}
		}
		
		if(Status & ADXL345_WATERMARK_INTERRUPT)
		{
			//ADXL345_FIFOClear(__ADXL345_FIFO);
	
			if(__ADXL345_Callbacks[Channel].WatermarkCallback != NULL)
			{
				__ADXL345_Callbacks[Channel].WatermarkCallback();
			}
		}

		if(Status & ADXL345_OVERRUN_INTERRUPT)
		{
			if(__ADXL345_Callbacks[Channel].OverrunCallback != NULL)
			{
				__ADXL345_Callbacks[Channel].OverrunCallback();
			}
		}
	}
}

/** @brief	ADXL345 interrupt handler for interrupt 1.
 */
static inline void __ADXL345_InterruptHandler1(void)
{
	__ADXL345_InterruptHandler(0);
}

/** @brief	ADXL345 interrupt handler for interrupt 2.
 */
static inline void __ADXL345_InterruptHandler2(void)
{
	__ADXL345_InterruptHandler(1);
}

/** @brief	Calculate the resolution in mg / bit.
 */
static void ADXL345_CalcResolution(ADXL345_Range_t Range, Bool_t ResolutionMode)
{
	if(ResolutionMode == TRUE)
	{
		// +- 16 G with 13-Bit resolution
		__ADXL345_Resolution = ((float)(0x04 << ADXL345_RANGE_16G)) / (0x02 << 0x0C);
	}
	else
	{
		// 10-Bit resolution
		__ADXL345_Resolution = ((float)(0x04 << Range)) / (0x02 << 0x09);
	}
}

/** @brief	Save the device configuration.
 */
static I2C_Error_t ADXL345_SaveConfig(void)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_GetDataRate(&__OldRate);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_IsFullResolution(&__OldIsFullResolution);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_GetAlignment(&__OldAlignment);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	return ADXL345_GetMeasurementRange(&__OldRange);
}

/** @brief	Restore the device configuration.
 */
static I2C_Error_t ADXL345_RestoreConfig(void)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_SetDeviceMode(ADXL345_MODE_STANDBY);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SetDataRate(__OldRate);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SwitchFullResolution(__OldIsFullResolution);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SetAlignment(__OldAlignment);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	return ADXL345_SetMeasurementRange(__OldRange);
}

const I2C_Error_t ADXL345_Init(I2CM_Config_t* Config, const ADXL345_Configuration_t* Device)
{
	uint8_t ID = 0x00;
	Bool_t Passed = FALSE;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	if(Config != NULL)
	{
		ADXL345_INIT(Config);
	}
	
	__ADXL345_FullRessolution = FALSE;
	__ADXL345_SupplyVoltage = Device->SupplyVoltage;
	
	ErrorCode = ADXL345_ReadDevID(&ID);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Check the device ID
	if(ID != ADXL345_ID)
	{
		return I2C_DATA_ERROR;
	}
	
	// Run a self test to check the device
	ErrorCode = ADXL345_RunSelfTest(&Passed);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	if(Device->FIFO)
	{
		ADXL345_FIFOConfig((ADXL345_FIFOConfig_t*) Device->FIFO);
	}
	else
	{
		ADXL345_FIFODisable();
	}

	ErrorCode = ADXL345_SetMeasurementRange(Device->Range);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SwitchFullResolution(Device->FullResolution);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = ADXL345_SetAlignment(ADXL345_ALIGN_RIGHT);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = ADXL345_SetDataRate(Device->DataRate);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Disable and clear all interrupt bits
	uint8_t Data[3];
	uint8_t Register = 0x00;
	
	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_INT_ENABLE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_READBYTES(ADXL345_REGISTER_DATAX0, ADXL345_REGISTER_DATAZ1 - ADXL345_REGISTER_DATAX0 + 1, Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_INT_SOURCE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	return ADXL345_SetDeviceMode(ADXL345_MODE_STANDBY);
}

const I2C_Error_t ADXL345_ReadDevID(uint8_t* DeviceID)
{
	return ADXL345_READBYTE(ADXL345_REGISTER_DEVID, DeviceID);
}

const I2C_Error_t ADXL345_ClearAllInterrupts(void)
{
	uint8_t Dummy = 0x00;
	return ADXL345_READBYTE(ADXL345_REGISTER_INT_SOURCE, &Dummy);
}

const I2C_Error_t ADXL345_SwitchFullResolution(const Bool_t Enable)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	if(Enable == TRUE)
	{
		__ADXL345_FullRessolution = TRUE;
		Register |= (0x01 << ADXL345_FULL_RES);
	}
	else
	{
		__ADXL345_FullRessolution = FALSE;
		Register &= ~(0x01 << ADXL345_FULL_RES);
	}
	
	ADXL345_CalcResolution(__ADXL345_Range, __ADXL345_Resolution);
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
}

const I2C_Error_t ADXL345_IsFullResolution(Bool_t* Enable)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Enable = (Register & (0x01 << ADXL345_FULL_RES)) >> ADXL345_FULL_RES;

	return ErrorCode;
}

const I2C_Error_t ADXL345_SetMeasurementRange(const ADXL345_Range_t Range)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	__ADXL345_Range = Range;
	ADXL345_CalcResolution(__ADXL345_Range, __ADXL345_Resolution);

	Register = (Register & (~0x03)) | Range;
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
}

const I2C_Error_t ADXL345_GetMeasurementRange(ADXL345_Range_t* Range)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Range = Register & 0x03;
	
	return ErrorCode;
}

const I2C_Error_t ADXL345_SetSleepReadingFreq(const ADXL345_WakeUp_t Freq)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_POWER_CTL, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Register = (Register & (~0x03)) | Freq;
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_POWER_CTL, &Register);
}

const I2C_Error_t ADXL345_GetSleepReadingFreq(ADXL345_WakeUp_t* Freq)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_POWER_CTL, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Freq = Register & 0x03;
	
	return ErrorCode;
}

const I2C_Error_t ADXL345_SetAlignment(const ADXL345_Alignment_t Alignment)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Register = (Register & (~(0x01 << 0x02))) | ((Alignment & 0x01) << 0x02);
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
}
 
const I2C_Error_t ADXL345_GetAlignment(ADXL345_Alignment_t* Alignment)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Alignment = (Register & (0x01 << 0x02)) >> 0x02;
	
	return ErrorCode;
}

const I2C_Error_t ADXL345_SetDataRate(const ADXL345_DataRate_t Rate)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_BW_RATE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Register = (Register & (~0x0F)) | Rate;
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_BW_RATE, &Register);
}

const I2C_Error_t ADXL345_GetDataRate(ADXL345_DataRate_t* Rate)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_BW_RATE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Rate = (Register & 0x0F);
	
	return ErrorCode;
}

const const I2C_Error_t ADXL345_SetDeviceMode(const ADXL345_DeviceMode_t Mode)
{
	uint8_t BW_Rate = 0x00;
	uint8_t Power_Ctrl = 0x00;
	uint8_t Data_Format = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	switch(Mode)
	{
		case ADXL345_MODE_STANDBY:
		{
			// Clear the LP bit
			ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_BW_RATE, &BW_Rate);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
			
			BW_Rate &= ~(0x01 << ADXL345_LOW_POWER);
			
			ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_BW_RATE, &BW_Rate);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
			
			// Clear sleep, auto sleep and measure bit
			ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_POWER_CTL, &Power_Ctrl);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}

			Power_Ctrl &= ~((0x01 << ADXL345_AUTO_SLEEP) | (0x01 << ADXL345_MEASURE) | (0x01 << ADXL345_SLEEP));
			
			ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_POWER_CTL, &Power_Ctrl);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
			
			// Clear the self test bit
			ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Data_Format);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}

			Data_Format &= ~(0x01 << ADXL345_SELFTEST);
			
			ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_DATA_FORMAT, &Data_Format);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}

			break;
		}
		case ADXL345_MODE_LP:
		{
			// Set the LP bit
			ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_BW_RATE, &BW_Rate);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
			
			BW_Rate |= (0x01 << ADXL345_LOW_POWER);
			
			ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_BW_RATE, &BW_Rate);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
			
			break;
		}
		case ADXL345_MODE_SLEEP:
		{
			ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_POWER_CTL, &Power_Ctrl);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
		
			Power_Ctrl &= ~0x03;
			Power_Ctrl |= ((0x01 << ADXL345_AUTO_SLEEP) | (0x01 << ADXL345_SLEEP));
			
			ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_POWER_CTL, &Power_Ctrl);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
			
			break;
		}
		case ADXL345_MODE_MEASURE:
		{
			ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_POWER_CTL, &Power_Ctrl);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}

			Power_Ctrl |= (0x01 << ADXL345_MEASURE);
			
			ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_POWER_CTL, &Power_Ctrl);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
			
			for(uint16_t i = 0x00; i < 0x00; i++);
			
			break;
		}
		
		case ADXL345_MODE_SELFTEST:
		{
			ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Data_Format);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}

			Data_Format |= (0x01 << ADXL345_SELFTEST);
			
			ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_DATA_FORMAT, &Data_Format);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}

			break;
		}
	}

	return ErrorCode;
}

const I2C_Error_t ADXL345_GetData(ADXL345_DataPoint_t* Data)
{
	return ADXL345_READBYTES(ADXL345_REGISTER_DATAX0, ADXL345_REGISTER_DATAZ1 - ADXL345_REGISTER_DATAX0 + 1, (uint8_t*)Data);
}

const I2C_Error_t ADXL345_WaitForData(ADXL345_DataPoint_t* Data)
{
	uint8_t Status = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	// Wait for data
	while(!(Status & (0x01 << ADXL345_DATA_READY)))
	{
		ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_INT_SOURCE, &Status);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}

	return ADXL345_GetData(Data);
}

const float ADXL345_GetResolution(void)
{
	return __ADXL345_Resolution;
}

const I2C_Error_t ADXL345_EnableAutoSleep(const uint8_t Thresh, const uint8_t Time)
{
	uint8_t Register = 0x00;
	I2C_Error_t	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_THRESH_INACT, &Thresh);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_TIME_INACT, &Time);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_POWER_CTL, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Register |= (0x01 << ADXL345_LINK) | (0x01 << ADXL345_AUTO_SLEEP);

	return ADXL345_WRITEBYTE(ADXL345_REGISTER_POWER_CTL, &Register);
}

const I2C_Error_t ADXL345_DisableAutoSleep(void)
{
	uint8_t Register = 0x00;

	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_POWER_CTL, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Register &= ~((0x01 << ADXL345_LINK) | (0x01 << ADXL345_AUTO_SLEEP));

	return ADXL345_WRITEBYTE(ADXL345_REGISTER_POWER_CTL, &Register);
}

const I2C_Error_t ADXL345_IsSleeping(Bool_t* Sleep)
{
	uint8_t Status = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_ACT_TAP_STATUS, &Status);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	*Sleep = (Status & (0x01 << ADXL345_ASLEEP)) >> ADXL345_ASLEEP;
	
	return I2C_NO_ERROR;
}

const I2C_Error_t ADXL345_FIFOConfig(const ADXL345_FIFOConfig_t* Config)
{
	uint8_t Register = (Config->Mode << 0x06) | (Config->Samples & 0x3F);
	
	if(Config->Mode == ADXL345_FIFO_TRIGGER)
	{
		Register |= (Config->Trigger << 0x05);
	}

	// Clear the FIFO
	for(uint8_t i = 0x00; i < ADXL345_FIFO_SIZE; i++)
	{
		ADXL345_DataPoint_t Dummy;
		ADXL345_GetData(&Dummy);
	}

	return ADXL345_WRITEBYTE(ADXL345_REGISTER_FIFO_CTL, &Register);
}

const I2C_Error_t ADXL345_FIFODisable(void)
{
	uint8_t Register = 0x00;

	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_FIFO_CTL, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Register &= ~0xC0;

	return ADXL345_WRITEBYTE(ADXL345_REGISTER_FIFO_CTL, &Register);
}

const I2C_Error_t ADXL345_FIFOTrigger(void)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_FIFO_CTL, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Register |= (0x01 << ADXL345_TRIGGER);

	return ADXL345_WRITEBYTE(ADXL345_REGISTER_FIFO_CTL, &Register);
}

const I2C_Error_t ADXL345_FIFOGetEntries(uint8_t* Entries)
{
	uint8_t Register = 0x00;
	
	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_FIFO_STATUS, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	*Entries = Register & 0x3F;

	return I2C_NO_ERROR;
}

const I2C_Error_t ADXL345_FIFOClear(ADXL345_DataPoint_t* Data)
{
	uint8_t Entries = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	ErrorCode = ADXL345_FIFOGetEntries(&Entries);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	for(uint8_t i = 0x00; i < Entries; i++)
	{
		ErrorCode = ADXL345_GetData(Data++);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}
	
	return ErrorCode;
}

const I2C_Error_t ADXL345_TapEnableAxes(const ADXL345_Axis_t Axes)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_TAP_AXES, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Register = (Register & (~0x07)) | Axes;

	return ADXL345_WRITEBYTE(ADXL345_REGISTER_TAP_AXES, &Register);
}

const I2C_Error_t ADXL345_TapDisableAxes(const ADXL345_Axis_t Axes)
{
	uint8_t Register = 0x00;

	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_TAP_AXES, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Register &= ~0x07;

	return ADXL345_WRITEBYTE(ADXL345_REGISTER_TAP_AXES, &Register);
}

const I2C_Error_t ADXL345_SingleTapConfig(const uint8_t Value, const uint8_t Duration, const ADXL345_Axis_t Axes)
{
	I2C_Error_t ErrorCode = ADXL345_TapEnableAxes(Axes);

	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_THRESH_TAP, &Value);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_DUR, &Duration);
}

const I2C_Error_t ADXL345_DoubleTapConfig(const uint8_t Value, const uint8_t Duration, const uint8_t Delay, const uint8_t Window, const ADXL345_Axis_t Axes)
{
	I2C_Error_t ErrorCode = ADXL345_TapEnableAxes(Axes);
	
	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_THRESH_TAP, &Value);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_DUR, &Duration);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_LATENT, &Delay);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_WINDOW, &Window);
}

const I2C_Error_t ADXL345_TapSuppress(const Bool_t Supress)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_TAP_AXES, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	if(Supress == TRUE)
	{
		Register |= (0x01 << ADXL345_SUPPRESS);
	}
	else
	{
		Register &= ~(0x01 << ADXL345_SUPPRESS);
	}
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_TAP_AXES, &Register);
}

const I2C_Error_t ADXL345_TapGetAxis(ADXL345_Axis_t* Axis)
{
	uint8_t Temp = 0x00;
	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_ACT_TAP_STATUS, &Temp);
	
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Axis = (ADXL345_Axis_t)(Temp & 0x07);
	
	return ErrorCode;
}

const I2C_Error_t ADXL345_ActivityConfig(const uint8_t Value, const ADXL345_Axis_t Axes, const Bool_t AC)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_THRESH_ACT, &Value);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_ACT_INACT_CTL, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Register = (Register & (~(0x07 << 0x04))) | (Axes << 0x04);
	
	if(AC == TRUE)
	{
		Register |= (0x01 << ADXL345_ACT_AC);
	}
	else
	{
		Register &= ~(0x01 << ADXL345_ACT_AC);
	}
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_ACT_INACT_CTL, &Register);
}

const I2C_Error_t ADXL345_ActivityGetAxis(ADXL345_Axis_t* Axis)
{
	uint8_t Temp = 0x00;
	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_ACT_TAP_STATUS, &Temp);
	
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	*Axis = (ADXL345_Axis_t)((Temp & 0x70) >> 0x04);
	
	return ErrorCode;
}

const I2C_Error_t ADXL345_InactivityConfig(const uint8_t Value, const uint8_t Time, const ADXL345_Axis_t Axes, const Bool_t AC)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_THRESH_INACT, &Value);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_TIME_INACT, &Time);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_ACT_INACT_CTL, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Register = (Register & (~0x07)) | Axes;
	
	if(AC == TRUE)
	{
		Register |= (0x01 << ADXL345_INACT_AC);
	}
	else
	{
		Register &= ~(0x01 << ADXL345_INACT_AC);
	}
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_ACT_INACT_CTL, &Register);
}

const I2C_Error_t ADXL345_FreeFallConfig(const uint8_t Thresh, const uint8_t Time)
{
	I2C_Error_t ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_THRESH_FF, &Thresh);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	return ADXL345_WRITEBYTE(ADXL345_REGISTER_TIME_FF, &Time);
}

const I2C_Error_t ADXL345_RunSelfTest(Bool_t* Passed)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	int32_t AverageX = 0x00;
	int32_t AverageY = 0x00;
	int32_t AverageZ = 0x00;
	__ADXL345_AverageNoSelfTest.X = 0x00;
	__ADXL345_AverageNoSelfTest.Y = 0x00;
	__ADXL345_AverageNoSelfTest.Z = 0x00;
	__ADXL345_AverageSelfTest.X = 0x00;
	__ADXL345_AverageSelfTest.Y = 0x00;
	__ADXL345_AverageSelfTest.Z = 0x00;
	
	// Save the old settings
	ErrorCode = ADXL345_SaveConfig();
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	/* Configure the device for self test (see data sheet)
		-> 3200 Hz data rate
		-> 16 G measurement range
		-> Full resolution
		-> Alignment right
		-> Normal mode		
	*/
	ErrorCode = ADXL345_SetDataRate(ADXL345_RATE_3200HZ);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SetMeasurementRange(ADXL345_RANGE_16G);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SetAlignment(ADXL345_ALIGN_RIGHT);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = ADXL345_SwitchFullResolution(TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SetDeviceMode(ADXL345_MODE_STANDBY);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	/*
		Run the self test
			-> Take n samples without self test mode
			-> Average them
			-> Take n samples with self test mode
			-> Average them
			-> Compare the values with the given values for 16 G in the data sheet
	*/
	ErrorCode = ADXL345_SetDeviceMode(ADXL345_MODE_MEASURE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Wait for the first data
	for(uint16_t i = 0x00; i < 0xFFFF; i++);
	
	// Get the data points without self test mode
	for(uint8_t i = 0x00; i < ADXL345_CALIBRATION_LENGTH; i++)
	{
		ADXL345_DataPoint_t Data;
		
		ErrorCode = ADXL345_GetData(&Data);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
		
		// Use a 32 bit variable for the average to avoid an overflow
		AverageX += Data.X;
		AverageY += Data.Y;
		AverageZ += Data.Z;
	}
	
	__ADXL345_AverageNoSelfTest.X = AverageX / ADXL345_CALIBRATION_LENGTH;
	__ADXL345_AverageNoSelfTest.Y = AverageY / ADXL345_CALIBRATION_LENGTH;
	__ADXL345_AverageNoSelfTest.Z = AverageZ / ADXL345_CALIBRATION_LENGTH;
	
	ErrorCode = ADXL345_SetDeviceMode(ADXL345_MODE_SELFTEST);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Wait for the first data
	for(uint16_t i = 0x00; i < 0xFFFF; i++);

	// Get the data points with self test mode
	AverageX = 0x00;
	AverageY = 0x00;
	AverageZ = 0x00;
	for(uint8_t i = 0x00; i < ADXL345_CALIBRATION_LENGTH; i++)
	{
		ADXL345_DataPoint_t Data;
		
		ErrorCode = ADXL345_GetData(&Data);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
		
		// Use a 32 bit variable for the average to avoid an overflow
		AverageX += Data.X;
		AverageY += Data.Y;
		AverageZ += Data.Z;
	}
	
	__ADXL345_AverageSelfTest.X = AverageX / ADXL345_CALIBRATION_LENGTH;
	__ADXL345_AverageSelfTest.Y = AverageY / ADXL345_CALIBRATION_LENGTH;
	__ADXL345_AverageSelfTest.Z = AverageZ / ADXL345_CALIBRATION_LENGTH;

	// Get the difference
	AverageX = __ADXL345_AverageSelfTest.X - __ADXL345_AverageNoSelfTest.X;
	AverageY = __ADXL345_AverageSelfTest.Y - __ADXL345_AverageNoSelfTest.Y;
	AverageZ = __ADXL345_AverageSelfTest.Z - __ADXL345_AverageNoSelfTest.Z;

	// Scale the output
	switch(__ADXL345_SupplyVoltage)
	{
		case ADXL345_SUPPLY_2V:
		{
			AverageX = AverageX * 0.64;
			AverageY = AverageY * 0.64;
			AverageZ = AverageZ * 0.8;
			
			break;
		}
		case ADXL345_SUPPLY_2V5:
		{
			break;
		}
		case ADXL345_SUPPLY_3V3:
		{
			AverageX = AverageX * 1.77;
			AverageY = AverageY * 1.77;
			AverageZ = AverageZ * 1.47;
			
			break;
		}
		case ADXL345_SUPPLY_3V6:
		{
			AverageX = AverageX * 2.11;
			AverageY = AverageY * 2.11;
			AverageZ = AverageZ * 1.69;
			
			break;
		}
	}	
	
	// Disable the self test mode and restore the old settings
	return ADXL345_RestoreConfig();
}

const I2C_Error_t ADXL345_RunOffsetCalibration(Bool_t* Passed)
{
	ADXL345_DataPoint_t Point;
	int32_t OffsetX = 0x00;
	int32_t OffsetY = 0x00;
	int32_t OffsetZ = 0x00;
	uint8_t Offset[3];

	// Get the old settings
	I2C_Error_t ErrorCode = ADXL345_SaveConfig();
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Check if the device is in the right position (x = 0 g, y = 0 g, z = +1 g)
	ErrorCode = ADXL345_SetDeviceMode(ADXL345_MODE_MEASURE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_GetData(&Point);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Check the position with some default values. X and Y are small and Z is big if the sensor position is right
	if((((Point.X > 10) | (Point.X < -10)) || ((Point.Y > 10) | (Point.Y < -10))) && (Point.Z < 100))
	{
		*Passed = FALSE;
	}

	/* Configure the device for offset calibration (see data sheet)
		-> 3200 Hz data rate
		-> 16 G measurement range
		-> Full resolution
		-> Alignment right
		-> Normal mode		
	*/
	ErrorCode = ADXL345_SetDataRate(ADXL345_RATE_3200HZ);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SetMeasurementRange(ADXL345_RANGE_16G);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = ADXL345_SetAlignment(ADXL345_ALIGN_RIGHT);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = ADXL345_SwitchFullResolution(TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	/*
		Run the calibration
			-> Take n samples without self test mode
			-> Average them
			-> Calculate the offset
			-> Write the values into the register
	*/
	ErrorCode = ADXL345_SetDeviceMode(ADXL345_MODE_MEASURE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Wait for the first data
	for(uint16_t i = 0x00; i < 0xFFFF; i++);

	// Get the data points without self test mode
	for(uint8_t i = 0x00; i < ADXL345_CALIBRATION_LENGTH; i++)
	{
		ADXL345_DataPoint_t Data;
		
		ErrorCode = ADXL345_GetData(&Data);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
		
		// Use a 32 bit variable for the average to avoid an overflow
		OffsetX += Data.X;
		OffsetY += Data.Y;
		OffsetZ += Data.Z;
	}

	Offset[0] = OffsetX / ADXL345_CALIBRATION_LENGTH;
	Offset[1] = OffsetY / ADXL345_CALIBRATION_LENGTH;
	Offset[2] = (OffsetZ / ADXL345_CALIBRATION_LENGTH) - 0x100;

	// Calculate the calibration values (see data sheet for formula)
	for(int8_t i = 0x00; i < 0x03; i++)
	{
		Offset[i] = -(Offset[i] >> 0x02);
	}

	// Write the values to the device
	ErrorCode = ADXL345_WRITEBYTES(ADXL345_REGISTER_OFSX, ADXL345_REGISTER_OFSZ - ADXL345_REGISTER_OFSX + 1, Offset);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Restore the old settings
	return ADXL345_RestoreConfig();
}

const I2C_Error_t ADXL345_EnableInterrupts(const ADXL345_InterruptConfig_t* Config)
{
	uint8_t Register = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	// Define the interrupt configuration, depending on the architecture
	GPIO_InterruptConfig_t ConfigGPIO = {
		.Channel = Config->IOChannel,
		
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			.Port = Config->Port,
			.Pin = Config->Pin,
			.InterruptLevel = Config->InterruptLevel,
		#endif
	};
	
	ErrorCode = ADXL345_DisableInterrupts(Config);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}	
		
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	if(Config->Sense == ADXL345_POL_HIGH)
	{
		Register &= ~(0x01 << ADXL345_INT_INVERT);
		ConfigGPIO.Type = GPIO_SENSE_RISING;
	}
	else
	{
		Register |= (0x01 << ADXL345_INT_INVERT);
		ConfigGPIO.Type = GPIO_SENSE_FALLING;
	}
	
	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_DATA_FORMAT, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	if(Config->DeviceChannel == ADXL345_INTERRUPT_1)
	{
		ConfigGPIO.Callback = __ADXL345_InterruptHandler1;
	}
	else
	{
		ConfigGPIO.Callback = __ADXL345_InterruptHandler2;
	}

	// Configure the device interrupt channel
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_INT_MAP, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	if(Config->DeviceChannel == ADXL345_INTERRUPT_1)
	{
		Register &= ~Config->Type;
	}
	else
	{
		Register |= Config->Type;
	}

	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_INT_MAP, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Clear the interrupt bits
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_INT_SOURCE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Enable the interrupt on the device
	ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_INT_ENABLE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Register |= Config->Type;
	
	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_INT_ENABLE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	GPIO_SetDirection(ConfigGPIO.Port, ConfigGPIO.Pin, GPIO_DIRECTION_IN);
	GPIO_InstallCallback(&ConfigGPIO);

	return I2C_NO_ERROR;
}

const I2C_Error_t ADXL345_DisableInterrupts(const ADXL345_InterruptConfig_t* Config)
{
	uint8_t Register = 0x00;
	
	// Disable the interrupt on the device
	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_INT_ENABLE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Register &= ~Config->Type;
	
	ErrorCode = ADXL345_WRITEBYTE(ADXL345_REGISTER_INT_ENABLE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	#if(MCU_ARCH == MCU_ARCH_XMEGA)
		GPIO_RemoveCallback(Config->Port, Config->IOChannel);
	#else
		GPIO_RemoveCallback(Config->IOChannel);
	#endif
	
	return ErrorCode;
}

const I2C_Error_t ADXL345_InstallCallback(const ADXL345_InterruptConfig_t* Config)
{
	I2C_Error_t ErrorCode = ADXL345_EnableInterrupts(Config);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	if(Config->Type & ADXL345_DATARDY_INTERRUPT)
	{
		__ADXL345_Callbacks[Config->DeviceChannel].DataRdyCallback = Config->Callbacks->DataRdyCallback;
	}
	
	if(Config->Type & ADXL345_SINGLE_TAP_INTERRUPT)
	{
		__ADXL345_Callbacks[Config->DeviceChannel].SingleTapCallback = Config->Callbacks->SingleTapCallback;
	}
	
	if(Config->Type & ADXL345_DOUBLE_TAP_INTERRUPT)
	{
		__ADXL345_Callbacks[Config->DeviceChannel].DoubleTapCallback = Config->Callbacks->DoubleTapCallback;
	}
	
	if(Config->Type & ADXL345_ACTIVITY_INTERRUPT)
	{
		__ADXL345_Callbacks[Config->DeviceChannel].ActivityCallback = Config->Callbacks->ActivityCallback;
	}
	
	if(Config->Type & ADXL345_INACTIVITY_INTERRUPT)
	{
		__ADXL345_Callbacks[Config->DeviceChannel].InactivityCallback = Config->Callbacks->InactivityCallback;
	}
	
	if(Config->Type & ADXL345_FREEFALL_INTERRUPT)
	{
		__ADXL345_Callbacks[Config->DeviceChannel].FreeFallCallback = Config->Callbacks->FreeFallCallback;
	}
	
	if(Config->Type & ADXL345_WATERMARK_INTERRUPT)
	{
		__ADXL345_Callbacks[Config->DeviceChannel].WatermarkCallback = Config->Callbacks->WatermarkCallback;
	}
	
	if(Config->Type & ADXL345_OVERRUN_INTERRUPT)
	{
		__ADXL345_Callbacks[Config->DeviceChannel].OverrunCallback = Config->Callbacks->OverrunCallback;
	}
	
	return ErrorCode;
}

const I2C_Error_t ADXL345_RemoveCallback(const ADXL345_InterruptConfig_t* Config)
{
	uint8_t Register = 0x00;
	
	// Disable the interrupt on the device
	I2C_Error_t ErrorCode = ADXL345_READBYTE(ADXL345_REGISTER_INT_ENABLE, &Register);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Register &= ~Config->Type;

	return ADXL345_WRITEBYTE(ADXL345_REGISTER_INT_ENABLE, &Register);
}

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	void ADXL345C_ChangeInterruptLevel(const ADXL345_InterruptConfig_t* Config)
	{
		GPIO_ChangeInterruptLevel(Config->Port, Config->Pin, Config->IOChannel, Config->InterruptLevel);
	}
#endif