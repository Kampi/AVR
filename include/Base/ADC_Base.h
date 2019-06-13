/*
 * ADC_Base.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for Atmel AVR ADC

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

/** @file ADC_Base.h
 *  @brief Common definitions for AVR ADC.
 *
 *  @author Daniel Kampert
 */

#ifndef ADC_BASE_H_
#define ADC_BASE_H_

 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Base/AVR8/ADC_AVR8.h"
 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Base/XMega/ADC_XMega.h"
 #endif
 
 #include "Board.h"

/** @brief			ADC callback definition
 *  @param Device	ADC device
 *  @param Channel	ADC channel
 */
 typedef void (*ADC_Callback_t)(uint8_t Device, uint8_t Channel);
 
 /** @brief ADC conversion mode.
  */
 typedef enum
 {
	 ADC_CONVMODE_UNSIGNED = 0x00,					/**< Unsigned mode */ 
	 ADC_CONVMODE_SIGNED = 0x01,					/**< Signed mode */ 
 } ADC_ConversionMode_t;

 /** @brief ADC callback types.
  */
 typedef enum
 {
	 ADC_CONVERSION_COMPLETE_INTERRUPT = 0x01,		/**< ADC conversion complete interrupt */ 
	 ADC_THRESH_BELOW_INTERRUPT = 0x02,				/**< ADC threshold below interrupt */ 
	 ADC_THRESH_ABOVE_INTERRUPT = 0x04,				/**< ADC threshold above interrupt */ 
 } ADC_CallbackType_t;

#endif /* ADC_BASE_H_ */