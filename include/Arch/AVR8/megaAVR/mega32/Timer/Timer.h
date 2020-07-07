/*
 * Timer0.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 megaAVR Timer modules.

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

/** @file Arch/AVR8/megaAVR/mega32/Timer/Timer.h
 *  @brief Driver for Atmel AVR8 megaAVR Timer modules.
 *
 *  This contains the prototypes and definitions for the AVR8 Timer driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef TIMER_H_
#define TIMER_H_

 #include "Common/Common.h"
 
 #include "Arch/AVR8/megaAVR/mega32/CPU/CPU.h"
 
 /** @brief	Timer callback definition.
 */
 typedef void (*Timer_Callback_t)(void);

 /** @brief	Timer count directions.
 */
 typedef enum
 {
	 TIMER_DIRECTION_TOP = 0x00,							/**< Count to top, Interrupt when overflow */
	 TIMER_DIRECTION_BOTTOM = 0x01,							/**< Count to bottom, Interrupt when underflow */
 } Timer_Direction_t;

 /** @brief Timer prescaler.
  */
 typedef enum
 {
	 TIMER_PRESCALER_1 = 0x01,								/**< Prescaler 1 */
	 TIMER_PRESCALER_2 = 0x02,								/**< Prescaler 2 */
	 TIMER_PRESCALER_4 = 0x03,								/**< Prescaler 4 */
	 TIMER_PRESCALER_8 = 0x04,								/**< Prescaler 8 */
	 TIMER_PRESCALER_64 = 0x05,								/**< Prescaler 64 */
	 TIMER_PRESCALER_256 = 0x06,							/**< Prescaler 256 */
	 TIMER_PRESCALER_1024 = 0x07,							/**< Prescaler 1024 */
 } Timer_Prescaler_t;

 /** @brief Timer callback types.
  */
 typedef enum
 {
	 TIMER_OVERFLOW_INTERRUPT = 0x01,						/**< Timer overflow interrupt */
	 TIMER_COMPMATCH_INTERRUPT = 0x02,						/**< Timer output compare match interrupt */
	 TIMER_COMPMATCH_A_INTERRUPT = 0x03,					/**< Timer output compare match A interrupt */
	 TIMER_COMPMATCH_B_INTERRUPT = 0x04,					/**< Timer output compare match B interrupt */
	 TIMER_CAPTURE_INTERRUPT = 0x05,						/**< Timer input capture interrupt */
 } Timer_CallbackType_t;

 /** @brief Timer device modes.
  */
 typedef enum
 {
	 TIMER_MODE_0 = 0x00,									/**< Timer mode 0 */
	 TIMER_MODE_1 = 0x01,									/**< Timer mode 1 */
	 TIMER_MODE_2 = 0x02,									/**< Timer mode 2 */
	 TIMER_MODE_3 = 0x03,									/**< Timer mode 3 */
	 TIMER_MODE_4 = 0x04,									/**< Timer mode 4 */
	 TIMER_MODE_5 = 0x05,									/**< Timer mode 5 */
	 TIMER_MODE_6 = 0x06,									/**< Timer mode 6 */
	 TIMER_MODE_7 = 0x07,									/**< Timer mode 7 */
	 TIMER_MODE_8 = 0x08,									/**< Timer mode 8 */
	 TIMER_MODE_9 = 0x09,									/**< Timer mode 9 */
	 TIMER_MODE_10 = 0x0A,									/**< Timer mode 10 */
	 TIMER_MODE_11 = 0x0B,									/**< Timer mode 11 */
	 TIMER_MODE_12 = 0x0C,									/**< Timer mode 12 */
	 TIMER_MODE_13 = 0x0D,									/**< Timer mode 13 */
	 TIMER_MODE_14 = 0x0E,									/**< Timer mode 14 */
	 TIMER_MODE_15 = 0x0F,									/**< Timer mode 15 */
 } Timer_Mode_t;

 /** @brief Timer capture compare channels.
  */
 typedef enum
 {
	 TIMER_CCA = 0x00,										/**< Capture compare channel A */
	 TIMER_CCB = 0x01,										/**< Capture compare channel B */
	 TIMER_CCC = 0x02,										/**< Capture compare channel C */
	 TIMER_CCD = 0x03,										/**< Capture compare channel D */
 } Timer_CCChannel_t;
 
 /** @brief Timer compare output configuration for AVR8 devices.
  * 		NOTE: The function is depending on the timer mode! Please check the data sheet.
  */
 typedef enum
 {
	 TIMER_COMPARE_MODE_0 = 0x00,							/**< Compare mode 0 */
	 TIMER_COMPARE_MODE_1 = 0x01,							/**< Compare mode 1 */
	 TIMER_COMPARE_MODE_2 = 0x02,							/**< Compare mode 2 */
	 TIMER_COMPARE_MODE_3 = 0x03,							/**< Compare mode 3 */
 } Timer_CompareOutput_t;

 /** @brief External clock settings for timer.
  */
 typedef enum
 {
	 TIMER_EXT_CLOCK_FALL = 0x00,							/**< Set external clock on falling edge */
	 TIMER_EXT_CLOCK_RISE = 0x01,							/**< Set external clock on rising edge */
 } Timer_ExtClock_t;

 /** @brief	Timer 0 configuration object.
 */
 typedef struct
 {
	 Timer_Prescaler_t Prescaler;							/**< CTimer clock prescaler */
	 Timer_CompareOutput_t CompareOutput;					/**< Compare output settings */
	 uint8_t Period;										/**< Timer period \n
																 NOTE: You have to set it once. The timer get reloaded during each interrupt. */ 
	 uint8_t Compare;										/**< Compare value \n
																 NOTE: ONly needed if you use the \ref Timer0_Config_t.CompareOutput. */
 } Timer0_Config_t;

 /** @brief	Timer 0 waveform configuration object.
 */
 typedef struct
 {
	 Timer_Prescaler_t Prescaler;							/**< CTimer clock prescaler */ 
	 Timer_CompareOutput_t CompareOutput;					/**< Compare output settings */
	 Timer_Mode_t Mode;										/**< Timer mode for waveform generation */
	 uint8_t Period;										/**< Timer period */
	 uint8_t Compare;										/**< Compare value \n
																 NOTE: ONly needed if you use the \ref Timer0_Config_t.CompareOutput. */
 } Timer0_WaveConfig_t;

 /** @brief	Timer 0 interrupt configuration object.
 */
 typedef struct
 {
	 Timer_CallbackType_t Source;
	 Timer_Callback_t Callback;
 } Timer0_InterruptConfig_t;

 /** @brief	Enable timer0.
  */
 static inline void Timer0_Enable(void) __attribute__ ((always_inline));
 static inline void Timer0_Enable(void)
 {
	 TCCR0 |= (0x01 << CS00);
 }
 
 /** @brief	Disable timer0.
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

#endif /* TIMER_H_ */