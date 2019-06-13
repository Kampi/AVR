/*
 * DS1307.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Dallas DS1307 RTC

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
 *		 - Implement complete IRQ support. Use IRQ driven transmission to send and receive multiple bytes.Bug in "EnableInterrupt"-Function.
 */

#ifndef DS1307_H_
#define DS1307_H_
 
 #include "Board.h"

 /** @ingroup I2C-Addresses */
 /*\@{*/
	#define DS1307_ADDRESS						0x68						/**< DS1307 I2C RTC device address */
 /*\@}*/
 
 #ifndef DS1307_USE_IRQ
	 #define DS1307_USE_IRQ						DISABLE
 #endif

 /** @brief	Default settings for the RTC if the interrupt configuration is invalid
  */
 #if(DS1307_USE_IRQ == ENABLE)
	#if(!defined DS1307_INT_PORT || !defined DS1307_INT_PIN || !defined DS1307_INT_CHANNEL || !defined DS1307_INT_PULLUP || !defined DS1307_INT_FREQUENCY || (!defined DS1307_INT_PRIORITY && MCU_ARCH == MCU_ARCH_XMEGA))
		#error "No valid RTC interrupt configuration!"
	#endif
 #endif
 
 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/ATmega/I2C/I2C.h"
	 
	 #define DS1307_I2C_INIT(Config)							I2CM_Init(Config)															/**< Generic I2C initialization macro for the RTC */
	 #define DS1307_I2C_WRITE(Register, Length, Data)			I2CM_WriteBytes(DS1307_ADDRESS, Register, Length, Data)						/**< Generic I2C write macro for the RTC */
	 #define DS1307_I2C_READ(Register, Length, Data)			I2CM_ReadBytes(DS1307_ADDRESS, Register, Length, Data)						/**< Generic I2C read macro for the RTC */
	 #define DS1307_I2C_WRITEREG(Register, Data)				I2CM_WriteBytes(DS1307_ADDRESS, Register, 1, Data)							/**< Generic I2C write register macro for the RTC */
	 #define DS1307_I2C_READREG(Register, Data)					I2CM_ReadBytes(DS1307_ADDRESS, Register, 1, Data)							/**< Generic I2C read register macro for the RTC */
	 
	 #if(DS1307_USE_IRQ == ENABLE)
		#error "Not supported"
	 #endif
 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	/** @brief	TWI interface for device when using XMega architecture.
	 */
	#ifndef DS1307_INTERFACE
		#define DS1307_INTERFACE				&TWIC
		#warning "No I2C interface specified. Use default!"
	#endif
 
	 #include "Arch/XMega/I2C/I2C.h"
	 
	 #define DS1307_I2C_INIT(Config)							I2CM_Init(Config)															/**< Generic I2C initialization macro for the RTC */
	 #define DS1307_I2C_WRITE(Register, Length, Data)			I2CM_WriteBytes(DS1307_INTERFACE, DS1307_ADDRESS, Register, Length, Data)	/**< Generic I2C write macro for the RTC */
	 #define DS1307_I2C_READ(Register, Length, Data)			I2CM_ReadBytes(DS1307_INTERFACE, DS1307_ADDRESS, Register, Length, Data)	/**< Generic I2C read macro for the RTC */
	 
	 #define DS1307_I2C_WRITEREG(Register, Data)				I2CM_WriteBytes(DS1307_INTERFACE, DS1307_ADDRESS, Register, 1, Data)		/**< Generic I2C write register macro for the RTC */
	 #define DS1307_I2C_READREG(Register, Data)					I2CM_ReadBytes(DS1307_INTERFACE, DS1307_ADDRESS, Register, 1, Data)			/**< Generic I2C read register macro for the RTC */
	 #define DS1307_I2C_STATUS()								I2CM_Status(DS1307_INTERFACE)												/**< Generic I2C read message status macro for the RTC */
 #else
	 #error "No valid mcu"
 #endif

 #include "Common/time_avr.h"

 /** @brief			DS1307 interrupt handler
  *  @param Time	RTC time
  */
 typedef void (*DS1307_Callback_t)(Time_t Time);

 /** 
  * DS1307 SQW frequencies
  */
 typedef enum
 {
	 DS1307_SQW_1HZ = 0x00,						/**< Frequency 1 Hz */
	 DS1307_SQW_4KHZ = 0x01,					/**< Frequency 4 kHz */
	 DS1307_SQW_8KHZ = 0x02,					/**< Frequency 8 kHz */
	 DS1307_SQW_32KHZ = 0x03,					/**< Frequency 32 kHz */
 } DS1307_SQWFreq_t;
 
 /** @brief			Initialize the DS1307 RTC and the I2C interface.
  *  @param Config	Pointer to I2C master configuration struct
  *					NOTE: Set it to *NULL if you have initialized the I2C already
  *  @param	Time	Initial time for the RTC.
  *					NOTE: Set it to *NULL if you have don´t want to set a time
  *  @return		I2C error code
  */
 const I2C_Error_t DS1307_Init(I2CM_Config_t* Config, const Time_t* Time);
 
 /** @brief		Set the clock hold bit of the RTC.
  *  @return	I2C communication error
  */
 const I2C_Error_t DS1307_HoldClock(void);
 
 /** @brief			Change the hour mode of the RTC.
  *  @param Mode	Hour mode
  *  @return		I2C communication error
  */
 const I2C_Error_t DS1307_ChangeHourMode(const HourMode_t Mode);
 
 /** @brief				Install a new callback for DS1307 interrupts.
  *						NOTE: You have to use interrupt support to use the callbacks.
  *							  The callback is triggered during each DS1307 interrupt.
  *  @param Callback	Function pointer to DS1307 callback function
  */
 void DS1307_InstallCallback(DS1307_Callback_t Callback);
 
 /** @brief		Remove an installed callback.
  */
 void DS1307_RemoveCallback(void);

 /** @brief				Enable interrupt support for the DS1307.
  *						NOTE: This function is called automatically during the initialization of the RTC.
  *  @param Frequency	SQW output frequency
  *  @return			I2C error code
  */
 const I2C_Error_t DS1307_EnableInterrupts(DS1307_SQWFreq_t Frequency);

 /** @brief		Disable the interrupt support for the DS1307.
  *  @return	I2C error code
  */
 const I2C_Error_t DS1307_DisableInterrupts(void);
 
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