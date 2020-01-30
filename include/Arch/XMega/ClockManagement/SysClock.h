/*
 * SysClock.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega system clock.

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

/** @file Arch/XMega/ClockManagement/SysClock.h
 *  @brief Driver for Atmel AVR XMega clock system. 
 *
 *  This file contains the prototypes and definitions for the Atmel AVR XMega clock system.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef SYSCLOCK_H_
#define SYSCLOCK_H_

 #include "Common/Common.h"

 #include "Arch/XMega/CPU/CPU.h"

 /** @brief Clock sources.
  */
 typedef enum
 {
	 CLOCK_SOURCE_INT2MHZ = 0x00,			/**< 2 MHz internal oscillator */
	 CLOCK_SOURCE_INT32MHZ = 0x01,			/**< 32 MHz internal oscillator */
	 CLOCK_SOURCE_INT32KHZ = 0x02,			/**< 32 kHz internal low power oscillator */
	 CLOCK_SOURCE_EXT = 0x03,				/**< External oscillator */
	 CLOCK_SOURCE_PLL = 0x04,				/**< PLL as oscillator source */
	 CLOCK_SOURCE_EXT32KHZ = 0x05,			/**< 32 kHz external oscillator */
 } ClockSource_t;

 /** @brief Clock prescaler A.
  */
 typedef enum
 {
	 SYSCLOCK_PRESCALER_A_1 = 0x00,			/**< Prescaler 1 */
	 SYSCLOCK_PRESCALER_A_2 = 0x01,			/**< Prescaler 2 */
	 SYSCLOCK_PRESCALER_A_4 = 0x03,			/**< Prescaler 4 */
	 SYSCLOCK_PRESCALER_A_8 = 0x05,			/**< Prescaler 8 */
	 SYSCLOCK_PRESCALER_A_16 = 0x07,		/**< Prescaler 16 */
	 SYSCLOCK_PRESCALER_A_32 = 0x09,		/**< Prescaler 32 */
	 SYSCLOCK_PRESCALER_A_64 = 0x0B,		/**< Prescaler 64 */
	 SYSCLOCK_PRESCALER_A_128 = 0x0D,		/**< Prescaler 128 */
	 SYSCLOCK_PRESCALER_A_256 = 0x0F,		/**< Prescaler 256 */
	 SYSCLOCK_PRESCALER_A_512 = 0x11,		/**< Prescaler 1024 */
 } ClockPrescalerA_t;

 /** @brief Clock prescaler B and C.
  */
 typedef enum
 {
	 SYSCLOCK_PRESCALER_BC_1_1 = 0x00,		/**< Prescaler B 1, C 1 */
	 SYSCLOCK_PRESCALER_BC_1_2 = 0x01,		/**< Prescaler B 1, C 2 */
	 SYSCLOCK_PRESCALER_BC_4_1 = 0x02,		/**< Prescaler B 4, C 1 */
	 SYSCLOCK_PRESCALER_BC_2_2 = 0x03,		/**< Prescaler B 2, C 2 */ 
 } ClockPrescalerBC_t;

 /** @brief USB clock prescaler.
  */
 typedef enum
 {
	 SYSCLOCK_USB_PRESCALER_1 = 0x00,		/**< Prescaler 1 */
	 SYSCLOCK_USB_PRESCALER_2 = 0x01,		/**< Prescaler 2 */
	 SYSCLOCK_USB_PRESCALER_4 = 0x02,		/**< Prescaler 4 */
	 SYSCLOCK_USB_PRESCALER_8 = 0x03,		/**< Prescaler 8 */
	 SYSCLOCK_USB_PRESCALER_16 = 0x04,		/**< Prescaler 16 */
	 SYSCLOCK_USB_PRESCALER_32 = 0x05,		/**< Prescaler 32 */
 } USBClockPrescaler_t;

 /** @brief USB clock settings.
  */
 typedef enum
 {
	 USB_CLOCK_6MHZ = 0x00,					/**< 6 MHz USB clock */
	 USB_CLOCK_48MHZ = 0x01,				/**< 48 MHz USB clock */
 } USB_Clock_t;

 /** @brief USB clock source.
  */
 typedef enum
 {
	 USB_SOURCE_PLL = 0x00,					/**< PLL as USB clock source */
	 USB_SOURCE_RC32 = 0x01,				/**< Internal 32 MHz oscillator as clock source */
 } USB_ClockSource_t;

 /** @brief			Enable a clock source.
  *  @param Clock	Clock source
  */
 static inline void SysClock_EnableClock(const ClockSource_t Clock) __attribute__((always_inline));
 static inline void SysClock_EnableClock(const ClockSource_t Clock)
 {
	OSC.CTRL |= (0x01 << Clock);
	while(!(OSC.STATUS & (0x01 << Clock)));	
 }
 
 /** @brief	Enable the clock for the USB module.
  */
 static inline void SysClock_EnableUSBClock(void) __attribute__((always_inline));
 static inline void SysClock_EnableUSBClock(void)
 {
	CLK.USBCTRL |= 0x01;
 }
 
 /** @brief	Disable the clock for the USB module.
  */
 static inline void SysClock_DisableUSBClock(void) __attribute__((always_inline));
 static inline void SysClock_DisableUSBClock(void)
 {
	 CLK.USBCTRL &= ~0x01;
 }

 /** @brief			Set the clock for the USB module.
  *  @param Source	USB clock source
  */
 static inline void SysClock_SetUSBClockSource(const USB_ClockSource_t Source) __attribute__((always_inline));
 static inline void SysClock_SetUSBClockSource(const USB_ClockSource_t Source)
 {
	  CLK.USBCTRL = (CLK.USBCTRL & (~(0x03 << 0x01))) | (Source << 0x01);
 }

 /** @brief				Set the prescaler for the USB clock.
  *  @param Prescaler	USB clock prescaler
  */
 static inline void SysClock_SetUSBPrescaler(const USBClockPrescaler_t Prescaler) __attribute__((always_inline));
 static inline void SysClock_SetUSBPrescaler(const USBClockPrescaler_t Prescaler)
 {
	  CLK.USBCTRL = (CLK.USBCTRL & (~(0x07 << 0x03))) | (Prescaler << 0x03);
 }

 /** @brief			Switch the system clock source.
  *					NOTE: You have to enable the clock source first!
  *  @param Source	Clock source
  */
 static inline void SysClock_SetClockSource(const ClockSource_t Source) __attribute__((always_inline));
 static inline void SysClock_SetClockSource(const ClockSource_t Source)
 {
	 uint8_t Flags = CPU_IRQSave();
	 
	 /* 
	  * Use inline assembler, because otherwise the code will only run with at least
	  * optimization level -O1. This level is bad for debugging purposes. You can avoid
	  * this issue by using inline assembler.
	 */
	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&CLK.CTRL), 
						"r" (Source & 0x07), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);

	 CPU_IRQRestore(Flags);
 }

 /** @brief				Set the prescaler A for the main clock.
  *  @param Prescaler	Prescaler value
  */
 static inline void SysClock_SetClockPrescalerA(const ClockPrescalerA_t Prescaler) __attribute__((always_inline));
 static inline void SysClock_SetClockPrescalerA(const ClockPrescalerA_t Prescaler)
 {
	 uint8_t Flags = CPU_IRQSave();

	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&CLK.PSCTRL), 
						"r" ((CLK.PSCTRL & (~(0x1F << 0x02))) | (Prescaler << 0x02)), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);

	 CPU_IRQRestore(Flags);
 }

 /** @brief				Set the prescaler B/C for the main clock.
  *  @param Prescaler	Prescaler value
  */
 static inline void SysClock_SetClockPrescalerBC(const ClockPrescalerBC_t Prescaler) __attribute__((always_inline));
 static inline void SysClock_SetClockPrescalerBC(const ClockPrescalerBC_t Prescaler)
 {
	 // ToDo
 }

 /** @brief			Enable the clock for the USB module.
  *  @param Source	Clock source
  *  @param Clock	USB clock frequency
  */
 static inline void SysClock_ConfigUSBClock(const USB_ClockSource_t Source, const USB_Clock_t Clock) __attribute__((always_inline));
 static inline void SysClock_ConfigUSBClock(const USB_ClockSource_t Source, const USB_Clock_t Clock)
 {
	 USBClockPrescaler_t Prescaler = SYSCLOCK_USB_PRESCALER_1;
	 
	 if(Clock == USB_CLOCK_6MHZ)
	 {
		 Prescaler = CLK_USBPSDIV_8_gc;
	 }
	
	 if(Source == USB_SOURCE_RC32)
	 {
		 if(!(OSC.STATUS & OSC_RC32MEN_bm))
		 {
			 SysClock_EnableClock(CLOCK_SOURCE_INT32MHZ);
		 }
		
		 SysClock_SetUSBClockSource(USB_SOURCE_RC32);
	 }

	 SysClock_SetUSBPrescaler(Prescaler);
	 SysClock_EnableUSBClock();
 }

 /** @brief	Initialize the CPU clock.
  */
 static inline void SysClock_Init(void) __attribute__ ((always_inline));
 static inline void SysClock_Init(void)
 {
	 #if(F_CPU == 32000000UL)
		 SysClock_EnableClock(CLOCK_SOURCE_INT32MHZ);
		 SysClock_SetClockSource(CLOCK_SOURCE_INT32MHZ);
	 #elif(F_CPU == 2000000UL)
		 SysClock_EnableClock(CLOCK_SOURCE_INT2MHZ);
		 SysClock_SetClockSource(CLOCK_SOURCE_INT2MHZ);
	 #else
	 #warning "F_CPU not valid! Use default 2 MHz clock."
		 SysClock_EnableClock(CLOCK_SOURCE_INT2MHZ);
		 SysClock_SetClockSource(CLOCK_SOURCE_INT2MHZ);
	 #endif
 }
 
 /** @brief		Get the current main system clock.
  *  @return	Clock frequency
  */
 static inline const uint32_t SysClock_GetClock(void) __attribute__ ((always_inline));
 static inline const uint32_t SysClock_GetClock(void)
 {
	 if(OSC.CTRL & OSC_RC32MEN_bm)
	 {
		 return 32000000;
	 }
	 
	 // 2 MHz oscillator is always enabled
	 return 2000000;
 }

  /** @brief	Get the current clock from the clk_Per4 domain.
  *  @return	CPU clock frequency
  */
 uint32_t SysClock_GetClockPer4(void);

 /** @brief		Get the current clock from the clk_Per2 domain.
  *  @return	CPU clock frequency
  */
 uint32_t SysClock_GetClockPer2(void);
 
 /** @brief		Get the current clock from the clk_Per domain.
  *  @return	Clock frequency
  */
 uint32_t SysClock_GetClockPer(void);

#endif /* SYSCLOCK_H_ */