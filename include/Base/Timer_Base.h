/*
 * Timer_Base.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for Atmel AVR Timer

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

/** @file Timer_Base.h
 *  @brief Common definitions for AVR Timer devices.
 *
 *  @author Daniel Kampert
 */

#ifndef TIMER_BASE_H_
#define TIMER_BASE_H_

 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Base/AVR8/Timer_AVR8.h"
 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	#include "Base/XMega/Timer_XMega.h"
 #endif

 #include "Common/types.h"

 /** @brief	Timer callback definition
 */
 typedef void (*Timer_Callback_t)(void);

 /** @brief Timer prescaler.
  */
 typedef enum
 {
	 TIMER_PRESCALER_1 = 0x01,						/**< Prescaler 1 */ 
	 TIMER_PRESCALER_2 = 0x02,						/**< Prescaler 2 */ 
	 TIMER_PRESCALER_4 = 0x03,						/**< Prescaler 4 */ 
	 TIMER_PRESCALER_8 = 0x04,						/**< Prescaler 8 */ 
	 TIMER_PRESCALER_64 = 0x05,						/**< Prescaler 64 */ 
	 TIMER_PRESCALER_256 = 0x06,					/**< Prescaler 256 */ 
	 TIMER_PRESCALER_1024 = 0x07,					/**< Prescaler 1024 */ 
 } Timer_Prescaler_t;

 /** @brief Timer callback types.
  */
 typedef enum
 {
	 TIMER_OVERFLOW_INTERRUPT = 0x01,				/**< Timer overflow interrupt */ 
	 TIMER_ERROR_INTERRUPT = 0x02,					/**< Timer error interrupt */ 
	 TIMER_CCA_INTERRUPT = 0x10,					/**< Timer capture compare A interrupt */ 
	 TIMER_CCB_INTERRUPT = 0x20,					/**< Timer capture compare B interrupt */ 
	 TIMER_CCC_INTERRUPT = 0x40,					/**< Timer capture compare C interrupt */ 
	 TIMER_CCD_INTERRUPT = 0x80,					/**< Timer capture compare D interrupt */ 
	 TIMER_COMPMATCH_INTERRUPT = 0x02,				/**< Timer output compare match interrupt */ 
	 TIMER_COMPMATCH_A_INTERRUPT = 0x03,			/**< Timer output compare match A interrupt */ 
	 TIMER_COMPMATCH_B_INTERRUPT = 0x04,			/**< Timer output compare match B interrupt */ 
	 TIMER_CAPTURE_INTERRUPT = 0x05,				/**< Timer input capture interrupt */ 
 } Timer_CallbackType_t;

 /** @brief Timer device modes.
  */
 typedef enum
 {
	 TIMER_MODE_0 = 0x00,							/**< Timer mode 0 */ 
	 TIMER_MODE_1 = 0x01,							/**< Timer mode 1 */ 
	 TIMER_MODE_2 = 0x02,							/**< Timer mode 2 */ 
	 TIMER_MODE_3 = 0x03,							/**< Timer mode 3 */ 
	 TIMER_MODE_4 = 0x04,							/**< Timer mode 4 */ 
	 TIMER_MODE_5 = 0x05,							/**< Timer mode 5 */ 
	 TIMER_MODE_6 = 0x06,							/**< Timer mode 6 */ 
	 TIMER_MODE_7 = 0x07,							/**< Timer mode 7 */ 
	 TIMER_MODE_8 = 0x08,							/**< Timer mode 8 */ 
	 TIMER_MODE_9 = 0x09,							/**< Timer mode 9 */ 
	 TIMER_MODE_10 = 0x0A,							/**< Timer mode 10 */ 
	 TIMER_MODE_11 = 0x0B,							/**< Timer mode 11 */ 
	 TIMER_MODE_12 = 0x0C,							/**< Timer mode 12 */ 
	 TIMER_MODE_13 = 0x0D,							/**< Timer mode 13 */ 
	 TIMER_MODE_14 = 0x0E,							/**< Timer mode 14 */ 
	 TIMER_MODE_15 = 0x0F,							/**< Timer mode 15 */ 
 } Timer_Mode_t;

 /** @brief Timer capture compare channels.
  */
 typedef enum
 {
	 TIMER_CCA = 0x00,								/**< Capture compare channel A */ 
	 TIMER_CCB = 0x01,								/**< Capture compare channel B */ 
	 TIMER_CCC = 0x02,								/**< Capture compare channel C */ 
	 TIMER_CCD = 0x03,								/**< Capture compare channel D */ 
 } Timer_CCChannel_t;
 
#endif /* TIMER_BASE_H_ */