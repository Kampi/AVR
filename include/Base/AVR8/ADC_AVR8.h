/*
 * ADC_AVR8.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for Atmel AVR8 ADC

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

/** @file Base/AVR8/ADC_AVR8.h
 *  @brief Common definitions for for Atmel AVR8 ADC
 *
 *  @author Daniel Kampert
 */

#ifndef ADC_AVR8_H_
#define ADC_AVR8_H_

 /** @brief	ADC auto trigger sources
 */
 typedef enum
 {
	 ADC_AUTOTRIGGER_FREERUN = 0x00,				/**< Free run mode */
	 ADC_AUTOTRIGGER_AC = 0x00,						/**< Analog comparator */
	 ADC_AUTOTRIGGER_EXTINT0 = 0x00,				/**< External Interrupt Request 0 */
	 ADC_AUTOTRIGGER_COMP0 = 0x00,					/**< Timer/Counter 0 Compare Match */
	 ADC_AUTOTRIGGER_OVF0 = 0x00,					/**< Timer/Counter 0 Overflow */
	 ADC_AUTOTRIGGER_COMP1B = 0x00,					/**< Timer/Counter 1 Compare Match B */
	 ADC_AUTOTRIGGER_OVF1 = 0x00,					/**< Timer/Counter 1 Overflow */
	 ADC_AUTOTRIGGER_CAP1 = 0x00,					/**< Timer/Counter 1 Capture Event */
 } ADC_AutoTrigger_t;
 
 /** @brief ADC voltage reference.
 */
 typedef enum
 {
	ADC_REFERENCE_AREF = 0x00,						/**< Use AREF pin */ 
	ADC_REFERENCE_AVCC = 0x01,						/**< Use AVCC with external capacitor at AREF pin */ 
	ADC_REFERENCE_INT = 0x03,						/**< Internal 2.56 V Reference */
 } ADC_Reference_t;

 /** @brief ADC clock prescaler.
 */
 typedef enum
 {
	ADC_PRESCALER_2 = 0x01,							/**< Clock prescaler 2 */ 
	ADC_PRESCALER_4 = 0x02,							/**< Clock prescaler 4 */ 
	ADC_PRESCALER_8 = 0x03,							/**< Clock prescaler 8 */ 
	ADC_PRESCALER_16 = 0x04,						/**< Clock prescaler 16 */ 
	ADC_PRESCALER_32 = 0x05,						/**< Clock prescaler 32 */ 
	ADC_PRESCALER_64 = 0x06,						/**< Clock prescaler 64 */ 
	ADC_PRESCALER_128 = 0x07,						/**< Clock prescaler 128 */ 
 } ADC_Prescaler_t;

 /** @brief	ADC conversion format.
 */
 typedef enum
 {
	ADC_CONV_FORM_RIGHT = 0x00,						/**< 10 bit resolution right adjusted */ 
	ADC_CONV_FORM_LEFT = 0x01,						/**< 10 bit resolution left adjusted */ 
 } ADC_ConversionFormat_t;

 /** @brief	ADC input modes.
 */
 typedef enum
 {
	 ADC_SE_ADC0 = 0x00,							/**< Single ended ADC0 */
	 ADC_SE_ADC1 = 0x01,							/**< Single ended ADC1 */
	 ADC_SE_ADC2 = 0x02,							/**< Single ended ADC2 */
	 ADC_SE_ADC3 = 0x03,							/**< Single ended ADC3 */
	 ADC_SE_ADC4 = 0x04,							/**< Single ended ADC4 */
	 ADC_SE_ADC5 = 0x05,							/**< Single ended ADC5 */
	 ADC_SE_ADC6 = 0x06,							/**< Single ended ADC6 */
	 ADC_SE_ADC7 = 0x07,							/**< Single ended ADC7 */
	 ADC_P_ADC0_N_ADC0_10 = 0x08,					/**< Differential positive ADC0, negative ADC0, Gain 10x */
	 ADC_P_ADC1_N_ADC0_10 = 0x09,					/**< Differential positive ADC1, negative ADC0, Gain 10x */
	 ADC_P_ADC0_N_ADC0_200 = 0x0A,					/**< Differential positive ADC0, negative ADC0, Gain 200x */
	 ADC_P_ADC1_N_ADC0_200 = 0x0B,					/**< Differential positive ADC1, negative ADC0, Gain 200x */
	 ADC_P_ADC2_N_ADC2_10 = 0x0C,					/**< Differential positive ADC2, negative ADC2, Gain 10x */
	 ADC_P_ADC3_N_ADC2_10 = 0x0D,					/**< Differential positive ADC3, negative ADC2, Gain 10x */
	 ADC_P_ADC2_N_ADC2_200 = 0x0E,					/**< Differential positive ADC2, negative ADC2, Gain 200x */
	 ADC_P_ADC3_N_ADC2_200 = 0x0F,					/**< Differential positive ADC2, negative ADC3, Gain 200x */
	 ADC_P_ADC0_N_ADC1_1 = 0x10,					/**< Differential positive ADC0, negative ADC1, Gain 1x */
	 ADC_P_ADC1_N_ADC1_1 = 0x11,					/**< Differential positive ADC1, negative ADC1, Gain 1x */
	 ADC_P_ADC2_N_ADC1_1 = 0x12,					/**< Differential positive ADC2, negative ADC1, Gain 1x */
	 ADC_P_ADC3_N_ADC1_1 = 0x13,					/**< Differential positive ADC3, negative ADC1, Gain 1x */
	 ADC_P_ADC4_N_ADC1_1 = 0x14,					/**< Differential positive ADC4, negative ADC1, Gain 1x */
	 ADC_P_ADC5_N_ADC1_1 = 0x15,					/**< Differential positive ADC5, negative ADC1, Gain 1x */
	 ADC_P_ADC6_N_ADC1_1 = 0x16,					/**< Differential positive ADC6, negative ADC1, Gain 1x */
	 ADC_P_ADC7_N_ADC1_1 = 0x17,					/**< Differential positive ADC7, negative ADC1, Gain 1x */
	 ADC_P_ADC0_N_ADC1_2 = 0x18,					/**< Differential positive ADC0, negative ADC2, Gain 1x */
	 ADC_P_ADC1_N_ADC1_2 = 0x19,					/**< Differential positive ADC1, negative ADC2, Gain 1x */
	 ADC_P_ADC2_N_ADC1_2 = 0x1A,					/**< Differential positive ADC2, negative ADC2, Gain 1x */
	 ADC_P_ADC3_N_ADC1_2 = 0x1B,					/**< Differential positive ADC3, negative ADC2, Gain 1x */
	 ADC_P_ADC4_N_ADC1_2 = 0x1C,					/**< Differential positive ADC4, negative ADC2, Gain 1x */
	 ADC_P_ADC5_N_ADC1_2 = 0x1D,					/**< Differential positive ADC5, negative ADC2, Gain 1x */
	 ADC_SE_122V = 0x1E,							/**< Single ended 1.22 V */
	 ADC_SE_GND = 0x1F,								/**< Single ended GND */
 } ADC_InputModes_t;

#endif /* ADC_AVR8_H_ */