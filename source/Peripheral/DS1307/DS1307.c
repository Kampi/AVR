/*
 * DS1307.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Dallas DS1307 RTC

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

/** @file DS1307.c
 *  @brief Driver for DS1307 real time clock.
 *
 *  This file contains the implementation of the RTC driver.
 *
 *  @author Daniel Kampert
 */

#include <stddef.h>

#include "Peripheral/DS1307/DS1307.h"

/** @defgroup DS1307
 *  @{
 */
	/** @defgroup DS1307-Register
	 *  DS1307 register addresses.
	 *  @{
	 */
		#define DS1307_REGISTER_SECONDS			0x00
		#define DS1307_REGISTER_MINUTES			0x01
		#define DS1307_REGISTER_HOURS			0x02
		#define DS1307_REGISTER_DAY				0x03
		#define DS1307_REGISTER_DATE			0x04
		#define DS1307_REGISTER_MONTH			0x05
		#define DS1307_REGISTER_YEAR			0x06
		#define DS1307_REGISTER_CONTROL			0x07
	/** @} */ // end of DS1307-Register
	 
 	/** @defgroup DS1307-Control
	 *  DS1307 control bits.
	 *  @{
	 */ 
		#define DS1307_OUT						0x07
		#define DS1307_SQWE						0x04
		#define DS1307_RS1						0x01
		#define DS1307_RS0						0x00
	/** @} */ // end of DS1307-Control
/** @} */ // end of DS1307


#ifndef DOXYGEN
	struct
	{
		DS1307_Callback_t DS1307_SQW_Callback;
	} DS1307_Callbacks;
#endif

/** @brief	DS1307 interrupt handler.
 */
static inline void DS1307_Interrupthandler(void)
{
	Time_t CurrentTime;
	
	// Get the current time from the RTC
	I2C_Error_t ErrorCode = DS1307_GetTime(&CurrentTime);
	if(ErrorCode != I2C_NO_ERROR)
	{
		// Return zero if error occurs
		CurrentTime = (Time_t){0, 0, 0, 0, 0, 0, 0, 0, 0};
	}

	if(DS1307_Callbacks.DS1307_SQW_Callback != NULL)
	{
		DS1307_Callbacks.DS1307_SQW_Callback(CurrentTime);
	}
}

const I2C_Error_t DS1307_EnableInterrupts(DS1307_SQWFreq_t Frequency)
{
	uint8_t Dummy = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	// Define the interrupt configuration, depending on the architecture
	#if(MCU_ARCH == MCU_ARCH_XMEGA)
		GPIO_InterruptConfig_t ConfigGPIO = {
			.Port = DS1307_INT_PORT,
			.Pin = DS1307_INT_PIN,
			.Channel = DS1307_INT_CHANNEL,
			.InterruptLevel = DS1307_INT_PRIORITY,
			.Type = DS1307_INT_SENSE,
			.Callback = DS1307_Interrupthandler
		};
	#else
		GPIO_InterruptConfig_t ConfigGPIO = {
			.Channel = DS1307_INT_CHANNEL,
			.Type = DS1307_INT_SENSE,
			.Callback = DS1307_Interrupthandler
		};
	#endif
	
	GPIO_SetDirection(DS1307_INT_PORT, DS1307_INT_PIN, GPIO_DIRECTION_IN);
	
	// Enable pull up, depending on architecture
	#if(DS1307_INT_PULLUP)
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			GPIO_PullConfig(DS1307_INT_PORT, DS1307_INT_PIN, GPIO_OUTPUTCONFIG_PULLUP);
		#else
			GPIO_Set(DS1307_INT_PORT, DS1307_INT_PIN);
		#endif
	#endif

	GPIO_InstallCallback(&ConfigGPIO);
	
	ErrorCode = DS1307_I2C_READREG(DS1307_REGISTER_CONTROL, &Dummy);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Set interrupt enable bit
	Dummy |= (0x01 << 0x04);
	
	// Set new output frequency
	Dummy &= ~0x03;
	Dummy |= Frequency;

	// Write new control byte
	ErrorCode = DS1307_I2C_WRITEREG(DS1307_REGISTER_CONTROL, &Dummy);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	return DS1307_I2C_READREG(DS1307_REGISTER_CONTROL, &Dummy);
}

const I2C_Error_t DS1307_DisableInterrupts(void)
{
	uint8_t Dummy = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	// Disable GPIO interrupt
	#if(MCU_ARCH == MCU_ARCH_XMEGA)
		GPIO_RemoveCallback(DS1307_INT_PORT, DS1307_INT_CHANNEL);
	#else
		GPIO_RemoveCallback(DS1307_INT_CHANNEL);
	#endif

	ErrorCode = DS1307_I2C_READREG(DS1307_REGISTER_CONTROL, &Dummy);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Clear SQW enable bit
	Dummy &= ~(0x01 << 0x04);

	return DS1307_I2C_WRITEREG(DS1307_REGISTER_CONTROL, &Dummy);
}

const I2C_Error_t DS1307_Init(I2CM_Config_t* Config, const Time_t* Time)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	if(Config != NULL)
	{
		DS1307_I2C_INIT(Config);
	}

	if(DS1307_USE_IRQ)
	{
		ErrorCode = DS1307_EnableInterrupts(DS1307_INT_FREQUENCY);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
}

	if(Time != NULL)
	{
		ErrorCode = DS1307_SetTime(Time);
	}
	
	return ErrorCode;
}

const I2C_Error_t DS1307_HoldClock(void)
{
	uint8_t Dummy = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR; 
	
	ErrorCode = DS1307_I2C_READREG(DS1307_REGISTER_SECONDS, &Dummy);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set CH-Bit
	Dummy |= (0x01 << 0x07);

	return DS1307_I2C_WRITEREG(DS1307_REGISTER_SECONDS, &Dummy);
}

const I2C_Error_t DS1307_ChangeHourMode(const HourMode_t Mode)
{
	uint8_t HourRegister = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR; 

	ErrorCode = DS1307_I2C_READREG(DS1307_REGISTER_HOURS, &HourRegister);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Change from 24-Hour mode to 12-Hour mode
	if(Mode == MODE_12_HOUR)
	{
		HourRegister = BCD2Dec(HourRegister);

		// Check if AM or PM
		if(HourRegister > 12)
		{
			HourRegister -= 12;
			HourRegister |= (0x01 << 0x05);
		}
		else
		{
			HourRegister &= ~(0x01 << 0x05);
		}
		
		HourRegister = Dec2BCD(HourRegister);

		// Set 12-Hour mode
		HourRegister |= (0x01 << 0x06);
	}
	// Change from 12-Hour mode to 24-Hour mode
	else if(Mode == MODE_24_HOUR)
	{
		// Set 24-Hour mode
		HourRegister &= ~(0x01 << 0x06);

		if(HourRegister & (0x01 << 0x05))
		{
			HourRegister &= ~(0x01 << 0x05);
			HourRegister += 18;
		}
	}

	return DS1307_I2C_WRITEREG(DS1307_REGISTER_HOURS, &HourRegister);
}

void DS1307_InstallCallback(DS1307_Callback_t Callback)
{
	DS1307_Callbacks.DS1307_SQW_Callback = Callback;
}

void DS1307_RemoveCallback(void)
{
	DS1307_Callbacks.DS1307_SQW_Callback = NULL;
}

const I2C_Error_t DS1307_SetTime(const Time_t* Time)
{
	uint8_t* Buffer = (uint8_t*)Time;

	// Convert the values
	for(uint8_t i = 0x00; i < 0x07; i++)
	{
		Buffer[i] = Dec2BCD(Buffer[i]);
	}

	// Set hour mode
	if(Time->HourMode == MODE_12_HOUR)
	{
		// Check if time is valid and correct it, if necessary
		if(Time->Hour > 12)
		{
			Buffer[(int)offsetof(Time_t, Hour)] = Time->Hour - 12;
		}

		Buffer[(int)offsetof(Time_t, Hour)] |= (0x01 << 0x06) | (Time->MeridiemMode << 0x05);
	}

	return DS1307_I2C_WRITE(0x00, 0x07, Buffer);
}

const I2C_Error_t DS1307_GetTime(Time_t* Time)
{
	uint8_t Buffer[7];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode = DS1307_I2C_READ(0x00, 0x07, Buffer);

	Time->Second = BCD2Dec(Buffer[(int)offsetof(Time_t, Second)]);
	Time->Minute = BCD2Dec(Buffer[(int)offsetof(Time_t, Minute)]);

	// Check if 12 hour mode is enabled
	if(Buffer[(int)offsetof(Time_t, Hour)] & (0x01 << 0x06))
	{
		Time->HourMode = MODE_12_HOUR;
		Time->MeridiemMode = (Time->Hour & (0x01 << 0x05)) >> 0x05;

		Buffer[(int)offsetof(Time_t, Hour)] &= ~((0x01 << 0x05) | (0x01 << 0x06));
		
		Time->Hour = BCD2Dec(Buffer[(int)offsetof(Time_t, Hour)]);

		if(Time->Hour > 12)
		{
			Time->Hour -= 12;
		}
	}
	else
	{
		Time->HourMode = MODE_24_HOUR;
		Time->Hour = BCD2Dec(Buffer[(int)offsetof(Time_t, Hour)]);
	}

	Time->DayOfWeek = BCD2Dec(Buffer[(int)offsetof(Time_t, DayOfWeek)]);
	Time->Day = BCD2Dec(Buffer[(int)offsetof(Time_t, Day)]);
	Time->Month = BCD2Dec(Buffer[(int)offsetof(Time_t, Month)]);
	Time->Year = BCD2Dec(Buffer[(int)offsetof(Time_t, Year)]);

	return ErrorCode;	
}