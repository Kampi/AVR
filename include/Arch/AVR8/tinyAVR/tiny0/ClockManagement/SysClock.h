/*
 * SysClock.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR tiny0 system clock.

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

/** @file Arch/AVR8/tinyAVR/tiny0/ClockManagement/SysClock.h
 *  @brief Driver for Atmel AVR tiny0 clock system. 
 *
 *  This file contains the prototypes and definitions for the Atmel AVR tiny0 clock system.
 *
 *  @author Daniel Kampert
 *  @bug - Check if NVM can read fuses to detect the right clock
 */

#ifndef SYSCLOCK_H_
#define SYSCLOCK_H_

 #include "Common/Common.h"

 /** @brief Clock sources.
  */
 typedef enum
 {
	 CLOCK_SOURCE_INTOSC = 0x00,			/**< 16/20 MHz internal oscillator */
	 CLOCK_SOURCE_INT32KHZ = 0x01,			/**< 32 kHz internal oscillator */
	 CLOCK_SOURCE_EXT = 0x03,				/**< External clock */
 } ClockSource_t;

 /** @brief Main clock prescaler.
  */
 typedef enum
 {
	 SYSCLOCK_PRESCALER_2 = 0x00,			/**< Prescaler 2 */
	 SYSCLOCK_PRESCALER_4 = 0x01,			/**< Prescaler 4 */
	 SYSCLOCK_PRESCALER_8 = 0x02,			/**< Prescaler 8 */
	 SYSCLOCK_PRESCALER_16 = 0x03,			/**< Prescaler 16 */
	 SYSCLOCK_PRESCALER_32 = 0x04,			/**< Prescaler 32 */
	 SYSCLOCK_PRESCALER_64 = 0x05,			/**< Prescaler 64 */
	 SYSCLOCK_PRESCALER_6 = 0x08,			/**< Prescaler 6 */
	 SYSCLOCK_PRESCALER_10 = 0x09,			/**< Prescaler 10 */
	 SYSCLOCK_PRESCALER_12 = 0x0A,			/**< Prescaler 12 */
	 SYSCLOCK_PRESCALER_24 = 0x0B,			/**< Prescaler 24 */
	 SYSCLOCK_PRESCALER_48 = 0x0C,			/**< Prescaler 48 */
 } ClockPrescaler_t;

 /** @brief			Switch the system clock source.
  *  @param Source	Clock source
  */
 static inline void SysClock_SetClockSource(const ClockSource_t Source) __attribute__((always_inline));
 static inline void SysClock_SetClockSource(const ClockSource_t Source)
 {
	 /*
	  * Use inline assembler, because otherwise the code will only run with at least
	  * optimization level -O1. This level is bad for debugging purposes. You can avoid
	  * this issue by using inline assembler.
	 */
	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&CLKCTRL.MCLKCTRLA),
						"r" (Source & 0x03), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);
 }

 /** @brief			Enable/Disable the clock prescaler.
  *  @param Enable	Enable/Disable
  */
 static inline void SysClock_SwitchPrescaler(const bool Enable) __attribute__((always_inline));
 static inline void SysClock_SwitchPrescaler(const bool Enable)
 {
	 uint8_t Temp = CLKCTRL.MCLKCTRLB;

	 if(Enable)
	 {
		 Temp |= CLKCTRL_PEN_bm;
	 }
	 else
	 {
		 Temp &= ~CLKCTRL_PEN_bm;
	 }

	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&CLKCTRL.MCLKCTRLB),
						"r" (Temp),
						"M" (CCP_IOREG_gc),
						"i" (&CCP)
					: "r16", "r30"
				);
 }

 /** @brief				Set the prescaler for the main clock.
  *  @param Prescaler	Prescaler value
  */
 static inline void SysClock_SetClockPrescaler(const ClockPrescaler_t Prescaler) __attribute__((always_inline));
 static inline void SysClock_SetClockPrescaler(const ClockPrescaler_t Prescaler)
 {
	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&CLKCTRL.MCLKCTRLB), 
						"r" ((Prescaler & 0x0F) << 0x01), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);
 }

 /** @brief		Get the current main system clock.
  *  @return	Clock frequency
  */
 static inline const uint32_t SysClock_GetClock(void) __attribute__ ((always_inline));
 static inline const uint32_t SysClock_GetClock(void)
 {
	 if(CLKCTRL.MCLKCTRLA & CLKCTRL_CLKSEL_OSCULP32K_gc)
	 {
		 return 32000;
	 }
	 else if(CLKCTRL.MCLKCTRLA & CLKCTRL_CLKSEL_EXTCLK_gc)
	 {
		 return F_CPU;
	 }

	 return F_CPU;
 }

 /** @brief	Initialize the CPU clock.
  */
 static inline void SysClock_Init(void) __attribute__ ((always_inline));
 static inline void SysClock_Init(void)
 {
 }

 /** @brief		Get the current clock from the clk_Per domain.
  *  @return	Clock frequency
  */
 uint32_t SysClock_GetClockPer(void);

#endif /* SYSCLOCK_H_ */