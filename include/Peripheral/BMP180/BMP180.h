/*
 * BMP180.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for BMP180 I2C pressure sensor.

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

/** @file Peripheral/BMP180/BMP180.h
 *  @brief Driver for the Bosch BMP180 I2C pressure sensor.
 *
 *  This file contains the prototypes and definitions for the Bosch BMP180 pressure sensor driver.
 *
 *  @author Daniel Kampert
 */

#ifndef BMP180_H_
#define BMP180_H_

 #include "Config_BMP180.h"
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
	 #error "Architecture not supported for BMP180!"
 #endif

 /** 
  * BMP180 chip ID
  */
 #define BMP180_ID							0x55

 /** @ingroup I2C-Addresses */
 /*\@{*/
	 #define BMP180_ADDRESS					0x77																							/**< BMP180 I2C pressure sensor device address */
 /*\@}*/

 /**
  * BMP180 oversampling settings
  */
 typedef enum
 {
	BMP180_OSS_1 = 0x00,								/**< Average of a single measurement */
	BMP180_OSS_2 = 0x01,								/**< Average of two measurements */
	BMP180_OSS_4 = 0x02,								/**< Average of four measurements */
	BMP180_OSS_8 = 0x03,								/**< Average of eight measurements */
 } BMP180_OSS_t;

 /**
  * BMP180 calibration coefficients
  */
 typedef struct
 {
	 int16_t AC1;										/**< AC1 parameter */
	 int16_t AC2;										/**< AC2 parameter */
	 int16_t AC3;										/**< AC3 parameter */
	 uint16_t AC4;										/**< AC4 parameter */
	 uint16_t AC5;										/**< AC5 parameter */
	 uint16_t AC6;										/**< AC6 parameter */
	 int16_t B1;										/**< B1 parameter */
	 int16_t B2;										/**< B2 parameter */
	 int16_t MB;										/**< MB parameter */
	 int16_t MC;										/**< MC parameter */
	 int16_t MD;										/**< MD parameter */
 } __attribute__((packed)) BMP180_CalibCoef_t;

 /**
  * BMP180 data point
  */
 typedef struct 
 {
	 float Temperature;									/**< Temperature in Degree Celsius */
	 uint32_t Pressure;									/**< Pressure in Pa */
 } BMP180_DataPoint_t;

 /** @brief			Initialize the BMP180 pressure sensor and the I2C interface.
  *  @param Config	Pointer to I2C master configuration object
  *					NOTE: Set it to *NULL if you have initialized the I2C already
  *  @return		I2C error code
  */
 const I2C_Error_t BMP180_Init(I2CM_Config_t* Config); 
 
 /** @brief		Reset the device.
  *  @return	I2C error code
  */
 const I2C_Error_t BMP180_Reset(void);
 
 /** @brief				Read the calibration coefficients from the sensor.
  *  @param CalibCoef	Pointer to #BMP180_CalibCoef_t structure
  *  @return			I2C error code
  */
 const I2C_Error_t BMP180_ReadCalibration(BMP180_CalibCoef_t* CalibCoef);

 /** @brief				Run an uncalibrated temperature measurement.
  *  @param DataPoint	Pointer to #BMP180_DataPoint_t structure
  *  @return			I2C error code
  */
 const I2C_Error_t BMP180_MeasureTemperature(BMP180_DataPoint_t* DataPoint);

 /** @brief				Run an uncalibrated pressure measurement.
 *  @param OSS			Oversampling factor
  *  @param DataPoint	Pointer to #BMP180_DataPoint_t structure
  *  @return			I2C error code
  */
 const I2C_Error_t BMP180_MeasurePressure(const BMP180_OSS_t OSS, BMP180_DataPoint_t* DataPoint);

 /** @brief				Run a single measurement.
						NOTE: You have run a calibration first!
  *  @param OSS			Oversampling factor
  *  @param CalibCoef	Calibration coefficients
  *  @param DataPoint	Pointer do #BMP180_DataPoint_t structure
  *  @return			I2C error code
  */
 const I2C_Error_t BMP180_SingleMeasurement(const BMP180_OSS_t OSS, const BMP180_CalibCoef_t* CalibCoef, BMP180_DataPoint_t* DataPoint);

#endif /* BMP180_H_ */