/*
 * I2C.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Wrapper for AVR I2C driver.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */ 

/** @file Arch/I2C.h
 *  @brief Wrapper for AVR I2C driver.
 *		   NOTE: Use this for peripheral devices such as memories to support different interfaces and architectures.
 *
 *  @author Daniel Kampert
 */
 
#ifndef I2C_WRAPPER_H_
#define I2C_WRAPPER_H_

 #include "Definitions.h"

 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "XMega/GPIO/GPIO.h"
	 #include "XMega/ClockManagement/SysClock.h"
	 #include "XMega/I2C/I2C.h"

	 #define I2CM_INIT(Config)																I2CM_Init(Config)
	 #define I2CM_WRITEBYTE(Interface, Address, Data, Stop)									I2CM_WriteByte(Interface, Address, Data, Stop)
	 #define I2CM_READBYTE(Interface, Address, Data, Stop)									I2CM_ReadByte(Interface, Address, Data, Stop)
	 #define I2CM_WRITEBYTES(Interface, Address, Bytes, Data, Stop)							I2CM_WriteBytes(Interface, Address, Bytes, Data, Stop)
	 #define I2CM_READBYTES(Interface, Address, Bytes, Data, Stop)							I2CM_ReadBytes(Interface, Address, Bytes, Data, Stop)

 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "AVR8/GPIO/GPIO.h"
	 #include "AVR8/I2C/I2C.h"
	 
	  #define I2CM_INIT(Config)																I2CM_Init(Config)
	  #define I2CM_WRITEBYTE(Interface, Address, Data, Stop)								I2CM_WriteByte(Address, Data, Stop)
	  #define I2CM_READBYTE(Interface, Address, Data, Stop)									I2CM_ReadByte(Address, Data, Stop)
	  #define I2CM_WRITEBYTES(Interface, Address, Bytes, Data, Stop)						I2CM_WriteBytes(Address, Bytes, Data, Stop)
	  #define I2CM_READBYTES(Interface, Address, Bytes, Data, Stop)							I2CM_ReadBytes(Address, Bytes, Data, Stop)
 #else
	  #error "Architecture not supported"
 #endif

#endif /* I2C_WRAPPER_H_ */