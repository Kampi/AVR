/*
 * W5100.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for W5100 Ethernet controller.

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

/** @file Peripheral/W5100/W5100.h
 *  @brief	Driver for WIZnet W5100 Ethernet controller.
 *
 *  This file contains the prototypes and definitions for the Ethernet controller driver.
 *
 *  @author Daniel Kampert
 */

#ifndef W5100_H_
#define W5100_H_

 #include "Common/Common.h"

 #if(!defined W5100_INTERFACE_TYPE && (MCU_ARCH == MCU_ARCH_XMEGA))
	 #define W5100_INTERFACE_TYPE		INTERFACE_SPI
 #endif

 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #if(MCU_NAME == MCU_NAME_ATMEGA32)
		 #include "Arch/AVR8/megaAVR/mega32/GPIO/GPIO.h"
		 #include "Arch/AVR8/megaAVR/mega32/SPI/SPI.h"
	 #else
		 #error "Invalid CPU for W5100!"
	 #endif

 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/GPIO/GPIO.h"
	 #if(W5100_INTERFACE_TYPE == INTERFACE_USART_SPI)
		 #include "Arch/XMega/USART/USART.h"
	 #elif(W5100_INTERFACE_TYPE == INTERFACE_SPI)
		 #include "Arch/XMega/SPI/SPI.h"
	 #else
		 #error "Interface not supported for W5100!"
	 #endif
 #else
	 #error "Architecture not supported for W5100!"
 #endif

 /** @brief IP address object definition.
  */
 typedef uint8_t			IPAddress[4];

 /** @brief MAC address object definition.
  */
 typedef uint8_t			MACAddress[6];

 /** @brief W5100 error codes.
  */
 typedef enum
 {
	 W5100_NO_ERROR = 0x00,								/**< No error */
	 W5100_NOT_INITIALIZED = 0x01,						/**< Device not initialized */
	 W5100_INVALID_PARAM = 0x02,							/**< Invalid function parameter */
	 W5100_TIMEOUT = 0x03,								/**< Connection timeout */
	 W5100_SOCKET_ERROR = 0x04,							/**< Socket error */
 } W5100_ErrorCode_t;

 /** @brief Supported protocols of the W5100.
  */
 typedef enum
 {
	 W5100_PROT_CLOSED = 0x00,							/**<  */
	 W5100_PROT_TCP = 0x01,								/**< TCP protocol */
	 W5100_PROT_UDP = 0x02,								/**< UDP protocol */
	 W5100_PROT_MACRAW = 0x03,							/**<  */
	 W5100_PROT_PPPOE = 0x04,							/**< Transmit buffer full */
 } W5100_Protocol_t;

 /** @brief Supported memory sizes of the W5100.
  */
 typedef enum
 {
	 W5100_MEM_1K = 0x00,								/**< Memory size 1KB */
	 W5100_MEM_2K = 0x01,								/**< Memory size 2KB */
	 W5100_MEM_4K = 0x02,								/**< Memory size 4KB */
	 W5100_MEM_8K = 0x03,								/**< Memory size 8KB */
 } W5100_Memory_t;

 /** @brief Status codes used by the W5100.
  */
 typedef enum
 {
	 W5100_SOCK_CLOSED = 0x00,							/**< No operation occurs and all resources for the connection is released. */
	 W5100_SOCK_INIT = 0x13,							/**< This is the initial step for TCP connection establishment of a socket. */
	 W5100_SOCK_LISTEN = 0x14,							/**< */
	 W5100_SOCK_SYNSENT = 0x15,							/**< */
	 W5100_SOCK_SYNRECV = 0x16,							/**< */
	 W5100_SOCK_ESTABLISHED = 0x17,						/**< */
	 W5100_SOCK_FIN_WAIT = 0x18,						/**< */
	 W5100_SOCK_CLOSING = 0x1A,							/**< */
	 W5100_SOCK_TIME_WAIT = 0x1B,						/**< */
	 W5100_SOCK_CLOSE_WAIT = 0x1C,						/**< */
	 W5100_SOCK_LAST_ACK = 0x1D,						/**< */
	 W5100_SOCK_ARP = 0x21,								/**< */
	 W5100_SOCK_UDP = 0x22,								/**< */
	 W5100_SOCK_IPRAW = 0x32,							/**< */
	 W5100_SOCK_MACRAW = 0x42,							/**< */
	 W5100_SOCK_PPOE = 0x5F,							/**< */
 } W5100_Status_t;

 /** @brief W5100 configuration object.
  */
 typedef struct  W5100_cfg_t
 {
	 bool DisablePing;									/**< Set to #true to disable ping block */
	 MACAddress MAC;									/**< MAC address for the device */
	 IPAddress IP;										/**< IP address for the device */
	 IPAddress Submask;									/**< Subnet mask for the device */
	 IPAddress Gateway;									/**< Gateway address for the device */
 } W5100_Config_t;

 /** @brief				Initialize the W5100 controller.
  *  @param Config		Pointer to SPI configuration object
  *						NOTE: Set it to #NULL if you have initialized the SPI already
  *	 @param Eth_Config	Pointer to Ethernet configuration object
  *  @return			Error code
  */
 W5100_ErrorCode_t W5100_Init(SPIM_Config_t* Config, W5100_Config_t* Eth_Config);

 /** @brief		Perform a software reset of the Ethernet controller.
  *  @return	Error code
  */
 W5100_ErrorCode_t W5100_SoftReset(void);

 /** @brief	Perform a hardware reset of the Ethernet controller.
  */
 #ifdef W5100_USE_EXT_RESET
	void W5100_HardReset(void);
 #endif

 /** @brief			Set the gateway address for the W5100.
  *  @param Address	Pointer to gateway address
  *  @return		Error code
  */
 W5100_ErrorCode_t W5100_SetGateway(IPAddress Address);

 /** @brief			Set the MAC address for the W5100.
  *  @param Address	Pointer to MAC address
  *  @return		Error code
  */
 W5100_ErrorCode_t W5100_SetMAC(MACAddress Address);

 /** @brief			Set the subnet mask for the W5100.
  *  @param Address	Pointer to subnet mask address
  *  @return		Error code
  */
 W5100_ErrorCode_t W5100_SetSubnetMask(IPAddress Address);

 /** @brief			Set the IP address for the W5100.
  *  @param Address	Pointer to IP address
  *  @return		Error code
  */
 W5100_ErrorCode_t W5100_SetIP(IPAddress Address);

 /** @brief			Set the available memory for a given socket.
  *  @param Socket	Socket index (must between 0 and 3)
  *  @param Address	Pointer to gateway address
  *	 @return		Error code
  */
 W5100_ErrorCode_t W5100_SetSocketSize(uint8_t Socket, W5100_Memory_t Memory);

 /** @brief			Get the status of the target socket. Must performed in a loop to process the requests.
  *  @param Socket	Socket index (must between 0 and 3)
  *  @param Status	Pointer to status
  *	 @return		Error code
  */
 W5100_ErrorCode_t W5100_GetState(uint8_t Socket, W5100_Status_t* Status);

 /** @brief				Open a socket for a new connection.
  *  @param Socket		Socket index (must between 0 and 3)
  *  @param Protocol	Ethernet protocol used by this socket
  *  @param Port		Ethernet port used by this socket
  *  @param RxMemory	Available memory for the Rx buffer of the socket (max. 8 kB for all sockets)
  *  @param TxMemory	Available memory for the Tx buffer of the socket (max. 8 kB for all sockets)
  *	 @param Timeout		Timeout in ms
  *	 @return			Error code
  */
 W5100_ErrorCode_t W5100_Open(uint8_t Socket, W5100_Protocol_t Protocol, uint16_t Port, W5100_Memory_t RxMemory, W5100_Memory_t TxMemory, uint32_t Timeout);

 /** @brief			Disconnect the socket (blocking function).
  *  @param Socket	Socket index (must between 0 and 3)
  *	 @param Timeout	Timeout in ms
  *	 @return		Error code
  */
 W5100_ErrorCode_t W5100_Disconnect(uint8_t Socket, uint32_t Timeout);

 /** @brief			Close an open socket.
  *  @param Socket	Socket index (must between 0 and 3)
  *	 @param Timeout	Timeout in ms
  *	 @return		Error code
  */
 W5100_ErrorCode_t W5100_Close(uint8_t Socket, uint32_t Timeout);

 /** @brief			Put the server into listen mode.
  *  @param Socket	Socket index (must between 0 and 3)
  *	 @param Timeout	Timeout in ms
  *	 @return		Error code
  */
 W5100_ErrorCode_t W5100_Listen(uint8_t Socket, uint32_t Timeout);

 /** @brief			Get the received bytes from a socket.
  *  @param Socket	Socket index (must between 0 and 3)
  *	 @param Bytes	Pointer to received bytes
  *	 @return		Error code
  */
 W5100_ErrorCode_t W5100_GetBytes(uint8_t Socket, uint16_t* Bytes);

 /** @brief			Read the received data from a given socket.
  *  @param Socket	Socket index (must between 0 and 3)
  *	 @param Buffer	Pointer to data buffer
  *  @param Length	Data length
  *	 @return		Error code
  */
 W5100_ErrorCode_t W5100_Receive(uint8_t Socket, uint8_t* Buffer, uint16_t Length);

 W5100_ErrorCode_t W5100_Send(uint8_t Socket, uint8_t* Buffer, uint16_t Length, uint32_t Timeout);

#endif /* W5100_H_ */