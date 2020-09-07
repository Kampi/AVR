/*
 * NEC_IR.h
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

/** @file Interfaces/NEC_IR/NEC_IR.h
 *  @brief NEC IR interface driver for AVR.
 *		   Please check 
 *				https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol
 *				https://www.vishay.com/docs/80071/dataform.pdf
 *		   when you need additional information.
 *
 *  This contains the prototypes and definitions for the IR interface.
 *
 *  @author Daniel Kampert
 *  @bug	No known bugs
 */

#ifndef NEC_IR_H_
#define NEC_IR_H_
 
 #include "Common/Common.h"
 
 #include "IR_Codes.h"

 #ifndef IR_TIMEOUT
	 #define IR_TIMEOUT							5000
 #endif

 /** @brief States for the IR receiver state machine.
  */
 typedef enum
 {
	 IR_STATE_IDLE = 0x00,						/**< Idle state. Wait for a transmission begin */
	 IR_STATE_REC_SPACE = 0x01,					/**< Wait for a space from the receiver */
	 IR_STATE_REC_BURST = 0x02,					/**< Wait for a burst from the receiver */
	 IR_STATE_STOP = 0x03,						/**< State after stopping the state machine or when a transmission is complete */
 } IR_RecState_t;

 /** @brief IR message object.
  */
 typedef struct
 {
	 uint8_t Length;							/**< Length of the message in bytes \n
													 (set by the state machine) */
	 union										/**< Message data of the received message */
	 {
		 uint8_t Field[4];						/**< Message data as byte array */
		 uint32_t Value;						/**< Message data as 32-bit integer */
	 } Data;
	 bool Valid;								/**< Set to #true when the message is valid \n
													 (set by the state machine) */
	 bool IsRepeat;								/**< Set to #true when a repeat code is received \n
													 (set by the state machine) */
 } __attribute__((packed)) IR_Message_t;

 /** @brief	Initialize the IR interface.
  */
 void IR_Init(void);
 
 /** @brief			Get a new message from the IR interface.
  *  @param Message	Pointer to IR message object.
  *  @return		#true when the message is valid. #false when the message is invalid or a timeout occur.
  *					NOTE: You can specify the timeout with the #IR_TIMEOUT macro!
  */
 bool IR_GetMessage(IR_Message_t* Message);

#endif /* IR_H_ */