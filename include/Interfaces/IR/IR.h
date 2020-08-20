/*
 * IR.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR NEC IR interface implementation.

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

/** @file Interfaces/IR/IR.h
 *  @brief AVR NEC IR interface implementation.
 *
 *  This contains the prototypes and definitions for the IR interface.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef IR_H_
#define IR_H_
 
 #include "Common/Common.h"
 
 #include "Services/SystemTimer/SystemTimer.h"

 /** @defgroup IR
  *  @{
  */
	 /** @defgroup IR-Errors
	  *  IR communication error codes.
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

 /** @brief	Initialize the IR interface.
  */
 void IR_Init(void);
 
 void IR_Read(void);

#endif /* IR_H_ */