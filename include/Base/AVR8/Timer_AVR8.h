/*
 * Timer_AVR8.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for Atmel AVR8 Timer

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

/** @file Base/AVR8/Timer_AVR8.h
 *  @brief Common definitions for AVR8 Timer devices.
 *
 *  @author Daniel Kampert
 */

#ifndef TIMER_AVR8_H_
#define TIMER_AVR8_H_

 /** @brief Timer compare output configuration for AVR8 devices.
  * 		NOTE: The function is depending on the timer mode! Please check the data sheet.
  */
 typedef enum
 {
	 TIMER_COMPARE_MODE_0 = 0x00,					/**< Compare mode 0 */ 
	 TIMER_COMPARE_MODE_1 = 0x01,					/**< Compare mode 1 */ 
	 TIMER_COMPARE_MODE_2 = 0x02,					/**< Compare mode 2 */ 
	 TIMER_COMPARE_MODE_3 = 0x03,					/**< Compare mode 3 */ 
 } Timer_CompareOutput_t;

 /** @brief External clock settings for timer.
  */
 typedef enum
 {
	 TIMER_EXT_CLOCK_FALL = 0x00,					/**< Set external clock on falling edge */ 
	 TIMER_EXT_CLOCK_RISE = 0x01,					/**< Set external clock on rising edge */ 
 } Timer_ExtClock_t;
 
#endif /* TIMER_AVR8_H_ */