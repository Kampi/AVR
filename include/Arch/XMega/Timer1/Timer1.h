/*
 * Timer1.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega Timer type 1.

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

/** @file Arch/XMega/Timer1/Timer1.h
 *  @brief Driver for XMega Timer type 1.
 *
 *  This contains the prototypes and definitions for the XMega Timer1 driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef TIMER1_H_
#define TIMER1_H_

 #include "Common/Common.h"

 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/EventSystem/EventSystem.h"
 #include "Arch/XMega/PowerManagement/PowerManagement.h"

 #include "Base/Timer_Base.h"

 /** 
  * Timer1 configuration object
  */
 typedef struct
 {
	 TC1_t* Device;									/**< Pointer to Timer1 device object */ 
	 Timer_Prescaler_t Prescaler;					/**< Clock prescaler */ 
	 uint16_t Period;								/**< Timer period */ 
 } Timer1_Config_t;

 /** 
  * Timer1 waveform configuration object
  */
 typedef struct 
 {
	 TC1_t* Device;									/**< Pointer to Timer1 device object */ 
	 Timer_CCChannel_t Channel;						/**< Capture or compare channel */ 
	 Timer_Prescaler_t Prescaler;					/**< Clock prescaler */ 
	 Timer_Mode_t Mode;								/**< Timer mode */ 
	 uint16_t Period;								/**< Timer period */ 
	 uint16_t DutyCycle;							/**< Timer duty cycle */ 
	 Bool_t Invert;									/**< Set to *TRUE to invert the WGM pin */ 
 } Timer1_WaveConfig_t;
 
 /** 
  * Timer1 interrupt configuration object
  */
 typedef struct
 {
	 TC1_t* Device;									/**< Pointer to Timer1 device object */
	 Timer_CallbackType_t Source;					/**< Timer interrupt type */
	 Interrupt_Level_t InterruptLevel;				/**< Interrupt level */
	 Timer_Callback_t Callback;						/**< Function pointer to timer callback */
 } Timer1_InterruptConfig_t;

 /** @brief				Clear an interrupt flag.
  *  @param Device		Pointer to Timer1 object
  *  @param Callback	Interrupt type
  */
 static inline void Timer1_ClearFlag(TC1_t* Device, const Timer_CallbackType_t Callback) __attribute__ ((always_inline));
 static inline void Timer1_ClearFlag(TC1_t* Device, const Timer_CallbackType_t Callback)
 {
	 Device->INTFLAGS |= Callback;	
 }

 /** @brief			Initialize a Timer1 device.
  *  @param Config	Pointer to Timer1 configuration struct
  */
 void Timer1_Init(Timer1_Config_t* Config);
 
 /** @brief			Initialize a Timer1 device with waveform option.
  *  @param Config	Pointer to Timer1 waveform configuration struct
  */
 void Timer1_WaveInit(Timer1_WaveConfig_t* Config);
 
 /** @brief			Install a new callback for a Timer1 device.
  *  @param Config	Pointer to interrupt configuration structure
  */
 void Timer1_InstallCallback(Timer1_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Device		Pointer to Timer1 object
  *  @param Callback	Callback which should be removed
  */
 void Timer1_RemoveCallback(TC1_t* Device, const Timer_CallbackType_t Callback);
 
 /** @brief					Change the interrupt level of a timer interrupt.
  *  @param Device			Pointer to Timer1 object
  *  @param Callback		Callback which should be changed
  *  @param InterruptLevel	New interrupt level.
  */
 void Timer1_ChangeInterruptLevel(TC1_t* Device, const Timer_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel);

#endif /* TIMER1_H_  */ 