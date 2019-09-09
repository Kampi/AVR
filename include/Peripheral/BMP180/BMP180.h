/*
 * BMP180.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for BMP180 I2C pressure sensor.

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

/** @file Peripheral/BMP180/BMP180.h
 *  @brief Driver for the Bosch BMP180 I2C pressure sensor.
 *
 *  This file contains the prototypes and definitions for the Bosch BMP180 pressure sensor driver.
 *
 *  @author Daniel Kampert
 */

#ifndef BMP180_H_
#define BMP180_H_

 #include "Config_BMP180.h"

 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/GPIO/GPIO.h"
	 #include "Arch/XMega/ClockManagement/SysClock.h"
	 #include "Arch/XMega/I2C/I2C.h"

	 #define BMP180_I2CM_INIT(Config)														I2CM_Init(Config)
	 #define BMP180_I2CM_WRITEBYTE(Interface, Address, Data, Stop)							I2CM_WriteByte(Interface, Address, Data, Stop)
	 #define BMP180_I2CM_READBYTE(Interface, Address, Data, Stop)							I2CM_ReadByte(Interface, Address, Data, Stop)
	 #define BMP180_I2CM_WRITEBYTES(Interface, Address, Bytes, Data, Stop)					I2CM_WriteBytes(Interface, Address, Bytes, Data, Stop)
	 #define BMP180_I2CM_READBYTES(Interface, Address, Bytes, Data, Stop)					I2CM_ReadBytes(Interface, Address, Bytes, Data, Stop)

 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/GPIO/GPIO.h"
	 #include "Arch/AVR8/I2C/I2C.h"
	 
	  #define BMP180_I2CM_INIT(Config)														I2CM_Init(Config)
	  #define BMP180_I2CM_WRITEBYTE(Interface, Address, Data, Stop)							I2CM_WriteByte(Address, Data, Stop)
	  #define BMP180_I2CM_READBYTE(Interface, Address, Data, Stop)							I2CM_ReadByte(Address, Data, Stop)
	  #define BMP180_I2CM_WRITEBYTES(Interface, Address, Bytes, Data, Stop)					I2CM_WriteBytes(Address, Bytes, Data, Stop)
	  #define BMP180_I2CM_READBYTES(Interface, Address, Bytes, Data, Stop)					I2CM_ReadBytes(Address, Bytes, Data, Stop)
 #else
	  #error "Architecture not supported"
 #endif
 
 #include "Common/Common.h"
 #include "Common/types.h"

 /** @ingroup I2C-Addresses */
 /*\@{*/
	 #define BMP180_ADDRESS					0x77																							/**< BMP180 I2C pressure sensor device address */
 /*\@}*/

 /** @brief			Initialize the BMP180 pressure sensor and the I2C interface.
  *  @param Config	Pointer to I2C master configuration object
  *					NOTE: Set it to *NULL if you have initialized the I2C already
  *  @return		I2C error code
  */
 const I2C_Error_t BMP180_Init(I2CM_Config_t* Config);

#endif /* BMP180_H_ */