/*
 * MLX90393.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Melexis MLX90393 I2C magnetic sensor.

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

/** @file Peripheral/MLX90399/MLX90393.h
 *  @brief Driver for Melexis MLX90393 I2C magnetic sensor.
 *
 *  This file contains the prototypes and definitions for the Melexis MLX90393 I2C driver.
 *
 *  @author Daniel Kampert
 *  @bug - Under construction
 *		 - Add SPI support
 *		 - Add timeout for "Single" function
 */

#ifndef MLX90393_H_
#define MLX90393_H_

 #include "Definitions.h"
 #include "Arch/I2C.h"
 
 #include "Config_MLX90393.h"
 
 /** @ingroup I2C-Addresses */
 /*\@{*/
	#define MLX90393_ADDRESS					((MLX90393_EE_I2C_ADDR << 0x02) | \
												(MLX90393_A1 << 0x05) | \
												MLX90393_A0)						/**< MLX90393 I2C magnetic sensor device address */
 /*\@}*/

 #if(MLX90393_INTERFACE_TYPE == INTERFACE_I2C)
 	#define MLX90393_COMM_ERROR			I2C_Error_t
 #elif((MLX90393_INTERFACE_TYPE == INTERFACE_USART_SPI) | (MLX90393_INTERFACE_TYPE == INTERFACE_SPI))
	 // Todo
 #else
	 #error "Invalid interface for MXL90393!"
 #endif

 /** 
  * MLX90393 wake on change modes
  */
 typedef enum
 {
	 MLX90393_WAKE_ABSOLUTE = 0x00,					/**< Use the measurement at t = 0 as reference */
	 MLX90393_WAKE_DIFFERENTIAL = 0x01,				/**< Use the measurement at t - 1 as reference */
 } MLX90393_WakeMode_t;
 
 /** 
  * MLX90393 axis
  */
 typedef enum
 {
	 MLX90393_AXIS_T = 0x01,						/**< t axis */
	 MLX90393_AXIS_X = 0x02,						/**< x axis */
	 MLX90393_AXIS_Y = 0x04,						/**< y axis */
	 MLX90393_AXIS_Z = 0x08,						/**< z axis */
 } MLX90393_Axis_t;

 /** 
  * MLX90393 device modes
  */
 typedef enum
 {
	 MLX90393_MODE_BOTH = 0x00,						/**< I2C or SPI mode (according to CS) */
	 MLX90393_MODE_SPI = 0x02,	 					/**< SPI mode */
	 MLX90393_MODE_I2C = 0x03,						/**< I2C mode */
 } MLX90393_Mode_t;

 /** 
  * MLX90393 hall configuration object
  */
 typedef struct 
 {
	 uint8_t Periods:2;								/**< Number of amplifier chopping periods inside one spinning phase */
	 uint8_t Duration:2;							/**< Duration of one Hall plate spinning phase in clock cycles */
 } MLX90393_HallConf_t;

 /** 
  * MLX90393 configuration object
  */
 typedef struct
 {
	 uint8_t Gain;									/**< Gain setting */
	 MLX90393_HallConf_t HallConfig;				/**< Number of amplifier chopping periods inside one spinning phase */
	 uint8_t Resolution;							/**< Initial resolution mode for all three axes */
	 uint8_t DigFilt;								/**< Sample count for the digital filter */
	 uint8_t Oversampling;							/**< Oversampling rate for the ADC */
 } MLX90393_Config_t;

 /** 
  * MLX90393 data point
  */
 typedef struct 
 {
	 uint16_t x;									/**< Data for x axis */
	 uint16_t y;									/**< Data for y axis */
	 uint16_t z;									/**< Data for z axis */
 } MLX90393_DataPoint_t;

 /** 
  * MLX90393 threshold settings
  */
 typedef struct 
 {
	 int8_t x;										/**< Threshold for x axis */
	 int8_t y;										/**< Threshold for y axis */
	 int16_t z;										/**< Threshold for z axis */
 } MLX90393_Threshold_t;

 /** @brief			Initialize the MLX90393 magnetic sensor and the I2C interface.
  *  @param Config	Pointer to SPI master configuration object
  *					NOTE: Set it to *NULL if you have initialized the I2C already
  *  @param Device	Pointer to MLX90393 configuration object
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_Init(I2CM_Config_t* Config, const MLX90393_Config_t* Device);

 /** @brief			Reset the device.
  *  @param	Status	Status byte
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_Reset(uint8_t* Status);

 /** @brief			Enable/Disable all plates for z-measurement.
  *  @param Enable	Enable/Disable
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SwitchZSeries(const Bool_t Enable);

 /** @brief			Enable/Disable the built in self test.
  *  @param Enable	Enable/Disable
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SwitchSelfTest(const Bool_t Enable);

 /** @brief			Set the hall config of the device.
  *  @param Config	Hall configuration
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SetHallConfig(const MLX90393_HallConf_t Config);

 /** @brief			Get the hall config of the device.
  *  @param Config	Pointer to hall configuration
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_GetHallConfig(MLX90393_HallConf_t* Config);

 /** @brief			Set the gain of the device.
  *  @param Gain	Sensor gain
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SetGain(const uint8_t Gain);

 /** @brief			Get the gain of the device.
  *  @param Gain	Sensor gain
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_GetGain(uint8_t* Gain);

 /** @brief			Set the digital filter samples of the device.
  *  @param Samples	Sample count
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SetDigFilt(const uint8_t Samples);

 /** @brief			Get the digital filter samples of the device.
  *  @param Samples	Sample count
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_GetDigFilt(uint8_t* Filter);

 /** @brief		Set the oversampling rate for the magnetic measurement.
  *  @param OSR	Oversampling rate
  *  @return	Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SetOSR(const uint8_t OSR);

 /** @brief		Get the oversampling rate for the magnetic measurement.
  *  @param OSR	Oversampling rate
  *  @return	Error code
  */
 const MLX90393_COMM_ERROR MLX90393_GetOSR(uint8_t* OSR);

 /** @brief		Set the oversampling rate for the temperature ADC.
  *  @param OSR	Oversampling rate
  *  @return	Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SetOSR2(const uint8_t OSR);

 /** @brief		Get the oversampling rate for the temperature ADC.
  *  @param OSR	Oversampling rate
  *  @return	Error code
  */
 const MLX90393_COMM_ERROR MLX90393_GetOSR2(uint8_t* OSR);

 /** @brief				Set the resolution of the device.
  *  @param Axis		Axis mask
  *  @param Resolution	Axis resolution
  *  @return			Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SetResolution(const MLX90393_Axis_t Axis, const uint8_t Resolution);

 /** @brief				Get the resolution of the device.
  *  @param Axis		Target axis
  *  @param Resolution	Axis resolution
  *  @return			Error code
  */
 const MLX90393_COMM_ERROR MLX90393_GetResolution(const MLX90393_Axis_t Axis, uint8_t* Resolution);
 
 /** @brief			Get the device mode.
  *  @param Mode	Pointer to device mode
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_GetMode(MLX90393_Mode_t* Mode);

 /** @brief			Read the measurement result and the status byte.
  *  @param Mask	Axis mask
  *  @param Bytes	Length of data (including status byte)
  *  @param Data	Pointer to measurement data and status byte
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_ReadMeasurementData(const uint8_t Mask, uint8_t Bytes, uint8_t* Data);

 /** @brief			Run a single measurement.
  *  @param	Mask	Axis mask
  *  @param Data	Pointer to data point
  *  @return		Error code
  */
 const MLX90393_COMM_ERROR MLX90393_Single(const MLX90393_Axis_t Mask, MLX90393_DataPoint_t* Data);

 /** @brief				Put the device in Wake-up on change mode.
  *  @param Mode		Wake-up on change mode
  *  @param	Mask		Axis mask
  *  @param Threshold	Sensor threshold
  *  @return			Error code
  */
 const MLX90393_COMM_ERROR MLX90393_WOC(const MLX90393_WakeMode_t Mode, const MLX90393_Axis_t Mask, const MLX90393_Threshold_t* Threshold);

 /** @brief				Configure the device for burst mode.
  *  @param DataRate	Burst mode data rate, defined as T_Interval = DataRate * 20ms
  *  @param	Mask		Axis mask
  *  @param DataReady	Pointer to interrupt configuration for data ready callback. Set to #NULL if you don´t need it
  *  @return			Error code
  */
 const MLX90393_COMM_ERROR MLX90393_Burst(const uint8_t DataRate, const MLX90393_Axis_t Mask, GPIO_InterruptConfig_t* DataReady);

 /** @brief		Exit from current measurement mode.
  *  @return	Error code
  */
 const MLX90393_COMM_ERROR MLX90393_Cancel(void);

 /** @brief				Write a new threshold into the sensor.
  *  @param	Threshold	Pointer to threshold
  *  @return			Error code
  */
 const MLX90393_COMM_ERROR MLX90393_SetThreshold(const MLX90393_Threshold_t* Threshold);






void Bla(void);

#endif /* MLX90393_H_ */