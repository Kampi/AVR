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
 *		   Please check https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol if you need additional information.
 *
 *  This contains the prototypes and definitions for the IR interface.
 *
 *  @author Daniel Kampert
 *  @bug	No known bugs
 */

#ifndef IR_H_
#define IR_H_
 
 #include "Common/Common.h"

 /** @brief States for the IR receiver state machine
  */
 typedef enum
 {
	 IR_STATE_IDLE = 0x00,						/**< Idle state. Wait for a transmission begin. */
	 IR_STATE_REC_ONE = 0x01,					/**< Read a logical '1' from the receiver. */
	 IR_STATE_REC_ZERO = 0x02,					/**< Read a logical '0' from the receiver. */
	 IR_STATE_STOP = 0x03,						/**< State after stopping the state machine or when a transmission is complete. */
 } IR_RecState_t;

 /** @brief	Initialize the IR interface.
  */
 void IR_Init(void);
 
 void IR_Read(void);

#endif /* IR_H_ */