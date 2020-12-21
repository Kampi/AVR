/*
 * HW_Config.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Hardware configuration file for the NEC IR interface.

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

/** @file Interfaces/IR-Remote/HW_Config.c
 *  @brief Hardware configuration file for the NEC IR interface.
 *
 *  @author Daniel Kampert
 */

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#include "Arch/XMega/Timer/Timer.h"
#else
	#error "Architecture not supported for IR remote service!"
#endif

#include "Interfaces/IR-Remote/NEC_IR.h"

/** @brief Timer configuration object to create an interrupt every 50 us.
 */
Timer0_Config_t _IR_TimerConfig = 
{
	.Device = &TCD0,
	.Prescaler = TIMER_PRESCALER_8
};

/** @brief Timer interrupt configuration object.
 */
Timer0_InterruptConfig_t _IR_TimerInterrupt =
{
	.Device = &TCD0,
	.Source = TIMER_OVERFLOW_INTERRUPT,
	.InterruptLevel = IR_INTERRUPT_LEVEL
};