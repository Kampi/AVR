/*
 * can_node.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Board definitions for can nodes from Kampis-Elektroecke

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

/** @file can_node.h
 *  @brief Hardware definitions for my private CAN Node board.
 *
 *  @author Daniel Kampert
 */

#ifndef CAN_NODE_H_
#define CAN_NODE_H_

 /*
	Library specific definitions
 */
 #include "Definitions.h"

 /*
	Define the MCU for the used board
 */
 #include "Common/Board/MCU/ATmega32.h"
 
 #include "Arch/AVR8/ATmega/ADC/ADC.h"
 #include "Arch/AVR8/ATmega/CPU/CPU.h"
 #include "Arch/AVR8/ATmega/I2C/I2C.h"
 #include "Arch/AVR8/ATmega/SPI/SPI.h"
 #include "Arch/AVR8/ATmega/GPIO/GPIO.h"
 #include "Arch/AVR8/ATmega/USART/USART.h"
 #include "Arch/AVR8/ATmega/Timer0/Timer0.h"
 #include "Arch/AVR8/ATmega/Timer1/Timer1.h"
 #include "Peripheral/MCP2515/MCP2515.h"

#endif /* CAN_NODE_H_ */