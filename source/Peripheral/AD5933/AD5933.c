/*
 * AD5933.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AD5933 I2C Impedance converter.

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

/** @file Peripheral/AD5933/AD5933.c
 *  @brief Driver for the Analog Devices AD5933 I2C impedance converter.
 *
 *  This file contains the implementation of the impedance converter driver.
 *
 *  @author Daniel Kampert
 */

#include <math.h>
#include "Peripheral/AD5933/AD5933.h"

/** @defgroup AD5933
 *  @{
 */
 	/** @defgroup AD5933-Register
	 *  AD5933 register addresses.
	 *  @{
	 */
		#define AD5933_REGISTER_CONTROL			0x80
		#define AD5933_REGISTER_START_FREQ		0x82
		#define AD5933_REGISTER_FREQ_INCR		0x85
		#define AD5933_REGISTER_INCREMENTS		0x88
		#define AD5933_REGISTER_CYCLES			0x8A
		#define AD5933_REGISTER_STATUS			0x8F
		#define AD5933_REGISTER_TEMPERATURE		0x92
		#define AD5933_REGISTER_REAL			0x94
		#define AD5933_REGISTER_IMAG			0x96
	/** @} */ // end of AD5933-Register
	 
 	/** @defgroup AD5933-Size
	 *  Register sizes.
	 *  @{
	 */
		#define AD5933_SIZE_CONTROL				2
		#define AD5933_SIZE_START_FREQ			3
		#define AD5933_SIZE_FREQ_INCR			3
		#define AD5933_SIZE_INCREMENTS			2
		#define AD5933_SIZE_CYCLES				2
		#define AD5933_SIZE_TEMPERATURE			2
		#define AD5933_SIZE_REAL				2
		#define AD5933_SIZE_IMAG				2
	/** @} */ // end of AD5933-Size
	 
 	/** @defgroup AD5933-Commands
	 *  AD5933 commands.
	 *  @{
	 */
		#define AD5933_CMD_BLOCK_WRITE			0xA0
		#define AD5933_CMD_BLOCK_READ			0xA1
		#define AD5933_CMD_ADD_POINTER			0xB0
	/** @} */ // end of AD5933-Commands

 	/** @defgroup AD5933-Status
	 *  AD5933 status bits.
	 *  @{
	 */
		#define AD5933_STATUS_VALID_TEMP		0x01
		#define AD5933_STATUS_VALID_DATA		0x02
		#define AD5933_STATUS_SWEEP_COMPLETE	0x04
	/** @} */ // end of AD5933-Status
	 
 	/** @defgroup AD5933-Control
	 *  AD5933 control bits.
	 *  @{
	 */ 
		#define AD5933_RESET					0x04
	/** @} */ // end of AD5933-Control
/** @} */ // end of AD5933

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#define AD5933_I2CM_INIT(Config)														I2CM_Init(Config)
	#define AD5933_I2CM_WRITEBYTE(Data, Stop)												I2CM_WriteByte(&AD5933_INTERFACE, AD5933_ADDRESS, Data, Stop)
	#define AD5933_I2CM_READBYTE(Data, Stop)												I2CM_ReadByte(&AD5933_INTERFACE, AD5933_ADDRESS, Data, Stop)
	#define AD5933_I2CM_WRITEBYTES(Bytes, Data, Stop)										I2CM_WriteBytes(&AD5933_INTERFACE, AD5933_ADDRESS, Bytes, Data, Stop)
	#define AD5933_I2CM_READBYTES(Bytes, Data, Stop)										I2CM_ReadBytes(&AD5933_INTERFACE, AD5933_ADDRESS, Bytes, Data, Stop)
#elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #define AD5933_I2CM_INIT(Config)														I2CM_Init(Config)
	 #define AD5933_I2CM_WRITEBYTE(Data, Stop)												I2CM_WriteByte(AD5933_ADDRESS, Data, Stop)
	 #define AD5933_I2CM_READBYTE(Data, Stop)												I2CM_ReadByte(AD5933_ADDRESS, Data, Stop)
	 #define AD5933_I2CM_WRITEBYTES(Bytes, Data, Stop)										I2CM_WriteBytes(AD5933_ADDRESS, Bytes, Data, Stop)
	 #define AD5933_I2CM_READBYTES(Bytes, Data, Stop)										I2CM_ReadBytes(AD5933_ADDRESS, Bytes, Data, Stop)
#else
	 #error "Architecture not supported fpr AD5933!"
#endif

static bool UseDegree = false;

/** @brief			Calculate the phase between the real and the imaginary part of a value.
 *  @param Real		Real part
 *  @param Imag		Imag part
 *  @param Degree	Set this to #true to convert the result in degree
 *  @return			Angle in degree or radians
 */
static float AD5933_CalcPhase(const float Real, const float Imag, const bool Degree)
{
	double Angle = 0.0;

	if(Imag >= 0)
	{
		Angle = atan2(Imag, Real);
	}
	else
	{
		Angle = 2 * M_PI + atan2(Imag, Real);
	}

	if(Degree == true)
	{
		Angle = (180.0 / M_PI) * Angle;
	}

	return Angle;
}

const I2C_Error_t AD5933_Init(I2CM_Config_t* Config, const AD5933_Configuration_t* Device)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	UseDegree = false;

	// Initialize the i2c interface
	if(Config != NULL)
	{
		AD5933_I2CM_INIT(Config);
	}

	// Reset the device to stop frequency sweeps
	ErrorCode = AD5933_Reset();
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set the device into standby mode
	ErrorCode = AD5933_SetMode(AD5933_MODE_STANDBY);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	return AD5933_ConfigDevice(Device);
}

const I2C_Error_t AD5933_Reset(void)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {AD5933_REGISTER_CONTROL + 1, 0x00};

	// Read low byte of the control register
	ErrorCode = AD5933_I2CM_WRITEBYTE(Data[0], false) | AD5933_I2CM_READBYTE(&Data[1], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set reset bit
	Data[1] |= (0x01 << AD5933_RESET); 

	return AD5933_I2CM_WRITEBYTES(2, Data, true);
}

const I2C_Error_t AD5933_SetMode(const AD5933_Mode_t Mode)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {AD5933_REGISTER_CONTROL, 0x00};

	ErrorCode = AD5933_I2CM_WRITEBYTE(Data[0], false) | AD5933_I2CM_READBYTE(&Data[1], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Remove old mode bits
	Data[1] &= 0x0F;

	// Set new mode bits
	Data[1] |= (Mode << 0x04);

	return AD5933_I2CM_WRITEBYTES(2, Data, true);
}

const I2C_Error_t AD5933_GetMode(AD5933_Mode_t* Mode)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {AD5933_REGISTER_CONTROL, 0x00};

	if(Mode == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Read high byte of the control register
	ErrorCode = AD5933_I2CM_WRITEBYTE(Data[0], false) | AD5933_I2CM_READBYTE(&Data[1], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	*Mode = (Data[1] & 0xF0) >> 0x04;

	return ErrorCode;
}

const I2C_Error_t AD5933_SetClockSource(const AD5933_Clock_t Clock)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {AD5933_REGISTER_CONTROL + 1, 0x00};

	// Read low byte of the control register
	ErrorCode = AD5933_I2CM_WRITEBYTE(Data[0], false) | AD5933_I2CM_READBYTE(&Data[1], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set the gain
	if(Clock == AD5933_CLOCK_INT)
	{
		Data[1] &= ~(0x01 << 0x00);
	}
	else
	{
		Data[1] |= (0x01 << 0x00);
	}

	return AD5933_I2CM_WRITEBYTES(2, Data, true);
}

const I2C_Error_t AD5933_ReadStatus(uint8_t* Status)
{
	if(Status == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	return AD5933_I2CM_WRITEBYTE(AD5933_REGISTER_STATUS, true) | AD5933_I2CM_READBYTE(Status, false);
}

const I2C_Error_t AD5933_SetVoltage(const AD5933_OutputVoltage_t Voltage)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {AD5933_REGISTER_CONTROL, 0x00};
	 
	// Read old configuration
	ErrorCode = AD5933_I2CM_WRITEBYTE(Data[0], false) | AD5933_I2CM_READBYTE(&Data[1], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	 
	// Clear the old voltage bits
	Data[1] &= ~(0x03 << 0x01);
	 
	// Set the new voltage bits
	Data[1] |= (Voltage << 0x01);

	return AD5933_I2CM_WRITEBYTES(2, Data, true);
}
 
const I2C_Error_t AD5933_GetVoltage(AD5933_OutputVoltage_t* Voltage)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {AD5933_REGISTER_CONTROL, 0x00};

	if(Voltage == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Read configuration
	ErrorCode = AD5933_I2CM_WRITEBYTE(Data[0], false) | AD5933_I2CM_READBYTE(&Data[1], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Filter the voltage bits
	*Voltage = (Data[1] & 0x06) >> 0x01;
	
	return ErrorCode;
}

const I2C_Error_t AD5933_SetGain(const AD5933_Gain_t Gain)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {AD5933_REGISTER_CONTROL, 0x00};
	 
	// Read old config
	ErrorCode = AD5933_I2CM_WRITEBYTE(Data[0], false) | AD5933_I2CM_READBYTE(&Data[1], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	 
	// Set the gain
	if(Gain == AD5933_GAIN_5)
	{
		Data[1] &= ~(0x01 << 0x00);
	}
	else
	{
		Data[1] |= (0x01 << 0x00);
	}
 
	return AD5933_I2CM_WRITEBYTES(2, Data, true);
}
 
const I2C_Error_t AD5933_GetGain(AD5933_Gain_t* Gain)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	uint8_t Data[2] = {AD5933_REGISTER_CONTROL, 0x00};

	if(Gain == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Read config
	ErrorCode = AD5933_I2CM_WRITEBYTE(Data[0], false) | AD5933_I2CM_READBYTE(&Data[1], true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Filter the gain bit
	*Gain = Data[1] & 0x01;
	 
	return ErrorCode;
}

const I2C_Error_t AD5933_ConfigSettling(const uint16_t Cylcles, const AD5933_SettlingMultiplier_t Mult)
{
	uint8_t Data[4] = {AD5933_CMD_BLOCK_WRITE, 2, (Mult << 0x01) | ((0x100 & Cylcles) >> 0x08), Cylcles & 0xFF};

	return (AD5933_I2CM_WRITEBYTE(AD5933_REGISTER_CYCLES, true) | AD5933_I2CM_WRITEBYTES(sizeof(Data), Data, true));
}

const I2C_Error_t AD5933_SetStartFrequency(const uint32_t Frequency)
{
	uint32_t FrequencyCode = 0x00;
	uint8_t Data[5] = {AD5933_CMD_BLOCK_WRITE, AD5933_SIZE_START_FREQ, 0x00, 0x00, 0x00};

	// Calculate frequency start code
	FrequencyCode = (uint32_t)((Frequency / ((AD5933_MCLK * 1000000) / 4.0)) * (float)134217728);

	Data[4] = FrequencyCode & 0xFF;
	Data[3] = (FrequencyCode >> 0x08) & 0xFF;
	Data[2] = (FrequencyCode >> 0x10) & 0xFF;

	return (AD5933_I2CM_WRITEBYTE(AD5933_REGISTER_START_FREQ, true) | AD5933_I2CM_WRITEBYTES(sizeof(Data), Data, true));
}

const I2C_Error_t AD5933_SetFrequencyIncrement(const uint32_t Increment)
{
	uint32_t FrequencyIncrementCode = 0x00;
	uint8_t Data[5] = {AD5933_CMD_BLOCK_WRITE, AD5933_SIZE_FREQ_INCR, 0x00, 0x00, 0x00};

	// Calculate frequency increment code
	FrequencyIncrementCode = (uint32_t)((Increment / ((AD5933_MCLK * 1000000) / 4.0)) * (float)134217728);

	Data[2] = FrequencyIncrementCode & 0xFF;
	Data[1] = (FrequencyIncrementCode >> 0x08) & 0xFF;
	Data[0] = (FrequencyIncrementCode >> 0x10) & 0xFF;

	return (AD5933_I2CM_WRITEBYTE(AD5933_REGISTER_FREQ_INCR, true) | AD5933_I2CM_WRITEBYTES(sizeof(Data), Data, true));
}

const I2C_Error_t AD5933_SetIncrements(const uint16_t Increments)
{
	uint8_t Data[4] = {AD5933_CMD_BLOCK_WRITE, AD5933_SIZE_INCREMENTS, 0x00, 0x00};

	Data[1] = Increments & 0xFF;
	Data[0] = (Increments >> 0x08) & 0xFF;

	return (AD5933_I2CM_WRITEBYTE(AD5933_REGISTER_INCREMENTS, true) | AD5933_I2CM_WRITEBYTES(sizeof(Data), Data, true));
}

const I2C_Error_t AD5933_GetData(ComplexNumber_t* Output)
{
	uint8_t Data[4];
	uint8_t Status = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(Data == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	while(!(Status & AD5933_STATUS_VALID_DATA))
	{
		ErrorCode = AD5933_ReadStatus(&Status);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}
	
	Data[0] = AD5933_CMD_BLOCK_READ;
	Data[1] = 4;
	ErrorCode = AD5933_I2CM_WRITEBYTE(AD5933_REGISTER_REAL, true) | AD5933_I2CM_WRITEBYTES(2, Data, false) | AD5933_I2CM_READBYTES(sizeof(Data), Data, true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	Output->Real = (Data[0] << 0x08) + Data[1];
	Output->Imag = (Data[2] << 0x08) + Data[3];
	
	return ErrorCode;
}

const I2C_Error_t AD5933_ReadTemperature(int16_t* Temperature)
{
	uint8_t Data[2];
	int16_t Temp = 0x00;
	uint8_t Status = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(Temperature == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Start a new measurement
	ErrorCode = AD5933_SetMode(AD5933_MODE_TEMP);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Wait until a result is available
	ErrorCode = AD5933_ReadStatus(&Status);
	while(!(Status & AD5933_STATUS_VALID_TEMP))
	{
		ErrorCode = AD5933_ReadStatus(&Status);
	}

	Data[0] = AD5933_CMD_BLOCK_READ;
	Data[1] = 4;
	ErrorCode = AD5933_I2CM_WRITEBYTE(AD5933_REGISTER_TEMPERATURE, true) | AD5933_I2CM_WRITEBYTES(2, Data, false) | AD5933_I2CM_READBYTES(sizeof(Data), Data, true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Temp = (Data[0] << 0x08) | Data[1];
	if(Temp > 0x2000)
	{
		Temp -= 0x4000;
	}

	*Temperature = Temp >> 0x05;

	return I2C_NO_ERROR;
}

const I2C_Error_t AD5933_ConfigDevice(const AD5933_Configuration_t* Config)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(Config == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	ErrorCode = AD5933_SetVoltage(Config->OutputVoltage);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = AD5933_SetGain(Config->Gain);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	return AD5933_SetClockSource(Config->ClockSource);
}

const I2C_Error_t AD5933_ConfigSingleFrequency(const AD5933_SingleFrequency_t* Config)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(Config == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Set the start frequency
	ErrorCode = AD5933_SetStartFrequency(Config->Frequency);

	// Read old config
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = AD5933_ConfigSettling(Config->SettlingCycles, Config->Mult);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	return AD5933_SetMode(AD5933_MODE_STANDBY);
}

const I2C_Error_t AD5933_ConfigSweep(const AD5933_SweepSetup_t* SweepSetup)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(SweepSetup == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Set the start frequency
	ErrorCode = AD5933_SetStartFrequency(SweepSetup->StartFrequency);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set frequency increments
	ErrorCode = AD5933_SetFrequencyIncrement(SweepSetup->FrequencyIncrement);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set number of increments
	ErrorCode = AD5933_SetIncrements(SweepSetup->Cycles);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = AD5933_ConfigSettling(SweepSetup->Cycles, SweepSetup->Mult);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	return AD5933_SetMode(AD5933_MODE_STANDBY);
}

void AD5933_EnableDegree(const bool Degree)
{
	if(Degree == true)
	{
		UseDegree = true;
	}
	else
	{
		UseDegree = false;
	}
}

const I2C_Error_t AD5933_SingleCalibration(AD5933_CalPoint_t* CalibrationPoint, const float Reference)
{
	float Mag = 0.0;
	ComplexNumber_t Data;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(CalibrationPoint == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	// Enable the output frequency
	ErrorCode = AD5933_EnableFrequency();
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Get the data
	ErrorCode = AD5933_GetData(&Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Mag = sqrtf((Data.Real * Data.Real) + (Data.Imag * Data.Imag));
	CalibrationPoint->GainFactor = 1.0 / (Mag * Reference);
	CalibrationPoint->Phase = AD5933_CalcPhase(Data.Real, Data.Imag, UseDegree);

	return ErrorCode;
}

const I2C_Error_t AD5933_SingleMeasurement(const AD5933_CalPoint_t* CalibrationPoint, AD5933_DataPoint_t* Impedance)
{
	float Mag = 0.0;
	ComplexNumber_t Data;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if((CalibrationPoint == NULL) || (Impedance == NULL))
	{
		return I2C_INVALID_PARAM;
	}

	ErrorCode = AD5933_SetMode(AD5933_MODE_REPEAT_FREQ);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = AD5933_GetData(&Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Mag = sqrtf((Data.Real * Data.Real) + (Data.Imag * Data.Imag));
	Impedance->Impedance = 1.0 / (Mag * CalibrationPoint->GainFactor);
	Impedance->Phase = AD5933_CalcPhase(Data.Real, Data.Imag, UseDegree) - CalibrationPoint->Phase;

	return AD5933_SetMode(AD5933_MODE_POWER_DOWN);
}

const I2C_Error_t AD5933_SweepCalibration(AD5933_CalPoint_t* CalibrationPoint, const float Reference)
{
	float Mag = 0.0;
	ComplexNumber_t Data;
	uint8_t Status = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(CalibrationPoint == NULL)
	{
		return I2C_INVALID_PARAM;
	}

	ErrorCode = AD5933_EnableFrequency();
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	while(!(Status & AD5933_STATUS_SWEEP_COMPLETE))
	{
		if(Status & AD5933_STATUS_VALID_DATA)
		{
			// Get the data
			ErrorCode = AD5933_GetData(&Data);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}

			Mag = sqrtf((Data.Real * Data.Real) + (Data.Imag * Data.Imag));
			CalibrationPoint->GainFactor = 1.0 / (Mag * Reference);
			CalibrationPoint->Phase = AD5933_CalcPhase(Data.Real, Data.Imag, UseDegree);
			CalibrationPoint++;

			// Increase the frequency
			ErrorCode = AD5933_SetMode(AD5933_MODE_INCR_FREQ);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
		}

		// Update status
		ErrorCode = AD5933_ReadStatus(&Status);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}

	return ErrorCode;
}
 
const I2C_Error_t AD5933_SweepMeasurment(const AD5933_CalPoint_t* CalibrationPoint, AD5933_DataPoint_t* Impedance)
{
	float Mag = 0.0;
	ComplexNumber_t Data;
	uint8_t Status = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if((CalibrationPoint == NULL) || (Impedance == NULL))
	{
		return I2C_INVALID_PARAM;
	}

	ErrorCode = AD5933_EnableFrequency();
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	while(!(Status & AD5933_STATUS_SWEEP_COMPLETE))
	{
		if(Status & AD5933_STATUS_VALID_DATA)
		{
			// Get the data
			ErrorCode = AD5933_GetData(&Data);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}

			Mag = sqrtf((Data.Real * Data.Real) + (Data.Imag * Data.Imag));
			Impedance->Impedance = 1.0 / (Mag * CalibrationPoint->GainFactor);
			Impedance->Phase = AD5933_CalcPhase(Data.Real, Data.Imag, UseDegree) - CalibrationPoint->Phase;
			CalibrationPoint++;
			Impedance++;

			// Increase the frequency
			ErrorCode = AD5933_SetMode(AD5933_MODE_INCR_FREQ);
			if(ErrorCode != I2C_NO_ERROR)
			{
				return ErrorCode;
			}
		}

		// Update status
		ErrorCode = AD5933_ReadStatus(&Status);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}

	return AD5933_SetMode(AD5933_MODE_POWER_DOWN);
}

const I2C_Error_t AD5933_EnableFrequency(void)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	// Turn device into standby mode
	ErrorCode = AD5933_SetMode(AD5933_MODE_STANDBY);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Reset the device to interrupt an active sweep
	ErrorCode = AD5933_Reset();
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Initialize with start frequency
	ErrorCode = AD5933_SetMode(AD5933_MODE_INIT_FREQ);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	return AD5933_SetMode(AD5933_MODE_START_FREQ);
}