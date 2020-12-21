/*
 * ATtiny406.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: MCU specific definitions and constants for the Microchip ATtiny406.

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

/** @file Common/Board/MCU/ATtiny406.h
 *  @brief MCU specific definitions and constants for the Microchip ATtiny406.
 *
 *  @author Daniel Kampert
 */

#ifndef ATTINY406_H_
#define ATTINY406_H_

 #include "Common/Common.h"

 /*
	GPIO
 */
 #define GPIO_PORT_COUNT								7							/**< GPIO port count */ 
 
 /*
	USART
 */
 #define USART_DEVICES									1							/**< USART device count in MCU */
 #define USART_XDIR0_PIN								0							/**< Pin number of USART XDIR pin */
 #define USART_XCK0_PIN									1							/**< Pin number of USART XCK pin */
 #define USART_RX0_PIN									3							/**< Pin number of USART RxD pin */
 #define USART_TX0_PIN									2							/**< Pin number of USART TxD pin */

#endif /* ATTINY406_H_ */