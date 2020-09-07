/*
 * RTC.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR RTC service.

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

/** @file Services/RTC/RTC.c
 *  @brief AVR RTC service.
 *
 *  This contains the implementation of the RTC service.
 *
 *  @author Daniel Kampert
 */

#include "Services/RTC/RTC.h"

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#if(defined RTC32)
		#include "Arch/XMega/RTC32/RTC32.h"
	#else
		#include "Arch/XMega/RTC/RTC.h"
	#endif

	#include "Arch/XMega/CPU/CPU.h"
	#include "Arch/XMega/PMIC/PMIC.h"
#else
	#error "Architecture not supported for system timer service!"
#endif