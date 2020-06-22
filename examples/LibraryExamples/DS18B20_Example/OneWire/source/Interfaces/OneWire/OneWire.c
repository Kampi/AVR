/*
 * OneWire.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: 1-Wire interface driver for AVR.

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

/** @file Interfaces/OneWire/OneWire.c
 *  @brief 1-Wire interface driver for AVR.
 *
 *  This file contains the implementation for the 1-Wire interface.
 *	NOTE: Please check the Maxim Integrated Application note when you need additional information
 *  https://pdfserv.maximintegrated.com/en/an/AN126.pdf
 *
 *  @author Daniel Kampert
 */

#include "Interfaces/OneWire/OneWire.h"

/** @defgroup OneWire
 *  @{
 */
 	/** @defgroup OneWire-Commands
	 *  OneWire commands.
	 *  @{
	 */
		#define ONEWIRE_READ_ROM							0x33
		#define ONEWIRE_MATCH_ROM							0x55
		#define ONEWIRE_SKIP_ROM							0xCC
		#define ONEWIRE_ALARM_SEARCH						0xEC
		#define ONEWIRE_SEARCH_ROM							0xF0
	/** @} */ // end of OneWire-Commands
/** @} */ // end of OneWire

#if(defined(ONEWIRE_USE_OVERDRIVE))
	#define ONEWIRE_DELAY_A									1.0
	#define ONEWIRE_DELAY_B									7.5
	#define ONEWIRE_DELAY_C									7.5
	#define ONEWIRE_DELAY_D									2.5
	#define ONEWIRE_DELAY_E									1.0
	#define ONEWIRE_DELAY_F									7
	#define ONEWIRE_DELAY_G									2.5
	#define ONEWIRE_DELAY_H									70
	#define ONEWIRE_DELAY_I									8.5
	#define ONEWIRE_DELAY_J									40
#else
	#define ONEWIRE_DELAY_A									6
	#define ONEWIRE_DELAY_B									64
	#define ONEWIRE_DELAY_C									60
	#define ONEWIRE_DELAY_D									10
	#define ONEWIRE_DELAY_E									9
	#define ONEWIRE_DELAY_F									55
	#define ONEWIRE_DELAY_G									0
	#define ONEWIRE_DELAY_H									480
	#define ONEWIRE_DELAY_I									70
	#define ONEWIRE_DELAY_J									410
#endif

/*
	Architecture specific definitions
*/
#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#if(ONEWIRE_INTERFACE == INTERFACE_GPIO)
		#include "Arch/XMega/CPU/CPU.h"
		#include "Arch/XMega/GPIO/GPIO.h"
	#else
		#error "Interface not supported for 1-Wire!"
	#endif
#elif(MCU_ARCH == MCU_ARCH_AVR8)
	#if(MCU_FAMILY_TINY0)
		#if(ONEWIRE_INTERFACE == INTERFACE_GPIO)
			#include "Arch/AVR8/tinyAVR/tiny0/CPU/CPU.h"
			#include "Arch/AVR8/tinyAVR/tiny0/GPIO/GPIO.h"
		#elif(ONEWIRE_INTERFACE == INTERFACE_USART)
			#include "Arch/AVR8/tinyAVR/tiny0/USART/USART.h"
		#else
			#error "Interface not supported for 1-Wire!"
		#endif
	#else
		#error "Family not supported for 1-Wire!"
	#endif
#else
	#error "Architecture not supported for 1-Wire!"
#endif

static uint8_t __LastFamilyDiscrepancy;
static uint8_t __LastDiscrepancy;
static Bool_t __LastDevice;
static Bool_t __SearchActive;
static Bool_t __isAlarm;

static const uint8_t __OneWire_CRCTable[] = 
{
	0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
	157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
	35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
	190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
	70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
	219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
	101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
	248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
	140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
	17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
	175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
	50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
	202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
	87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
	233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
	116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

/** @brief			Run a search algorithm to identify each device on the bus.
 *					NOTE: Visit https://www.maximintegrated.com/en/design/technical-documents/app-notes/1/187.html
 *					for more detailed information.
 *  @param ROM		Pointer to array of #OneWire_ROM_t objects
 *  @param isAlarm	#TRUE when a alarm search should be done
 *  @return			1-Wire error
 */
static OneWire_Error_t OneWire_SearchROM(const OneWire_ROM_t* ROM, const Bool_t isAlarm)
{
	OneWire_Error_t ErrorCode = ONEWIRE_NO_ERROR;
	uint8_t* pROM = (uint8_t*)ROM;
	uint8_t id_bit;
	uint8_t cmp_id_bit;
	uint8_t ROM_Byte = 0x00;
	uint8_t CRC8 = 0x00;
	uint8_t search_direction = 0x00;
	uint8_t last_zero = 0x00;
	uint8_t ROM_Mask = 0x01;
	uint8_t id_bit_number = 0x01;

	if(ROM == NULL)
	{
		return ONEWIRE_PARAMETER_ERROR;
	}

	// Last device detected?
	if(!__LastDevice)
	{
		// Reset the devices
		ErrorCode = OneWire_Reset();
		if(ErrorCode != ONEWIRE_NO_ERROR)
		{
			__LastDiscrepancy = 0x00;
			__LastDevice = FALSE;
			__LastDiscrepancy = 0x00;

			return ErrorCode;
		}

		// Transmit the search command
		if(isAlarm == TRUE)
		{
			OneWire_WriteByte(ONEWIRE_ALARM_SEARCH);
		}
		else
		{
			OneWire_WriteByte(ONEWIRE_SEARCH_ROM);
		}

		// Run the search algorithm
		do
		{
			// Read the true value and the complement value of the answer bit
			id_bit = OneWire_ReadBit();
			cmp_id_bit = OneWire_ReadBit();

			// Check the values of the bits (see Table 1 of the algorithm)
			if((id_bit == 0x01) && (cmp_id_bit == 0x01))
			{
				// No devices participating in search
				break;
			}
			else
			{
				if(id_bit == cmp_id_bit)
				{
					if(id_bit_number == __LastDiscrepancy)
					{
						search_direction = 0x01;
					}
					else
					{
						if(id_bit_number > __LastDiscrepancy)
						{
							search_direction = 0x00;
						}
						else
						{
							// Set 'search_direction' to 'id_bit_number' from the current ROM byte
							search_direction = (((*(pROM + ROM_Byte)) & ROM_Mask) > 0);
						}
					}
					
					// If 0 was picked then record its position in LastZero
					if(search_direction == 0x00)
					{
						last_zero = id_bit_number;

						// Check for Last discrepancy in family
						if(last_zero < 0x09)
						{
							__LastFamilyDiscrepancy = last_zero;
						}
					}
				}
				else
				{
					search_direction = id_bit;
				}

				// Set the bit in the ROM byte according to the ROM mask
				if(search_direction == 0x01)
				{
					*(pROM + ROM_Byte) |= ROM_Mask;
				}
				else
				{
					*(pROM + ROM_Byte) &= ~ROM_Mask;
				}

				OneWire_WriteBit(search_direction);

				id_bit_number++;

				// Increase the byte counter when the mask is 0 and reset mask
				ROM_Mask <<= 0x01;
				if(ROM_Mask == 0x00)
				{
					CRC8 = __OneWire_CRCTable[CRC8 ^ *(pROM + ROM_Byte)];
					ROM_Byte++;
					ROM_Mask = 0x01;
				}
			}
		}while(ROM_Byte < 0x08);

		if(!((id_bit_number < 65) || (CRC8 != 0x00)))
		{
			__LastDiscrepancy = last_zero;

			// Check for last device
			if(__LastDiscrepancy == 0x00)
			{
				__LastDevice = TRUE;
				__SearchActive = FALSE;
			}
		}
	}

	// No device found
	if((ErrorCode != ONEWIRE_NO_ERROR) || !(*pROM))
	{
		__LastDiscrepancy = 0x00;
		__LastDevice = FALSE;
		__LastFamilyDiscrepancy = 0x00;
	
		if(!(*pROM))
		{
			ErrorCode = ONEWIRE_CRC_ERROR;
		}
	}

	return ErrorCode;
}

OneWire_Error_t OneWire_Init(void)
{
	__SearchActive = FALSE;

	#if(ONEWIRE_INTERFACE == INTERFACE_GPIO)
		// Set DQ as output and enable the pull up resistor for the input state
		GPIO_SetDirection(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ), GPIO_DIRECTION_OUT);

		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			GPIO_SetPullConfig(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ), GPIO_OUTPUTCONFIG_WIREDANDUP);
			GPIO_Set(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));
		#else
			GPIO_Set(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));
		#endif
	#elif(ONEWIRE_INTERFACE == INTERFACE_USART)
		USART_OneWire_Init();
	#endif

	// Give the devices on the bus some time to initialize
	_delay_us(100);

	return OneWire_Reset();
}

uint8_t OneWire_CRC(const uint8_t Length, const uint8_t* Data)
{
	uint8_t CRC_Temp = 0x00;

	for(uint8_t i = 0x00; i < Length; i++)
	{
		CRC_Temp = __OneWire_CRCTable[CRC_Temp ^ *(Data++)];
	}

	return CRC_Temp;
}

OneWire_Error_t OneWire_StartSearch(const OneWire_ROM_t* ROM, const Bool_t isAlarm)
{
	__LastFamilyDiscrepancy = 0x00;
	__LastDiscrepancy = 0;
	__LastDevice = FALSE;
	__SearchActive = TRUE;
	__isAlarm = isAlarm;

	if(ROM == NULL)
	{
		return ONEWIRE_PARAMETER_ERROR;
	}

	return OneWire_SearchROM(ROM, __isAlarm);
}

OneWire_Error_t OneWire_SearchNext(const OneWire_ROM_t* ROM)
{
	if(__SearchActive == TRUE)
	{
		return OneWire_SearchROM(ROM, __isAlarm);
	}

	return ONEWIRE_INACTIVE_SEARCH;
}

Bool_t OneWire_IsLast(void)
{
	return __LastDevice;
}

OneWire_Error_t OneWire_StopSearch(void)
{
	__SearchActive = FALSE;

	return OneWire_Reset();
}

OneWire_Error_t OneWire_ReadROM(const OneWire_ROM_t* ROM)
{
	if(ROM == NULL)
	{
		return ONEWIRE_PARAMETER_ERROR;
	}

	uint8_t* pROM = (uint8_t*)ROM;

	OneWire_Error_t ErrorCode = OneWire_Reset();
	if(ErrorCode == ONEWIRE_NO_DEVICE)
	{
		return ErrorCode;
	}

	OneWire_WriteByte(ONEWIRE_READ_ROM);

	for(uint8_t i = 0x00; i < sizeof(OneWire_ROM_t); i++)
	{
		*(pROM++) = OneWire_ReadByte();
	}

	return ONEWIRE_NO_ERROR;
}

OneWire_Error_t OneWire_SelectDevice(const OneWire_ROM_t* ROM)
{
	OneWire_Error_t ErrorCode = OneWire_Reset();
	if(ErrorCode == ONEWIRE_NO_DEVICE)
	{
		return ErrorCode;
	}

	if(ROM == NULL)
	{
		OneWire_WriteByte(ONEWIRE_SKIP_ROM);
	}
	else
	{
		uint8_t* ROM_Temp = (uint8_t*)ROM;

		OneWire_WriteByte(ONEWIRE_MATCH_ROM);

		// Transmit the ROM bytes
		for(uint8_t i = 0x00; i < 0x08; i++)
		{
			OneWire_WriteByte(*(ROM_Temp++));
		}
	}

	return ONEWIRE_NO_ERROR;
}

void OneWire_WriteByte(const uint8_t Data)
{
	// Data is transmitted LSB first (check the example in the app note)
	for(uint8_t i = 0x01; i != 0x00; i <<= 0x01)
	{
		OneWire_WriteBit(Data & i);
	}
}

uint8_t OneWire_ReadByte(void)
{
	uint8_t Data = 0x00;

	// Data is received LSB first (check the example in the app note)
	for(uint8_t i = 0x00; i < 0x08; i++)
	{
		Data = Data | (OneWire_ReadBit() << i);
	}

	return Data;
}

OneWire_Error_t OneWire_Reset(void)
{
	#if(ONEWIRE_INTERFACE == INTERFACE_GPIO)
		uint8_t State = 0x00;

		// Save the SREG and disable global interrupts
		uint8_t Reg = CPU_IRQSave();

		// Send the reset instruction
		_delay_us(ONEWIRE_DELAY_G);
		GPIO_Clear(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));
		_delay_us(ONEWIRE_DELAY_H);
		GPIO_Set(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));

		// Switch the direction to INPUT to read the bus state
		_delay_us(ONEWIRE_DELAY_I);
		GPIO_SetDirection(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ), GPIO_DIRECTION_IN);
		State = GPIO_Read(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));
		_delay_us(ONEWIRE_DELAY_J);

		// Clear the output and switch the direction back to OUTPUT
		GPIO_SetDirection(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ), GPIO_DIRECTION_OUT);

		// Restore SREG
		CPU_IRQRestore(Reg);

		if(State != 0x00)
		{
			return ONEWIRE_RESET_ERROR;
		}
	#elif(ONEWIRE_INTERFACE == INTERFACE_USART)
		if(!USART_OneWire_Reset())
		{
			return ONEWIRE_RESET_ERROR;
		}
	#endif

	return ONEWIRE_NO_ERROR;
}

void OneWire_WriteBit(const uint8_t Bit)
{
	#if(ONEWIRE_INTERFACE == INTERFACE_GPIO)
		// Save the SREG and disable global interrupts
		uint8_t Reg = CPU_IRQSave();

		GPIO_Clear(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));

		if(Bit) _delay_us(ONEWIRE_DELAY_A);
		else _delay_us(ONEWIRE_DELAY_C);

		GPIO_Set(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));

		if(Bit)	_delay_us(ONEWIRE_DELAY_B);
		else _delay_us(ONEWIRE_DELAY_D);

		// Restore SREG
		CPU_IRQRestore(Reg);
	#elif(ONEWIRE_INTERFACE == INTERFACE_USART)
		USART_OneWire_WriteBit(Bit);
	#endif
}

uint8_t OneWire_ReadBit(void)
{
	uint8_t Data = 0x00;

	#if(ONEWIRE_INTERFACE == INTERFACE_GPIO)
		// Save the SREG and disable global interrupts
		uint8_t Reg = CPU_IRQSave();

		GPIO_Clear(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));
		_delay_us(ONEWIRE_DELAY_A);
		GPIO_Set(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));
		_delay_us(ONEWIRE_DELAY_E);

		// Switch the direction to INPUT to read the bus state
		GPIO_SetDirection(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ), GPIO_DIRECTION_IN);
		Data = GPIO_Read(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ));
		_delay_us(ONEWIRE_DELAY_F);

		// Switch the direction back to OUTPUT
		GPIO_SetDirection(GET_PERIPHERAL(ONEWIRE_DQ), GET_INDEX(ONEWIRE_DQ), GPIO_DIRECTION_OUT);

		// Restore SREG
		CPU_IRQRestore(Reg);
	#elif(ONEWIRE_INTERFACE == INTERFACE_USART)
		return USART_OneWire_ReadBit();
	#endif

	return Data;
}