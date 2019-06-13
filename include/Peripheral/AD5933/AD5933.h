/*
 * AD5933.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the AD5933 impedance converter.

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

/** @file Peripheral/AD5933/AD5933.h
 *  @brief Driver for the Analog Devices AD5933 impedance converter.
 *
 *  This file contains the prototypes and definitions for the AD5933 impedance converter driver.
 *
 *  @author Daniel Kampert
 *  @bug -AVR8 support for AD5933_BlockRead and AD5933_BlockWrite function not implemented
 */

#ifndef AD5933_H_
#define AD5933_H_

 #include "Config_AD5933.h"
 
 #include "Arch/I2C.h"

 /** 
  * AD5933 clock frequency
  */
 #define AD5933_MCLK						16.000f

 /** @ingroup I2C-Addresses */
 /*\@{*/
	 #define AD5933_ADDRESS					0x0D																							/**< AD5933 I2C impedance converter device address */
 /*\@}*/
 
 #include "Common/types.h"

 /** 
  * AD5933 output voltages
  */
 typedef enum
 {
	AD5933_OUTPUT_2V	= 0x00,							/**< 2 V p-p */
	AD5933_OUTPUT_200MV = 0x01,							/**< 200 mV p-p */
	AD5933_OUTPUT_400MV = 0x02,							/**< 400 mV p-p */
	AD5933_OUTPUT_1V	= 0x03, 						/**< 1 V p-p */
 } AD5933_OutputVoltage_t;
 
 /** 
  * AD5933 input gain
  */
 typedef enum
 {
	AD5933_GAIN_5 = 0x00,								/**< Gain x5 */
	AD5933_GAIN_1 = 0x01,								/**< Gain x1 */
 } AD5933_Gain_t;

 /** 
  * AD5933 device modes
  */
 typedef enum
 {
	AD5933_MODE_NO_OPERATION	= 0x00,					/**< No operation */
	AD5933_MODE_INIT_FREQ		= 0x01,					/**< Initialize frequency sweep */
	AD5933_MODE_START_FREQ		= 0x02,					/**< Start frequency sweep */
	AD5933_MODE_INCR_FREQ		= 0x03,					/**< Increase frequency */
	AD5933_MODE_REPEAT_FREQ		= 0x04,					/**< Repeat */
	AD5933_MODE_TEMP			= 0x09,					/**< Measure temperature */
	AD5933_MODE_POWER_DOWN		= 0x0A,					/**< Power down */
	AD5933_MODE_STANDBY			= 0x0B,					/**< Standby */
 } AD5933_Mode_t;

 /** 
  * AD5933 clock sources
  */
 typedef enum
 {
	AD5933_CLOCK_INT = 0x00,							/**< Internal oscillator */
	AD5933_CLOCK_EXT = 0x01,							/**< External oscillator */
 } AD5933_Clock_t;

 /** 
  * AD5933 settling time multiplier
  */
 typedef enum
 {
	AD5933_SETTLING_X1 = 0x00,							/**< Settling time x1 */
	AD5933_SETTLING_X2 = 0x01,							/**< Settling time x2 */
	AD5933_SETTLING_X4 = 0x03,							/**< Settling time x4 */
 } AD5933_SettlingMultiplier_t;

 /** 
  * AD5933 calibration point definition
  */
 typedef struct 
 {
	 float GainFactor;									/**< Value for the gain factor */
	 float Phase;										/**< Value for the phase in radians */
 } AD5933_CalPoint_t;

 /** 
  * AD5933 data point definition
  */
 typedef struct 
 {
	 float Impedance;									/**< Value for the magnitude */
	 float Phase;										/**< Value for the phase in radians */
 } AD5933_DataPoint_t;

 /** 
  * AD5933 frequency sweep configuration object
  */
 typedef struct
 {
	 uint32_t StartFrequency;							/**< Sweep start frequency */
	 uint32_t FrequencyIncrement;						/**< Frequency step size */
	 uint16_t Cycles;	 								/**< Frequency step count */
	 uint16_t SettlingCycles;	 						/**< Settling cycles */
	 AD5933_SettlingMultiplier_t Mult;	 				/**< Settling multiplier */
 } AD5933_SweepSetup_t;

 /** 
  * AD5933 single frequency configuration object
  */
 typedef struct
 {
	 uint32_t Frequency;								/**< Output frequency */
	 uint16_t SettlingCycles;	 						/**< Settling cycles */
	 AD5933_SettlingMultiplier_t Mult;	 				/**< Settling multiplier */
 } AD5933_SingleFrequency_t;

 /** 
  * AD5933 device configuration object
  */
 typedef struct
 {
	AD5933_Clock_t ClockSource;							/**< Device clock source */
	AD5933_OutputVoltage_t OutputVoltage;				/**< Output voltage */
	AD5933_Gain_t Gain;									/**< Input gain */
 } AD5933_Configuration_t;

 /** @brief			Initialize the AD5933 impedance controller and the I2C interface.
  *  @param Config	Pointer to I2C master configuration object
  *					NOTE: Set it to *NULL if you have initialized the I2C already
  *  @param Device	Pointer to AD5933 configuration object
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_Init(I2CM_Config_t* Config, const AD5933_Configuration_t* Device);
 
 /** @brief		Reset the device and stop an active frequency sweep.
  *  @return	I2C error code
  */
 const I2C_Error_t AD5933_Reset(void);
 
 /** @brief			Set the device mode of the impedance converter.
  *  @param Mode	Device mode
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_SetMode(const AD5933_Mode_t Mode);

 /** @brief			Read the device mode of the impedance converter.
  *  @param Mode	Device mode
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_GetMode(AD5933_Mode_t* Mode);

 /** @brief			Set the clock source of the impedance converter.
  *  @param Clock	Clock source
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_SetClockSource(const AD5933_Clock_t Clock);

 /** @brief			Read the device status.
  *  @param Status	Device status
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_ReadStatus(uint8_t* Status);

 /** @brief			Set the output voltage range of the device.
  *  @param Voltage	Output voltage
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_SetVoltage(const AD5933_OutputVoltage_t Voltage);
 
 /** @brief			Get the output voltage range of the device.
  *  @param Voltage	Output voltage
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_GetVoltage(AD5933_OutputVoltage_t* Voltage);
 
 /** @brief			Set the input gain of the device.
  *  @param Gain	Input gain
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_SetGain(const AD5933_Gain_t Gain);
 
 /** @brief			Get the input gain of the device.
  *  @param Gain	Input gain
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_GetGain(AD5933_Gain_t* Gain);

 /** @brief			Configure the settling time.
  *  @param Cycles	Number of cycles
  *  @param Mult	Settling time multiplier
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_ConfigSettling(const uint16_t Cylcles, const AD5933_SettlingMultiplier_t Mult);

 /** @brief				Set the start frequency.
  *  @param Frequency	Start frequency.
  *  @return			I2C error code
  */
 const I2C_Error_t AD5933_SetStartFrequency(const uint32_t Frequency);
 
 /** @brief				Set the step size of a frequency increment.
  *  @param Increment	Frequency increment with each step
  *  @return			I2C error code
  */
 const I2C_Error_t AD5933_SetFrequencyIncrement(const uint32_t Increment);

 /** @brief				Set the number of increments.
  *  @param Increments	Number of frequency increments.
  *  @return			I2C error code
  */
 const I2C_Error_t AD5933_SetIncrements(const uint16_t Increments);

 /** @brief			Get the conversion data.
  *  @param Data	Pointer to complex number
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_GetData(ComplexNumber_t* Data);

 /** @brief			Run a temperature measurement.
  *  @param Temp	Temperature
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_ReadTemperature(int16_t* Temp);

 /** @brief			Configure the device.
  *  @param Config	Pointer to measurement configuration object
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_ConfigDevice(const AD5933_Configuration_t* Config);

 /** @brief			Configure a single output frequency .
  *  @param Config	Pointer to sweep configuration object
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_ConfigSingleFrequency(const AD5933_SingleFrequency_t* Config);

 /** @brief			Configure a new frequency sweep.
  *  @param Config	Pointer to sweep configuration object
  *  @return		I2C error code
  */
 const I2C_Error_t AD5933_ConfigSweep(const AD5933_SweepSetup_t* SweepSetup);

 /** @brief			Set the angle format to radians or degree.
  *  @param Degree	#TRUE to set the angle format to degree.
  */
 void AD5933_EnableDegree(const Bool_t Degree);

 /** @brief						Calibrate the impedance converter with a single frequency.
								The calibration error is +-0,5% in with a small range at the calibration point (~ +-3k ohms).
  *  @param	CalibrationPoint	Pointer to calibration data
  *  @param Reference			Calibration reference
  *  @return					I2C error code
  */
 const I2C_Error_t AD5933_SingleCalibration(AD5933_CalPoint_t* CalibrationPoint, const float Reference);
 
 /** @brief						Run a measurement with a single frequency.
								NOTE: You have to calibrate the device first!
  *  @param	CalibrationPoint	Pointer to calibration data
  *  @param Impedance			Pointer to impedance data
  *  @return					I2C error code
  */
 const I2C_Error_t AD5933_SingleMeasurement(const AD5933_CalPoint_t* CalibrationPoint, AD5933_DataPoint_t* Impedance);

 /** @brief						Run a calibration with the programmed frequency sweep.
  *  @param	CalibrationPoint	Pointer to calibration data array
  *  @param Reference			Calibration reference
  *  @return					I2C error code
  */
 const I2C_Error_t AD5933_SweepCalibration(AD5933_CalPoint_t* CalibrationPoint, const float Reference);
 
 /** @brief						Run a measurement with the programmed frequency sweep.
								NOTE: You have to calibrate the device first!
  *  @param	CalibrationPoint	Pointer to calibration data array
  *  @param Impedance			Pointer to impedance data array
  *  @return					I2C error code
  */
 const I2C_Error_t AD5933_SweepMeasurment(const AD5933_CalPoint_t* CalibrationPoint, AD5933_DataPoint_t* Impedance);
 
 /** @brief					Run a measurement with the programmed frequency sweep by using a two point calibration.
  *  @param	dG				Gain delta between two frequency points
  *  @param dF				Frequency delta between two frequency points
  *  @param dP				Phase delta between two frequency points
  *  @param Increment		Frequency increment
  *  @param GainOffset		Gain offset
  *  @param PhaseOffset		Phase offset
  *  @param Impedance		Pointer to impedance data array
  *  @return				I2C error code
  */
 const I2C_Error_t AD5933_SweepMeasurment2PC(const float dG, const float dF, const float dP, const uint32_t Increment, const float GainOffset, const float PhaseOffset, AD5933_DataPoint_t* Impedance);

 /** @brief		Enable the output with the programmed start frequency.
  *  @return	I2C error code
  */
 const I2C_Error_t AD5933_EnableFrequency(void);

#endif /* AD5933_H_ */