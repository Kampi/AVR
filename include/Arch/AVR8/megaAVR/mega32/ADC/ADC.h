/*
 * ADC.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 megaAVR ADC module.

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

/** @file Arch/AVR8/megaAVR/ADC/ADC.h
 *  @brief Driver for Atmel AVR8 megaAVR ADC module.
 *
 *  This contains the prototypes and definitions for the megaAVR ADC driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef ADC_H_
#define ADC_H_

 #include "Common/Common.h"

 #include "Arch/AVR8/megaAVR/CPU/CPU.h"

/** @brief			ADC callback definition.
 *  @param Device	ADC device
 *  @param Channel	ADC channel
 */
 typedef void (*ADC_Callback_t)(uint8_t Device, uint8_t Channel);

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

 /** @brief ADC voltage reference.
 */
 typedef enum
 {
	ADC_REFERENCE_AREF = 0x00,						/**< Use AREF pin */ 
	ADC_REFERENCE_AVCC = 0x01,						/**< Use AVCC with external capacitor at AREF pin */ 
	ADC_REFERENCE_INT = 0x03,						/**< Internal 2.56 V Reference */
 } ADC_Reference_t;

 /** @brief	ADC conversion format.
 */
 typedef enum
 {
	ADC_CONV_FORM_RIGHT = 0x00,						/**< 10 bit resolution right adjusted */ 
	ADC_CONV_FORM_LEFT = 0x01,						/**< 10 bit resolution left adjusted */ 
 } ADC_ConversionFormat_t;

 /** @brief ADC configuration object.
  */
 typedef struct
 {
	 ADC_InputModes_t InputMode;					/**< Input mode */
	 ADC_ConversionFormat_t Format;					/**< Conversion format */
	 ADC_Reference_t Reference;						/**< Reference voltage */
	 ADC_Prescaler_t Prescaler;						/**< Clock prescaler */
 } ADC_Config_t;

 /** @brief	Enable an ADC module.
  */
 static inline void ADC_Enable(void) __attribute__ ((always_inline));
 static inline void ADC_Enable(void)
 {
	 ADCSRA |= (0x01 << ADEN);
 }
 
 /** @brief	Disable an ADC module.
  */
 static inline void ADC_Disable(void) __attribute__ ((always_inline));
 static inline void ADC_Disable(void)
 {
	 ADCSRA &= ~(0x01 << ADEN);
 }

 /** @brief				Set the ADC prescaler.
  *  @param Prescaler	ADC prescaler
  */
 static inline void ADC_SetPrescaler(const ADC_Prescaler_t Prescaler) __attribute__ ((always_inline));
 static inline void ADC_SetPrescaler(const ADC_Prescaler_t Prescaler)
 {
	 ADCSRA = (ADCSRA & (~0x07)) | Prescaler;
 }

 /** @brief		Get the ADC prescaler.
  *  @return	ADC prescaler
  */
 static inline ADC_Prescaler_t ADC_GetPrescaler(void) __attribute__ ((always_inline));
 static inline ADC_Prescaler_t ADC_GetPrescaler(void)
 {
	 return ADCSRA & 0x07;
 }
 
 /** @brief			Set the ADC conversion format.
  *  @param Format	Conversion format
  */
 static inline void ADC_SetFormat(const ADC_ConversionFormat_t Format) __attribute__ ((always_inline));
 static inline void ADC_SetFormat(const ADC_ConversionFormat_t Format)
 {
	 ADMUX = (ADMUX & (~(0x01 << ADLAR))) | (Format << ADLAR);
 }

 /** @brief			Set the ADC conversion format.
  *  @return		Conversion format
  */
 static inline ADC_ConversionFormat_t ADC_GetFormat(void) __attribute__ ((always_inline));
 static inline ADC_ConversionFormat_t ADC_GetFormat(void)
 {
	 return ((ADMUX & (0x01 << ADLAR)) >> ADLAR);
 }

 /** @brief			Set the ADC input source.
  *  @param Input	ADC input mode
  */
 static inline void ADC_SetInput(const ADC_InputModes_t Input) __attribute__ ((always_inline));
 static inline void ADC_SetInput(const ADC_InputModes_t Input)
 {
	 ADMUX = (ADMUX & (~0x1F)) | (Input & 0x1F);
 }

 /** @brief				Set the ADC reference.
  *  @param Reference	ADC reference
  */
 static inline void ADC_SetReference(const ADC_Reference_t Reference) __attribute__ ((always_inline));
 static inline void ADC_SetReference(const ADC_Reference_t Reference)
 {
	 ADMUX = (ADMUX & (~(0x03 << 0x06))) | (Reference << 0x06);
 }

 /** @brief		Get the ADC reference.
  *  @return	ADC reference
  */
 static inline ADC_Reference_t ADC_GetReference(void) __attribute__ ((always_inline));
 static inline ADC_Reference_t ADC_GetReference(void)
 {
	 return ((ADMUX & (0x03 << 0x06)) >> 0x06);
 }

 /** @brief			Enable the auto trigger mode for the ADC.
  *  @param Trigger	ADC auto trigger source
  */
 static inline void ADC_EnableAutoTrigger(const ADC_AutoTrigger_t Trigger) __attribute__ ((always_inline));
 static inline void ADC_EnableAutoTrigger(const ADC_AutoTrigger_t Trigger)
 {
	 SFIOR = (SFIOR & (~(0x07 << 0x05))) | Trigger;
	 ADCSRA |= (0x01 << ADATE);
 }

 /** @brief	Disable the auto trigger mode for the ADC.
  */
 static inline void ADC_DisableAutoTrigger(void) __attribute__ ((always_inline));
 static inline void ADC_DisableAutoTrigger(void)
 {
	 ADCSRA &= ~(0x01 << ADATE);
 }

 /** @brief		Get the current auto trigger source.
  *  @return	Auto trigger source
  */
 static inline ADC_AutoTrigger_t ADC_GetAutoTrigger(void) __attribute__ ((always_inline));
 static inline ADC_AutoTrigger_t ADC_GetAutoTrigger(void)
 {
	 return (SFIOR >> 0x05);
 }

 /** @brief	Start a new conversion.
  */
 static inline void ADC_StartConversion(void) __attribute__ ((always_inline));
 static inline void ADC_StartConversion(void)
 {
	 ADCSRA |= (0x01 << ADSC);
 }
 
 /** @brief	Read the conversion result
  */
 static inline uint16_t ADC_ReadResult(void) __attribute__ ((always_inline));
 static inline uint16_t ADC_ReadResult(void)
 {
	 while(!(ADCSRA & (0x01 << ADIF)));

	 return ADC;
 }

 /** @brief			Configure the ADC.
  *  @param Config	Pointer to ADC configuration object
  */
 void ADC_Init(ADC_Config_t* Config);

 /** @brief				Install a new ADC conversion complete callback.
  *  @param Callback	Function pointer to ADC callback.
  */
 void ADC_Channel_InstallCallback(ADC_Callback_t Callback);
 
 /** @brief	Remove an installed callback.
  */
 void ADC_Channel_RemoveCallback(void);

#endif /* ADC_H_ */