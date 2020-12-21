/*
 * RTC32.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega RTC32.

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

/** @file RTC32/RTC32.h
 *  @brief Driver for XMega RTC32.
 *
 *  @author Daniel Kampert
 */

#ifndef RTC32_H_
#define RTC32_H_

 #include <avr/io.h>
 #include <avr/interrupt.h>
 
 static inline uint8_t RTC32_Sync(void) __attribute__ ((always_inline));
 static inline uint8_t RTC32_Sync(void)
 {
	 return (RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm);
 }
 
 void RTC32_Init(void);
 uint8_t RTC32_GetCount(void);
 void RTC32_EnableInterrupts(void);
 void RTC32_DisableInterrupts(void);

#endif /* RTC32_H_ */