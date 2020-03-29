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
		#define DS1307_12_24					0x06
		#define DS1307_MERIDIEM					0x05
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
	#if(defined DS1307_USE_IRQ)
		struct
		{
			DS1307_Callback_t DS1307_SQW_Callback;
		} __RTCCallbacks;
	#endif
#endif

/** @brief	DS1307 interrupt handler.
 */
#if(defined DS1307_USE_IRQ)
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

		if(__RTCCallbacks.DS1307_SQW_Callback != NULL)
		{
			__RTCCallbacks.DS1307_SQW_Callback(CurrentTime);
		}
	}
#endif

#if(defined DS1307_USE_IRQ)
	const I2C_Error_t DS1307_EnableInterrupts(const DS1307_InterruptConfig_t* Config)
	{
		uint8_t Data[2] = {DS1307_REGISTER_CONTROL, 0x00};
		I2C_Error_t ErrorCode = I2C_NO_ERROR;

		// Define the interrupt configuration, depending on the architecture
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			GPIO_InterruptConfig_t ConfigGPIO = {
				.Port = Config->Port,
				.Pin = Config->Pin,
				.Channel = Config->Channel,
				.InterruptLevel = Config->Level,
				.Sense = Config->Sense,
				.Callback = DS1307_Interrupthandler
			};
		#else
			GPIO_InterruptConfig_t ConfigGPIO = {
				.Channel = Config->Channel,
				.Sense = Config->Sense,
				.Callback = DS1307_Interrupthandler
			};
		#endif
	
		GPIO_SetDirection(GET_PERIPHERAL(DS1307_INT), GET_INDEX(DS1307_INT), GPIO_DIRECTION_IN);
	
		// Enable pull up, depending on architecture
		#if(defined DS1307_INT_PULLUP)
			#if(MCU_ARCH == MCU_ARCH_XMEGA)
				GPIO_SetPullConfig(GET_PERIPHERAL(DS1307_INT), GET_INDEX(DS1307_INT), GPIO_OUTPUTCONFIG_PULLUP);
			#else
				GPIO_Set(GET_PERIPHERAL(DS1307_INT), GET_INDEX(DS1307_INT));
			#endif
		#endif

		GPIO_InstallCallback(&ConfigGPIO);
		
		if(Config->Callback == NULL)
		{
			return I2C_INVALID_PARAM;
		}
		else
		{
			__RTCCallbacks.DS1307_SQW_Callback = Config->Callback;
		}
	
		ErrorCode = DS1307_I2CM_WRITEBYTE(Data[0], false) | DS1307_I2CM_READBYTE(&Data[1], true);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	
		// Set interrupt enable bit
		Data[1] |= (0x01 << DS1307_SQWE);
	
		// Set new output frequency
		Data[1] &= ~0x03;
		Data[1] |= Config->Freq & 0x03;

		return DS1307_I2CM_WRITEBYTES(sizeof(Data), Data, true);
	}

	const I2C_Error_t DS1307_DisableInterrupts(const DS1307_InterruptConfig_t* Config)
	{
		uint8_t Data[2] = {DS1307_REGISTER_CONTROL, 0x00};
		I2C_Error_t ErrorCode = I2C_NO_ERROR;
		
		// Disable GPIO interrupt
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			GPIO_RemoveCallback(Config->Port, Config->Channel);
		#else
			GPIO_RemoveCallback(Config->Channel);
		#endif

		ErrorCode = DS1307_I2CM_WRITEBYTE(Data[0], false) | DS1307_I2CM_READBYTE(&Data[1], true);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}

		// Clear SQW enable bit
		Data[1] &= ~(0x01 << DS1307_SQWE);

		return DS1307_I2CM_WRITEBYTES(sizeof(Data), Data, true);
	}
#endif

#if(defined DS1307_USE_IRQ)
	const I2C_Error_t DS1307_Init(I2CM_Config_t* Config, const Time_t* Time, DS1307_InterruptConfig_t* IntConfig)
#else
	const I2C_Error_t DS1307_Init(I2CM_Config_t* Config, const Time_t* Time)
#endif
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	if(Config != NULL)
	{
		DS1307_I2CM_INIT(Config);
	}

	#if(defined DS1307_USE_IRQ)
	{
		ErrorCode = DS1307_EnableInterrupts(IntConfig);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}
	#endif

	if(Time != NULL)
	{
		ErrorCode = DS1307_SetTime(Time);
		if(ErrorCode != I2C_NO_ERROR)
		{
			return ErrorCode;
		}
	}

	return ErrorCode;
}

const I2C_Error_t DS1307_HoldClock(void)
{
	uint8_t Data = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR; 

	ErrorCode = DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_SECONDS, false) | DS1307_I2CM_READBYTE(&Data, true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set CH-Bit
	Data |= (0x01 << 0x07);

	return DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_SECONDS, true) | DS1307_I2CM_WRITEBYTE(Data, true);
}

const I2C_Error_t DS1307_SetHourMode(const HourMode_t Mode)
{
	uint8_t Data = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR; 

	ErrorCode = DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_HOURS, false) | DS1307_I2CM_READBYTE(&Data, true);
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
			Data |= (0x01 << DS1307_MERIDIEM);
		}
		else
		{
			Data &= ~(0x01 << DS1307_MERIDIEM);
		}
		
		Data = Dec2BCD(Data);

		// Set 12-Hour mode
		Data |= (0x01 << DS1307_12_24);
	}
	// Change from 12-Hour mode to 24-Hour mode
	else if(Mode == MODE_24_HOUR)
	{
		// Set 24-Hour mode
		Data &= ~(0x01 << DS1307_12_24);

		if(Data & (0x01 << DS1307_MERIDIEM))
		{
			Data &= ~(0x01 << DS1307_MERIDIEM);
			Data += 0x12;
		}
	}

	return DS1307_I2CM_WRITEBYTE(DS1307_REGISTER_HOURS, true) | DS1307_I2CM_WRITEBYTE(Data, true);
}

#if(defined DS1307_USE_IRQ)
	void DS1307_InstallCallback(DS1307_Callback_t Callback)
	{
		__RTCCallbacks.DS1307_SQW_Callback = Callback;
	}

	void DS1307_RemoveCallback(void)
	{
		__RTCCallbacks.DS1307_SQW_Callback = NULL;
	}
#endif

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
		// Check if time is valid and correct it, if necessary. Also set the AM/PM bit
		if(Time->Hour > 12)
		{
			Data[3] = (Time->Hour - 12) | (0x01 << DS1307_MERIDIEM);
		}
		else
		{
			Data[3] &= ~(0x01 << DS1307_MERIDIEM);
		}
		
		// Set the hour mode bit
		Data[3] |= (0x01 << DS1307_12_24);
	}

	return DS1307_I2CM_WRITEBYTES(sizeof(Data), Data, true);
}

const I2C_Error_t DS1307_GetTime(Time_t* Time)
{
	uint8_t Buffer[7];
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode = DS1307_I2CM_WRITEBYTE(0x00, false) | DS1307_I2CM_READBYTES(sizeof(Buffer), Buffer, true);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	Time->Second = BCD2Dec(Buffer[0]);
	Time->Minute = BCD2Dec(Buffer[1]);

	// Check if 12 hour mode is enabled
	if(Buffer[2] & (0x01 << DS1307_12_24))
	{
		Time->HourMode = MODE_12_HOUR;
		Time->MeridiemMode = (Time->Hour >> DS1307_MERIDIEM) & 0x01;

		Buffer[2] &= ~((0x01 << 0x05) | (0x01 << 0x06));
		
		Time->Hour = BCD2Dec(Buffer[2]);

		if(Time->Hour > 12)
		{
			Time->Hour -= 12;
		}
	}
	else
	{
		Time->HourMode = MODE_24_HOUR;
		Time->Hour = BCD2Dec(Buffer[2]);
	}

	Time->DayOfWeek = BCD2Dec(Buffer[3]);
	Time->Day = BCD2Dec(Buffer[4]);
	Time->Month = BCD2Dec(Buffer[5]);
	Time->Year = BCD2Dec(Buffer[6]);

	return ErrorCode;	
}