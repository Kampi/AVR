/*
 * Joystick.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for simple joysticks.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file Peripheral/Joystick.h
 *  @brief Driver for simple joysticks.
 *
 *  This contains the prototypes and definitions for the joystick driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

 #include "Common.h"
 
 typedef enum
 {
	 JOYSTICK_NO_ACTION = 0x00,                             /**< Nothing happens */
	 JOYSTICK_LEFT = (0x01 << 0x06),                        /**< Joystick is pushed left */
	 JOYSTICK_RIGHT = (0x01 << 0x04) >> 0x01,               /**< Joystick is pushed right */
	 JOYSTICK_UP = (0x01 << 0x07),                          /**< Joystick is pushed up */
	 JOYSTICK_DOWN = (0x01 << 0x05) >> 0x01,                /**< Joystick is pushed down */
	 JOYSTICK_PRESS = (0x01 << 0x05),                       /**< Joystick is pressed */
 } Joystick_Action_t;

 /** @brief	Initialize the joystick.
  */
 static inline void Joystick_Init(void) __attribute__ ((always_inline));
 static inline void Joystick_Init(void)
 {
	 PORTB.DIR &= ~((0x01 << 0x05) | (0x01 << 0x06) | (0x01 << 0x07));
	 PORTE.DIR &= ~((0x01 << 0x04) | (0x01 << 0x05));

	 PORTB.OUT |= ((0x01 << 0x05) | (0x01 << 0x06) | (0x01 << 0x07));
	 PORTE.OUT |= ((0x01 << 0x04) | (0x01 << 0x05));
 }

 /** @brief		Initialize the joystick.
  *  @return	Status of the joystick
  */
 static inline Joystick_Action_t Joystick_Read(void) __attribute__ ((always_inline));
 static inline Joystick_Action_t Joystick_Read(void)
 {
	 return (((uint8_t)~PORTB.IN & ((0x01 << 0x05) | (0x01 << 0x06) | (0x01 << 0x07))) | 
			(((uint8_t)~PORTE.IN & ((0x01 << 0x04) | (0x01 << 0x05))) >> 0x01));
 }


#endif /* JOYSTICK_H_ */