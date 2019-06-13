/*
 * Board.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Board selection file. 

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

/** @file Board.h
 *  @brief Board selection for several AVR boards.
 *
 *  Use this file to choose the correct includes for a given board.
 *
 *  @author Daniel Kampert
 */

#ifndef BOARD_H_
#define BOARD_H_
 
 #include "Common/Common.h"
 
 #ifdef BOARD
	#if(BOARD == BOARD_XMEGA256A3_XPLAINED)
		#include "Common/Board/XMegaA3XPlained.h"
	#elif(BOARD == BOARD_XMEGA384C3_XPLAINED)
		#include "Common/Board/XMegaC3XPlained.h"
	#elif(BOARD == BOARD_CAN_NODE)
		#include "Common/Board/can_node.h"
	#elif(BOARD == BOARD_RN_CONTROL)
		#include "Common/Board/RNControl.h"
	#elif(BOARD == BOARD_AT90USBKEY)
		#include "Common/Board/AT90USBKey.h"
	#endif
 #endif

#endif /* BOARD_H_ */