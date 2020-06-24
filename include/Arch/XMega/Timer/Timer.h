/*
 * Timer.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega Timer.

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

/** @file Arch/XMega/Timer/Timer.h
 *  @brief Driver for XMega Timer.
 *
 *  This contains the prototypes and definitions for the XMega Timer driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef TIMER_H_
#define TIMER_H_

 #include "Common/Common.h"

 #include "Arch/XMega/GPIO/GPIO.h"
 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/EventSystem/EventSystem.h"
 #include "Arch/XMega/ClockManagement/SysClock.h"
 #include "Arch/XMega/PowerManagement/PowerManagement.h"

 /** @brief	ID declaration for the different MCU types.
  */
 #define TIMER0C_ID		0							/**< Timer0 C ID */
 #define TIMER0D_ID		1							/**< Timer0 D ID */
 #define TIMER0E_ID		2							/**< Timer0 E ID */
 #define TIMER0F_ID		3							/**< Timer0 F ID */

 /** @brief	Timer callback definition
 */
 typedef void (*Timer_Callback_t)(void);

 /** @brief Timer prescaler.
  */
 typedef enum
 {
	 TIMER_PRESCALER_1 = 0x01,						/**< Prescaler 1 */ 
	 TIMER_PRESCALER_2 = 0x02,						/**< Prescaler 2 */ 
	 TIMER_PRESCALER_4 = 0x03,						/**< Prescaler 4 */ 
	 TIMER_PRESCALER_8 = 0x04,						/**< Prescaler 8 */ 
	 TIMER_PRESCALER_64 = 0x05,						/**< Prescaler 64 */ 
	 TIMER_PRESCALER_256 = 0x06,					/**< Prescaler 256 */ 
	 TIMER_PRESCALER_1024 = 0x07,					/**< Prescaler 1024 */ 
 } Timer_Prescaler_t;

 /** @brief Timer callback types.
  */
 typedef enum
 {
	 TIMER_OVERFLOW_INTERRUPT = 0x01,				/**< Timer overflow interrupt */ 
	 TIMER_ERROR_INTERRUPT = 0x02,					/**< Timer error interrupt */ 
	 TIMER_CCA_INTERRUPT = 0x10,					/**< Timer capture compare A interrupt */ 
	 TIMER_CCB_INTERRUPT = 0x20,					/**< Timer capture compare B interrupt */ 
	 TIMER_CCC_INTERRUPT = 0x40,					/**< Timer capture compare C interrupt */ 
	 TIMER_CCD_INTERRUPT = 0x80,					/**< Timer capture compare D interrupt */ 
	 TIMER_COMPMATCH_INTERRUPT = 0x02,				/**< Timer output compare match interrupt */ 
	 TIMER_COMPMATCH_A_INTERRUPT = 0x03,			/**< Timer output compare match A interrupt */ 
	 TIMER_COMPMATCH_B_INTERRUPT = 0x04,			/**< Timer output compare match B interrupt */ 
	 TIMER_CAPTURE_INTERRUPT = 0x05,				/**< Timer input capture interrupt */ 
 } Timer_CallbackType_t;

 /** @brief Timer device modes.
  */
 typedef enum
 {
	 TIMER_MODE_0 = 0x00,							/**< Timer mode 0 */ 
	 TIMER_MODE_1 = 0x01,							/**< Timer mode 1 */ 
	 TIMER_MODE_2 = 0x02,							/**< Timer mode 2 */ 
	 TIMER_MODE_3 = 0x03,							/**< Timer mode 3 */ 
	 TIMER_MODE_4 = 0x04,							/**< Timer mode 4 */ 
	 TIMER_MODE_5 = 0x05,							/**< Timer mode 5 */ 
	 TIMER_MODE_6 = 0x06,							/**< Timer mode 6 */ 
	 TIMER_MODE_7 = 0x07,							/**< Timer mode 7 */ 
	 TIMER_MODE_8 = 0x08,							/**< Timer mode 8 */ 
	 TIMER_MODE_9 = 0x09,							/**< Timer mode 9 */ 
	 TIMER_MODE_10 = 0x0A,							/**< Timer mode 10 */ 
	 TIMER_MODE_11 = 0x0B,							/**< Timer mode 11 */ 
	 TIMER_MODE_12 = 0x0C,							/**< Timer mode 12 */ 
	 TIMER_MODE_13 = 0x0D,							/**< Timer mode 13 */ 
	 TIMER_MODE_14 = 0x0E,							/**< Timer mode 14 */ 
	 TIMER_MODE_15 = 0x0F,							/**< Timer mode 15 */ 
 } Timer_Mode_t;

 /** @brief Timer capture compare channels.
  */
 typedef enum
 {
	 TIMER_CCA = 0x00,								/**< Capture compare channel A */ 
	 TIMER_CCB = 0x01,								/**< Capture compare channel B */ 
	 TIMER_CCC = 0x02,								/**< Capture compare channel C */ 
	 TIMER_CCD = 0x03,								/**< Capture compare channel D */ 
 } Timer_CCChannel_t;

 /** @brief	Timer commands
 */
 typedef enum
 {
	 TIMER_COMMAND_NONE = 0x00,						/**< Empty command */ 
	 TIMER_COMMAND_UPDATE = 0x01,					/**< Force update */ 
	 TIMER_COMMAND_RESTART = 0x02,					/**< Force restart */ 
	 TIMER_COMMAND_RESET = 0x03,					/**< Force reset */ 
 } Timer_Command_t;

 /** @brief	Timer count directions.
 */
 typedef enum
 {
	 TIMER_DIRECTION_TOP = 0x00,					/**< Count to top, Interrupt when overflow */ 
	 TIMER_DIRECTION_BOTTOM = 0x01,					/**< Count to bottom, Interrupt when underflow */ 
 } Timer_Direction_t;

 /** @brief	Macro to increase the duty cycle by a given value.
  *
  *	NOTE: Default value is 1.
  */
 #define TIMER0_INCREASEDUTY(Timer, Channel, ...)			Timer0_IncreaseDutyCycle(Timer, Channel, (1, ##__VA_ARGS__))
 
 /** @brief	Macro to decrease the duty cycle by a given value.
  *
  *	NOTE: Default value is 1.
  */
 #define TIMER0_DECREASEDUTY(Timer, Channel, ...)			Timer0_DecreaseDutyCycle(Timer, Channel, (1, ##__VA_ARGS__))

 /** 
  * Timer0 input capture modes
  */
 typedef enum
 {
	 TIMER_INPUT_CAPTURE = 0x01,					/**< Input capture mode */
	 TIMER_FRQ_CAPTURE = 0x05,						/**< Frequency capture mode */
	 TIMER_PW_CAPTURE = 0x06,						/**< Pulse width capture mode */
 } Timer0_CaptureMode_t;

 /** 
  * Timer0 configuration object
  */
 typedef struct
 {
	 TC0_t* Device;									/**< Pointer to Timer0 device object */ 
	 Timer_Prescaler_t Prescaler;					/**< Clock prescaler */ 
	 uint16_t Period;								/**< Timer period */ 
 } Timer0_Config_t;


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
  * Timer0 waveform configuration object
  */
 typedef struct 
 {
	 TC0_t* Device;									/**< Pointer to Timer0 device object */ 
	 Timer_CCChannel_t Channel;						/**< Capture or compare channel */
	 Timer_Prescaler_t Prescaler;					/**< Clock prescaler */ 
	 Timer_Mode_t Mode;								/**< Timer mode */ 
	 uint16_t Period;								/**< Timer period */ 
	 uint16_t DutyCycle;							/**< Timer duty cycle */ 
	 bool EnableInvert;							/**< Set to #true to enable invert mode for the WGM pin */ 
 } Timer0_WaveConfig_t;
 
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
	 bool Invert;									/**< Set to #true to invert the WGM pin */ 
 } Timer1_WaveConfig_t;
 
 /** 
  * Timer0 compare configuration object
  */
 typedef struct 
 {
	 TC0_t* Device;									/**< Pointer to Timer0 device object */
	 Timer_Prescaler_t Prescaler;					/**< Clock prescaler */
	 PORT_t* Port;									/**< Input port for capture mode */
	 uint8_t Pin;									/**< Input pin for capture mode */
	 Timer0_CaptureMode_t Mode;						/**< Capture mode */
	 Timer_CCChannel_t Channel;						/**< Capture channel */
	 Event_Channel_t EChannel;						/**< Event channel for capture mode */
	 GPIO_InputSense_t Sense;						/**< GPIO input sense settings */
 } Timer0_CaptureConfig_t;
 
 /** 
  * Timer0 interrupt configuration object
  */
 typedef struct
 {
	 TC0_t* Device;									/**< Pointer to Timer0 device object */
	 Timer_CallbackType_t Source;					/**< Timer interrupt type */
	 Interrupt_Level_t InterruptLevel;				/**< Interrupt level */
	 Timer_Callback_t Callback;						/**< Function pointer to timer callback */
 } Timer0_InterruptConfig_t;

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

 /*
	 Timer 0
 */

 /** @brief			Enable/Disable split mode for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Enable	Enable/Disable
  */
 static inline void Timer0_SwitchSplit(TC0_t* Device, const bool Enable) __attribute__((always_inline));
 static inline void Timer0_SwitchSplit(TC0_t* Device, const bool Enable)
 {
	 if(Enable == true)
	 {
		 Device->CTRLE = (Device->CTRLE & (~0x03)) | 0x02;
	 }
	 else
	 {
		 Device->CTRLE = (Device->CTRLE & (~0x03));
	 }
 }

 /** @brief			Set the event channel for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Channel	Event channel
  */
 static inline void Timer0_SetEventChannel(TC0_t* Device, const Event_Channel_t Channel) __attribute__((always_inline));
 static inline void Timer0_SetEventChannel(TC0_t* Device, const Event_Channel_t Channel)
 {
	 Device->CTRLD |= (0x08 | (Channel & 0x0F));
 }

 /** @brief			Get the event channel for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @return		Event channel
  */
 static inline const Event_Channel_t Timer0_GetEventChannel(TC0_t* Device) __attribute__((always_inline));
 static inline const Event_Channel_t Timer0_GetEventChannel(TC0_t* Device)
 {
	 return (Device->CTRLA & 0x0F);
 }

 /** @brief			Set the capture mode for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Mode	Capture mode
  */
 static inline void Timer0_SetCaptureMode(TC0_t* Device, const Timer0_CaptureMode_t Mode) __attribute__((always_inline));
 static inline void Timer0_SetCaptureMode(TC0_t* Device, const Timer0_CaptureMode_t Mode)
 {
	 Device->CTRLD = (Device->CTRLD & (~(0x07 << 0x05))) | (Mode << 0x05);
 }
 /** @brief			Get the capture mode for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @return		Capture mode
  */
 static inline const Timer_Prescaler_t Timer0_GetCaptureMode(const TC0_t* Device) __attribute__((always_inline)); 
 static inline const Timer_Prescaler_t Timer0_GetCaptureMode(const TC0_t* Device)
 {
	 return (Device->CTRLD >> 0x05);
 }

 /** @brief				Set the prescaler for a Timer0 device.
  *  @param Device		Pointer to Timer0 object
  *  @param Prescaler	Timer prescaler
  */
 static inline void Timer0_SetPrescaler(TC0_t* Device, const Timer_Prescaler_t Prescaler) __attribute__((always_inline));
 static inline void Timer0_SetPrescaler(TC0_t* Device, const Timer_Prescaler_t Prescaler)
 {
	 Device->CTRLA = Prescaler;
 }
 /** @brief			Set the prescaler for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @return		Timer prescaler
  */
 static inline const Timer_Prescaler_t Timer0_GetPrescaler(const TC0_t* Device) __attribute__((always_inline)); 
 static inline const Timer_Prescaler_t Timer0_GetPrescaler(const TC0_t* Device)
 {
	 return (Device->CTRLA & 0x0F);
 }

 /** @brief			Set the mode for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Mode	Timer mode
  */
 static inline void Timer0_SetMode(TC0_t* Device, const Timer_Mode_t Mode) __attribute__((always_inline));
 static inline void Timer0_SetMode(TC0_t* Device, const Timer_Mode_t Mode)
 {
	 Timer_Mode_t TempMode = Mode;
	 
	 if((TempMode == TIMER_MODE_2) && (TempMode == TIMER_MODE_4))
	 {
		 TempMode = TIMER_MODE_0;
	 }
	 
	 Device->CTRLB = (Device->CTRLB & ~0x07) | TempMode;
 }

 /** @brief			Get the mode for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @return		Timer mode
  */
 static inline const Timer_Mode_t Timer0_GetMode(const TC0_t* Device) __attribute__((always_inline)); 
 static inline const Timer_Mode_t Timer0_GetMode(const TC0_t* Device)
 {
	 return (Device->CTRLB & 0x07);
 }

 /** @brief			Set the period for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Period	Timer period
  */
 static inline void Timer0_SetPeriod(TC0_t* Device, const uint16_t Period) __attribute__((always_inline));
 static inline void Timer0_SetPeriod(TC0_t* Device, const uint16_t Period)
 {
	 Device->PER = Period;
 }

 /** @brief			Enable a capture or compare channel for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Channel	Capture or compare channel
  */
 static inline void Timer0_EnableCC(TC0_t* Device, const Timer_CCChannel_t Channel) __attribute__((always_inline));
 static inline void Timer0_EnableCC(TC0_t* Device, const Timer_CCChannel_t Channel)
 {
	 Device->CTRLB |= (0x01 << (0x04 + Channel));
 }
 
 /** @brief			Disable a capture or compare channel for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Channel	Capture or compare channel
  */
 static inline void Timer0_DisableCC(TC0_t* Device, const Timer_CCChannel_t Channel) __attribute__((always_inline));
 static inline void Timer0_DisableCC(TC0_t* Device, const Timer_CCChannel_t Channel)
 {
	 Device->CTRLB &= ~(0x01 << (0x04 + Channel));
 }

 /** @brief				Set the count direction for a Timer0 device.
  *  @param Device		Pointer to Timer0 object
  *  @param Direction	Count direction
  */
 static inline void Timer0_SetDirection(TC0_t* Device, const Timer_Direction_t Direction) __attribute__((always_inline));
 static inline void Timer0_SetDirection(TC0_t* Device, const Timer_Direction_t Direction)
 {
	 if(Direction == TIMER_DIRECTION_BOTTOM)
	 {
		 Device->CTRLFSET |= 0x01;
	 }
	 else
	 {
		 Device->CTRLFCLR |= 0x01;
	 }
 }
 
 /** @brief			Lock a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  */
 static inline void Timer0_Lock(TC0_t* Device) __attribute__((always_inline));
 static inline void Timer0_Lock(TC0_t* Device)
 {
	 Device->CTRLFSET |= (0x01 << 0x01);
 }

 /** @brief			Unlock a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  */
 static inline void Timer0_Unlock(TC0_t* Device) __attribute__((always_inline));
 static inline void Timer0_Unlock(TC0_t* Device)
 {
	 Device->CTRLFCLR |= (0x01 << 0x01);
 }
 
 /** @brief			Execute a Timer0 command.
  *  @param Device	Pointer to Timer0 object
  *  @param Command	Timer command
  */
 static inline void Timer0_Command(TC0_t* Device, const Timer_Command_t Command) __attribute__((always_inline));
 static inline void Timer0_Command(TC0_t* Device, const Timer_Command_t Command)
 {
	 Device->CTRLFSET |= (Command << 0x02);
 }
 
 /** @brief				Clear an interrupt flag.
  *  @param Device		Pointer to Timer0 object
  *  @param Callback	Interrupt type
  */
 static inline void Timer0_ClearFlag(TC0_t* Device, const Timer_CallbackType_t Callback) __attribute__((always_inline));
 static inline void Timer0_ClearFlag(TC0_t* Device, const Timer_CallbackType_t Callback)
 {
	 Device->INTFLAGS |= Callback;	
 }
 
 /** @brief			Set the capture or compare value for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Channel	Capture or compare channel
  *  @param Value	Capture or compare value
  */
 static inline void Timer0_SetCC(TC0_t* Device, const Timer_CCChannel_t Channel, const uint16_t Value)  __attribute__((always_inline));
 static inline void Timer0_SetCC(TC0_t* Device, const Timer_CCChannel_t Channel, const uint16_t Value)
 {
	 register16_t* Timer = &Device->PER + ((Channel & 0x07) << 0x01);
	 *Timer = Value;
 }

 /** @brief			Get the capture or compare value for a Timer0 device.
  *  @param Device	Pointer to Timer0 object
  *  @param Channel	Capture or compare channel
  *  @return		Capture or compare value
  */
 static inline const uint16_t Timer0_GetCC(TC0_t* Device, const Timer_CCChannel_t Channel) __attribute__((always_inline));
 static inline const uint16_t Timer0_GetCC(TC0_t* Device, const Timer_CCChannel_t Channel)
 {
	 register16_t* Timer = &Device->PER + ((Channel & 0x07) << 0x01);
	 return *Timer;
 }

 /** @brief			Increase the capture or compare value by a given step.
  *  @param Device	Pointer to Timer0 object
  *  @param Channel	Capture or compare channel
  *  @param Step	Step size
  */
 static inline void Timer0_IncreaseDutyCycle(TC0_t* Device, const Timer_CCChannel_t Channel, const uint16_t Step) __attribute__((always_inline));
 static inline void Timer0_IncreaseDutyCycle(TC0_t* Device, const Timer_CCChannel_t Channel, const uint16_t Step)
 {
	 register16_t* Timer = &Device->PER + ((Channel & 0x07) << 0x01);
	 *Timer += Step;
 }
 
 /** @brief			Decrease the capture or compare value by a given step.
  *  @param Device	Pointer to Timer0 object
  *  @param Channel	Capture or compare channel
  *  @param Step	Step size
  */
 static inline void Timer0_DecreaseDutyCycle(TC0_t* Device, const Timer_CCChannel_t Channel, const uint16_t Step) __attribute__((always_inline));
 static inline void Timer0_DecreaseDutyCycle(TC0_t* Device, const Timer_CCChannel_t Channel, const uint16_t Step)
 {
	 register16_t* Timer = &Device->PER + ((Channel & 0x07) << 0x01);
	 *Timer -= Step;
 }

 /** @brief			Initialize a Timer0 device.
  *  @param Config	Pointer to Timer0 configuration struct
  */
 void Timer0_Init(Timer0_Config_t* Config);
 
 /** @brief			Initialize a Timer0 device with waveform option.
  *  @param Config	Pointer to Timer0 waveform configuration struct
  */
 void Timer0_WaveInit(Timer0_WaveConfig_t* Config);
 
 /** @brief			Initialize a Timer0 device with capture option.
  *  @param Config	Pointer to Timer0 capture configuration struct
  */
 void Timer0_CaptureInit(Timer0_CaptureConfig_t* Config);
 
 /** @brief			Install a new callback for a Timer0 device.
  *  @param Config	Pointer to interrupt configuration structure
  */
 void Timer0_InstallCallback(Timer0_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Device		Pointer to Timer0 object
  *  @param Callback	Callback which should be removed
  */
 void Timer0_RemoveCallback(TC0_t* Device, const Timer_CallbackType_t Callback);
 
 /** @brief					Change the interrupt level of a timer interrupt.
  *  @param Device			Pointer to Timer0 object
  *  @param Callback		Callback which should be changed
  *  @param InterruptLevel	New interrupt level
  */
 void Timer0_ChangeInterruptLevel(TC0_t* Device, const Timer_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel);

 /*
	 Timer 1
 */

 /** @brief				Clear an interrupt flag.
  *  @param Device		Pointer to Timer1 object
  *  @param Callback	Interrupt type
  */
 static inline void Timer1_ClearFlag(TC1_t* Device, const Timer_CallbackType_t Callback) __attribute__((always_inline));
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

#endif /* TIMER_H_  */ 