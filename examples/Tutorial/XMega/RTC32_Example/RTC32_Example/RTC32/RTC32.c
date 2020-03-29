/*
 * RTC32.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: RTC32 for XMega.

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

/** @file RTC32.c
 *  @brief Driver for XMega RTC32.
 *
 *  @author Daniel Kampert
 */

#include "RTC32.h"

void RTC32_Init(void)
{
	// Disable the RTC32 for writing the register
	RTC32.CTRL &= ~RTC32_ENABLE_bm;
	while(RTC32_Sync());

	RTC32.PER = 0x09;
	RTC32.COMP = 0x04;
	RTC32.CNT = 0x00;

	// Enable the RTC32 and wait for register sync
	RTC32.CTRL |= RTC32_ENABLE_bm;
	while(RTC32_Sync());
}

uint8_t RTC32_GetCount(void)
{
	// Start the synchronization of the CNT register from the RTC32 domain to the system domain
	RTC32.SYNCCTRL |= RTC32_SYNCCNT_bm;
	while(RTC32_Sync());
	
	return RTC32.CNT;
}

void RTC32_EnableInterrupts(void)
{
	// Enable interrupts
	RTC32.INTCTRL = RTC32_COMPINTLVL_LO_gc | RTC32_OVFINTLVL_LO_gc;
}

void RTC32_DisableInterrupts(void)
{
	// Disable interrupts
	RTC32.INTCTRL = 0x00;
}

ISR(RTC32_COMP_vect)
{
	// Toggle LED0 on compare interrupt
	PORTR.OUTTGL = (0x01 << 0x00);
}

ISR(RTC32_OVF_vect)
{
	// Toggle LED0 on overflow interrupt
	PORTR.OUTTGL = (0x01 << 0x01);
}