/*
 * ADC.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 ADC

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

#ifndef ADC_H_
#define ADC_H_

 #if(MCU_NAME == MCU_NAME_ATMEGA32)
	 #include "Common/Board/MCU/ATmega32.h"
 #else
	 #error "No valid mcu"
 #endif

 #include "Arch/AVR8/ATmega/CPU/CPU.h"

 #include "Base/ADC_Base.h"

 /** 
  * ADC configuration object
  */
 typedef struct
 {
	 ADC_InputModes_t InputMode;			/**< Input mode */
	 ADC_ConversionFormat_t Format;			/**< Conversion format */
	 ADC_Reference_t Reference;				/**< Reference voltage */
	 ADC_Prescaler_t Prescaler;				/**< Clock prescaler */
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

 /** @brief			Configure an ADC module.
  *  @param Config	Pointer to ADC configuration object
  */
 void ADC_Init(ADC_Config_t* Config);

 /** @brief				Install a new ADC conversion complete interrupt callback.
  *  @param Callback	Function pointer to ADC callback.
  */
 void ADC_Channel_InstallCallback(ADC_Callback_t Callback);
 
 /** @brief		Remove an installed callback.
  */
 void ADC_Channel_RemoveCallback(void);

#endif /* ADC_H_ */