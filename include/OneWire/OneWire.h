/*
 * OneWire.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: 1-Wire driver for AVR.

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

/** @file OneWire/OneWire.h
 *  @brief 1-Wire driver for AVR.
 *		   Please check https://pdfserv.maximintegrated.com/en/an/AN126.pdf if you need additional information.
 *
 *  This file contains the prototypes and definitions for the 1-Wire service.
 *
 *  @author Daniel Kampert
 *  @bugs	No known bugs
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

 #include "Common/Common.h"

 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/CPU/CPU.h"
	 #include "Arch/XMega/GPIO/GPIO.h"
 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/CPU/CPU.h"
	 #include "Arch/AVR8/GPIO/GPIO.h"
 #else
	 #error "Architecture not supported for 1-Wire!"
 #endif

 /** @defgroup OneWire
  *  @{
  */
	 /** @defgroup OneWire-Errors
	  *  1-Wire error codes.
	  *  @{
	  */
	 typedef enum
	 {
		 ONEWIRE_NO_ERROR = 0x00,						/**< No error */
		 ONEWIRE_INVALID_ADDRESS = 0x01,				/**< Invalid 1-Wire device address */
		 ONEWIRE_CRC_ERROR = 0x02,						/**< CRC doesn't match */
		 ONEWIRE_NO_DEVICE = 0x03,						/**< No 1-Wire device found */
		 ONEWIRE_PARAMETER_ERROR = 0x04,				/**< General parameter error */
		 ONEWIRE_INACTIVE_SEARCH = 0x05,				/**< No ROM search active */
		 ONEWIRE_RESET_ERROR = 0x06,					/**< Error during 1-Wire reset */
	 } OneWire_Error_t;
	/** @} */ // end of OneWire-Errors
 /** @} */ // end of OneWire

 /** @brief	1-Wire power states.
  */
 typedef enum
 {
	 ONEWIRE_POWER_PARSITIC = 0x00,						/**< Parasitic power supply */
	 ONEWIRE_POWER_EXTERNAL = 0x01,						/**< External power supply */
 } OneWire_PowerState_t;

 /** @brief	1-Wire ROM object.
  */
 typedef struct
 {
	 uint8_t FamilyCode;								/**< 8-Bit Family code */
	 uint8_t SerialNumber[6];							/**< 48-Bit Serial number */
	 uint8_t Checksum;									/**< 8-Bit CRC checksum */
 } __attribute__((packed)) OneWire_ROM_t;
 
 /** @brief		Initialize the 1-Wire driver.
  *  @return	1-Wire error
  */
 OneWire_Error_t OneWire_Init(void);

 /** @brief		Perform a 1-Wire reset and check if a device is present.
  *  @return	1-Wire error
  */
 OneWire_Error_t OneWire_Reset(void);

 /** @brief			Initialize a new ROM search and search the first device.
  *  @param ROM		Pointer to #OneWire_ROM_t object
  *  @param isAlarm	#TRUE when a alarm search should be done
  *  @return		1-Wire error
  */
 OneWire_Error_t OneWire_StartSearch(const OneWire_ROM_t* ROM, const Bool_t isAlarm);
 
 /** @brief		Search the next 1-wire device.
  *				NOTE: You have to use #OneWire_StartSearch first!
  *  @param ROM	Pointer to #OneWire_ROM_t object
  *  @return	1-Wire error
  */
 OneWire_Error_t OneWire_SearchNext(const OneWire_ROM_t* ROM);
 
 /** @brief		Check if the last device was found.
  *  @return	#TRUE when last.
  */
 Bool_t OneWire_IsLast(void);
 
 /** @brief		Interrupt an active 1-wire search and perform a 1-wire reset.
  *  @return	1-Wire error
  */
 OneWire_Error_t OneWire_StopSearch(void);

 /** @brief			Calculate the CRC with given data.
  *					NOTE: Please check https://www.maximintegrated.com/en/design/technical-documents/app-notes/2/27.html 
  *					for additional information.
  *  @param Length	Length of data
  *  @param Data	Pointer to data
  *  @return		Calculated CRC
  */
 uint8_t OneWire_CRC(const uint8_t Length, const uint8_t* Data);

 /** @brief		Returns the ROM code of a single device.
  *				NOTE: This instruction can only be used when a single device is attached to the bus.
  *  @param ROM	Pointer to #OneWire_ROM_t object
  *  @return	1-Wire error
  */
 OneWire_Error_t OneWire_ReadROM(const OneWire_ROM_t* ROM);

 /** @brief			Start a new message transmission and select the target 1-Wire device.
  *  @param ROM		Device ROM ID
  *					NOTE: Set to #NULL to address all devices
  *  @return		1-Wire error
  */
 OneWire_Error_t OneWire_SelectDevice(const OneWire_ROM_t* ROM);

 /** @brief		Transmit one bit with the 1-Wire bus.
  *  @param Bit	Data bit
  */
 void OneWire_WriteBit(const uint8_t Bit);
 
 /** @brief			Transmit one data byte with the 1-Wire bus.
  *  @param Data	Data byte
  */
 void OneWire_WriteByte(const uint8_t Data);

 /** @brief		Receive one bit from the 1-Wire bus.
  *  @return	Received bit
  */
 uint8_t OneWire_ReadBit(void);

 /** @brief		Read one byte from the 1-Wire bus.
  *  @return	Received data byte
  */
 uint8_t OneWire_ReadByte(void);

#endif /* ONEWIRE_H_ */