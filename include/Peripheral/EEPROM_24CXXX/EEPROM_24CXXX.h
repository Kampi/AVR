/*
 * EEPROM_24CXXX.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel 24Cxxx I2C EEPROMs

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

/** @file Peripheral/EEPROM_24CXXX/EEPROM_24CXXX.h
 *  @brief Driver for Atmel 24Cxxx I2C EEPROMs.
 *
 *  This file contains the prototypes and definitions for the 24Cxxx I2C EEPROM driver.
 *
 *  @author Daniel Kampert
 */

#ifndef EEPROM_24CXXX_H_
#define EEPROM_24CXXX_H_

 /** @ingroup I2C-Addresses */
 /*\@{*/
	 #define EEPROM_24CXXX_ADDRESS						0x50						/**< 24XXX EEPROM I2C device address */
 /*\@}*/

 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/ATmega/I2C/I2C.h"
 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/I2C/I2C.h"
 #elif(MCU_ARCH == MCU_ARCH_UC3)
	 #include "Arch/UC3/I2C/I2C.h"
 #else
	 #error "No valid mcu"
 #endif
 
 
 /** @brief			Initialize the EEPROM and the I2C interface.
  *  @param Config	Pointer to I2C master configuration struct
  *					NOTE: Set it to *NULL if you have initialized the I2C already
  *  @return		I2C error code
  */
 void EEPROM_24CXXX_Init(I2CM_Config_t* Config);
 
 /** @brief			Send a single byte to the EEPROM.
  *  @param Address	EEPROM address
  *	 @param Data	Data byte
  *  @return		I2C error code
  */
 I2C_Error_t EEPROM_24CXXX_WriteByte(const uint16_t Address, const uint8_t Data);
 
 /** @brief			Read a single byte from the EEPROM.
  *  @param Address	EEPROM address
  *	 @param Data	Pointer to data byte
  *  @return		I2C error code
  */
 I2C_Error_t EEPROM_24CXXX_ReadByte(const uint16_t Address, uint8_t* Data);
 
 /** @brief			Write n bytes to the EEPROM.
  *  @param Address	EEPROM address
  *  @param Length	Number of data bytes
  *	 @param Data	Pointer to data
  *  @return		I2C error code
  */
 I2C_Error_t EEPROM_24CXXX_WriteBytes(const uint16_t Address, const uint16_t Length, const uint8_t* Data);
 
 /** @brief			Read n bytes from the EEPROM.
  *  @param Address	EEPROM address
  *  @param Length	Number of data bytes
  *	 @param Data	Pointer to data
  *  @return		I2C error code
  */
 I2C_Error_t EEPROM_24CXXX_ReadBytes(const uint16_t Address, const uint16_t Length, uint8_t* Data);
 
#endif /* EEPROM_24CXXX_H_ */