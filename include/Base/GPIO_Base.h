/*
 * ADC_Base.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for Atmel AVR ADC

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

/** @file GPIO_Base.h
 *  @brief Common definitions for AVR GPIO interface.
 *
 *  @author Daniel Kampert
 */

#ifndef GPIO_BASE_H_
#define GPIO_BASE_H_

 #include "Common/types.h"
 
 /** @brief			GPIO interrupt handler
  */
 typedef void (*GPIO_Callback_t)(void);

 /** @brief GPIO output directions.
  */
 typedef enum
 {
	 GPIO_DIRECTION_IN = 0x00,					/**< Direction input */ 
	 GPIO_DIRECTION_OUT = 0x01,					/**< Direction output */ 
 } GPIO_Direction_t;

#endif /* GPIO_BASE_H_ */