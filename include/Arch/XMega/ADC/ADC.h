/*
 * ADC.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega ADC

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

/** @file Arch/XMega/ADC/ADC.h
 *  @brief Driver for XMega ADC module. 
 *
 *  This file contains the prototypes and definitions for the XMega ADC driver.
 *
 *  @author Daniel Kampert
 *  @bug	- No compare mode
 */

#ifndef ADC_H_
#define ADC_H_

 #include "Common/Common.h"

 #include "Arch/XMega/GPIO/GPIO.h"
 #include "Arch/XMega/PMIC/PMIC.h"

 /** @brief	ID declaration for the different MCU types.
  */
 #define ADCA_ID			0			/**< Analog/digital converter A ID */
 #define ADCB_ID			1			/**< Analog/digital converter B ID */

/** @brief			ADC callback definition.
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

 /** @brief	ADC DMA request types.
  */
 typedef enum
 {
	 ADC_DMA_OFF = 0x00,							/**< No DMA request */
	 ADC_DMA_CH01 = 0x01,							/**< Common request for ADC channels 0 and 1 */
	 ADC_DMA_CH012 = 0x02,							/**< Common request for ADC channels 0, 1 and 2 */
	 ADC_DMA_CH0123 = 0x03,							/**< Common request for ADC channels 0, 1, 2 and 3 */
 } ADC_DMARequest_t;

 /** @brief	ADC channel measurement mode.
  */
 typedef enum
 {
	ADC_CHAN_INTERNAL = 0x00,						/**< Internal ADC channel */
	ADC_CHAN_SINGLE = 0x01,							/**< Single ended channel */
	ADC_CHAN_DIFF = 0x02,							/**< Differential channel */
	ADC_CHAN_DIFFWGAIN = 0x03,						/**< Differential channel with gain */
 } ADC_ChannelMode_t;

 /** @brief	ADC gain settings.
  */
 typedef enum
 {
	ADC_CHAN_GAIN_1 = 0x00,							/**< ADC gain x1 */
	ADC_CHAN_GAIN_2 = 0x01,							/**< ADC gain x2 */
	ADC_CHAN_GAIN_4 = 0x02,							/**< ADC gain x4 */
	ADC_CHAN_GAIN_8 = 0x03,							/**< ADC gain x8 */
	ADC_CHAN_GAIN_16 = 0x04,						/**< ADC gain x16 */
	ADC_CHAN_GAIN_32 = 0x05,						/**< ADC gain x32 */
	ADC_CHAN_GAIN_64 = 0x06,						/**< ADC gain x64 */
 } ADC_ChannelGain_t;

 /** @brief	ADC input channels.
  */
 typedef enum
 {
	 ADC_CHANNEL_INPUT_0 = 0x00,					/**< ADC channel 0 */
	 ADC_CHANNEL_INPUT_1 = 0x01,					/**< ADC channel 1 */
	 ADC_CHANNEL_INPUT_2 = 0x02,					/**< ADC channel 2 */
	 ADC_CHANNEL_INPUT_3 = 0x03,					/**< ADC channel 3 */
	 ADC_CHANNEL_INPUT_4 = 0x04,					/**< ADC channel 4 */
	 ADC_CHANNEL_INPUT_5 = 0x05,					/**< ADC channel 5 */
	 ADC_CHANNEL_INPUT_6 = 0x06,					/**< ADC channel 6 */
	 ADC_CHANNEL_INPUT_7 = 0x07,					/**< ADC channel 7 */
	 ADC_CHANNEL_INPUT_8 = 0x08,					/**< ADC channel 8 */
	 ADC_CHANNEL_INPUT_9 = 0x09,					/**< ADC channel 9 */
	 ADC_CHANNEL_INPUT_10 = 0x0A,					/**< ADC channel 10 */
	 ADC_CHANNEL_INPUT_11 = 0x0B,					/**< ADC channel 11 */
 } ADC_ChannelMux_t;

 /** @brief	ADC sweep settings.
  */
 typedef enum
 {
	 ADC_SWEEP_ONLY_0 = 0x00,						/**< Sweep only channel 0 */
	 ADC_SWEEP_0_TO_1 = 0x01,						/**< Sweep channel 0 to 1 */
	 ADC_SWEEP_0_TO_2 = 0x02,						/**< Sweep channel 0 to 2 */
	 ADC_SWEEP_0_TO_3 = 0x03,						/**< Sweep channel 0 to 3 */
 } ADC_Sweep_t;

 /** @brief	ADC event inputs.
  */
 typedef enum
 {
	 ADC_EVENT_0123 = 0x00,							/**< Event channels 0, 1, 2, 3 as inputs */
	 ADC_EVENT_1234 = 0x01,							/**< Event channels 1, 2, 3, 4 as inputs */
	 ADC_EVENT_2345 = 0x02,							/**< Event channels 2, 3, 4, 5 as inputs */
	 ADC_EVENT_3456 = 0x03,							/**< Event channels 3, 4, 5, 6 as inputs */
	 ADC_EVENT_4567 = 0x04,							/**< Event channels 4, 5, 6, 7 as inputs */
	 ADC_EVENT_567 = 0x05,							/**< Event channels 5, 6, 7 as inputs */
	 ADC_EVENT_67 = 0x06,							/**< Event channels 6, 7 as inputs */
	 ADC_EVENT_7 = 0x07,	 						/**< Event channel 7 as input */
 } ADC_EventChannel_t;

 /** @brief	ADC event modes.
  */
 typedef enum
 {
	 ADC_EMODE_NONE = 0x00,	 						/**< No event inputs */
	 ADC_EMODE_CH0 = 0x01,	 						/**< Event channel with the lowest number defined by EVSEL triggers conversion on ADC channel 0 */
	 ADC_EMODE_CH01 = 0x02,							/**< Event channels with the two lowest numbers defined by EVSEL trigger conversions on ADC channels 0 and 1, respectively */
	 ADC_EMODE_CH012 = 0x03,						/**< Event channels with the three lowest numbers defined by EVSEL trigger conversions on ADC channels 0, 1, and 2, respectively */
	 ADC_EMODE_CH0123 = 0x04,						/**< Event channels defined by EVSEL trigger conversion on ADC channels 0, 1, 2, and 3, respectively */
	 ADC_EMODE_SWEEP = 0x05,						/**< One sweep of all ADC channels defined by SWEEP on incoming event channel with the lowest number defined by EVSEL */
	 ADC_EMODE_SYNCSWEEP = 0x04,					/**< One sweep of all active ADC channels defined by SWEEP on incoming event channel with the lowest number defined by EVSE. In addition the ADC is flushed and restarted for accurate timing */
 } ADC_EventMode_t;

 /** @brief	ADC channel interrupt configuration object.
  */
 typedef struct
 {
	 ADC_t* Device;									/**< Pointer to ADC object */
	 ADC_CH_t* Channel;								/**< Pointer to ADC channel object */
	 ADC_CallbackType_t Source;						/**< ADC interrupt type */
	 Interrupt_Level_t InterruptLevel;				/**< Interrupt level */
	 ADC_Callback_t Callback;						/**< Function pointer to ADC callback */
 } ADC_InterruptConfig_t;

 /** @brief	ADC configuration object.
  */
 typedef struct
 {
	 ADC_t* Device;									/**< Pointer to ADC object */

	 #if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
		 ADC_DMARequest_t DMARequest;				/**< ADC DMA request settings */
	 #endif

	 ADC_ConversionMode_t ConversionMode;			/**< Conversion mode */
	 ADC_ConversionFormat_t Format;					/**< Conversion format */
	 ADC_Reference_t Reference;						/**< Reference voltage */
	 ADC_Prescaler_t Prescaler;						/**< Clock prescaler */

	 #if(MCU_NAME == MCU_NAME_ATXMEGA384C3)
		uint8_t SamplingTime;						/**< ADC sampling time */
	 #endif
 } ADC_Config_t;

 /** @brief	ADC channel configuration object.
  */
 typedef struct
 {
	 ADC_t* Device;									/**< Pointer to ADC object */
	 ADC_CH_t* Channel;								/**< Pointer to ADC channel object */
	 ADC_ChannelGain_t Gain;						/**< Channel gain */
	 ADC_ChannelMode_t Mode;						/**< Channel mode */
	 ADC_ChannelMux_t Input;						/**< Input pin for ADC channel */
 } ADC_ChannelConfig_t;

 /** @brief			Enable an ADC module.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_Enable(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_Enable(ADC_t* Device)
 {
	 Device->CTRLA |= ADC_ENABLE_bm;
 }

 /** @brief			Disable an ADC module.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_Disable(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_Disable(ADC_t* Device)
 {
	 Device->CTRLA &= ~ADC_ENABLE_bm;
 }

 /** @brief					Set the ADC conversion mode.
  *  @param Device			Pointer to ADC object
  *  @param ConversionMode	Conversion mode
  */
 static inline void ADC_SetConversionMode(ADC_t* Device, const ADC_ConversionMode_t ConversionMode) __attribute__((always_inline));
 static inline void ADC_SetConversionMode(ADC_t* Device, const ADC_ConversionMode_t ConversionMode)
 {
	 if(ConversionMode == ADC_CONVMODE_UNSIGNED)
	 {
		 Device->CTRLB &= ~(ADC_CONVMODE_SIGNED << 0x04);
	 }
	 else
	 {
		 Device->CTRLB |= (ADC_CONVMODE_SIGNED << 0x04);
	 }
 }

 /** @brief			Get the ADC conversion mode.
  *  @param Device	Pointer to ADC object
  *  @return		Conversion mode
  */
 static inline const ADC_ConversionMode_t ADC_GetConversionMode(ADC_t* Device) __attribute__((always_inline));
 static inline const ADC_ConversionMode_t ADC_GetConversionMode(ADC_t* Device)
 {
	 return ((Device->CTRLB & 0x10) >> 0x04);
 }

 /** @brief				Set the ADC prescaler.
  *  @param Device		Pointer to ADC object
  *  @param Prescaler	ADC prescaler
  */
 static inline void ADC_SetPrescaler(ADC_t* Device, const ADC_Prescaler_t Prescaler) __attribute__((always_inline));
 static inline void ADC_SetPrescaler(ADC_t* Device, const ADC_Prescaler_t Prescaler)
 {
	 Device->PRESCALER = (Device->PRESCALER & (~0x07)) | Prescaler;
 }

 /** @brief			Get the ADC prescaler.
  *  @param Device	Pointer to ADC object
  *  @return		ADC prescaler
  */
 static inline const ADC_Prescaler_t ADC_GetPrescaler(ADC_t* Device) __attribute__((always_inline));
 static inline const ADC_Prescaler_t ADC_GetPrescaler(ADC_t* Device)
 {
	 return Device->PRESCALER;
 }

 /** @brief			Set the ADC conversion format.
  *  @param Device	Pointer to ADC object
  *  @param Format	Conversion format
  */
 static inline void ADC_SetFormat(ADC_t* Device, const ADC_ConversionFormat_t Format) __attribute__((always_inline));
 static inline void ADC_SetFormat(ADC_t* Device, const ADC_ConversionFormat_t Format)
 {
	 Device->CTRLB = (Device->CTRLB & (~0x03)) | Format;
 }

 /** @brief			Set the ADC conversion format.
  *  @param Device	Pointer to ADC object
  *  @return		Conversion format
  */
 static inline const ADC_ConversionFormat_t ADC_GetFormat(ADC_t* Device) __attribute__((always_inline));
 static inline const ADC_ConversionFormat_t ADC_GetFormat(ADC_t* Device)
 {
	 return ((Device->CTRLB & 0x06) >> 0x01);
 }

 /** @brief				Set the ADC reference.
  *  @param Device		Pointer to ADC object
  *  @param Reference	ADC reference
  */
 static inline void ADC_SetReference(ADC_t* Device, const ADC_Reference_t Reference) __attribute__((always_inline));
 static inline void ADC_SetReference(ADC_t* Device, const ADC_Reference_t Reference)
 {
	 Device->REFCTRL = (Device->REFCTRL & (~(Reference << 0x04))) | (Reference << 0x04);
 }

 /** @brief				Get the ADC reference.
  *  @param Device		Pointer to ADC object
  *  @return			ADC reference
  */
 static inline const ADC_Reference_t ADC_GetReference(ADC_t* Device) __attribute__((always_inline));
 static inline const ADC_Reference_t ADC_GetReference(ADC_t* Device)
 {
	 return ((Device->REFCTRL & 0x30) >> 0x04);
 }

 #if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
	 /** @brief			Set the ADC DMA request type.
	  *  @param Device	Pointer to ADC object
	  *  @param Request	Settings for DMA request
	  */
	 static inline void ADC_SetDMARequest(ADC_t* Device, const ADC_DMARequest_t Request) __attribute__((always_inline));
	 static inline void ADC_SetDMARequest(ADC_t* Device, const ADC_DMARequest_t Request)
	 {
		 Device->CTRLA = (Device->CTRLA & (~(Request << 0x06))) | (Request << 0x06);
	 }

	 /** @brief			Get the ADC DMA request settings.
	  *  @param Device	Pointer to ADC object
	  *  @return		Settings for DMA request
	  */
	 static inline const ADC_DMARequest_t ADC_GetDMARequest(ADC_t* Device) __attribute__((always_inline));
	 static inline const ADC_DMARequest_t ADC_GetDMARequest(ADC_t* Device)
	 {
		 return (Device->CTRLA & 0xC0) >> 0x06;
	 }
 #endif

 #if(MCU_NAME == MCU_NAME_ATXMEGA384C3)
	 /** @brief			Set the ADC sampling time.
	  *  @param Device	Pointer to ADC object
	  *  @param Time	ADC sampling time in number of half prescaled clock cycles
	  */
	 static inline void ADC_SetSamplingTime(ADC_t* Device, const uint8_t Time) __attribute__((always_inline));
	 static inline void ADC_SetSamplingTime(ADC_t* Device, const uint8_t Time)
	 {
		 Device->SAMPCTRL = Time;
	 }

	 /** @brief			Get the ADC sampling time.
	  *  @param Device	Pointer to ADC object
	  *  @return		ADC sampling time in number of half prescaled clock cycles
	  */
	 static inline const uint8_t ADC_GetSamplingTime(ADC_t* Device) __attribute__((always_inline));
	 static inline const uint8_t ADC_GetSamplingTime(ADC_t* Device)
	 {
		 return Device->SAMPCTRL;
	 }
 #endif

 /** @brief			Enable the bandgap reference.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_EnableBandgap(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_EnableBandgap(ADC_t* Device)
 {
	 Device->REFCTRL |= ADC_BANDGAP_bm;
 }

 /** @brief			Disable the bandgap reference.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_DisableBandgap(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_DisableBandgap(ADC_t* Device)
 {
	 Device->REFCTRL &= ~ADC_BANDGAP_bm;
 }

 /** @brief			Enable the temperature sensor.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_EnableTempRef(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_EnableTempRef(ADC_t* Device)
 {
	 Device->REFCTRL |= ADC_TEMPREF_bm;
 }

 /** @brief			Disable the temperature sensor.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_DisableTempRef(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_DisableTempRef(ADC_t* Device)
 {
	 Device->REFCTRL &= ~ADC_TEMPREF_bm;
 }

 /** @brief			Flush the ADC pipeline.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_Flush(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_Flush(ADC_t* Device)
 {
	Device->CTRLA |= ADC_FLUSH_bm;
 }

 /** @brief			Configure an ADC module.
  *  @param Config	Pointer to ADC configuration object
  */
 void ADC_Init(ADC_Config_t* Config);

 /** @brief			Configure the ADC for event integration.
  *  @param Device	Pointer to ADC object
  *  @param Channel	Event channel for the ADC
  *  @param Mode	Event trigger mode
  */
 void ADC_ConfigEvent(ADC_t* Device, const ADC_EventChannel_t Channel, const ADC_EventMode_t Mode);

 /** @brief				Configure the channel sweep of an ADC module.
  *  @param Device		Pointer to ADC object
  *  @param SweepOption	ADC channel sweep settings
  */
 void ADC_ConfigSweep(ADC_t* Device, const ADC_Sweep_t SweepOption);

 /** @brief			Calibrate an ADC module.
  *  @param Device	Pointer to ADC object
  */
 void ADC_Calibrate(ADC_t* Device);

 /** @brief			Enable free run mode for the ADC module.
  *  @param Device	Pointer to ADC object
  */
 void ADC_EnableFreeRun(ADC_t* Device);

 /** @brief			Disable free run mode for the ADC module.
  *  @param Device	Pointer to ADC object
  */
 void ADC_DisableFreeRun(ADC_t* Device);

 /** @brief			Configure an ADC channel.
  *  @param Config	Pointer to ADC channel configuration object
  */
 void ADC_Channel_Init(ADC_ChannelConfig_t* Config);

 /** @brief			Start a new conversion.
  *  @param Config	Pointer to ADC configuration object
  */
 void ADC_Channel_StartConversion(ADC_CH_t* Channel);

 /** @brief			Read the conversion result of an ADC channel.
  *  @param Channel	Pointer to ADC channel object
  *  @return		Conversion result
  */
 uint16_t ADC_Channel_ReadResult(ADC_CH_t* Channel);

 /** @brief					Change the interrupt level of an installed ADC channel interrupt.
  *  @param Channel			Pointer to ADC channel object
  *  @param InterruptLevel	New interrupt level
  */
 void ADC_Channel_ChangeInterruptLevel(ADC_CH_t* Channel, const Interrupt_Level_t InterruptLevel);

 /** @brief			Install a new ADC interrupt callback.
  *  @param Config	Pointer to ADC interrupt configuration object
  */
 void ADC_Channel_InstallCallback(ADC_InterruptConfig_t* Config);

 /** @brief				Remove an installed callback.
  *  @param Channel		Pointer to ADC channel object
  *  @param Callback	Callback which should be removed
  */
 void ADC_Channel_RemoveCallback(ADC_CH_t* Channel, const ADC_CallbackType_t Callback);

#endif /* ADC_H_ */