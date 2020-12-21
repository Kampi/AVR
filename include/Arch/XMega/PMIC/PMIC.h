/*
 * PMIC.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega programmable interrupt controller.

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

/** @file Arch/XMega/PMIC/PMIC.h
 *  @brief Driver for the Atmel AVR XMega programmable interrupt controller.
 *
 *  This contains the prototypes and definitions for the XMega programmable interrupt controller.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef PMIC_H_
#define PMIC_H_

 #include "Common/Common.h"

 /** @brief Interrupt levels for peripheral modules.
  */
 typedef enum
 {
	 INT_LVL_LO = 0x01,				/**< Priority low */ 
	 INT_LVL_MED = 0x02,			/**< Priority medium */ 
	 INT_LVL_HI = 0x03,				/**< Priority high */ 
 } Interrupt_Level_t;
 
 /** @brief			Enable a specific interrupt level.
  *  @param Mask	Interrupt level mask
  */
 static inline void PMIC_EnableInterruptLevel(const Interrupt_Level_t Mask) __attribute__((always_inline));
 static inline void PMIC_EnableInterruptLevel(const Interrupt_Level_t Mask)
 {
	 if(Mask & INT_LVL_LO)
	 {
		 PMIC.CTRL |= 0x01;
	 }

	 if(Mask & INT_LVL_MED)
	 {
		 PMIC.CTRL |= 0x02;
	 }
	 
	 if(Mask & INT_LVL_HI)
	 {
		 PMIC.CTRL |= 0x04;
	 }
 }

 /** @brief			Disable a specific interrupt level.
  *  @param Mask	Interrupt level mask
  */
 static inline void PMIC_DisableInterruptLevel(const Interrupt_Level_t Mask) __attribute__((always_inline));
 static inline void PMIC_DisableInterruptLevel(const Interrupt_Level_t Mask)
 {
	 if(Mask & INT_LVL_LO)
	 {
		 PMIC.CTRL &= ~0x01;
	 }

	 if(Mask & INT_LVL_MED)
	 {
		 PMIC.CTRL &= ~0x02;
	 }
	 
	 if(Mask & INT_LVL_HI)
	 {
		 PMIC.CTRL &= ~0x04;
	 }
 }


 /** @brief	Enable all interrupt level.
  */
 static inline void PMIC_EnableAllInterruptLevel(void) __attribute__((always_inline));
 static inline void PMIC_EnableAllInterruptLevel(void)
 {
	 PMIC.CTRL |= 0x07;
 }

 /** @brief	Disable all interrupt level.
  */
 static inline void PMIC_DisableAllInterruptLevel(void) __attribute__((always_inline));
 static inline void PMIC_DisableAllInterruptLevel(void)
 {
	 PMIC.CTRL &= ~0x07;
 }

 /** @brief	Enable round-robin scheduling.
  */
 static inline void PMIC_EnableRoundRobin(void) __attribute__((always_inline));
 static inline void PMIC_EnableRoundRobin(void)
 {
	 PMIC.CTRL |= PMIC_RREN_bm;
 }

 /** @brief	Disable round-robin scheduling.
  */
 static inline void PMIC_DisableRoundRobin(void) __attribute__((always_inline));
 static inline void PMIC_DisableRoundRobin(void)
 {
	 PMIC.CTRL &= ~PMIC_RREN_bm;
 }

#endif /* PMIC_H_ */