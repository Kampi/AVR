/*
 * PCA9685.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the PCA9685 PWM controller.

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

/** @file Peripheral/PCA9685/PCA9685.h
 *  @brief Driver for the NXP PCA9685 PWM controller.
 *
 *  This file contains the prototypes and definitions for the PWM controller driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef PCA9685_H_
#define PCA9685_H_
 
 #include "Config_PCA9685.h"
 #include "Common/Common.h"
 
 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/GPIO/GPIO.h"
	 #include "Arch/XMega/ClockManagement/SysClock.h"
	 #include "Arch/XMega/I2C/I2C.h"
 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/GPIO/GPIO.h"
	 #include "Arch/AVR8/I2C/I2C.h"
 #else
	 #error "Architecture not supported for PCA9685!"
 #endif
 
 /** @ingroup I2C-Addresses */
 /*\@{*/
	#define PCA9685_ADDRESS							(0x40 | (PCA9685_A5 << 0x05) | \
															(PCA9685_A4 << 0x04) | \
															(PCA9685_A3 << 0x03) | \
															(PCA9685_A2 << 0x02) | \
															(PCA9685_A1 << 0x01) | \
															PCA9685_A0)					/**< PCA9685 I2C PWM controller device address */
 /*\@}*/

 /** 
  * Max time value for PCA9685
  */
 #define PCA9685_TIME_RANGE							4096							

 /** 
  * PCA9685 clock sources
  */
 typedef enum
 {
	 PCA9685_CLOCK_INT = 0x00,						/**< Internal oscillator */
	 PCA9685_CLOCK_EXT = 0x01,						/**< External clock */
 } PCA9685_ClockSource_t;

 /** 
  * PCA9685 output change options
  */
 typedef enum
 {
	 PCA9685_OCH_STOP = 0x00,						/**< Output change on STOP command */
	 PCA9685_OCH_ACK = 0x01,						/**< Output change on ACK */
 } PCA9685_OutputChange_t;

 /** 
  * PCA9685 output drive options
  */
 typedef enum
 {
	 PCA9685_OUTDRV_OD = 0x00,						/**< Open drain output drive */
	 PCA9685_OUTDRV_TP = 0x01,						/**< Totem pole output drive */
 } PCA9685_OutputDrive_t;

 /** 
  * PCA9685 output enable mode
  */
 typedef enum
 {
	 PCA9685_OUTNE_MODE_0 = 0x00,					/**< OE = 1 -> LEDn = 0 */
	 PCA9685_OUTNE_MODE_1 = 0x01,					/**< OE = 1 -> LEDn = 1 when OUTDRV = 1
																-> LEDn = high impedance when OUTDRV = 0 */
	 PCA9685_OUTNE_MODE_2 = 0x02,					/**< OE = 1 -> LEDn = high impedance */
 } PCA9685_OutputEnable_t;

 /** 
  * PCA9685 output channel
  */
 typedef enum
 {
	 PCA9685_CHANNEL_0 = 0x00,						/**< Channel 0 */
	 PCA9685_CHANNEL_1 = 0x01,						/**< Channel 1 */
	 PCA9685_CHANNEL_2 = 0x02,						/**< Channel 2 */
	 PCA9685_CHANNEL_3 = 0x03,						/**< Channel 3 */
	 PCA9685_CHANNEL_4 = 0x04,						/**< Channel 4 */
	 PCA9685_CHANNEL_5 = 0x05,						/**< Channel 5 */
	 PCA9685_CHANNEL_6 = 0x06,						/**< Channel 6 */
	 PCA9685_CHANNEL_7 = 0x07,						/**< Channel 7 */
	 PCA9685_CHANNEL_8 = 0x08,						/**< Channel 8 */
	 PCA9685_CHANNEL_9 = 0x09,						/**< Channel 9 */
	 PCA9685_CHANNEL_10 = 0x0A,						/**< Channel 10 */
	 PCA9685_CHANNEL_11 = 0x0B,						/**< Channel 11 */
	 PCA9685_CHANNEL_12 = 0x0C,						/**< Channel 12 */
	 PCA9685_CHANNEL_13 = 0x0D,						/**< Channel 13 */
	 PCA9685_CHANNEL_14 = 0x0E,						/**< Channel 14 */
	 PCA9685_CHANNEL_15 = 0x0F,						/**< Channel 15 */
	 PCA9685_CHANNEL_ALL = 0x10,					/**< All channels */
 } PCA9685_Channel_t;
 
 /** @brief			Initialize the PCA9685 PWM controller and the I2C interface.
  *  @param Config	Pointer to I2C master configuration object
  *					NOTE: Set it to #NULL if you have initialized the I2C already
  *  @param Source	Clock source
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_Init(I2CM_Config_t* Config, const PCA9685_ClockSource_t Source);

 /** @brief			Perform a software reset of the PWM controller.
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_Reset(void);

 /** @brief			Set the clock source for the PWM controller.
  *  @param Source	Clock source
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SetClockSource(const PCA9685_ClockSource_t Source);

 /** @brief			Set the output change mode for the PWM controller.
  *  @param Mode	Output change mode
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SetOutputChange(const PCA9685_OutputChange_t Mode);

 /** @brief			Set the clock source for the PWM controller.
  *  @param Drive	Output drive mode
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SetOutputDrive(const PCA9685_OutputDrive_t Drive);

 /** @brief			Set the output enable mode for the PWM controller.
  *  @param Mode	Output drive mode
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SetOutputEnable(const PCA9685_OutputEnable_t Mode);

 /** @brief			Restart all PWM channels.
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_Restart(void);

 /** @brief			Enable/Disable sleep mode for the PWM controller.
  *  @param	Enable	Enable/Disable
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SwitchSleep(const Bool_t Enable);

 /** @brief			Enable/Disable sub 1 address call mode for the PWM controller.
  *  @param	Enable	Enable/Disable
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SwitchSub1(const Bool_t Enable);
 
 /** @brief			Enable/Disable sub 2 address call mode for the PWM controller.
  *  @param	Enable	Enable/Disable
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SwitchSub2(const Bool_t Enable);
 
 /** @brief			Enable/Disable sub 3 address call mode for the PWM controller.
  *  @param	Enable	Enable/Disable
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SwitchSub3(const Bool_t Enable);

 /** @brief			Enable/Disable all call mode for the PWM controller.
  *  @param	Enable	Enable/Disable
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SwitchAllCall(const Bool_t Enable);

 /** @brief			Enable/Disable invert mode for the PWM controller.
  *  @param	Enable	Enable/Disable
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SwitchInvert(const Bool_t Enable);

 /** @brief				Set the prescaler for the PWM controller.
  *  @param	Prescaler	Prescaler
  *  @return			I2C error code
  */
 const I2C_Error_t PCA9685_SetPrescaler(const uint8_t Prescaler);

 /** @brief				Get the prescaler for the PWM controller.
  *  @param	Prescaler	Prescaler
  *  @return			I2C error code
  */
 const I2C_Error_t PCA9685_GetPrescaler(uint8_t* Prescaler);

 /** @brief			Set the channel value for a given channel.
  *  @param	Channel	PWM channel
  *  @param On		Channel on value
  *  @param Off		Channel off value
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SetChannel(const PCA9685_Channel_t Channel, const uint16_t On, const uint16_t Off);

 /** @brief			Set the duty cycle for a given channel.
  *  @param	Channel	PWM channel
  *  @param Duty	Duty cycle (between 0% and 100%)
  *  @return		I2C error code
  */
 const I2C_Error_t PCA9685_SetDuty(const PCA9685_Channel_t Channel, const float Duty);

 #if(defined PCA9685_OE)
	 /** @brief			Enable/Disable the outputs of the PWM controller.
	  *  @param	Enable	Enable/Disable
	  */
	 void PCA9685_SwitchOutputEnable(const Bool_t Enable);
 #else
	 #warning "No OE support for PCA9685"
 #endif
 
#endif /* PCA9685_H_ */