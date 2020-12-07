/*
 * MCP2515.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for MCP2515 SPI CAN controller.

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

/** @file Peripheral/MCP2515/MCP2515.h
 *  @brief	Driver for Microchip MCP2515 SPI CAN controller.
 *			NOTE: This driver needs interrupt support!
 *
 *  This file contains the prototypes and definitions for the Microchip MCP2515 CAN controller driver.
 *
 *  @bug	Error handling is missing
 *  @author Daniel Kampert
 */

#ifndef MCP2515_H_
#define MCP2515_H_

 #include "Common/Common.h"

 #if(!defined MCP2515_INT)
	 #warning "Invalid configuration for the MCP2515 interrupt. Use default."

	 #define MCP2515_INT			PORTD, 2
 #endif

 #if(!defined MCP2515_INTERFACE_TYPE && (MCU_ARCH == MCU_ARCH_XMEGA))
	 #define MCP2515_INTERFACE_TYPE		INTERFACE_SPI
 #endif

 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #if(MCU_NAME == MCU_NAME_ATMEGA32)
		 #include "Arch/AVR8/megaAVR/mega32/GPIO/GPIO.h"
		 #include "Arch/AVR8/megaAVR/mega32/SPI/SPI.h"
	 #else
		 #error "Invalid CPU for MCP2515!"
	 #endif

 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/GPIO/GPIO.h"
	 #if(MCP2515_INTERFACE_TYPE == INTERFACE_USART_SPI)
		 #include "Arch/XMega/USART/USART.h"
	 #elif(MCP2515_INTERFACE_TYPE == INTERFACE_SPI)
		 #include "Arch/XMega/SPI/SPI.h"
	 #else
		 #error "Interface not supported for MCP2515!"
	 #endif
 #else
	 #error "Architecture not supported for MCP2515!"
 #endif

 #define MCP2515_MAX_DATABYTES		8					/**< Max. data bytes per message */

 /** @brief MCP2515 error codes.
  */
 typedef enum
 {
	MCP2515_NO_ERROR = 0,								/**< No error */ 
	MCP2515_INVALID_IDENTIFIER = -1,					/**< Invalid identifier */
	MCP2515_TX_BUFFER_FULL = -2,						/**< Transmit buffer full */
 } MCP2515_ErrorCode_t;

 /** @brief MCP2515 clock out prescaler.
  */
 typedef enum
 {
	 MCP2515_CLOCKOUT_1 = 0x00,							/**< Prescaler 1 */
	 MCP2515_CLOCKOUT_2 = 0x01,							/**< Prescaler 2 */
	 MCP2515_CLOCKOUT_4 = 0x02,							/**< Prescaler 4 */
	 MCP2515_CLOCKOUT_8 = 0x03,							/**< Prescaler 8 */
 } MCP2515_ClockOutPrescaler_t;

 /** @brief MCP2515 synchronization jump width length.
  */
 typedef enum
 {
	 MCP2515_SJW_1 = 0x00,								/**< 1 x Tq */
	 MCP2515_SJW_2 = 0x01,								/**< 2 x Tq */
	 MCP2515_SJW_3 = 0x02,								/**< 3 x Tq */
	 MCP2515_SJW_4 = 0x03,								/**< 4 x Tq */
 } MCP2515_SJW_t;

 /** @brief MCP2515 device modes.
  */
 typedef enum
 {
	MCP2515_NORMAL_MODE = 0x00,							/**< Normal mode */
	MCP2515_SLEEP_MODE = 0x01,							/**< Sleep mode */
	MCP2515_LOOPBACK_MODE = 0x02,						/**< Loop back mode */
	MCP2515_LISTEN_ONLY_MODE = 0x03,					/**< Listen only mode */
	MCP2515_CONFIG_MODE = 0x04,							/**< Configuration mode */
 } MCP2515_DeviceMode_t;

 /** @brief MCP2515 message priorities.
  */
 typedef enum
 {
	MCP2515_PRIO_LOWEST		= 0x00,						/**< Lowest priority */
	MCP2515_PRIO_LOW		= 0x01,						/**< Low priority */
	MCP2515_PRIO_HIGH		= 0x02,						/**< High priority */
	MCP2515_PRIO_HIGHEST	= 0x03,						/**< Highest priority */
 } MCP2515_MessagePriority_t;

 /** @brief MCP2515 filter configuration.
  */
 typedef enum
 {
	MCP2515_FILTER_RECEIVE_ALL = 0x00,					/**< Receive all */
	MCP2515_FILTER_RECEIVE_STD = 0x01,					/**< Receive only standard messages */
	MCP2515_FILTER_RECEIVE_EXT = 0x02,					/**< Receive only extended messages */
	MCP2515_FILTER_OFF = 0x03,							/**< Filter off */
 } MCP2515_FilterRule_t;

 /** @brief MCP2515 transmit buffer.
  */
 typedef enum
 {
	MCP2515_TX0 = 0x01,									/**< Transmit buffer 0 */
	MCP2515_TX1 = 0x02,									/**< Transmit buffer 1 */
	MCP2515_TX2 = 0x04,									/**< Transmit buffer 2 */
 } MCP2515_TransmitBuffer_t;

 /** @brief MCP2515 receive buffer.
  */
 typedef enum
 {
	 MCP2515_RX0 = 0x00,								/**< Receive buffer 0 */
	 MCP2515_RX1 = 0x01,								/**< Receive buffer 1 */
 } MCP2515_ReceiveBuffer_t;

 /** @brief MCP2515 acceptance filter.
  */
 typedef enum
 {
	MCP2515_RXF0 = 0x00,								/**< Receive filter 0 (used by RXB0) */
	MCP2515_RXF1 = 0x01,								/**< Receive filter 1 (used by RXB0) */
	MCP2515_RXF2 = 0x02,								/**< Receive filter 2 (used by RXB1) */
	MCP2515_RXF3 = 0x03,								/**< Receive filter 3 (used by RXB1) */
	MCP2515_RXF4 = 0x04,								/**< Receive filter 4 (used by RXB1) */
	MCP2515_RXF5 = 0x05,								/**< Receive filter 5 (used by RXB1) */
 } MCP2515_Filter_t;

 /** @brief MCP2515 message types.
  */
 typedef enum
 {
	MCP2515_STANDARD_DATA	= 0x00,						/**< Standard data frame */
	MCP2515_STANDARD_REMOTE = 0x01,						/**< Standard remote frame */
	MCP2515_EXTENDED_DATA	= 0x02,						/**< Extended data frame */
	MCP2515_EXTENDED_REMOTE = 0x03,						/**< Extended remote frame */
 } MCP2515_MessageType_t;

 /** @brief MCP2515 callback types.
  */
 typedef enum
 {
	 MCP2515_RX_INTERRUPT = 0x01,						/**< MCP2515 receive message interrupt */
	 MCP2515_TX_INTERRUPT = 0x02,						/**< MCP2515 transmit message interrupt */
	 MCP2515_ERROR_INTERRUPT = 0x04,					/**< MCP2515 error interrupt */
	 MCP2515_WAKE_INTERRUPT = 0x08,						/**< MCP2515 wakeup interrupt */ 
 } MCP2515_CallbackType_t;

 /** @brief MCP2515 message object.
  */
 typedef struct
 {
	 MCP2515_MessageType_t Type;						/**< Message type */
	 uint32_t ID;										/**< Message ID */
	 uint8_t Length;									/**< Message length \n
															 NOTE: #Length is only needed when the message
															 is configured as #MCP2515_STANDARD_DATA or
															 #MCP2515_EXTENDED_DATA message! */
	 uint8_t* pData;									/**< Pointer to message data */
 } CAN_Message_t;

 /** @brief	MCP2515 interrupt handler
 */
 typedef void (*MCP2515_Callback_t)(void);
 
 /** @brief			MCP2515 error handler
  *  @param Error	Error bits
 */
 typedef void (*MCP2515_ErrorCallback_t)(uint8_t Error);

 /** 
  * MCP2515 timing configuration object
  */
 typedef struct
 {
	 MCP2515_SJW_t SJW;									/**< Synchronization jump width length */
	 uint8_t Prescaler;									/**< Baud rate prescaler */
	 bool EnableTrippeSample;							/**< Set to #true to sample the bus line three times at the same point */
	 bool PS2UseCNF3;									/**< Set to #true to use #PHSEG2 for the length of PS2. Otherwise use PS1 and IPT */
	 uint8_t PHSEG1;									/**< PS1 Length */
	 uint8_t PHSEG2;									/**< PS2 Length */
	 uint8_t PRSEG;										/**< Propagation Segment Length */
 } MCP2515_TimingConfig_t;

 /** 
  * MCP2515 filter configuration object
  */
 typedef struct
 {
	MCP2515_Filter_t Filter;							/**< Target filter */
	uint16_t Mask;										/**< Filter mask */
	uint16_t ID;										/**< Filter ID \n
															  NOTE: Set it to zero when you only want to set the mask. */
	bool UseExtended;									/**< Set this to #true if you want to use extended frames */ 
 } MCP2515_FilterConfig_t;

 /**
  * MCP2515 configuration object
  */
 typedef struct
 {
	 PORT_t* Port;										/**< MCP2515 interrupt port */
	 uint8_t Pin;										/**< MCP2515 interrupt pin */
	 GPIO_InterruptChannel_t Channel;					/**< MCP2515 interrupt channel */
	 MCP2515_CallbackType_t Source;						/**< MCP2515 interrupt type */
	 
     #if(MCU_ARCH == MCU_ARCH_XMEGA)
		Interrupt_Level_t InterruptLevel;				/**< Interrupt level */
	 #endif

	 MCP2515_TimingConfig_t* pTiming;					/**< Pointer to optional CAN timing configuration \n
															 NOTE: Set to #NULL when you don´t want to use it. */
	 bool EnableOneShot;								/**< Set to #true to enable one shot mode */
	 bool EnableLoopBack;								/**< Set to #true to enable loop back mode */
	 bool EnableRollover;								/**< Set to #true to enable rollover mode for receive buffer 0 */
	 bool EnableWakeUpFilter;							/**< Set to #true to enable the Wake-up filter */
	 MCP2515_ErrorCallback_t ErrorCallback;				/**< Function pointer to MCP2515 error callback \n
															 NOTE: Set it to #NULL if you do not want to use it. */
	 MCP2515_Callback_t RxCallback;						/**< Function pointer to MCP2515 message received callback \n
															 NOTE: Set it to #NULL if you do not want to use it. */
	 MCP2515_Callback_t TxCallback;						/**< Function pointer to MCP2515 message transmit callback \n
															 NOTE: Set it to #NULL if you do not want to use it. */
	 MCP2515_Callback_t WakeCallback;					/**< Function pointer to MCP2515 wake callback \n
															 NOTE: Set it to #NULL if you do not want to use it. */
 } MCP2515_Config_t;

 /** @brief					Initialize the MCP2515 CAN controller.
  *  @param Config			Pointer to SPI configuration object
  *							NOTE: Set it to #NULL if you have initialized the SPI already.
  *  @param DeviceConfig	Pointer to MCP2515 configuration object
  */
 void MCP2515_Init(SPIM_Config_t* Config, const MCP2515_Config_t* DeviceConfig);

 /** @brief					Configure the bit timing.
  *  @param Config			Pointer to bit timing configuration object
  */
 void MCP2515_ConfigureTiming(const MCP2515_TimingConfig_t* Config);
 
 /** @brief	Reset the MCP2515 controller.
  */
 void MCP2515_Reset(void);

 /** @brief				Enable the clock output pin.
  *  @param Prescaler	Clock prescaler
  */
 void MPC2515_EnableClockOut(const MCP2515_ClockOutPrescaler_t Prescaler);

 /** @brief	Disable the clock output pin.
  */
 void MPC2515_DisableClockOut(void);

 /** @brief			Install a new MCP2515 callback.
  *  @param Config	Pointer to configuration object
  */
 void MCP2515_InstallCallback(const MCP2515_Config_t* Config);
 
 /** @brief					Remove an installed callback.
  *  @param CallbackType	MCP2515 callback type
  */
 void MCP2515_RemoveCallback(const MCP2515_CallbackType_t Callback);

 /** @brief			Change the interrupt priority for an installed callback. \n
  *					NOTE: Only available with XMega architecture
  *  @param Config	MCP2515 interrupt configuration
  */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 void MCP2515_ChangeInterruptLevel(const MCP2515_Config_t* Config);
 #endif

 /** @brief			Change the device mode.
  *  @param Mode	Device mode
  */
 void MCP2515_SetDeviceMode(const MCP2515_DeviceMode_t Mode);

 /** @brief		Get the device mode.
  *  @return	Device mode
  */
 MCP2515_DeviceMode_t MCP2515_GetDeviceMode(void);

 /** @brief			Enable/disable the filter for a receive buffer.
  *  @param Buffer	Receive buffer
  *  @param Filter	Filter settings
  */
 void MCP2515_SwitchFilter(const MCP2515_ReceiveBuffer_t Buffer, const MCP2515_FilterRule_t Filter);
 
 /** @brief			Configure a receive filter.
  *  @param Config	Pointer to filter configuration
  */
 void MCP2515_ConfigFilter(const MCP2515_FilterConfig_t* Config); 

 /** @brief			Request a message transmission.
  *  @param Buffer	Transmit buffer
  */
 void MCP2515_RequestTransmission(const MCP2515_TransmitBuffer_t Buffer);

 /** @brief			Abort a message transmission.
  *  @param Buffer	Transmit buffer
  */
 void MPC2515_AbortTransmission(const MCP2515_TransmitBuffer_t Buffer);

 /** @brief	Abort all message transmissions.
  */
 void MPC2515_AbortAllTransmission(void);

 /** @brief			Read a message from the CAN controller.
  *  @param Message	Pointer to CAN message object
  */
 void MCP2515_ReadMessage(CAN_Message_t* Message);

 /** @brief				Send a CAN message.
  *  @param Message		Pointer to CAN message object
  *  @param Priority	Message priority
  *  @return			Error code
  */
 MCP2515_ErrorCode_t MCP2515_PrepareMessage(CAN_Message_t* Message, const MCP2515_MessagePriority_t Priority);
 
#endif /* MCP2515_H_ */