/*
 * ADXL345.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Analog Devices ADXL345 Accelerometer.

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

/** @file Peripheral/ADXL345/ADXL345.h
 *  @brief Driver for Analog Devices ADXL345 Accelerometer.
 *
 *  This file contains the prototypes and definitions for the ADXL345 driver.
 *
 *  @author Daniel Kampert
 *  @bug - Under construction
 *		 - Self test does not work
 *		 - SPI support not ready
 */

#ifndef ADXL345_H_
#define ADXL345_H_

 #include "Config_ADXL345.h"

 /** @ingroup I2C-Addresses */
 /*\@{*/
	#define ADXL345_ADDRESS					0x53					/**< ADXL345 I2C accelerometer device address */
 /*\@}*/

 /** @ingroup Device-ID */
 /*\@{*/
	 #define ADXL345_ID						0xE5					/**< ADXL345 I2C accelerometer device ID */
 /*\@}*/

 #define ADXL345_FIFO_SIZE					32						/**< Size of the FIFO in bytes */

 #if(ADXL345_INTERFACE_TYPE == USART_SPI_INTERFACE || ADXL345_INTERFACE_TYPE == SPI_INTERFACE)
	 #if(!defined ADXL345_SS_PORT || !defined ADXL345_SS_PIN)
		#warning "Invalid configuration for the ADXL345 SPI mode chip select. Use default."

		#define ADXL345_SS_PORT					&PORTE
		#define ADXL345_SS_PIN					4
	 #endif
 #endif
 
 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/ATmega/GPIO/GPIO.h"
	 
	 #if(ADXL345_INTERFACE == INTERFACE_I2C)
		#include "Arch/AVR8/ATmega/I2C/I2C.h"
		
		#define ADXL345_INIT(Config)							I2CM_Init(Config)														/**< Generic I2C initialization macro for the accelerometer */
		#define ADXL345_WRITEBYTE(Register, Data)				I2CM_WriteBytes(ADXL345_ADDRESS, Register, 1, Data)						/**< Generic I2C write byte macro for the accelerometer */
		#define ADXL345_READBYTE(Register, Data)				I2CM_ReadBytes(ADXL345_ADDRESS, Register, 1, Data)						/**< Generic I2C read bytes macro for the accelerometer */
		#define ADXL345_READBYTES(Register, Length, Data)		I2CM_ReadBytes(ADXL345_ADDRESS, Register, Length, Data)					/**< Generic I2C read bytes macro for the accelerometer */
		#define ADXL345_WRITEBYTES(Register, Length, Data)		I2CM_WriteBytes(ADXL345_ADDRESS, Register, Length, Data)				/**< Generic I2C write bytes macro for the accelerometer */
	 #elif(ADXL345_INTERFACE == INTERFACE_SPI)
		#include "Arch/AVR8/SPI/SPI.h"
	
		/*
			ToDo
		*/
	 #endif
 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/GPIO/GPIO.h"
 
 	 #if(ADXL345_INTERFACE_TYPE == INTERFACE_I2C)
 		 #include "Arch/XMega/I2C/I2C.h"
		 
		 /** @brief	TWI interface for device when using XMega architecture.
		  */
		 #ifndef ADXL345_INTERFACE
			 #define ADXL345_INTERFACE			&TWIC
			 #warning "No I2C interface specified. Use default!"
		 #endif

		 #define ADXL345_INIT(Config)						I2CM_Init(Config)																/**< Generic I2C initialization macro for the accelerometer */
		 #define ADXL345_WRITEBYTE(Register, Data)			I2CM_WriteBytes(ADXL345_INTERFACE, ADXL345_ADDRESS, Register, 1, Data)			/**< Generic I2C write byte macro for the accelerometer */
		 #define ADXL345_READBYTE(Register, Data)			I2CM_ReadBytes(ADXL345_INTERFACE, ADXL345_ADDRESS, Register, 1, Data)			/**< Generic I2C read byte macro for the accelerometer */
		 #define ADXL345_READBYTES(Register, Length, Data)	I2CM_ReadBytes(ADXL345_INTERFACE, ADXL345_ADDRESS, Register, Length, Data)		/**< Generic I2C read bytes macro for the accelerometer */
		 #define ADXL345_WRITEBYTES(Register, Length, Data)	I2CM_WriteBytes(ADXL345_INTERFACE, ADXL345_ADDRESS, Register, Length, Data)		/**< Generic I2C write bytes macro for the accelerometer */
	#elif(ADXL345_INTERFACE_TYPE == INTERFACE_SPI)
		 #include "Arch/XMega/SPI/SPI.h"
		 
		 /** @brief	TWI interface for device when using XMega architecture.
		  */
		 #ifndef ADXL345_INTERFACE
			 #define ADXL345_BUS_INTERFACE			&SPIC
			 #warning "No SPI interface specified. Use default!"
		 #endif
		 
		 /*
			 ToDo
		 */
		 #define ADXL345_INIT(Config)						SPI_Init(Config)																/**< Generic SPI initialization macro for the accelerometer */
	#elif(ADXL345_INTERFACE_TYPE == INTERFACE_USART_SPI)
		 #include "Arch/XMega/USART/USART.h"
		 
		 /** @brief	TWI interface for device when using XMega architecture.
		  */
		 #ifndef ADXL345_INTERFACE
			 #define ADXL345_BUS_INTERFACE			&USARTC0
			 #warning "No USART-SPI interface specified. Use default!"
		 #endif
		 
		 /*
			 ToDo
		 */
		 #define ADXL345_INIT(Config)						USART_SPI_Init(Config)															/**< Generic USART-SPI initialization macro for the accelerometer */
	#endif
 #else
	#error "No valid mcu"
 #endif

 #include "Common/types.h"
 
 #ifndef ADXL345_CALIBRATION_LENGTH
	 #define ADXL345_CALIBRATION_LENGTH		100				/**< Default calibration length */
 #endif

 /** 
  * ADXL345 callback types
  */
 typedef enum
 {
	 ADXL345_OVERRUN_INTERRUPT = 0x01,	 					/**< Overrun interrupt */
	 ADXL345_WATERMARK_INTERRUPT = 0x02,	 				/**< Watermark interrupt */
	 ADXL345_FREEFALL_INTERRUPT = 0x04,	 					/**< Free fall interrupt */
	 ADXL345_INACTIVITY_INTERRUPT = 0x08,	 				/**< Inactivity interrupt */
	 ADXL345_ACTIVITY_INTERRUPT = 0x10,	 					/**< Activity interrupt */
	 ADXL345_DOUBLE_TAP_INTERRUPT = 0x20,	 				/**< Double tap interrupt */
	 ADXL345_SINGLE_TAP_INTERRUPT = 0x40,	 				/**< Single tap interrupt */
	 ADXL345_DATARDY_INTERRUPT = 0x80,	 					/**< Data ready interrupt */
 } ADXL345_CallbackType_t;

 /** 
  * ADXL345 measurement axis
  */
 typedef enum
 {
	 ADXL345_AXIS_Z = 0x01,	 								/**< x-axis */
	 ADXL345_AXIS_Y = 0x02,	 								/**< y-axis */
	 ADXL345_AXIS_X = 0x04,	 								/**< z-axis */
 } ADXL345_Axis_t;

 /** 
  * ADXL345 device modes
  */
 typedef enum
 {
	ADXL345_MODE_STANDBY = 0x00,	 						/**< Standby mode */
	ADXL345_MODE_LP = 0x01,	 								/**< Low power mode */
	ADXL345_MODE_SLEEP = 0x02, 								/**< Sleep mode 
																 NOTE: This mode use 8 Hz as default reading frequency */
	ADXL345_MODE_MEASURE = 0x03, 							/**< Measurement mode */
	ADXL345_MODE_SELFTEST = 0x04, 							/**< Self test mode */
 } ADXL345_DeviceMode_t;

 /** 
  * ADXL345 reading frequencies during sleep mode
  */
 typedef enum
 {
	 ADXL_WAKE_8HZ = 0x00, 									/**< 8 Hz reading frequency */
	 ADXL_WAKE_4HZ = 0x01, 									/**< 4 Hz reading frequency */
	 ADXL_WAKE_2HZ = 0x02, 									/**< 2 Hz reading frequency */
	 ADXL_WAKE_1HZ = 0x03, 									/**< 1 Hz reading frequency */
 } ADXL345_WakeUp_t;

 /** 
  * ADXL345 FIFO modes
  */
 typedef enum
 {
	 ADXL345_FIFO_BYPASS = 0x00, 							/**< FIFO disabled */
	 ADXL345_FIFO_FIFO = 0x01, 								/**< FIFO collects new data unless he is full */
	 ADXL345_FIFO_STREAM = 0x02, 							/**< FIFO collects new data and overwrite the old if it is full */
	 ADXL345_FIFO_TRIGGER = 0x03, 							/**< FIFO collects data when triggered and collect until it is full */
 } ADXL345_FIFOMode_t;
 
 /** 
  * ADXL345 measurement alignment
  */
 typedef enum
 {
	 ADXL345_ALIGN_RIGHT = 0x00,	 						/**< Result alignment right */
	 ADXL345_ALIGN_LEFT = 0x01,	 							/**< Result alignment left */
 } ADXL345_Alignment_t;

 /** 
  * ADXL345 measurement range
  */
 typedef enum
 {
	 ADXL345_RANGE_2G = 0x00,	 							/**< Measurement range 2g */
	 ADXL345_RANGE_4G = 0x01,	 							/**< Measurement range 4g */
	 ADXL345_RANGE_8G = 0x02,	 							/**< Measurement range 8g */
	 ADXL345_RANGE_16G = 0x03,	 							/**< Measurement range 16g */
 } ADXL345_Range_t;

 /** 
  * ADXL345 data rate settings
  */
 typedef enum
 {
	ADXL345_RATE_0100HZ = 0x00,	 							/**< Data rate 0.1 Hz */
	ADXL345_RATE_0200HZ = 0x01,	 							/**< Data rate 0.2 Hz */
	ADXL345_RATE_0390HZ = 0x02,	 							/**< Data rate 0.39 Hz */
	ADXL345_RATE_0780HZ = 0x03,	 							/**< Data rate 0.78 Hz */
	ADXL345_RATE_1HZ56 = 0x04,	 							/**< Data rate 1.56 Hz */
	ADXL345_RATE_3HZ13 = 0x05,	 							/**< Data rate 3.13 Hz */
	ADXL345_RATE_6HZ25 = 0x06,	 							/**< Data rate 6.25 Hz */
	ADXL345_RATE_12HZ5 = 0x07,		 						/**< Data rate 12.5 Hz */
	ADXL345_RATE_25HZ = 0x08,	 							/**< Data rate 25 Hz */
	ADXL345_RATE_50HZ = 0x09,	 							/**< Data rate 50 Hz */
	ADXL345_RATE_100HZ = 0x0A,	 							/**< Data rate 100 Hz */
	ADXL345_RATE_200HZ = 0x0B,	 							/**< Data rate 200 Hz */
	ADXL345_RATE_400HZ = 0x0C,	 							/**< Data rate 400 Hz */
	ADXL345_RATE_800HZ = 0x0D,	 							/**< Data rate 800 Hz */
	ADXL345_RATE_1600HZ = 0x0E,	 							/**< Data rate 1600 Hz */
	ADXL345_RATE_3200HZ = 0x0F,		 						/**< Data rate 3200 Hz */
 } ADXL345_DataRate_t;

 /** 
  * ADXL345 interrupt channel
  */
 typedef enum
 {
	 ADXL345_INTERRUPT_1 = 0x00,	 						/**< Interrupt channel 1 */
	 ADXL345_INTERRUPT_2 = 0x01,	 						/**< Interrupt channel 2 */
 } ADXL345_InterruptChannel_t;

 /** 
  * ADXL345 interrupt priority
  */
 typedef enum
 {
	 ADXL345_POL_HIGH = 0x00,	 							/**< Interrupt polarity high */
	 ADXL345_POL_LOW = 0x01,	 							/**< Interrupt polarity low */
 } ADXL345_InterruptPolarity_t;

 /** 
  * ADXL345 supply voltages
  */
 typedef enum
 {
	ADXL345_SUPPLY_2V = 0x00,								/**< 2 V supply voltage */
	ADXL345_SUPPLY_2V5 = 0x01,								/**< 2.5 V supply voltage */
	ADXL345_SUPPLY_3V3 = 0x02,								/**< 3.3 V supply voltage */
	ADXL345_SUPPLY_3V6 = 0x03,								/**< 3.6 V supply voltage */
 } ADXL345_Supply_t;

 /** 
  * ADXL345 data point object
  */
 typedef struct
 {
	 int16_t X;	 											/**< Data for X axis */
	 int16_t Y;	 											/**< Data for Y axis */
	 int16_t Z;	 											/**< Data for Z axis */
 } ADXL345_DataPoint_t;

 /** @brief	ADXL345 FIFO configuration object
  */
 typedef struct  
 {
	 ADXL345_FIFOMode_t Mode;	 							/**< FIFO mode */
	 ADXL345_InterruptChannel_t Trigger;	 				/**< Trigger event channel.
																  NOTE: This is only needed when *Mode = ADXL345_FIFO_TRIGGER */
	 uint8_t Samples;	 									/**< Samples needed for specific event.
																 NOTE: Function depends on the mode settings! */
 } ADXL345_FIFOConfig_t;

 /** @brief	ADXL345 callbacks object
  */
 typedef struct
 {
	 void (*DataRdyCallback)();	 							/**< Data ready callback */
	 void (*SingleTapCallback)();	 						/**< Single tap callback */
	 void (*DoubleTapCallback)();	 						/**< Double tap callback */
	 void (*ActivityCallback)();	 						/**< Activity callback */
	 void (*InactivityCallback)();	 						/**< Inactivity callback */
	 void (*FreeFallCallback)();	 						/**< Free fall callback */
	 void (*WatermarkCallback)();	 						/**< Watermark callback */
	 void (*OverrunCallback)();	 							/**< Overrun callback */
 } ADXL345_Callbacks_t;

 /**
  * ADXL345 interrupt configuration object
  */
 typedef struct
 {
	 #if(MCU_ARCH == MCU_ARCH_AVR8)
		volatile uint8_t* Port;								/**< Pointer to port object */
	 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
		PORT_t* Port;										/**< Pointer to port object */
		Interrupt_Level_t InterruptLevel;					/**< Interrupt level */
	 #endif
	 
	 uint8_t Pin;
	 
	 GPIO_InterruptChannel_t IOChannel;						/**< GPIO Interrupt channel */
	 ADXL345_InterruptChannel_t DeviceChannel;				/**< Device Interrupt channel */
	 ADXL345_InterruptPolarity_t Sense;						/**< ADXL345 interrupt polarity */
	 ADXL345_CallbackType_t Type;							/**< ADXL345 interrupt type */
	 ADXL345_Callbacks_t* Callbacks;						/**< ADXL345 callbacks object */
 } ADXL345_InterruptConfig_t;

 /** 
  * ADXL345 device configuration object
  */
 typedef struct
 {
	ADXL345_Range_t Range;									/**< Measurement range */
	ADXL345_DataRate_t DataRate;							/**< Data rate */
	ADXL345_Supply_t SupplyVoltage;							/**< Supply voltage for the device
																 NOTE: This is needed for a successfully self test */
	Bool_t FullResolution;									/**< Set to #TRUE to enable full resolution measurement mode */
	ADXL345_FIFOConfig_t* FIFO;								/**< Pointer to FIFO configuration
																 NOTE: Set it to *NULL if you don´t want to use the FIFO */
 } ADXL345_Configuration_t;									

 /** @brief			Initialize the ADXL345 accelerometer and the I2C interface.
  *  @param Config	Pointer to I2C master configuration object
  *					NOTE: Set it to *NULL if you have initialized the I2C already
  *  @param Device	Pointer to AD5933 configuration object
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_Init(I2CM_Config_t* Config, const ADXL345_Configuration_t* Device);

 /** @brief				Read the ID from the device.
  *  @param DeviceID	Pointer to device ID
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_ReadDevID(uint8_t* DeviceID);

 /** @brief		Clear all interrupt flags of the device.
  *  @return	I2C error code
  */
 const I2C_Error_t ADXL345_ClearAllInterrupts(void);

 /** @brief			Enable/Disable full resolution mode.
  *  @param Enable	Enable/Disable
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_SwitchFullResolution(const Bool_t Enable);

 /** @brief			Get the resolution mode of the device.
  *  @param Enable	#TRUE if full resolution mode is active
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_IsFullResolution(Bool_t* Enable);

 /** @brief			Set the measurement range of the device.
  *  @param Range	Measurement range
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_SetMeasurementRange(const ADXL345_Range_t Range);
 
 /** @brief			Get the measurement range of the device.
  *  @param Range	Pointer to measurement range
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_GetMeasurementRange(ADXL345_Range_t* Range);

 /** @brief			Set the reading frequency for the sleep mode.
  *  @param Freq	Reading frequency
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_SetSleepReadingFreq(const ADXL345_WakeUp_t Freq);

 /** @brief			Get the reading frequency for the sleep mode.
  *  @param Freq	Pointer to reading frequency
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_GetSleepReadingFreq(ADXL345_WakeUp_t* Freq);

 /** @brief				Set the result alignment of the device.
  *  @param Alignment	Result alignment
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_SetAlignment(const ADXL345_Alignment_t Alignment);

 /** @brief				Get the result alignment of the device.
  *  @param Alignment	Result alignment
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_GetAlignment(ADXL345_Alignment_t* Alignment);

 /** @brief			Set the data rate of the device.
  *  @param Rate	Data rate
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_SetDataRate(const ADXL345_DataRate_t Rate);

 /** @brief			Get the data rate of the device.
  *  @param Rate	Data rate
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_GetDataRate(ADXL345_DataRate_t* Rate);

 /** @brief			Set the device mode of the ADXL345.
  *  @param Mode	Device mode
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_SetDeviceMode(const ADXL345_DeviceMode_t Mode);

 /** @brief			Get the data from all three sensors.
  *  @param Data	Pointer to data structure
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_GetData(ADXL345_DataPoint_t* Data);

 /** @brief			Wait for new data and get the data from all three sensors.
  *  @param Data	Pointer to data structure
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_WaitForData(ADXL345_DataPoint_t* Data);

 /** @brief		Get resolution of the sensor in mg / Bit.
  *  @return	Resolution of the sensor
  */
 const float ADXL345_GetResolution(void);

 /** @brief			Enable the auto sleep function of the device.
  *  @param	Thresh	Auto sleep threshold
  *  @param Time	Time before entering auto sleep (1 s / LSB)
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_EnableAutoSleep(const uint8_t Thresh, const uint8_t Time);

 /** @brief		Disable the auto sleep function of the device.
  *  @return	I2C error code
  */
 const I2C_Error_t ADXL345_DisableAutoSleep(void);

 /** @brief			Check if the device is sleeping.
  *  @param	Sleep	Pointer to data
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_IsSleeping(Bool_t* Sleep);

 /** @brief			Configure the device for FIFO mode.
  *  @param Config	Pointer to ADXL345 FIFO configuration object
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_FIFOConfig(const ADXL345_FIFOConfig_t* Config);
 
 /** @brief			Disable the FIFO mode of the device.
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_FIFODisable(void);
 
 /** @brief		Trigger the FIFO.
  *  @return	I2C error code
  */
 const I2C_Error_t ADXL345_FIFOTrigger(void);

 /** @brief			Get the number of entries in the FIFO.
  *  @param Entries	Pointer to data
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_FIFOGetEntries(uint8_t* Entries);
 
 /** @brief			Clear the FIFO and read all data.
  *  @param Data	Pointer to data array
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_FIFOClear(ADXL345_DataPoint_t* Data);

 /** @brief				Enable the given tap axes.
  *  @param Axes		Tap axes
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_TapEnableAxes(const ADXL345_Axis_t Axes);

 /** @brief				Disable the given tap axes.
  *  @param Axes		Tap axes
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_TapDisableAxes(const ADXL345_Axis_t Axes);

 /** @brief				Configure the device for a single tap event.
						NOTE: You have to install the callback separately!
  *  @param Value		Tap value (62,5 mg / LSB)
  *  @param Duration	Tap duration (625 us / LSB)
  *  @param Axes		Tap axes
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_SingleTapConfig(const uint8_t Value, const uint8_t Duration, const ADXL345_Axis_t Axes);

 /** @brief				Configure the device for a double tap event.
						NOTE: You have to install the callback separately!
  *  @param Value		Tap value (62,5 mg / LSB)
  *  @param Duration	Tap duration (625 us / LSB)
  *	 @param Delay		Delay between the first and the second tap
  *  @param Window		Time window for the two tap events
  *  @param Axes		Tap axes
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_DoubleTapConfig(const uint8_t Value, const uint8_t Duration, const uint8_t Delay, const uint8_t Window, const ADXL345_Axis_t Axes);

 /** @brief			Supress double tap detection.
  *  @param Supress	Enable/Disable the suppression of double tap events
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_TapSuppress(const Bool_t Supress);

 /** @brief			Get the first axis involved in a tap event.
  *  @param Axis	Pointer to axis data
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_TapGetAxis(ADXL345_Axis_t* Axis);

  /** @brief			Configure the device for an activity event.
						NOTE: You have to install the callback separately!
  *  @param Value		Activity limit
  *  @param Axes		Activity axes
  *  @param AC			Set to #TRUE to enable AC mode
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_ActivityConfig(const uint8_t Value, const ADXL345_Axis_t Axes, const Bool_t AC);

 /** @brief			Get the first axis involved in an activity event.
  *  @param Axis	Pointer to axis data
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_ActivityGetAxis(ADXL345_Axis_t* Axis);

 /** @brief				Configure the device for an inactivity event.
						NOTE: You have to install the callback separately!
  *  @param Value		Activity limit
  *  @param Time		Inactivity time (1 s / LSB)
  *  @param Axes		Activity axes
  *  @param AC			Set to #TRUE to enable AC mode
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_InactivityConfig(const uint8_t Value, const uint8_t Time, const ADXL345_Axis_t Axes, const Bool_t AC);
 
 /** @brief				Configure the device for an free fall event.
						NOTE: You have to install the callback separately!
  *  @param Thresh		Threshold value for all three axes
  *  @param Time		Free fall time (1 s / LSB)
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_FreeFallConfig(const uint8_t Thresh, const uint8_t Time);
 
 /** @brief			Run a device self test.
  *	 @param Passed	Pointer to data structure for self test result. #TRUE when successfull
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_RunSelfTest(Bool_t* Passed);

 /** @brief			Run a offset calibration of the device.
  *	 @param Passed	Pointer to data structure for self test result. #TRUE when successfull
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_RunOffsetCalibration(Bool_t* Passed);

 /** @brief			Enable interrupt support for the ADXL345.
  *  @param Config	Pointer to ADXL345 interrupt configuration object
  *  @return		I2C error code
  */
 const I2C_Error_t ADXL345_EnableInterrupts(const ADXL345_InterruptConfig_t* Config);

 /** @brief		Disable the interrupt support for the ADXL345.
  */
 const I2C_Error_t ADXL345_DisableInterrupts(const ADXL345_InterruptConfig_t* Config);

 /** @brief			Install a new ADXL345 device callback.
  *					NOTE: You have to enable the interrupts first!
  *  @param Config	Pointer to interrupt configuration structure.
  *  @return	I2C error code
  */
 const I2C_Error_t ADXL345_InstallCallback(const ADXL345_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Callback	Callback which should be removed
  *  @return			I2C error code
  */
 const I2C_Error_t ADXL345_RemoveCallback(const ADXL345_InterruptConfig_t* Callback);

 #if(MCU_ARCH == MCU_ARCH_XMEGA)
  /** @brief		Change the interrupt level for the ADXL345 interrupts.
   *  @param Config	Pointer to interrupt configuration object
   */
	void ADXL345C_ChangeInterruptLevel(const ADXL345_InterruptConfig_t* Config);
 #endif

#endif /* ADXL345_H_ */