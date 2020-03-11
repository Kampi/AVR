/*
 * DS18B20.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the DS18B20 1-Wire temperature sensor.

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

/** @file Peripheral/DS18B20/DS18B20.c
 *  @brief Driver for the Maxim Integrated DS18B20 1-Wire temperature sensor.
 *
 *  This file contains the implementation of the temperature sensor driver.
 *
 *  @author Daniel Kampert
 */

/** @defgroup DS18B20
 *  @{
 */
 	/** @defgroup DS18B20-Commands
	 *  DS18B20 commands.
	 *  @{
	 */
		#define DS18B20_CONVERT_T							0x44
		#define DS18B20_COPY_SCRATCHPAD						0x48
		#define DS18B20_WRITE_SCRATCHPAD					0x4E
		#define DS18B20_READ_POWER_SUPPLY					0xB4
		#define DS18B20_RECALL_EE							0xB8
		#define DS18B20_READ_SCRATCHPAD						0xBE
	/** @} */ // end of DS18B20-Commands
/** @} */ // end of DS18B20

#include "Peripheral/DS18B20/DS18B20.h"

/** @brief			Search all available DS18B20 devices on the bus.
 *  @param Found	Pointer to devices found
 *  @param Search	Maximum devices to search
 *  @param ROM		Pointer to #OneWire_ROM_t array
 *  @param isAlarm	#TRUE when a alarm search should be done
 *  @return			1-Wire error
 */
static OneWire_Error_t DS18B20_SearchDevices(uint8_t* Found, uint8_t Search, OneWire_ROM_t* ROM, const Bool_t isAlarm)
{
	uint8_t DevicesFound = 0x00;

	if(Found == NULL)
	{
		return ONEWIRE_PARAMETER_ERROR;
	}

	OneWire_Error_t ErrorCode = OneWire_StartSearch(ROM, isAlarm);
	if(ErrorCode == ONEWIRE_NO_ERROR)
	{
		if(ROM->FamilyCode == DS18B20_ID)
		{
			DevicesFound++;
			ROM++;
		}

		// Search additional devices
		while((!OneWire_IsLast()) && (DevicesFound < Search))
		{
			ErrorCode = OneWire_SearchNext(ROM);
			if(ErrorCode != ONEWIRE_NO_ERROR)
			{
				OneWire_StopSearch();
				break;
			}
			
			if(ROM->FamilyCode == DS18B20_ID)
			{
				DevicesFound++;
				ROM++;
			}
		}
	}

	*Found = DevicesFound;

	return ErrorCode;
}

OneWire_Error_t DS18B20_Init(void)
{
	#if(defined(ONEWIRE_USE_EXT_PULL))
		// Configure the switching output for the external pull-up as output
		GPIO_SetDirection(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL), GPIO_DIRECTION_OUT);
	#endif

	return OneWire_Init();
}

OneWire_Error_t DS18B20_GetDevices(uint8_t* Found, uint8_t Search, OneWire_ROM_t* ROM)
{
	return DS18B20_SearchDevices(Found, Search, ROM, FALSE);
}

OneWire_Error_t DS18B20_GetAlarmDevices(uint8_t* Found, uint8_t Search, OneWire_ROM_t* ROM)
{
	return DS18B20_SearchDevices(Found, Search, ROM, TRUE);
}

OneWire_Error_t DS18B20_GetROM(const OneWire_ROM_t* ROM)
{	
	OneWire_Error_t ErrorCode = OneWire_ReadROM(ROM);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	// Compare the CRC values
	uint8_t* pROM = (uint8_t*)ROM;
	if(OneWire_CRC(0x07, pROM) != ROM->Checksum)
	{
		return ONEWIRE_CRC_ERROR;
	}

	return ONEWIRE_NO_ERROR;
}

OneWire_Error_t DS18B20_WriteScratchpad(const OneWire_ROM_t* ROM, const uint8_t TH, const uint8_t TL, const uint8_t Config)
{
	OneWire_Error_t ErrorCode = OneWire_SelectDevice(ROM);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	OneWire_WriteByte(DS18B20_WRITE_SCRATCHPAD);
	OneWire_WriteByte(TH);
	OneWire_WriteByte(TL);
	OneWire_WriteByte(Config);

	return ONEWIRE_NO_ERROR;
}

OneWire_Error_t DS18B20_SaveScratchpad(const OneWire_ROM_t* ROM)
{	
	OneWire_Error_t ErrorCode = ONEWIRE_NO_ERROR;

	ErrorCode = OneWire_SelectDevice(ROM);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	#if(defined(ONEWIRE_USE_EXT_PULL))
		OneWire_PowerState_t UseParasite;
		
		// Check the power state of the device
		ErrorCode = DS18B20_CheckSupply(ROM, &UseParasite);
		if(ErrorCode != ONEWIRE_NO_ERROR)
		{
			return ErrorCode;
		}
		
		// Enable the external pull up when at least one device is powered over the parasite mode
		if(UseParasite == ONEWIRE_POWER_PARSITIC)
		{
			#if(defined(ONEWIRE_EXT_PULL_ACTIVE_LOW))
				GPIO_Clear(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#else
				GPIO_Set(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#endif
		}
	#endif
	
	OneWire_WriteByte(DS18B20_COPY_SCRATCHPAD);

	// Wait until the device has finished
	_delay_ms(10);

	#if(defined(ONEWIRE_USE_EXT_PULL))
		if(UseParasite == ONEWIRE_POWER_PARSITIC)
		{
			// Disable the external pull-up
			#if(defined(ONEWIRE_EXT_PULL_ACTIVE_LOW))
				GPIO_Set(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#else
				GPIO_Clear(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#endif
		}
	#endif

	return ONEWIRE_NO_ERROR;
}

OneWire_Error_t DS18B20_LoadScratchpad(const OneWire_ROM_t* ROM)
{
	uint8_t Status = 0x00;

	OneWire_Error_t ErrorCode = OneWire_SelectDevice(ROM);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	#if(defined(ONEWIRE_USE_EXT_PULL))
		OneWire_PowerState_t UseParasite;

		// Check the power state of the device
		ErrorCode = DS18B20_CheckSupply(ROM, &UseParasite);
		if(ErrorCode != ONEWIRE_NO_ERROR)
		{
			return ErrorCode;
		}

		// Enable the external pull up when at least one device is powered over the parasite mode
		if(UseParasite == ONEWIRE_POWER_PARSITIC)
		{
			#if(defined(ONEWIRE_EXT_PULL_ACTIVE_LOW))
				GPIO_Clear(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#else
				GPIO_Set(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#endif
		}
	#endif

	OneWire_WriteByte(DS18B20_RECALL_EE);

	#if(defined(ONEWIRE_USE_EXT_PULL))
		// Only use the delay when at least one device is powered over the parasite mode
		if(UseParasite == ONEWIRE_POWER_PARSITIC)
		{
			// Wait until the device has finished
			_delay_ms(10);
		
			// Disable the external pull-up
			#if(defined(ONEWIRE_EXT_PULL_ACTIVE_LOW))
				GPIO_Set(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#else
				GPIO_Clear(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#endif
		}
		else
		{
			do
			{
				Status = OneWire_ReadBit();
			}while(Status == 0x00);
		}
	#else
		do
		{
			Status = OneWire_ReadBit();
		}while(Status == 0x00);
	#endif

	return ONEWIRE_NO_ERROR;
}

OneWire_Error_t DS18B20_ReadScratchpad(const OneWire_ROM_t* ROM, const uint8_t Length, DS18B20_Scratchpad_t* Data)
{
	uint8_t* Data_Temp = (uint8_t*)Data;

	if(Data_Temp == NULL)
	{
		return ONEWIRE_PARAMETER_ERROR;
	}

	OneWire_Error_t ErrorCode = OneWire_SelectDevice(ROM);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	OneWire_WriteByte(DS18B20_READ_SCRATCHPAD);

	// Read the scratchpad
	for(uint8_t i = 0x00; i < Length; i++)
	{
		// Data is transmitted LSB first.
		*(Data_Temp + i) = OneWire_ReadByte();
	}

	// Abort transmission when all data are read
	if(Length < DS18B20_SCRATCHPAD_SIZE)
	{
		return OneWire_Reset();
	}

	return ONEWIRE_NO_ERROR;
}

OneWire_Error_t DS18B20_Measure(const OneWire_ROM_t* ROM, const DS18B20_Resolution_t Resolution, double* Temperature)
{
	uint8_t Temp[2];
	uint8_t Configuration[5];

	if(Temperature == NULL)
	{
		return ONEWIRE_PARAMETER_ERROR;
	}

	// Read the scratchpad to get TH, TL and the configuration value
	OneWire_Error_t ErrorCode = DS18B20_ReadScratchpad(ROM, sizeof(Configuration), (DS18B20_Scratchpad_t*)Configuration);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	// Set the resolution
	Configuration[4] &= ~0x60;
	Configuration[4] |= (Resolution & 0x03) << 0x05;

	// Write the configuration to the device
	ErrorCode = DS18B20_WriteScratchpad(ROM, Configuration[2], Configuration[3], Configuration[4]);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	#if(defined(ONEWIRE_USE_EXT_PULL))
		OneWire_PowerState_t UseParasite;

		// Check the power state of the device
		ErrorCode = DS18B20_CheckSupply(ROM, &UseParasite);
		if(ErrorCode != ONEWIRE_NO_ERROR)
		{
			return ErrorCode;
		}
	#endif

	// Start a new temperature measurement
	ErrorCode = OneWire_SelectDevice(ROM);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}
	OneWire_WriteByte(DS18B20_CONVERT_T);

	// Wait for the end of conversion
	#if(defined(ONEWIRE_USE_EXT_PULL))
		// Only use the delay when parasite power is used. Otherwise poll the device.
		if(UseParasite == ONEWIRE_POWER_PARSITIC)
		{
			// Enable the external pull up
			#if(defined(ONEWIRE_EXT_PULL_ACTIVE_LOW))
				GPIO_Clear(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#else
				GPIO_Set(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#endif

			if(Resolution == DS18B20_RESOLUTION_9)		_delay_ms(100);
			else if(Resolution == DS18B20_RESOLUTION_9)	_delay_ms(200);
			else if(Resolution == DS18B20_RESOLUTION_9)	_delay_ms(400);
			else										_delay_ms(8000);
			
			// Disable the external pull up
			#if(defined(ONEWIRE_EXT_PULL_ACTIVE_LOW))
				GPIO_Set(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#else
				GPIO_Clear(GET_PERIPHERAL(ONEWIRE_EXT_PULL), GET_INDEX(ONEWIRE_EXT_PULL));
			#endif
		}
		else
		{
			uint8_t Status = 0x00;

			do
			{
				Status = OneWire_ReadBit();
			}while(Status == 0x00);
		}
	#else
		uint8_t Status = 0x00;

		do
		{
			Status = OneWire_ReadBit();
		}while(Status == 0x00);
	#endif

	// Get the temperature data
	ErrorCode = DS18B20_ReadScratchpad(ROM, sizeof(Temp), (DS18B20_Scratchpad_t*)Temp);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	#if(defined(MCU_BIG_ENDIAN))
		*Temp = ENDIAN_SWAP_16(*Temp);
	#endif

	*Temperature = (float)((Temp[1] << 0x08) | Temp[0]) * 0.0625;

	return ONEWIRE_NO_ERROR;
}

OneWire_Error_t DS18B20_SetAlarm(const OneWire_ROM_t* ROM, const int8_t Max, const int8_t Min)
{
	DS18B20_Scratchpad_t Temp;

	// Get the config
	OneWire_Error_t ErrorCode = OneWire_SelectDevice(ROM) | DS18B20_ReadScratchpad(ROM, 5, &Temp);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	return DS18B20_WriteScratchpad(ROM, Max, Min, Temp.Config);
}

OneWire_Error_t DS18B20_CheckSupply(const OneWire_ROM_t* ROM, OneWire_PowerState_t* State)
{
	if(State == NULL)
	{
		return ONEWIRE_PARAMETER_ERROR;
	}

	OneWire_Error_t ErrorCode = ErrorCode = OneWire_SelectDevice(ROM);
	if(ErrorCode != ONEWIRE_NO_ERROR)
	{
		return ErrorCode;
	}

	OneWire_WriteByte(DS18B20_READ_POWER_SUPPLY);

	*State = OneWire_ReadBit();

	return ErrorCode;
}