/*
 * Timer0.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 Timer0 module.

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

/** @file Arch/AVR8/Timer0/Timer0.h
 *  @brief Driver for AVR8 Timer0 module.
 *
 *  This contains the prototypes and definitions for the AVR8 Timer0 driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef TIMER0_H_
#define TIMER0_H_

 #if(MCU_NAME == MCU_NAME_ATMEGA32)
	#include "Common/Board/MCU/ATmega32.h"
 #else
	#error "No valid mcu"
 #endif
 
 #include "Arch/AVR8/CPU/CPU.h"
 
 #include "Base/Timer_Base.h"
 
 /** 
  * Timer 0 configuration object
  */
 typedef struct
 {
	 Timer_Prescaler_t Prescaler;							/**< CTimer clock prescaler */ 
	 Timer_CompareOutput_t CompareOutput;					/**< Compare output settings */ 
	 uint8_t Period;										/**< Timer period 
																 NOTE: You have to set it once. The timer get reloaded during each interrupt. */ 
	 uint8_t Compare;										/**< Compare value
																 NOTE: ONly needed if you use the #CompareOutput. */ 
 } Timer0_Config_t;

 /** 
  * Timer 0 wave form configuration object
  */
 typedef struct
 {
	 Timer_Prescaler_t Prescaler;							/**< CTimer clock prescaler */ 
	 Timer_CompareOutput_t CompareOutput;					/**< Compare output settings */
	 Timer_Mode_t Mode;										/**< Timer mode for waveform generation */
	 uint8_t Period;										/**< Timer period */
	 uint8_t Compare;										/**< Compare value
																 NOTE: ONly needed if you use the #CompareOutput. */ 
 } Timer0_WaveConfig_t;

 /** 
  * Timer 0 interrupt configuration object
  */
 typedef struct
 {
	 Timer_CallbackType_t Source;
	 Timer_Callback_t Callback;
 } Timer0_InterruptConfig_t;

 /** @brief	Enable the timer.
  */
 static inline void Timer0_Enable(void) __attribute__ ((always_inline));
 static inline void Timer0_Enable(void)
 {
	 TCCR0 |= (0x01 << CS00);
 }
 
 /** @brief	Disable the timer.
  */
 static inline void Timer0_Disable(void) __attribute__ ((always_inline));
 static inline void Timer0_Disable(void)
 {
	 TCCR0 &= ~0x03;
 }

 /** @brief			Configure the external clock source on T0 pin.
  *  @param Clock	External clock settings
  */
 static inline void Timer0_SetExtClock(const Timer_ExtClock_t Clock) __attribute__ ((always_inline));
 static inline void Timer0_SetExtClock(const Timer_ExtClock_t Clock)
 {
	 TCCR0 = (TCCR0 & ~0x03) | CS02 | CS01 | (Clock & 0x01);
 }

 /** @brief				Set the prescaler of the timer.
  *  @param Prescaler	Clock prescaler
  */
 static inline void Timer0_SetPrescaler(const Timer_Mode_t Prescaler) __attribute__ ((always_inline));
 static inline void Timer0_SetPrescaler(const Timer_Mode_t Prescaler)
 {
	 TCCR0 = (TCCR0 & ~0x03) | (Prescaler & 0x07);
 }

 /** @brief			Set the mode of the timer.
  *  @param Mode	Timer mode
  */
 static inline void Timer0_SetMode(const Timer_Mode_t Mode) __attribute__ ((always_inline));
 static inline void Timer0_SetMode(const Timer_Mode_t Mode)
 {
	 uint8_t TempMode = (Mode & 0x01) << 0x06;
	 TempMode |= (Mode & 0x02) << 0x03;
	 TCCR0 = (TCCR0 & ~0x48) | TempMode;
 }
 
 /** @brief		Get the mode of the timer.
  *  @return	Timer mode
  */
 static inline Timer_Mode_t Timer0_GetMode(void) __attribute__ ((always_inline));
 static inline Timer_Mode_t Timer0_GetMode(void)
 {
	 uint8_t Temp = (TCCR0 & 0x40) >> 0x06;
	 Temp |= (TCCR0 & 0x08) >> 0x02;

	 return Temp;
 }
 
 /** @brief			Set the compare output mode of the timer.
  *  @param Mode	Compare output mode
  */
 static inline void Timer0_SetCompareOut(const Timer_CompareOutput_t Mode) __attribute__ ((always_inline));
 static inline void Timer0_SetCompareOut(const Timer_CompareOutput_t Mode)
 {
	 TCCR0 = (TCCR0 & ~0x30) | (Mode << 0x04);
 }
 
 /** @brief		Set the compare output mode of the timer.
  *  @return	Compare output mode
  */
 static inline Timer_CompareOutput_t Timer0_GetCompareOut(void) __attribute__ ((always_inline));
 static inline Timer_CompareOutput_t Timer0_GetCompareOut(void)
 {
	 return ((TCCR0 & ~0x30) >> 0x04);
 }

 /** @brief			Set the compare register of the timer.
  *  @param Compare	Compare value
  */
 static inline void Timer0_SetCompare(const uint8_t Compare) __attribute__ ((always_inline));
 static inline void Timer0_SetCompare(const uint8_t Compare)
 {
	 OCR0 = Compare;
 }
 
 /** @brief			Set the counter register of the timer.
  *  @param Count	Count value
  */
 static inline void Timer0_SetCounter(const uint8_t Count) __attribute__ ((always_inline));
 static inline void Timer0_SetCounter(const uint8_t Count)
 {
	 TCNT0 = Count;
 }

 /** @brief			Initialize the timer 0 device in timer mode.
  *  @param Config	Pointer to Timer 0 device configuration struct
  */
 void Timer0_Init(Timer0_Config_t* Config);

 /** @brief			Initialize the timer 0 device in wave generation mode.
  *  @param Config	Pointer to Timer 0 device configuration struct
  */
 void Timer0_WaveInit(Timer0_WaveConfig_t* Config);

 /** @brief			Install a new callback for the Timer0.
  *  @param Config	Pointer to configuration structure
  */
 void Timer0_InstallCallback(Timer0_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Callback	Callback which should be removed
  */
 void Timer0_RemoveCallback(const Timer_CallbackType_t Callback);

#endif /* TIMER0_H_ */