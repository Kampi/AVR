/*
 * DS1307.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for DS1307 RTC.

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

/** @file Peripheral/DS1307/DS1307.c
 *  @brief Driver for Dallas DS1307 real time clock.
 *
 *  This file contains the implementation of the RTC driver.
 *
 *  @author Daniel Kampert
 */

#include <stddef.h>

#include "Peripheral/__Rework__/DS1307/DS1307.h"

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

#if(MCU_ARCH == MCU_ARCH_AVR8)
	#define DS1307_I2CM_INIT(Config)							I2CM_Init(Config)
	#define DS1307_I2CM_WRITEBYTE(Data, Stop)					I2CM_WriteByte(DS1307_ADDRESS, Data, Stop)
	#define DS1307_I2CM_READBYTE(Data, Stop)					I2CM_ReadByte(DS1307_ADDRESS, Data, Stop)
	#define DS1307_I2CM_WRITEBYTES(Length, Data, Stop)			I2CM_WriteBytes(DS1307_ADDRESS, Length, Data, Stop)
	#define DS1307_I2CM_READBYTES(Length, Data, Stop)			I2CM_ReadBytes(DS1307_ADDRESS, Length, Data, Stop)
#elif(MCU_ARCH == MCU_ARCH_XMEGA)
	#define DS1307_I2CM_INIT(Config)							I2CM_Init(Config)
	#define DS1307_I2CM_WRITEBYTE(Data, Stop)					I2CM_WriteByte(&DS1307_INTERFACE, DS1307_ADDRESS, Data, Stop)
	#define DS1307_I2CM_READBYTE(Data, Stop)					I2CM_ReadByte(&DS1307_INTERFACE, DS1307_ADDRESS, Data, Stop)
	#define DS1307_I2CM_WRITEBYTES(Length, Data, Stop)			I2CM_WriteBytes(&DS1307_INTERFACE, DS1307_ADDRESS, Length, Data, Stop)
	#define DS1307_I2CM_READBYTES(Length, Data, Stop)			I2CM_ReadBytes(&DS1307_INTERFACE, DS1307_ADDRESS, Length, Data, Stop)
#else
	#error "Architecture not supported for DS1307!"
#endif

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

#if(defined DS1307_USE_IRQ)
	const I2C_Error_t DS1307_EnableInterrupts(DS1307_SQWFreq_t Frequency)
	{
		uint8_t Data = 0x00;
		I2C_Error_t ErrorCode = I2C_NO_ERROR;

		// Define the interrupt configuration, depending on the architecture
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			GPIO_InterruptConfig_t ConfigGPIO = {
				.Port = GET_PERIPHERAL(DS1307_INT),
				.Pin = GET_INDEX(DS1307_INT),
				.Channel = DS1307_INT_CHANNEL,
				.InterruptLevel = DS1307_INT_PRIO,
				.Sense = DS1307_INT_SENSE,
				.Callback = DS1307_Interrupthandler
			};
		#else
			GPIO_InterruptConfig_t ConfigGPIO = {
				.Channel = DS1307_INT_CHANNEL,
				.Sense = DS1307_INT_SENSE,
				.Callback = DS1307_Interrupthandler
			};
		#endif
	
		GPIO_SetDirection(GET_PERIPHERAL(DS1307_INT), GET_INDEX(DS1307_INT), GPIO_DIRECTION_IN);
	
		// Enable pull up, depending on architecture
		#if(DS1307_INT_PULLUP)
			#if(MCU_ARCH == MCU_ARCH_XMEGA)
				GPIO_SetPullConfig(GET_PERIPHERAL(DS1307_INT), GET_INDEX(DS1307_INT), GPIO_OUTPUTCONFIG_PULLUP);
			#else
				GPIO_Set(GET_PERIPHERAL(DS1307_INT), GET_INDEX(DS1307_INT));
			#endif
		#endif

		GPIO_InstallCallback(&ConfigGPIO);
	
		ErrorCode = DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_CONTROL, FALSE) | DS1307_I2CM_READBYTE(&Data, TRUE);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	
		// Set interrupt enable bit
		Data |= (0x01 << 0x04);
	
		// Set new output frequency
		Data &= ~0x03;
		Data |= Frequency;

		return DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_CONTROL, TRUE) | DS1307_I2CM_WRITEBYTE(Data, TRUE);
	}

	const I2C_Error_t DS1307_DisableInterrupts(void)
	{
		uint8_t Data = 0x00;
		I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
		// Disable GPIO interrupt
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			GPIO_RemoveCallback(GET_PERIPHERAL(DS1307_INT), DS1307_INT_CHANNEL);
		#else
			GPIO_RemoveCallback(DS1307_INT_CHANNEL);
		#endif

		ErrorCode = DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_CONTROL, FALSE) | DS1307_I2CM_READBYTE(&Data, TRUE);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}

		// Clear SQW enable bit
		Data &= ~(0x01 << 0x04);

		return DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_CONTROL, TRUE) | DS1307_I2CM_WRITEBYTE(Data, TRUE);
	}
#endif

const I2C_Error_t DS1307_Init(I2CM_Config_t* Config, const DS1307_Config_t* RTCConfig)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(Config != NULL)
	{
		DS1307_I2CM_INIT(Config);
	}

	#if(defined DS1307_USE_IRQ)
	{
		ErrorCode = DS1307_EnableInterrupts(RTCConfig->Freq);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}
	#endif

	if(RTCConfig->Time != NULL)
	{
		ErrorCode = DS1307_SetTime(RTCConfig->Time);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}

	return DS1307_SetHourMode(RTCConfig->Mode);
}

const I2C_Error_t DS1307_HoldClock(void)
{
	uint8_t Data = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR; 

	ErrorCode = DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_SECONDS, FALSE) | DS1307_I2CM_READBYTE(&Data, TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set CH-Bit
	Data |= (0x01 << 0x07);

	return DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_SECONDS, TRUE) | DS1307_I2CM_WRITEBYTE(Data, TRUE);
}

const I2C_Error_t DS1307_SetHourMode(const HourMode_t Mode)
{
	uint8_t Data = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR; 

	ErrorCode = DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_HOURS, FALSE) | DS1307_I2CM_READBYTE(&Data, TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Change from 24-Hour mode to 12-Hour mode
	if(Mode == MODE_12_HOUR)
	{
		Data = BCD2Dec(Data);

		// Check if AM or PM
		if(Data > 12)
		{
			Data -= 12;
			Data |= (0x01 << 0x05);
		}
		else
		{
			Data &= ~(0x01 << 0x05);
		}
		
		Data = Dec2BCD(Data);

		// Set 12-Hour mode
		Data |= (0x01 << 0x06);
	}
	// Change from 12-Hour mode to 24-Hour mode
	else if(Mode == MODE_24_HOUR)
	{
		// Set 24-Hour mode
		Data &= ~(0x01 << 0x06);

		if(Data & (0x01 << 0x05))
		{
			Data &= ~(0x01 << 0x05);
			Data += 0x12;
		}
	}

	return DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_HOURS, TRUE) | DS1307_I2CM_WRITEBYTE(Data, TRUE);
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
	uint8_t Data[8];

	Data[0] = 0x00;
	Data[1] = Dec2BCD(Time->Second);
	Data[2] = Dec2BCD(Time->Minute);
	Data[3] = Dec2BCD(Time->Hour);
	Data[4] = Dec2BCD(Time->DayOfWeek);
	Data[5] = Dec2BCD(Time->Day);
	Data[6] = Dec2BCD(Time->Month);
	Data[7] = Dec2BCD(Time->Year);

	// Set hour mode
	if(Time->HourMode == MODE_12_HOUR)
	{
		// Check if time is valid and correct it, if necessary
		if(Time->Hour > 12)
		{
			Data[(int)offsetof(Time_t, Hour)] = Time->Hour - 12;
		}

		Data[(int)offsetof(Time_t, Hour)] |= (0x01 << 0x06) | (Time->MeridiemMode << 0x05);
	}

	return DS1307_I2CM_WRITEBYTES(sizeof(Data), Data, TRUE);
}

const I2C_Error_t DS1307_GetTime(Time_t* Time)
{
	uint8_t Buffer[7];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode = DS1307_I2CM_WRITEBYTE(0x00, FALSE) | DS1307_I2CM_READBYTES(sizeof(Buffer), Buffer, TRUE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

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