/*
 * BH1750FVI.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for BH1750FVI I2C ambient light sensor.

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

/** @file Peripheral/BH1750FVI/BH1750FVI.h
 *  @brief Driver for the Rohm BH1750FVI I2C ambient light sensor.
 *
 *  This file contains the prototypes and definitions for the Rohm BH1750FVI ambient light sensor driver.
 *
 *  @author Daniel Kampert
 */

#ifndef BH1750FVI_H_
#define BH1750FVI_H_

 #include "Common/Common.h"

 #include <util/delay.h>

 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/GPIO/GPIO.h"
	 #include "Arch/XMega/I2C/I2C.h"
 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/GPIO/GPIO.h"
	 #include "Arch/AVR8/I2C/I2C.h"
 #else
	 #error "Architecture not supported for BH1750VI!"
 #endif

 /** @ingroup I2C-Addresses */
 /*\@{*/
 #if(defined(BH1750VI_USE_ADDR0) && defined(BH1750VI_USE_ADDR1))
	 #error "Invalid address configuration for BH1750VI!"
 #elif(defined(BH1750VI_USE_ADDR0))
	 #define BH1750VI_ADDRESS				0x5C
 #elif(defined(BH1750VI_USE_ADDR1))
	 #define BH1750VI_ADDRESS				0x23
 #else
	 #error "Invalid address configuration for BH1750VI!"
 #endif
 /*\@}*/

 /** @brief BH1750VI modes.
  */
 typedef enum
 {
	BH1750VI_MODE_PWR_DOWN = 0x00,					/**< Power down mode */
	BH1750VI_MODE_NORMAL = 0x01,					/**< Normal mode */
 } BH1750VI_DeviceMode_t;

 /** @brief BH1750VI resolution modes.
  */
 typedef enum
 {
	BH1750VI_RES_MED = 0x00,						/**< 1 lx Resolution */
	BH1750VI_RES_HIGH = 0x01,						/**< 0.5 lx Resolution */
	BH1750VI_RES_LOW = 0x03,						/**< 4 lx Resolution */
 } BH1750VI_Resolution_t;

 /** @brief			Initialize the BH1750FVI ambient light sensor and the I2C interface.
  *  @param Config	Pointer to I2C master configuration object
  *					NOTE: Set it to #NULL if you have initialized the I2C already
  *  @return		I2C error code
  */
 const I2C_Error_t BH1750VI_Init(I2CM_Config_t* Config);

 /** @brief		Reset the device.
  *  @return	I2C error code
  */
 const I2C_Error_t BH1750VI_Reset(void);

 /** @brief			Set the device mode.
  *  @param Mode	Device mode
  *  @return		I2C error code
  */
 const I2C_Error_t BH1750VI_SetMode(const BH1750VI_DeviceMode_t Mode);

 /** @brief			Adjust the optical window influence.
  *  @param Mtreg	Measurement time register
  *  @return		I2C error code
  */
 const I2C_Error_t BH1750VI_Adjust(const uint8_t Mtreg);

 /** @brief		Get the device mode.
  *  @return	Device mode
  */
 const BH1750VI_DeviceMode_t BH1750VI_GetMode(void);

 /** @brief				Perform a single measurement. The device switch to power down mode when the measurement finished.
  *  @param Resolution	Measurement resolution
  *  @param Result		Pointer to result
  *  @return			I2C error code
  */
 const I2C_Error_t BH1750VI_SingleMeasurement(const BH1750VI_Resolution_t Resolution, uint16_t* Result);

 /** @brief				Put the device into continuously measurement mode.
  *  @param Resolution	Measurement resolution
  *  @return			I2C error code
  */
 const I2C_Error_t BH1750VI_StartMeasurement(const BH1750VI_Resolution_t Resolution);
 
 /** @brief			Get the measurement result.
  *  @param Result	Pointer to measurement result
  *  @return		I2C error code
  */
 const I2C_Error_t BH1750VI_GetResult(uint16_t* Result);
 
 /** @brief		Stop the continuously measurement.
  *  @return	I2C error code
  */
 const I2C_Error_t BH1750VI_StopMeasurement(void);

#endif /* BH1750FVI_H_ */