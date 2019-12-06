/*
 * DAC.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega DAC module.

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

/** @file Arch/XMega/DAC/DAC.h
 *  @brief Driver for Atmel AVR XMega DAC module.
 *
 *  This file contains the prototypes and definitions for the Atmel AVR XMega DAC driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef DAC_H_
#define DAC_H_

 #include "Common/Common.h"

 /** @brief	DAC channels.
  */
 typedef enum
 {
	DAC_CHANNEL_0 = 0x01,				/**< DAC channel 0 */
	DAC_CHANNEL_1 = 0x02,				/**< DAC channel 1 */
	DAC_CHANNEL_INT = 0x04,				/**< Internal DAC channel for the ADC or the AC */
 } DAC_Channel_t;

 /** @brief	DAC channel configuration.
  */
 typedef enum
 {
	 DAC_SINGLE_CHANNEL_0 = 0x00,		/**< Use channel 0 only */
	 DAC_SINGLE_CHANNEL_1 = 0x01,		/**< Use channel 1 only */
	 DAC_DUAL_CHANNEL = 0x02			/**< Use both channels */
 } DAC_OutputConfig_t;

 /** @brief	DAC channel data adjustment.
  */
 typedef enum
 {
	 DAC_ADJUST_RIGHT = 0x00,			/**< Adjustment right */
	 DAC_ADJUST_LEFT = 0x01,			/**< Adjustment left */
 } DAC_Adjustment_t;

 /** @brief	DAC reference voltage.
  */
 typedef enum
 {
	 DAC_REFERENCE_INT1V = 0x00,		/**< Internal 1 V reference */
	 DAC_REFERENCE_AVCC = 0x01,			/**< Use AVCC pin as reference */
	 DAC_REFERENCE_AREFA = 0x02,		/**< Port A reference */
	 DAC_REFERENCE_AREFB = 0x03,		/**< Port B reference */
 } DAC_Reference_t;

 /** @brief	DAC event inputs.
  */
 typedef enum
 {
	 DAC_EVENT_0 = 0x00,				/**< Event channel 0 inputs */
	 DAC_EVENT_1 = 0x01,				/**< Event channel 1 inputs */
	 DAC_EVENT_2 = 0x02,				/**< Event channel 2 inputs */
	 DAC_EVENT_3 = 0x03,				/**< Event channel 3 inputs */
	 DAC_EVENT_4 = 0x04,				/**< Event channel 4 inputs */
	 DAC_EVENT_5 = 0x05,				/**< Event channel 5 inputs */
	 DAC_EVENT_6 = 0x06,				/**< Event channel 6 inputs */
	 DAC_EVENT_7 = 0x07,				/**< Event channel 7 inputs */
 } DAC_EventChannel_t;

 /** @brief	DAC configuration object.
  */
 typedef struct
 {
	DAC_t* Device;						/**< Pointer to DAC object */
	DAC_Channel_t Channel;				/**< DAC channel */
	DAC_OutputConfig_t OutputConfig;	/**< Channel selection */
	DAC_Adjustment_t Adjustment;		/**< DAC data adjustment */
	DAC_Reference_t Reference;			/**< Reference voltage */
	float RefVoltage;					/**< Reference voltage in [V] */
 } DAC_Config_t;

 /** @brief			Start the AES module.
  *  @param Config	Pointer to DAC configuration object
  */
 void DAC_Init(DAC_Config_t* Config);
 
 /** @brief			Calibrate a DAC module.
  *  @param Device	Pointer to DAC object
  */
 void DAC_Calibrate(DAC_t* Device, const DAC_Channel_t Channel);
 
 /** @brief			Enable low power mode for DAC module.
  *  @param Device	Pointer to DAC object
  */
 void DAC_EnableLowPower(DAC_t* Device);
 
 /** @brief			Disable low power mode for DAC module.
  *  @param Device	Pointer to DAC object
  */
 void DAC_DisableLowPower(DAC_t* Device);
 
 /** @brief			Enable DAC module.
  *  @param Device	Pointer to DAC object
  */
 void DAC_Enable(DAC_t* Device);
 
 /** @brief			Disable DAC module.
  *  @param Device	Pointer to DAC object
  */
 void DAC_Disable(DAC_t* Device);
 
 /** @brief			Enable channel for DAC module.
  *  @param Device	Pointer to DAC object
  *  @param Channel	DAC channel
  */
 void DAC_EnableChannel(DAC_t* Device, const DAC_Channel_t Channel);
 
 /** @brief			Disable channel for DAC module.
  *  @param Device	Pointer to DAC object
  *  @param Channel	DAC channel
  */
 void DAC_DisableChannel(DAC_t* Device, const DAC_Channel_t Channel);
 
 /** @brief			Get the active channel for DAC module.
  *  @param Device	Pointer to DAC object
  *  @return		Channel
  */
 DAC_Channel_t DAC_GetChannel(DAC_t* Device);
 
 /** @brief			Get the adjustment for DAC module.
  *  @param Device	Pointer to DAC object
  *  @param Config	Output configuration
  */
 void DAC_SetOutputConfig(DAC_t* Device, const DAC_OutputConfig_t Config);
 
 /** @brief			Get the output configuration for DAC module.
  *  @param Device	Pointer to DAC object
  *  @return		Output configuration
  */
 DAC_OutputConfig_t DAC_GetOutputConfig(DAC_t* Device);
 
 /** @brief				Set the reference for DAC module.
  *  @param Device		Pointer to DAC object
  *  @param Reference	Reference
  */
 void DAC_SetReference(DAC_t* Device, const DAC_Reference_t Reference);

 /** @brief				Set the voltage devider value.
  *  @param Reference	Reference voltage in [V]
  */
 void DAC_SetVoltageDivider(const float Reference);
 
 /** @brief			Get the reference for DAC module.
  *  @param Device	Pointer to DAC object
  *  @return		Reference
  */
 DAC_Reference_t DAC_GetReference(DAC_t* Device);
 
 /** @brief				Set the adjustment for DAC module.
  *  @param Device		Pointer to DAC object
  *  @param Adjustment	Adjustment
  */
 void DAC_SetAdjustment(DAC_t* Device, const DAC_Adjustment_t Adjustment);
 
 /** @brief			Get the adjustment for DAC module.
  *  @param Device	Pointer to DAC object
  *  @return		Adjustment
  */
 DAC_Adjustment_t DAC_GetAdjustment(DAC_t* Device);
 
 /** @brief			Set the output value of a DAC channel.
  *  @param Device	Pointer to DAC object
  *  @param Channel	DAC channel
  *  @param Value	Output value in binary format
  */
 void DAC_WriteChannel(DAC_t* Device, const DAC_Channel_t Channel, const uint16_t Value);
 
 /** @brief			Set the output voltage of a DAC module.
  *  @param Device	Pointer to DAC object
  *  @param Channel	DAC channel
  *  @param Voltage	Output voltage
  */
 void DAC_WriteVoltage(DAC_t* Device, const DAC_Channel_t Channel, const float Voltage);

 /** @brief					Configure the DAC for event integration.
  *  @param Device			Pointer to DAC object
  *  @param EventChannel	Event channel for the DAC
  *  @param Channel			DAC channel
  */
 void DAC_ConfigEvent(DAC_t* Device, const DAC_EventChannel_t EventChannel, const DAC_Channel_t Channel);

#endif /* DAC_H_ */ 