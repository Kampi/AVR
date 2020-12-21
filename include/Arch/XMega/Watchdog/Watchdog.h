/*
 * Watchdog.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega Watchdog timer.

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

/** @file Arch/XMega/Watchdog/Watchdog.h
 *  @brief Driver for XMega Watchdog timer.
 *
 *  This contains the implementation of the XMega watchdog driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

 /** 
  * Watchdog timer intervals
  */
 typedef enum
 {
	 WATCHDOG_TIMEOUT_8CLK = 0x00,			/**< 8 clock cycles */ 
	 WATCHDOG_TIMEOUT_16CLK = 0x01,			/**< 16 clock cycles */ 
	 WATCHDOG_TIMEOUT_32CLK = 0x02,			/**< 32 clock cycles */ 
	 WATCHDOG_TIMEOUT_64CLK = 0x03,			/**< 64 clock cycles */ 
	 WATCHDOG_TIMEOUT_128CLK = 0x04,		/**< 128 clock cycles */ 
	 WATCHDOG_TIMEOUT_256CLK = 0x05,		/**< 256 clock cycles */ 
	 WATCHDOG_TIMEOUT_512CLK = 0x06,		/**< 512 clock cycles */ 
	 WATCHDOG_TIMEOUT_1KCLK = 0x07,			/**< 1024 clock cycles */ 
	 WATCHDOG_TIMEOUT_2KCLK = 0x08,			/**< 2048 clock cycles */ 
	 WATCHDOG_TIMEOUT_4KCLK = 0x09,			/**< 4096 clock cycles */ 
	 WATCHDOG_TIMEOUT_8KCLK = 0x0A,			/**< 8192 clock cycles */ 
 } Watchdog_Time_t;


 /** @brief	Wait for sync.
  */
 static inline void Watchdog_Sync(void) __attribute__ ((always_inline));
 static inline void Watchdog_Sync(void)
 {
	 while(WDT.STATUS & WDT_SYNCBUSY_bm);
 }
 
 /** @brief	Reset the watchdog.
 */
 static inline void Watchdog_Reset(void) __attribute__ ((always_inline));
 static inline void Watchdog_Reset(void)
 {
	 asm volatile("wdr" :::);
 }

 /** @brief	Enable the watchdog.
 */
 static inline void Watchdog_Enable(void) __attribute__ ((always_inline));
 static inline void Watchdog_Enable(void)
 {
	 uint8_t Flags = CPU_IRQSave();
	 
	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"		"\n\t"
					"out   %3, r16"		"\n\t"
					"st     Z,  %1"		"\n\t"
					::	"r" (&WDT.CTRL), 
						"r" ((WDT.CTRL & ~0x02) | (WDT_ENABLE_bm | WDT_CEN_bm)), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);
	 
	 CPU_IRQRestore(Flags);
	 
	 Watchdog_Sync();
 }

 /** @brief	Disable the watchdog.
 */
 static inline void Watchdog_Disable(void) __attribute__ ((always_inline));
 static inline void Watchdog_Disable(void)
 {
	 uint8_t Flags = CPU_IRQSave();
	 
	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"		"\n\t"
					"out   %3, r16"		"\n\t"
					"st     Z,  %1"		"\n\t"
					::	"r" (&WDT.CTRL), 
						"r" ((WDT.CTRL & ~0x02) | WDT_CEN_bm), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);
	 
	 CPU_IRQRestore(Flags);
	 
	 Watchdog_Sync();
 }

 /** @brief			Set the timeout of the watchdog.
  *  @param Timeout Timeout in clock cycles of the 1 kHz ULP oscillator
 */
 static inline void Watchdog_SetTimeout(const Watchdog_Time_t Timeout) __attribute__ ((always_inline));
 static inline void Watchdog_SetTimeout(const Watchdog_Time_t Timeout)
 {
	 uint8_t Flags = CPU_IRQSave();
	 
	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"		"\n\t"
					"out   %3, r16"		"\n\t"
					"st     Z,  %1"		"\n\t"
					::	"r" (&WDT.CTRL), 
						"r" ((WDT.CTRL & ~((Timeout & 0x0F) << 0x02)) | (WDT_CEN_bm | ((Timeout & 0x0F) << 0x02))), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);
	 
	 CPU_IRQRestore(Flags);
	 
	 Watchdog_Sync();
 }

 /** @brief		Set the device into sleep mode.
  *  @return	Timeout in clock cycles of the 1 kHz ULP oscillator
 */
 static inline const Watchdog_Time_t Watchdog_GetTimeout(void) __attribute__ ((always_inline));
 static inline const Watchdog_Time_t Watchdog_GetTimeout(void)
 {
	 return (WDT.CTRL >> 0x02) & 0x0F;
 }

 /** @brief	Enable the window mode of the watchdog.
 */
 static inline void Watchdog_WindowEnable(void) __attribute__ ((always_inline));
 static inline void Watchdog_WindowEnable(void)
 {	 
	 uint8_t Flags = CPU_IRQSave();
	 
	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"		"\n\t"
					"out   %3, r16"		"\n\t"
					"st     Z,  %1"		"\n\t"
					::	"r" (&WDT.WINCTRL), 
						"r" ((WDT.WINCTRL & ~0x02) | (WDT_WEN_bm | WDT_WCEN_bm)), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);
	 
	 CPU_IRQRestore(Flags);
	 
	 Watchdog_Sync();
 }

 /** @brief	Disable the window mode of the watchdog.
 */
 static inline void Watchdog_WindowDisable(void) __attribute__ ((always_inline));
 static inline void Watchdog_WindowDisable(void)
 {	 
	 uint8_t Flags = CPU_IRQSave();

	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"		"\n\t"
					"out   %3, r16"		"\n\t"
					"st     Z,  %1"		"\n\t"
					::	"r" (&WDT.WINCTRL), 
						"r" ((WDT.WINCTRL & ~0x02) | WDT_WCEN_bm), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);
	 
	 CPU_IRQRestore(Flags);
 
	 Watchdog_Sync();
 }

 /** @brief			Set the window setting of the watchdog.
  *  @param Window	Window time in clock cycles of the 1 kHz ULP oscillator
 */
 static inline void Watchdog_SetWindow(const Watchdog_Time_t Window) __attribute__ ((always_inline));
 static inline void Watchdog_SetWindow(const Watchdog_Time_t Window)
 {	 
	 uint8_t Flags = CPU_IRQSave();
	
	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"		"\n\t"
					"out   %3, r16"		"\n\t"
					"st     Z,  %1"		"\n\t"
					::	"r" (&WDT.WINCTRL), 
						"r" ((WDT.WINCTRL & ~((Window & 0x0F) << 0x02)) | (WDT_CEN_bm | ((Window & 0x0F) << 0x02))), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);

	 CPU_IRQRestore(Flags);
	 
	 Watchdog_Sync();
 }

 /** @brief		Get the window setting of the watchdog.
  *  @return	Window time in clock cycles of the 1 kHz ULP oscillator
 */
 static inline const Watchdog_Time_t Watchdog_GetWindow(void) __attribute__ ((always_inline));
 static inline const Watchdog_Time_t Watchdog_GetWindow(void)
 {
	 return (WDT.WINCTRL >> 0x02) & 0x0F;
 }

#endif /* WATCHDOG_H_  */ 