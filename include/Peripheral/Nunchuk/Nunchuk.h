/*
 * Nunchuk.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the a Nintendo Nunchuk.

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

/** @file Peripheral/Nunchuk/Nunchuk.h
 *  @brief Driver for the a Nintendo Nunchuk.
 *
 *  This file contains the prototypes and definitions for the Nunchuk driver.
 *
 *  @author Daniel Kampert
 */

#ifndef NUNCHUK_H_
#define NUNCHUK_H_

 #include "Common/Common.h"
 
 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_AVR8)
 	 #if(MCU_NAME == MCU_NAME_ATMEGA32)
 		 #include "Arch/AVR8/megaAVR/mega32/GPIO/GPIO.h"
 		 #include "Arch/AVR8/megaAVR/mega32/I2C/I2C.h"
	 #else
		 #error "Invalid CPU for PCA9685!"
	 #endif 

 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/GPIO/GPIO.h"
	 #include "Arch/XMega/ClockManagement/SysClock.h"
	 #include "Arch/XMega/I2C/I2C.h"
 #else
	 #error "Architecture not supported for PCA9685!"
 #endif

 /** @brief	Nunchuck joystick data.
  */
 typedef struct
 {
	 uint8_t Joy_X;										/**< Joystick X axis */
	 uint8_t Joy_Y;										/**< Joystick Y axis */
	 int16_t Acc_X;										/**< Accelerometer X axis */
	 int16_t Acc_Y;										/**< Accelerometer Y axis */
	 int16_t Acc_Z;										/**< Accelerometer Z axis */
	 bool Button_Z;										/**< Z-button state \
															 NOTE: #true when pressed. */
	 bool Button_C;										/**< C-button state \
															 NOTE: #true when pressed. */
 } __attribute__((packed)) Nunchuk_Data_t;

 /** @ingroup I2C-Addresses */
 /*\@{*/
	#define NUNCHUK_ADDRESS							0x52			/**< Nintendo Nunchuk I2C device address */
 /*\@}*/
 
 /** @brief			Initialize the Nunchuk and the I2C interface.
  *  @param Config	Pointer to I2C master configuration object
  *					NOTE: Set it to #NULL if you have initialized the I2C already
  *  @return		I2C error code
  */
 const I2C_Error_t Nunchuk_Init(I2CM_Config_t* Config);

 /** @brief			Read new data from the Nunchuk.
  *  @param Data	Pointer to Nunchuk data object
  *  @return		I2C error code
  */
 const I2C_Error_t Nunchuk_Read(Nunchuk_Data_t* Data);

#endif /* NUNCHUK_H_ */