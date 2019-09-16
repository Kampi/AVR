/*
 * MCP2515.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for MCP2515 CAN controller.

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
 *  @brief	Driver for Microchip MCP2515 CAN controller.
 *			NOTE: This driver needs interrupt support!
 *
 *  This file contains the prototypes and definitions for the CAN controller driver.
 *
 *  @bug	Under construction
 *  @author Daniel Kampert
 */

#ifndef MCP2515_H_
#define MCP2515_H_

 #include "Config_MCP2515.h"
 #include "Common/Common.h"

 #if(!defined MCP2515_INT)
	 #warning "Invalid configuration for the MCP2515 interrupt. Use default."

	 #define MCP2515_INT			PORTD, 2
 #endif

 #if(!defined MCP2515_INTERFACE_TYPE && (MCU_ARCH == MCU_ARCH_XMEGA))
	 #define MCP2515_INTERFACE_TYPE		INTERFACE_SPI
 #endif

 #if(MCU_ARCH == MCU_ARCH_AVR8)
	#include "Arch/AVR8/GPIO/GPIO.h"
	#include "Arch/AVR8/SPI/SPI.h"
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

 #define MCP2515_MAX_DATABYTES		8				/**< Max. data bytes per message */

 /** 
  * MCP2515 error codes
  */
 typedef enum
 {
	MCP2515_NO_ERROR = 0,							/**< No error */ 
	MCP2515_INVALID_IDENTIFIER = -1,				/**< Invalid identifier */ 
 } MCP2515_ErrorCodes_t;

 /** 
  * MCP2515 modes
  */
 typedef enum
 {
	MCP2515_NORMAL_MODE = 0x00,						/**< Normal mode */ 
	MCP2515_SLEEP_MODE = 0x01,						/**< Sleep mode */ 
	MCP2515_LOOPBACK_MODE = 0x02,					/**< Loop back mode */ 
	MCP2515_LISTEN_ONLY_MODE = 0x03,				/**< Listen only mode */ 
	MCP2515_CONFIG_MODE = 0x04,						/**< Configuration mode */ 
 } MCP2515_DeviceMode_t;

 /** 
  * MCP2515 message priorities
  */
 typedef enum
 {
	MCP2515_PRIO_LOWEST		= 0x00,					/**< Lowest priority */ 
	MCP2515_PRIO_LOW		= 0x01,					/**< Low priority */ 
	MCP2515_PRIO_HIGH		= 0x02,					/**< High priority */ 
	MCP2515_PRIO_HIGHEST	= 0x03,					/**< Highest priority */ 
 } MCP2515_MessagePriority_t;

 /** 
  * MCP2515 filter configuration
  */
 typedef enum
 {
	MCP2515_FILTER_RECEIVE_ALL = 0x00,				/**< Receive all */ 
	MCP2515_FILTER_RECEIVE_STD = 0x01,				/**< Receive only standard messages */ 
	MCP2515_FILTER_RECEIVE_EXT = 0x02,				/**< Receive only extended messages */ 
	MCP2515_FILTER_OFF = 0x03,						/**< Filter off */ 
 } MCP2515_Filter_t;

 /** 
  * MCP2515 transmit buffer
  */
 typedef enum
 {
	MCP2515_TX0 = 0x00,								/**< Transmit buffer 0 */ 
	MCP2515_TX1 = 0x01,								/**< Transmit buffer 1 */ 
	MCP2515_TX2 = 0x02,								/**< Transmit buffer 2 */ 
 } MCP2515_TransmitBuffer_t;

 /** 
  * MCP2515 receive buffer
  */
 typedef enum
 {
	 MCP2515_RX0 = 0x00,							/**< Receive buffer 0 */ 
	 MCP2515_RX1 = 0x01,							/**< Receive buffer 1 */ 
 } MCP2515_ReceiveBuffer_t;

 /** 
  * MCP2515 receive filter
  */
 typedef enum
 {
	MCP2515_RXF_0 = 0x01,							/**< Receive filter 0 */ 
	MCP2515_RXF_1 = 0x02,							/**< Receive filter 1 */ 
	MCP2515_RXF_2 = 0x03,							/**< Receive filter 2 */ 
	MCP2515_RXF_3 = 0x04,							/**< Receive filter 3 */ 
	MCP2515_RXF_4 = 0x05,							/**< Receive filter 4 */ 
	MCP2515_RXF_5 = 0x06,							/**< Receive filter 5 */ 
 } MCP2515_ReceiveFilter_t;

 /** 
  * MCP2515 message types
  */
 typedef enum
 {
	MCP2515_STANDARD_DATA	= 0x00,					/**< Standard data frame */ 
	MCP2515_STANDARD_REMOTE = 0x01,					/**< Standard remote frame */ 
	MCP2515_EXTENDED_DATA	= 0x02,					/**< Extended data frame */ 
	MCP2515_EXTENDED_REMOTE = 0x03,					/**< Extended remote frame */ 
 } MCP2515_MessageType_t;

 /** 
  * MCP2515 callback types
  */
 typedef enum
 {
	 MCP2515_MESSAGE_INTERRUPT = 0x01,				/**< MCP2515 message interrupt */ 
	 MCP2515_STANDARD_DATA_INTERRUPT = 0x02,		/**< MCP2515 standard data frame interrupt */ 
	 MCP2515_EXTENDED_DATA_INTERRUPT = 0x04,		/**< MCP2515 extended data frame interrupt */ 
	 MCP2515_STANDARD_REMOTE_INTERRUPT = 0x08,		/**< MCP2515 standard remote frame interrupt */ 
	 MCP2515_EXTENDED_REMOTE_INTERRUPT = 0x10,		/**< MCP2515 extended remote frame interrupt */ 
 } MCP2515_CallbackType_t;

 /** 
  * MCP2515 filter configuration object
  */
 typedef struct
 {
	MCP2515_ReceiveFilter_t Filter;					/**< Target filter */ 
	uint16_t FilterMask;							/**< Filter mask */ 
	uint16_t FilterID;								/**< Filter ID */ 
	Bool_t Extended;								/**< Set this to #TRUE if you want to use extended frames */ 
 } MCP2515_FilterConfig_t;

 /** 
  * MCP2515 message object
  */
 typedef struct
 {
	 MCP2515_MessageType_t MessageType;				/**< Message type */ 
	 uint16_t MessageID;							/**< Message ID */ 
	 uint8_t MessageLength;							/**< Message length */ 
	 MCP2515_MessagePriority_t MessagePriority;		/**< Message priority */ 
	 uint8_t* Buffer;								/**< Pointer to message data */
 } CAN_Message_t;
 
 /** @brief			MCP2515 error interrupt handler
 */
 typedef void (*MCP2515_ErrorCallback_t)(void);
 
 /** @brief			MCP2515 message received interrupt handler
  *  @param Message	CAN message
  */
 typedef void (*MCP2515_MessageCallback_t)(CAN_Message_t Message);
 
 /** 
  * MCP2515 interrupt configuration object
  */
 typedef struct
 {
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		PORT_t* Port;
	 #elif(MCU_ARCH == MCU_ARCH_AVR8)
		volatile uint8_t* Port;
	 #endif

	 uint8_t Pin;
	 GPIO_InterruptChannel_t IOChannel;
	 MCP2515_CallbackType_t Source;						/**< MCP2515 interrupt type */
	 
     #if(MCU_ARCH == MCU_ARCH_XMEGA)
		Interrupt_Level_t InterruptLevel;				/**< Interrupt level */
	 #endif

	 Bool_t InternalPullUp;								/**< Set to #TRUE to enable internal pull up for interrupt I/O */
	 MCP2515_ErrorCallback_t ErrorCallback;				/**< Function pointer to MCP2515 message callback \n
															 NOTE: Set it to #NULL if you do not want to use it. */
	 MCP2515_MessageCallback_t MessageCallback;			/**< Function pointer to MCP2515 message callback \n
															 NOTE: Set it to #NULL if you do not want to use it. */
 } MCP2515_InterruptConfig_t;

 /** @brief					Initialize the MCP2515 controller.
  *  @param Config			Pointer to SPI configuration struct
  *							NOTE: Set it to #NULL if you have initialized the SPI already
  *  @param ConfigInterrupt	Pointer to MCP2515 interrupt configuration object
  */
 void MCP2515_Init(SPIM_Config_t* Config, const MCP2515_InterruptConfig_t* ConfigInterrupt);
 
 /** @brief		Reset the MCP2515 controller.
  */
 void MCP2515_Reset(void);
 
 /** @brief			Enable interrupt support for the MCU.
  *  @param Config	Pointer to MCP2515 interrupt configuration object
  */
 void MCP2515_EnableInterrupts(const MCP2515_InterruptConfig_t* Config);
 
 /** @brief			Disable interrupt support for the MCU.
 *  @param Config	Pointer to MCP2515 interrupt configuration object
  */
 void MCP2515_DisableInterrupts(const MCP2515_InterruptConfig_t* Config);
 
 /** @brief			Install a new MCP2515 callback.
  *  @param Config	Pointer to interrupt configuration
  */
 void MCP2515_InstallCallback(const MCP2515_InterruptConfig_t* Config);
 
 /** @brief					Remove an installed callback.
  *  @param CallbackType	MCP2515 callback type
  */
 void MCP2515_RemoveCallback(const MCP2515_CallbackType_t Callback);

  #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 void MCP2515_ChangeInterruptLevel(const MCP2515_InterruptConfig_t* Config);
 #endif

 /** @brief			Send a bit modify to the CAN controller.
  *  @param Address	Register address
  *  @param Mask	Register mask
  *  @param Value	Register value
  */
 void MCP2515_BitModify(const uint8_t Address, const uint8_t Mask, const uint8_t Value);
 
 /** @brief			Write into a register of the CAN controller.
  *  @param Address	Register Address
  *  @param Value	Register data
  */
 void MCP2515_WriteRegister(const uint8_t Address, const uint8_t Data);
 
 /** @brief			Read a register of the CAN controller.
  *  @param Address	Register Address
  *  @return		Register data
  */
 uint8_t MCP2515_ReadRegister(const uint8_t Address);

 /** @brief	Change the device mode.
  *  @param Mode	Device mode
  */
 void MCP2515_SetDeviceMode(const MCP2515_DeviceMode_t Mode);

 /** @brief	Get the device mode.
  *  @returnDevice mode
  */
 MCP2515_DeviceMode_t MCP2515_GetDeviceMode(void);

 /** @brief				Set the transmit priority for a transmit buffer.
  *  @param Buffer		Transmit buffer
  *  @param Priority	Message priority
  */
 void MCP2515_SetTxPriority(const MCP2515_TransmitBuffer_t Buffer, const MCP2515_MessagePriority_t Priority);
 
 /** @brief			Set the filter for a receive buffer.
  *  @param Buffer	Receive buffer
  *  @param Filter	Filter
  */
 void MCP2515_SetRxFilter(const MCP2515_ReceiveBuffer_t Buffer, const MCP2515_Filter_t Filter);
 
 /** @brief			Configure a receive filter.
  *  @param Config	Pointer to filter config
  */
 void MCP2515_ConfigFilter(const MCP2515_FilterConfig_t* Config); 

 /** @brief			Start a message transmission.
  *  @param Buffer	Transmit buffer for transmission
  */
 void MCP2515_StartTransmission(const MCP2515_TransmitBuffer_t Buffer);
 
 /** @brief			Read a message from the CAN controller.
  *  @param Message	Pointer to CAN message object
  */
 void MCP2515_ReadMessage(CAN_Message_t* Message);

 /** @brief			Send a CAN message.
  *  @param Message	Pointer to CAN message object
  *  @return		Error code
  */
 MCP2515_ErrorCodes_t MCP2515_SendMessage(CAN_Message_t* Message);
 
 /** @brief				Set the message identifier.
  *  @param Buffer		Target buffer
  *  @param Identifier	Message identifier
  */
 void MCP2515_SetIdentifier(const MCP2515_TransmitBuffer_t Buffer, const uint32_t Identifier);
 
 /** @brief		Read the Rx status.
  *  @return	Rx status
  */
 uint8_t MCP2515_ReadRxStatus(void);
 
#endif /* MCP2515_H_ */