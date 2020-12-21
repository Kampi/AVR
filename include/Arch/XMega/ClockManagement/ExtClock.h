/*
 * ExtClock.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega external clocks.

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

/** @file Arch/XMega/ClockManagement/ExtClock.h
 *  @brief Driver for Atmel AVR  XMega external clocks. 
 *
 *  This file contains the prototypes and definitions for the Atmel AVR XMega external clocks.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef EXTCLOCK_H_
#define EXTCLOCK_H_

 #include "Common/Common.h"

 /** @brief	Enable the low power mode for a external 32 kHz oscillator.
  */
 static inline void ExtClock_32kLowPowerEnable(void) __attribute__((always_inline));
 static inline void ExtClock_32kLowPowerEnable(void)
 {
	 OSC.XOSCCTRL |= OSC_X32KLPM_bm;
 }

 /** @brief	Disable the low power mode for a external 32 kHz oscillator.
  */
 static inline void ExtClock_32kLowPowerDisable(void) __attribute__((always_inline));
 static inline void ExtClock_32kLowPowerDisable(void)
 {
	 OSC.XOSCCTRL &= ~OSC_X32KLPM_bm;
 }

#endif /* EXTCLOCK_H_ */