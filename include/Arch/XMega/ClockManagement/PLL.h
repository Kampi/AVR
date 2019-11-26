/*
 * PLL.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega PLL module. 

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

/** @file Arch/XMega/ClockManagement/PLL.h
 *  @brief Driver for Atmel AVR XMega PLL module. 
 *
 *  This file contains the prototypes and definitions for the Atmel AVR XMega PLL driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef PLL_H_
#define PLL_H_
 
 #include "Common/Common.h"

 /** @brief PLL clock source.
  */
 typedef enum
 {
	 PLL_CLOCK_2MHZ = 0x00,			/**< 2 MHz internal oscillator */ 
	 PLL_CLOCK_32MHZ = 0x02,		/**< 32 MHz internal oscillator */ 
	 PLL_CLOCK_XOSC = 0x03,			/**< External crystal oscillator */ 
 } PLL_Clock_t;

 /** @brief PLL configuration object.
  */
 typedef struct
 {
	 PLL_Clock_t Clock;				/**< PLL clock source */ 
	 Bool_t DividedOutput;			/**< Clock division of 2 */ 
	 uint8_t Mult;					/**< PLL multiplier */ 
 } PLL_Config_t;

 /** @brief	Enable the PLL.
  */
 static inline void PLL_Enable(void) __attribute__((always_inline));
 static inline void PLL_Enable(void)
 {
	 OSC.CTRL |= OSC_PLLEN_bm;
	 while(!(OSC.STATUS & OSC_PLLRDY_bm));
 }

 /** @brief	Disable the PLL.
  */
 static inline void PLL_Disable(void) __attribute__((always_inline));
 static inline void PLL_Disable(void)
 {
	 OSC.CTRL &= ~OSC_PLLEN_bm;
 }
 
 /** @brief			Set the multiplication for the PLL.
  *  @param Mult	PLL multiplication
  */
 static inline void PLL_SetMultiplicator(const uint8_t Mult) __attribute__((always_inline));
 static inline void PLL_SetMultiplicator(const uint8_t Mult)
 {
	 // Resize value to 5 bit
	 uint8_t Mult_Int = Mult & 0x1F;
	
	 OSC.PLLCTRL = (OSC.PLLCTRL & (~0x1F)) | Mult_Int;
 }

 /** @brief		Get the multiplication for the PLL.
  *  @return	PLL multiplication
  */
 static inline uint8_t PLL_GetMultiplicator(void) __attribute__((always_inline));
 static inline uint8_t PLL_GetMultiplicator(void)
 {
	 return (OSC.PLLCTRL & 0x1F) >> 0x04;
 }

 /** @brief			Set the clock source for the PLL.
  *  @param Clock	PLL clock source
  */
 static inline void PLL_SetClockSource(const PLL_Clock_t Clock) __attribute__((always_inline));
 static inline void PLL_SetClockSource(const PLL_Clock_t Clock)
 {
	 OSC.PLLCTRL |= (Clock << 0x06);
 }

 /** @brief		Get the clock source for the PLL.
  *  @return	PLL clock source
  */
 static inline PLL_Clock_t PLL_GetClockSource(void) __attribute__((always_inline));
 static inline PLL_Clock_t PLL_GetClockSource(void)
 {
	 return (OSC.PLLCTRL & 0xC0) >> 0x06;
 }

 /** @brief			Enable/Disable the divided output for the PLL.
  *  @param Enable	Enable/Disable
  */
 static inline void PLL_SetDividedOutput(Bool_t Enable) __attribute__((always_inline));
 static inline void PLL_SetDividedOutput(Bool_t Enable)
 {
	 OSC.PLLCTRL = (OSC.PLLCTRL & (~(0x01 << 0x05))) | (Enable << 0x05);
 }

 /** @brief		Get the status of the divided output for the PLL.
  *  @return	*TRUE if divided output is enabled
  */
 static inline Bool_t PLL_GetDividedOutput(void) __attribute__((always_inline));
 static inline Bool_t PLL_GetDividedOutput(void)
 {
	 return (OSC.PLLCTRL & 0x20) >> 0x05;
 }

 /** @brief			Initialize the PLL device.
  *  @param Config	Pointer to PLL configuration struct
  */
 static inline void PLL_Init(PLL_Config_t* Config) __attribute__((always_inline));
 static inline void PLL_Init(PLL_Config_t* Config)
 {
	 PLL_SetClockSource(Config->Clock);
	 PLL_SetDividedOutput(Config->DividedOutput);
	 PLL_SetMultiplicator(Config->Mult);
	 PLL_Enable();
 }
 
 /** @brief			Get the PLL configuration.
  *  @param Config	Pointer to PLL configuration struct
  */
 static inline void PLL_GetConfig(PLL_Config_t* Config) __attribute__((always_inline));
 static inline void PLL_GetConfig(PLL_Config_t* Config)
 {
	 Config->Clock = PLL_GetClockSource();
	 Config->DividedOutput = PLL_GetDividedOutput();
	 Config->Mult = PLL_GetMultiplicator();
 }

#endif /* PLL_H_ */