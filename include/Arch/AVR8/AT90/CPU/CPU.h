/*
 * CPU.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 CPU

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */ 

/** @file Arch/AVR8/CPU/CPU.h
 *  @brief Driver for AVR8 CPU.
 *
 *  This file contains the prototypes and definitions for the XMega AES driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef CPU_H_
#define CPU_H_

 #include "Definitions.h"
 #include "Common/Common.h"
 
 #if(MCU_NAME == MCU_NAME_ATMEGA32)
	 #include "Common/Board/MCU/ATmega32.h"
 #elif(MCU_NAME == MCU_NAME_AT90USB1287)
	 #include "Common/Board/MCU/AT90USB1287.h"
 #endif

 /** 
  * CPU sleep modes
  */
 typedef enum
 {
	CPU_IDLE = 0x00,						/**< Idle */ 	
	CPU_ADC_NOISE_REDUCTION = 0x01,			/**< ADC noise reduction */ 	
	CPU_POWER_DOWN = 0x02,					/**< Power down */ 
	CPU_POWER_SAVE = 0x03,					/**< Power save */ 
	CPU_STANDBY = 0x06,						/**< Standby */ 
	CPU_EXT_STANDBY = 0x07,					/**< Extended standby */ 
 } CPU_SleepMode_t;

 /** @brief	Global enable for interrupts.
  */
 static inline void EnableGlobalInterrupts(void) __attribute__ ((always_inline));
 static inline void EnableGlobalInterrupts(void)
 {
	sei();
 }

 /** @brief	Global disable for interrupts.
  */
 static inline void DisableGlobalInterrupts(void) __attribute__ ((always_inline));
 static inline void DisableGlobalInterrupts(void)
 {
	cli();
 }
 
 /** @brief		Get the current CPU clock.
  *  @return	CPU clock frequency
  */
 static inline uint32_t SysClock_GetCPUClock(void) __attribute__ ((always_inline));
 static inline uint32_t SysClock_GetCPUClock(void)
 {
	 return F_CPU;
 }

#endif /* CPU_H_ */