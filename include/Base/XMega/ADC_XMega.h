/*
 * ADC_XMega.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for Atmel XMega ADC

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

/** @file Base/XMega/ADC_XMega.h
 *  @brief Common definitions for for Atmel XMega ADC
 *
 *  @author Daniel Kampert
 */

#ifndef ADC_XMEGA_H_
#define ADC_XMEGA_H_

 /** @brief	ADC voltage reference.
 */
 typedef enum
 {
	ADC_REFERENCE_INT1V = 0x00,						/**< 1 V reference */ 
	ADC_REFERENCE_INTVCC = 0x01,					/**< Internal Vcc / 1.6 reference */ 
	ADC_REFERENCE_AREFA = 0x02,						/**< Port A reference */ 
	ADC_REFERENCE_AREFB = 0x03,						/**< Port B reference */ 
	ADC_REFERENCE_INTVCC2 = 0x04					/**< Internal Vcc reference */ 
 } ADC_Reference_t;
 
 /** @brief	ADC clock prescaler.
 */
 typedef enum
 {
	ADC_PRESCALER_4 = 0x00,							/**< Clock prescaler 4 */ 
	ADC_PRESCALER_8 = 0x01,							/**< Clock prescaler 8 */ 
	ADC_PRESCALER_16 = 0x02,						/**< Clock prescaler 16 */ 
	ADC_PRESCALER_32 = 0x03,						/**< Clock prescaler 32 */ 
	ADC_PRESCALER_64 = 0x04,						/**< Clock prescaler 64 */ 
	ADC_PRESCALER_128 = 0x05,						/**< Clock prescaler 128 */ 
	ADC_PRESCALER_256 = 0x06,						/**< Clock prescaler 256 */ 
	ADC_PRESCALER_512 = 0x07,						/**< Clock prescaler 512 */ 
 } ADC_Prescaler_t;

 /** @brief	ADC conversion format.
 */
 typedef enum
 {
	ADC_CONV_FORM_12 = 0x00,						/**< 12 bit resolution right adjusted */ 
	ADC_CONV_FORM_8 = 0x01,							/**< 8 bit resolution */ 
	ADC_CONV_FORM_12_LEFT = 0x03,					/**< 12 bit resolution left adjusted */ 
 } ADC_ConversionFormat_t;

#endif /* ADC_XMEGA_H_ */