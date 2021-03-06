/*
 * DS1307.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for DS1307 RTC.

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

/** @file Peripheral/DS1307/DS1307.h
 *  @brief Driver for the Dallas DS1307 real time clock.
 *
 *  This file contains the prototypes and definitions for the RTC driver.
 *
 *  @author Daniel Kampert
 *  @bug - Under construction 
 *
 */

#ifndef DS1307_H_
#define DS1307_H_

 #include "Common/Common.h"
 
 /** @ingroup I2C-Addresses */
 /*\@{*/
	#define DS1307_ADDRESS						0x68						/**< DS1307 I2C RTC device address */
 /*\@}*/
 
 /** @brief	Default settings for the RTC if the interrupt configuration is invalid
  */
 #if(defined DS1307_USE_IRQ)
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		 // ToDo: Add check
	 #elif(MCU_ARCH == MCU_ARCH_AVR8)
		 // ToDo: Add check
	 #else
		 #error "Architecture not supported for DS1307 interrupt configuration!"
	 #endif
 #endif
 
 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #if(MCU_NAME == MCU_NAME_ATMEGA32)
		 #include "Arch/AVR8/megaAVR/I2C/I2C.h"
	 #else
		 #error "Invalid CPU for DS1307!"
	 #endif
	 
	 #if(defined DS1307_USE_IRQ)
		 #if(MCU_NAME == MCU_NAME_ATMEGA32)
			 #include "Arch/AVR8/megaAVR/GPIO/GPIO.h"
			 #include "Arch/AVR8/megaAVR/CPU//CPU.h"
		 #endif
	 #endif
 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/I2C/I2C.h"
	 
	 #if(defined DS1307_USE_IRQ)
		#include "Arch/XMega/GPIO/GPIO.h"
		#include "Arch/XMega/CPU/CPU.h"
		#include "Arch/XMega/PMIC/PMIC.h"
	 #endif
 #else
	 #error "Architecture not supported for DS1307!"
 #endif

 /** @brief			DS1307 interrupt handler
  *  @param Time	RTC time
  */
 #if(defined DS1307_USE_IRQ)
	 typedef void (*DS1307_Callback_t)(Time_t Time);
 #endif

 /** @brief	DS1307 SQW frequency.
  */
 typedef enum
 {
	 DS1307_SQW_1HZ = 0x00,						/**< Frequency 1 Hz */
	 DS1307_SQW_4KHZ = 0x01,					/**< Frequency 4 kHz */
	 DS1307_SQW_8KHZ = 0x02,					/**< Frequency 8 kHz */
	 DS1307_SQW_32KHZ = 0x03,					/**< Frequency 32 kHz */
 } DS1307_SQWFreq_t;

 /** @brief	DS1307 interrupt configuration object.
  */
 #if(defined DS1307_USE_IRQ)
	 typedef struct 
	 {
		DS1307_SQWFreq_t Freq;					/**< DS1307 SQW frequency */
		PORT_t* Port;							/**< MCU port for SQW output */
		uint8_t Pin;							/**< MCU pin for SQW output */
		GPIO_InterruptChannel_t Channel;		/**< MCU interrupt channel for SQW output */
		GPIO_InputSense_t Sense;				/**< MCU interrupt sense for SQW output */
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			Interrupt_Level_t Level;			/**< MCU interrupt level for SQW output \n
													 NOTE: Only for XMega architecture */
		#endif

		DS1307_Callback_t Callback;				/**< Callback for SQW interrupt */
	 } DS1307_InterruptConfig_t;
 #endif

 /** @brief				Initialize the DS1307 RTC and the I2C interface.
  *  @param Config		Pointer to I2C master configuration struct \n
  *						NOTE: Set it to #NULL if you have initialized the I2C already
  *  @param	Time		Initial time for the RTC \n
  *						NOTE: Set to #NULL if you don�t want to set the time
  *  @param IntConfig	DS1307 interrupt configuration \n
  *						NOTE: Only available when #DS1307_USE_IRQ is set
  *  @return			I2C error code
  */
 #if(defined DS1307_USE_IRQ)
	 const I2C_Error_t DS1307_Init(I2CM_Config_t* Config, const Time_t* Time, DS1307_InterruptConfig_t* IntConfig);
 #else
	 const I2C_Error_t DS1307_Init(I2CM_Config_t* Config, const Time_t* Time);
 #endif
 
 /** @brief		Set the clock hold bit of the RTC.
  *  @return	I2C error code
  */
 const I2C_Error_t DS1307_HoldClock(void);
 
 /** @brief			Change the hour mode of the RTC.
  *  @param Mode	Hour mode
  *  @return		I2C error code
  */
 const I2C_Error_t DS1307_SetHourMode(const HourMode_t Mode);
 
 /** @brief				Install a new callback for DS1307 interrupts.
  *						NOTE: You have to use interrupt support to use the callbacks.
  *							  The callback is triggered during each DS1307 interrupt.
  *  @param Callback	Function pointer to DS1307 callback function
  */
 #if(defined DS1307_USE_IRQ)
	 void DS1307_InstallCallback(DS1307_Callback_t Callback);
 
	/** @brief	Remove an installed callback.
	 */
	 void DS1307_RemoveCallback(void);

	/** @brief			Enable interrupt support for the DS1307.
	 *					NOTE: This function is called automatically during the initialization of the RTC.
	 *  @param Config	Pointer to RTC interrupt configuration object
	 *  @return			I2C error code
	 */
	 const I2C_Error_t DS1307_EnableInterrupts(const DS1307_InterruptConfig_t* Config);

	/** @brief			Disable the interrupt support for the DS1307.
	 *
	 *  @param Config	Pointer to RTC interrupt configuration object
	 *  @return			I2C error code
	 */
	 const I2C_Error_t DS1307_DisableInterrupts(const DS1307_InterruptConfig_t* Config);
 #endif
 
 /** @brief			Set the time of the RTC.
  *  @param Time	Pointer to time object
  *  @return		I2C error code
  */
 const I2C_Error_t DS1307_SetTime(const Time_t* Time);
 
 /** @brief			Get the time of the RTC.
  *  @param Time	Pointer to time object
  *  @return		I2C error code
  */
 const I2C_Error_t DS1307_GetTime(Time_t* Time);

#endif /* DS1307_H_ */