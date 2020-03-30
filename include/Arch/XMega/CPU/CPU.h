/*
 * CPU.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega CPU.

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

/** @file Arch/XMega/CPU/CPU.h
 *  @brief Driver for XMega CPU.
 *
 *  This file contains the prototypes and definitions for the XMega CPU driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef CPU_H_
#define CPU_H_

 #include "Common/Common.h"

 /** @brief	Analog startup time for the ADC, DAC and AC.
  */
 typedef enum
 {
	 ANALOG_STARTUP_NONE = 0x00,								/**< Direct startup */ 
	 ANALOG_STARTUP_2CLK = 0x01,								/**< Startup time 2 * CLK_PER */ 
	 ANALOG_STARTUP_8CLK = 0x02,								/**< Startup time 8 * CLK_PER */ 
	 ANALOG_STARTUP_32CLK = 0x03,	 							/**< Startup time 16 * CLK_PER */ 
 } AnalogStartUp_t;

 /** @brief	Create a memory barrier so the compiler doesn't change the code execution order.
  */
 #define Barrier()			asm volatile("" ::: "memory")

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

 /** @brief		Save the CPU flags and disable interrupts.
  *  @return	CPU flags
  */
 static inline volatile uint8_t CPU_IRQSave(void) __attribute__ ((always_inline));
 static inline volatile uint8_t CPU_IRQSave(void)
 {
	 volatile uint8_t Flags = SREG;
	 DisableGlobalInterrupts();
	 return Flags;
 }
 
 /** @brief			Restore the CPU flags.
  *  @param Flags	CPU flags
  */
 static inline void CPU_IRQRestore(volatile uint8_t Flags) __attribute__ ((always_inline));
 static inline void CPU_IRQRestore(volatile uint8_t Flags)
 {
	 Barrier();
	 SREG = Flags;
 }

 /** @brief			Set the analog startup time.
  *  @param Time	Startup time
  */
 static inline void CPU_SetAnalogStartup(const AnalogStartUp_t Time) __attribute__ ((always_inline));
 static inline void CPU_SetAnalogStartup(const AnalogStartUp_t Time)
 {
	 MCU.ANAINIT = Time & 0x03;
 }

 /** @brief		Get the analog startup time.
  *  @return	Startup time
  */
 static inline AnalogStartUp_t CPU_GetAnalogStartup(void) __attribute__ ((always_inline));
 static inline AnalogStartUp_t CPU_GetAnalogStartup(void)
 {
	 return MCU.ANAINIT;
 }

  /** @brief	Reset the CPU.
  */
 static inline void CPU_Reset(void) __attribute__ ((always_inline));
 static inline void CPU_Reset(void)
 {
	 uint8_t Flags = CPU_IRQSave();
	 
	 asm volatile(	 "movw r30,  %0"	"\n\t"
					 "ldi  r16,  %2"	"\n\t"
					 "out   %3, r16"	"\n\t"
					 "st     Z,  %1"	"\n\t"
					 ::	"r" (&RST.CTRL),
					 "r" (RST_SWRST_bm),
					 "M" (CCP_IOREG_gc),
					 "i" (&CCP)
					 : "r16", "r30"
				);

	 CPU_IRQRestore(Flags);
 }

#endif /* CPU_H_ */