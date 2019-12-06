/*
 * Joystick.h
 *
 *  Copyright (C) Daniel Kampert, 2018
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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
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

 #include "Definitions.h"
 #include "Common/Common.h"

 #if(BOARD == BOARD_NONE)
	 typedef Joystick_Action_t uint8_t;
	 
	 static inline void Joystick_Init(void) {}
	 static inline Joystick_Action_t Joystick_GetStatus(void) { return 0; }

 #elif(BOARD == BOARD_AT90USBKEY)
	 #include "Common/Board/Joystick/Joystick_USBKey.h"
 #endif
 
#endif /* JOYSTICK_H_ */