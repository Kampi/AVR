/*
 * DS18B20.h
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

/** @file Peripheral/DS18B20/DS18B20.h
 *  @brief Driver for the Maxim Integrated DS18B20 1-Wire temperature sensor.
 *
 *  This file contains the prototypes and definitions for the temperature sensor.
 *
 *  @author Daniel Kampert
 *  @bugs	No alarm support
 */

#ifndef DS18B20_H_
#define DS18B20_H_

 #include "Interfaces/OneWire/OneWire.h"

 /** @ingroup OneWire-FamilyCodes */
 /*\@{*/
	 #define DS18B20_ID						0x28		/**< DS18B20 1-Wire temperature sensor family code */
 /*\@}*/

 /** @brief	Size of the DS18B20 scratchpad in bytes.
  */
 #define DS18B20_SCRATCHPAD_SIZE			0x09

 /** @brief	DS18B20 measurement resolutions.
  */
 typedef enum
 {
	 DS18B20_RESOLUTION_9 = 0x00,						/**< 9 bit resolution */
	 DS18B20_RESOLUTION_10 = 0x01,						/**< 10 bit resolution */
	 DS18B20_RESOLUTION_11 = 0x02,						/**< 11 bit resolution */
	 DS18B20_RESOLUTION_12 = 0x03,						/**< 12 bit resolution */
 } DS18B20_Resolution_t;

 /** @brief	DS18B20 scratchpad object.
  */
 typedef struct
 {
	 uint8_t Temperature_LSB;							/**< LSB of the temperature */
	 uint8_t Temperature_MSB;							/**< MSB of the temperature */
	 uint8_t TH_User1;									/**< TH or User Byte 1 */
	 uint8_t TL_User2;									/**< TL or User Byte 2 */
	 uint8_t Config;									/**< Configuration register */
	 uint8_t Reserved1;									/**< Reserved (always 0xFF) */
	 uint8_t Reserved2;									/**< Reserved */
	 uint8_t Reserved3;									/**< Reserved (always 0xFF) */
	 uint8_t Checksum;									/**< CRC checksum */
 } __attribute__((packed)) DS18B20_Scratchpad_t;

 /** @brief		Initialize the DS18B20 temperature sensor.
  *  @return	1-Wire error
  */
 OneWire_Error_t DS18B20_Init(void);

 /** @brief			Get the IDs of all DS18B20 temperature sensors which are connected to the bus.
  *  @param Found	Pointer to devices found
  *  @param Search	Maximum devices to search
  *  @param ROM		Pointer to #OneWire_ROM_t array
  *  @return		1-Wire error
  */
 OneWire_Error_t DS18B20_GetDevices(uint8_t* Found, uint8_t Search, OneWire_ROM_t* ROM);

 /** @brief			Get the IDs of all DS18B20 temperature sensors in alarm mode which are connected to the bus.
  *  @param Found	Pointer to devices found
  *  @param Search	Maximum devices to search
  *  @param ROM		Pointer to #OneWire_ROM_t array
  *  @return		1-Wire error
  */
 OneWire_Error_t DS18B20_GetAlarmDevices(uint8_t* Found, uint8_t Search, OneWire_ROM_t* ROM);

 /** @brief		Returns the ROM code of a single device.
  *				NOTE: This instruction can only be used when a single device is attached to the bus.
  *  @param ROM	Pointer to #OneWire_ROM_t object
  *  @return	1-Wire error
  */
 OneWire_Error_t DS18B20_GetROM(const OneWire_ROM_t* ROM);

 /** @brief			Read bytes from the scratchpad.
  *  @param ROM		Device ROM ID
  *					NOTE: Set to #NULL if only one device is connected to the bus.
  *  @param Length	Bytes to read
  *  @param Data	Pointer to #DS18B20_Scratchpad_t object
  *  @return		1-Wire error
  */
 OneWire_Error_t DS18B20_ReadScratchpad(const OneWire_ROM_t* ROM, const uint8_t Length, DS18B20_Scratchpad_t* Data);

 /** @brief			Write TL, TH and a configuration byte into the scratchpad.
  *  @param ROM		Device ROM ID
  *					NOTE: Set to #NULL if only one device is connected to the bus.
  *  @param TH		TH data
  *  @param TL		TL data
  *  @param Config	Configuration data
  *  @return		1-Wire error
  */
 OneWire_Error_t DS18B20_WriteScratchpad(const OneWire_ROM_t* ROM, const uint8_t TH, const uint8_t TL, const uint8_t Config);

 /** @brief		Copy the content of the scratchpad into the EEPROM.
  *  @param ROM	Device ROM ID
  *				NOTE: Set to #NULL if only one device is connected to the bus.
  *  @return	1-Wire error
  */
 OneWire_Error_t DS18B20_SaveScratchpad(const OneWire_ROM_t* ROM);

 /** @brief		Load the content of the EEPROM into the scratchpad.
  *  @param ROM	Device ROM ID
  *				NOTE: Set to #NULL if only one device is connected to the bus.
  *  @return	1-Wire error
  */
 OneWire_Error_t DS18B20_LoadScratchpad(const OneWire_ROM_t* ROM);

 /** @brief				Perform a single temperature measurement.
  *  @param ROM			Device ROM ID
  *						NOTE: Set to #NULL to address all devices
  *  @param Resolution	Measurement resolution
  *  @param Temperature	Pointer to temperature output
  *						NOTE: Output will be in half precision float format!
  *  @return			1-Wire error
  */
 OneWire_Error_t DS18B20_Measure(const OneWire_ROM_t* ROM, const DS18B20_Resolution_t Resolution, double* Temperature);
 
 /** @brief		Configure the temperature alarm.
  *  @param ROM	Device ROM ID
  *				NOTE: Set to #NULL if only one device is connected to the bus.
  *  @param Max	Upper temperature limit
  *  @param Min	Lower temperature limit
  *  @return	1-Wire error
  */
 OneWire_Error_t DS18B20_SetAlarm(const OneWire_ROM_t* ROM, const int8_t Max, const int8_t Min);
 
 /** @brief			Check if a DS18B20 device is using parasite power.
  *  @param ROM		Device ROM ID
  *					NOTE: Set to #NULL to address all devices
  *	 @param State	Pointer to power state of the sensor.
  *  @return		1-Wire error
  */
 OneWire_Error_t DS18B20_CheckSupply(const OneWire_ROM_t* ROM, OneWire_PowerState_t* State);

#endif /* DS18B20_H_ */