/*
 * AC.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega analog comparator module.

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

/** @file Arch/XMega/AC/AC.h
 *  @brief Driver for Atmel AVR XMega analog comparator module.
 *
 *  This file contains the prototypes and definitions for the Atmel AVR XMega AC driver.
 *
 *  @author Daniel Kampert
 *  @bug - Event mode missing
 *		 - Calibration missing
 *		 - Update documentation
 */

#ifndef AC_H_
#define AC_H_

 #include "Common/Common.h"

 #include "Arch/XMega/GPIO/GPIO.h"
 #include "Arch/XMega/PMIC/PMIC.h"
 
 /** @brief	ID declaration for the different MCU types.
  */
 #define ACA_ID			0					/**< Analog comparator A ID */
 #define ACB_ID			1					/**< Analog comparator B ID */
 
 /** @brief	AC callback definition.
 */
 typedef void (*AC_Callback_t)(void);

 /** @brief	ADC comparator enumeration.
  */
 typedef enum
 {
	 AC_COMPARATOR_0 = 0x01,				/**< AC comparator 0 */
	 AC_COMPARATOR_1 = 0x02,				/**< AC comparator 1 */
 } AC_Comparator_t;

 /** @brief	AC callback types.
  */
 typedef enum
 {
	 AC_COMP0_INTERRUPT = 0x01,				/**< AC comparator 0 interrupt */ 
	 AC_COMP1_INTERRUPT = 0x02,				/**< AC comparator 1 interrupt */ 
	 AC_WINDOW_INTERRUPT = 0x04,			/**< AC window interrupt */ 
 } AC_CallbackType_t;
 
 /** @brief	AC interrupt modes.
  */
 typedef enum
 {
	 AC_SENSE_BOTH = 0x00,					/**< Comparator both edges interrupt */
	 AC_SENSE_FALLING = 0x02,				/**< Comparator falling edge interrupt */
	 AC_SENSE_RISING = 0x03,				/**< Comparator rising edge interrupt */
	 AC_WINDOW_ABOVE = 0x04,				/**< Above window interrupt */
	 AC_WINDOW_INSIDE = 0x05,				/**< Inside window interrupt */
	 AC_WINDOW_BELOW = 0x06,				/**< Below window interrupt */
	 AC_WINDOW_OUTSIDE = 0x07,				/**< Outside window interrupt */
 } AC_InterruptMode_t;

 /** @brief	AC current source options.
  */
 typedef enum
 {
	 AC_CURRENT_SOURCE_0 = 0x00,			/**< Current source 1 */
	 AC_CURRENT_SOURCE_1 = 0x01,			/**< Current source 2 */
 } AC_CurrentSource_t;
 
 /** @brief	AC hysteresis options.
  */
 typedef enum
 {
	 AC_HYSTERESIS_NO = 0x00,				/**< No hysteresis */
	 AC_HYSTERESIS_SMALL = 0x01,			/**< Small hysteresis */
	 AC_HYSTERESIS_LARGE = 0x02,			/**< Large hysteresis */
 } AC_Hysteresis_t;

 /** @brief	AC positive input pins.
  */
 typedef enum
 {
	 AC_MUX_P_PIN0 = 0x00,					/**< Pin 0 */
	 AC_MUX_P_PIN1 = 0x01,					/**< Pin 1 */
	 AC_MUX_P_PIN2 = 0x02,					/**< Pin 2 */
	 AC_MUX_P_PIN3 = 0x03,					/**< Pin 3 */
	 AC_MUX_P_PIN4 = 0x04,					/**< Pin 4 */
	 AC_MUX_P_PIN5 = 0x05,					/**< Pin 5 */
	 AC_MUX_P_PIN6 = 0x06,					/**< Pin 6 */
	 AC_MUX_P_DAC = 0x07,					/**< DAC output */
 } AC_MuxP_t;

 /** @brief	AC negative input pins.
  */
 typedef enum
 {
	 AC_MUX_N_PIN0 = 0x00,					/**< Pin 0 */
	 AC_MUX_N_PIN1 = 0x01,					/**< Pin 1 */
	 AC_MUX_N_PIN3 = 0x02,					/**< Pin 3 */
	 AC_MUX_N_PIN5 = 0x03,					/**< Pin 5 */
	 AC_MUX_N_PIN7 = 0x04,					/**< Pin 7 */
	 AC_MUX_N_DAC = 0x05,					/**< DAC output */
	 AC_MUX_N_BANDGAP = 0x06,				/**< Band gap voltage */
	 AC_MUX_N_SCALER = 0x07,				/**< Vcc Voltage scaler */
 } AC_MuxN_t;

 /** @brief	AC configuration object.
  */
 typedef struct
 {
	 AC_t* Device;							/**< Pointer to AC object */
	 AC_Comparator_t Comparator;			/**< Target comparator */
	 AC_Hysteresis_t Hysteresis;			/**< Hysteresis settings */
	 AC_MuxP_t Positive;					/**< Positive input pin */
	 AC_MuxN_t Negative;					/**< Negative input pin */
	 Bool_t EnableHighSpeed;				/**< Set to #TRUE to enable the high speed option */
 } AC_Config_t;

 /** @brief	AC interrupt configuration object.
  */
 typedef struct
 {
	 AC_t* Device;							/**< Pointer to AC object */
	 AC_CallbackType_t Source;				/**< AC interrupt type */
	 AC_InterruptMode_t Mode;				/**< AC interrupt mode */
	 Interrupt_Level_t InterruptLevel;		/**< Interrupt level */
	 AC_Callback_t Callback;				/**< Function pointer to AC callback */
 } AC_InterruptConfig_t;

 /** @brief			Enable the window mode of an AC device.
  *  @param Device	Pointer to AC object
  */
 static inline void AC_EnableWindowMode(AC_t* Device) __attribute__((always_inline));
 static inline void AC_EnableWindowMode(AC_t* Device)
 {
	 Device->WINCTRL |= (0x01 << 0x04);
 }

 /** @brief			Disable the window mode of an AC device.
  *  @param Device	Pointer to AC object
  */
 static inline void AC_DisableWindowMode(AC_t* Device) __attribute__((always_inline));
 static inline void AC_DisableWindowMode(AC_t* Device)
 {
	 Device->WINCTRL &= ~(0x01 << 0x04);
 }

 /** @brief			Get the scale value of an AC device.
  *  @param Device	Pointer to AC object
  *  @param Scale	Scale value
  */
 static inline void AC_SetScale(AC_t* Device, const uint8_t Scale) __attribute__((always_inline));
 static inline void AC_SetScale(AC_t* Device, const uint8_t Scale)
 {
	 Device->CTRLB = Scale;
 }

 /** @brief			Get the scale value of an AC device.
  *  @param Device	Pointer to AC object
  *  @return		Scale value
  */
 static inline uint8_t AC_GetScale(AC_t* Device) __attribute__((always_inline));
 static inline uint8_t AC_GetScale(AC_t* Device)
 {
	 return (Device->CTRLB & 0x3F);
 }

 /** @brief			Configure an AC module.
  *  @param Config	Pointer to AC configuration object
  */
 void AC_Init(AC_Config_t* Config);
 
 /** @brief			Calibrate an AC module.
  *  @param Device	Pointer to AC object
  */
 void AC_Calibrate(AC_t* Device);

 /** @brief				Enable a comparator of an AC device.
  *  @param Device		Pointer to AC object
  *  @param Comparator	Target comparator
  */
 void AC_Enable(AC_t* Device, AC_Comparator_t Comparator);
 
 /** @brief				Disable a comparator of an AC device.
  *  @param Device		Pointer to AC object
  *  @param Comparator	Target comparator
  */
 void AC_Disable(AC_t* Device, AC_Comparator_t Comparator);
 
 /** @brief				Enable the output of comparator.
  *  @param Device		Pointer to AC object
  *  @param Comparator	Target comparator
  */
 void AC_EnableOutput(AC_t* Device, AC_Comparator_t Comparator);
 
 /** @brief				Disable the output of comparator.
  *  @param Device		Pointer to AC object
  *  @param Comparator	Target comparator
  */
 void AC_DisableOutput(AC_t* Device, AC_Comparator_t Comparator);
 
 void AC_SetSpeed(AC_t* Device, AC_Comparator_t Comparator, Bool_t Speed);
 Bool_t AC_GetSpeed(AC_t* Device, AC_Comparator_t Comparator);
 void AC_SetHysteresis(AC_t* Device, AC_Comparator_t Comparator, AC_Hysteresis_t Hysteresis);
 AC_Hysteresis_t AC_GetHysteresis(AC_t* Device, AC_Comparator_t Comparator);
 void AC_SetMux(AC_t* Device, AC_Comparator_t Comparator, AC_MuxP_t Positive, AC_MuxN_t Negative);
 AC_MuxP_t AC_GetMuxPositive(AC_t* Device, AC_Comparator_t Comparator);
 AC_MuxN_t AC_GetMuxNegative(AC_t* Device, AC_Comparator_t Comparator);
 void AC_WaitForComparator(AC_t* Device, AC_Comparator_t Comparator);
 uint8_t AC_GetState(AC_t* Device, AC_Comparator_t Comparator);

 /** @brief			Install a new callback for a AC device.
  *  @param Config	Pointer to interrupt configuration structure.
  */
 void AC_InstallCallback(AC_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Device		Pointer to AC object
  *  @param Callback	Callback which should be removed.
  */
 void AC_RemoveCallback(AC_t* Device, AC_CallbackType_t CallbackType);
 
 /** @brief					Change the interrupt level of a AC interrupt.
  *  @param Device			Pointer to AC object
  *  @param Callback		Callback which should be changed.
  *  @param InterruptLevel	New interrupt level.
  */
 void AC_ChangeInterruptLevel(AC_t* Device, AC_CallbackType_t Callback, Interrupt_Level_t InterruptLevel);

#endif /* AC_H_ */