/*
 * Timer_XMega.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for Atmel XMega Timer

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

/** @file Base/XMega/Timer_XMega.h
 *  @brief Common definitions for XMega Timer devices.
 *
 *  @author Daniel Kampert
 */

#ifndef TIMER_XMEGA_H_
#define TIMER_XMEGA_H_

 /** @brief	Timer commands
 */
 typedef enum
 {
	 TIMER_COMMAND_NONE = 0x00,						/**< Empty command */ 
	 TIMER_COMMAND_UPDATE = 0x01,					/**< Force update */ 
	 TIMER_COMMAND_RESTART = 0x02,					/**< Force restart */ 
	 TIMER_COMMAND_RESET = 0x03,					/**< Force reset */ 
 } Timer_Command_t;

 /** @brief	Timer count directions.
 */
 typedef enum
 {
	 TIMER_DIRECTION_TOP = 0x00,					/**< Count to top, Interrupt when overflow */ 
	 TIMER_DIRECTION_BOTTOM = 0x01,					/**< Count to bottom, Interrupt when underflow */ 
 } Timer_Direction_t;
 
#endif /* TIMER_XMEGA_H_ */