/*
 * ADC.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AT90 ADC.

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file ADC/ADC.h
 *  @brief Driver for Atmel AT90 ADC.
 *
 *  This contains the prototypes and definitions for the AT90 ADC driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef ADC_H_
#define ADC_H_

 #include "Common.h"

 /** @brief			ADC callback definition.
  *  @param Channel	ADC channel
  *  @param Result	Conversion result
 */
 typedef void (*ADC_Callback_t)(const uint8_t Channel, const uint16_t Result);

 /** @brief			Set the conversion channel.
  *  @param Channel	ADC channel
  */
 static inline void ADC_SetChannel(const uint8_t Channel) __attribute__ ((always_inline));
 static inline void ADC_SetChannel(const uint8_t Channel)
 {
	 ADMUX &= ~0x07;
	 ADMUX |= (Channel & 0x07);
 }

 /** @brief	Start a new ADC conversion.
  */
 static inline void ADC_StartConversion(void) __attribute__ ((always_inline));
 static inline void ADC_StartConversion(void)
 {
	 ADCSRA |= (0x01 << ADSC);
 }

 /** @brief	Wait until a conversion has finished.
  */
 static inline void ADC_Wait(void) __attribute__ ((always_inline));
 static inline void ADC_Wait(void)
 {
	 while(ADCSRA & (0x01 << ADSC));
 }

 /** @brief				Initialize the ADC.
  *  @param Callback	ADC conversion complete callback
  */
 void ADC_Init(const ADC_Callback_t Callback);

#endif /* ADC_H_ */