/*
 * W5100.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 * File info: Driver for W5100 Ethernet controller.

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

/** @file Peripheral/W5100/W5100.c
 *  @brief Driver for the WIZnet W5100 Ethernet controller.
 *
 *  This file contains the implementation of the Ethernet controller driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/W5100/W5100.h"

/** @defgroup W5100
 *  @{
 */
	/** @defgroup W5100-Commands
	 *  W5100 commands.
	 *  @{
	 */
		#define W5100_CMD_WRITE					0xF0
		#define W5100_CMD_READ					0x0F
	/** @} */ // end of W5100-Commands

	/** @defgroup W5100-Register
	 *  W5100 register addresses.
	 *  @{
	 */
		#define W5100_REGISTER_MR				0x0000
		#define W5100_REGISTER_GAR0				0x0001
		#define W5100_REGISTER_GAR1				0x0002
		#define W5100_REGISTER_GAR2				0x0003
		#define W5100_REGISTER_GAR3				0x0004
		#define W5100_REGISTER_SUBR0			0x0005
		#define W5100_REGISTER_SUBR1			0x0006
		#define W5100_REGISTER_SUBR2			0x0007
		#define W5100_REGISTER_SUBR3			0x0008
		#define W5100_REGISTER_SHAR0			0x0009
		#define W5100_REGISTER_SHAR1			0x000A
		#define W5100_REGISTER_SHAR2			0x000B
		#define W5100_REGISTER_SHAR3			0x000C
		#define W5100_REGISTER_SHAR4			0x000D
		#define W5100_REGISTER_SHAR5			0x000E
		#define W5100_REGISTER_SIPR0			0x000F
		#define W5100_REGISTER_SIPR1			0x0010
		#define W5100_REGISTER_SIPR2			0x0011
		#define W5100_REGISTER_SIPR3			0x0012
		#define W5100_REGISTER_IR				0x0015
		#define W5100_REGISTER_IMR				0x0016
		#define W5100_REGISTER_RTR0				0x0017
		#define W5100_REGISTER_RTR1				0x0018
		#define W5100_REGISTER_RCR				0x0019
		#define W5100_REGISTER_RMSR				0x001A
		#define W5100_REGISTER_TMSR				0x001B
		#define W5100_REGISTER_PATR0			0x001C
		#define W5100_REGISTER_PATR1			0x001D
		#define W5100_REGISTER_PTIMER			0x0028
		#define W5100_REGISTER_PMAGIC			0x0029
		#define W5100_REGISTER_UIPR0			0x002A
		#define W5100_REGISTER_UIPR1			0x002B
		#define W5100_REGISTER_UIPR2			0x002C
		#define W5100_REGISTER_UIPR3			0x002D
		#define W5100_REGISTER_UPORT0			0x002E
		#define W5100_REGISTER_UPORT1			0x002F
	/** @} */ // end of W5100-Register

	/** @defgroup W5100-Offsets
	 *  W5100 register offset values.
	 *  @{
	 */
		#define W5100_OFFSET_MR					0x00
		#define W5100_OFFSET_CR					0x01
		#define W5100_OFFSET_IR					0x02
		#define W5100_OFFSET_SR					0x03
		#define W5100_OFFSET_PORT0				0x04
		#define W5100_OFFSET_PORT1				0x05
		#define W5100_OFFSET_DHAR0				0x06
		#define W5100_OFFSET_DHAR1				0x07
		#define W5100_OFFSET_DHAR2				0x08
		#define W5100_OFFSET_DHAR3				0x09
		#define W5100_OFFSET_DHAR4				0x0A
		#define W5100_OFFSET_DHAR5				0x0B
		#define W5100_OFFSET_DIPR0				0x0C
		#define W5100_OFFSET_DIPR1				0x0D
		#define W5100_OFFSET_DIPR2				0x0E
		#define W5100_OFFSET_DIPR3				0x0F
		#define W5100_OFFSET_DPORT0				0x10
		#define W5100_OFFSET_DPORT1				0x11
		#define W5100_OFFSET_MSSR0				0x12
		#define W5100_OFFSET_MSSR1				0x13
		#define W5100_OFFSET_PROTO				0x14
		#define W5100_OFFSET_TOS				0x15
		#define W5100_OFFSET_TTL				0x16
		#define W5100_OFFSET_TX_FSR0			0x20
		#define W5100_OFFSET_TX_FSR1			0x21
		#define W5100_OFFSET_TX_RD0				0x22
		#define W5100_OFFSET_TX_RD1				0x23
		#define W5100_OFFSET_TX_WR0				0x24
		#define W5100_OFFSET_TX_WR1				0x25
		#define W5100_OFFSET_RX_RSR0			0x26
		#define W5100_OFFSET_RX_RSR1			0x27
		#define W5100_OFFSET_RX_RD0				0x28
		#define W5100_OFFSET_RX_RD1				0x29
	/** @} */ // end of W5100-Offsets

 	/** @defgroup W5100-Commands
	 *  W5100 command codes.
	 *  @{
	 */
		#define W5100_CMD_OPEN					0x01
		#define W5100_CMD_LISTEN				0x02
		#define W5100_CMD_CONNECT				0x04
		#define W5100_CMD_DISCON				0x08
		#define W5100_CMD_CLOSE					0x10
		#define W5100_CMD_SEND					0x20
		#define W5100_CMD_SEND_MAC				0x21
		#define W5100_CMD_SEND_KEEP				0x22
		#define W5100_CMD_RECV					0x40
	/** @} */ // end of W5100-Commands

 	/** @defgroup W5100-Bits
	 *  W5100 bit positions.
	 *  @{
	 */
		#define W5100_BIT_RESET					0x07
		#define W5100_BIT_PB					0x04
	/** @} */ // end of W5100-Bits
/** @} */ // end of W5100

#if(MCU_ARCH == MCU_ARCH_AVR8)
	#define W5100_SPI_INIT(Config)								SPIM_Init(Config)
	#define W5100_SPI_TRANSMIT(Data)							SPIM_SendData(Data)
	#define W5100_SPI_CHIP_SELECT()								SPIM_SelectDevice(GET_PERIPHERAL(W5100_SS), GET_INDEX(W5100_SS))
	#define W5100_SPI_CHIP_DESELECT()							SPIM_DeselectDevice(GET_PERIPHERAL(W5100_SS), GET_INDEX(W5100_SS))
#elif(MCU_ARCH == MCU_ARCH_XMEGA)
	#if(W5100_INTERFACE_TYPE == INTERFACE_USART_SPI)
		#define W5100_SPI_INIT(Config)							USART_SPI_Init(Config)
		#define W5100_SPI_TRANSMIT(Data)						USART_SPI_SendData(&W5100_INTERFACE, Data)
		#define W5100_SPI_CHIP_SELECT()							USART_SPI_SelectDevice(GET_PERIPHERAL(W5100_SS), GET_INDEX(W5100_SS))
		#define W5100_SPI_CHIP_DESELECT()						USART_SPI_DeselectDevice(GET_PERIPHERAL(W5100_SS), GET_INDEX(W5100_SS))

	 #elif(W5100_INTERFACE_TYPE == INTERFACE_SPI) 
		#define W5100_SPI_INIT(Config)							SPIM_Init(Config)
		#define W5100_SPI_TRANSMIT(Data)						SPIM_SendData(&W5100_INTERFACE, Data)
		#define W5100_SPI_CHIP_SELECT()							SPIM_SelectDevice(GET_PERIPHERAL(W5100_SS), GET_INDEX(W5100_SS))
		#define W5100_SPI_CHIP_DESELECT()						SPIM_DeselectDevice(GET_PERIPHERAL(W5100_SS), GET_INDEX(W5100_SS))
	 #else
		 #error "Interface not supported for W5100!"
	 #endif
 #else
	#error "Architecture not supported for W5100!"
#endif

/** @brief Start address of the Tx memory.
 */
#define W5100_TX_BUFFER_BASE			0x4000

/** @brief Start address of the Rx memory.
 */
#define W5100_RX_BUFFER_BASE			0x6000

/** @brief Base address for the sockets.
 */
#define W5100_SOCKET_BASE				0x0400

/** @brief Address offset between each socket.
 */
#define W5100_SOCKET_ADDR_OFFSET		0x0100

/** @brief Minimum memory offset between each socket.
 */
#define W5100_SOCKET_MEM_OFFSET			0x0400

/** @brief			Calculate the base address of a given socket.
 *  @param Socket	Socket index (must between 0 and 3)
 */
#define W5100_SOCKET_ADDR(Socket)		(W5100_SOCKET_BASE + (Socket * W5100_SOCKET_ADDR_OFFSET))

static bool _W5100_IsInitialized = false;

/** @brief			Write the memory of the Ethernet controller.
 *  @param Address	Register address
 *  @param Data		Pointer to memory data
 *  @param Length	Data length
 */
static void W5100_WriteMemory(const uint16_t Address, const uint8_t* Data, const uint16_t Length)
{
	for(uint16_t i = 0x00; i < Length; i++)
	{
		W5100_SPI_CHIP_SELECT();
		W5100_SPI_TRANSMIT(W5100_CMD_WRITE);
		W5100_SPI_TRANSMIT(((Address + i) & 0xFF00) >> 0x08);
		W5100_SPI_TRANSMIT((Address + i) & 0xFF);
		W5100_SPI_TRANSMIT(Data[i]);
		W5100_SPI_CHIP_DESELECT();
	}
}

/** @brief			Read the memory of the Ethernet controller.
 *  @param Address	Register address
 *  @param Data		Pointer to memory data
 *  @param Length	Data length
 */
static void W5100_ReadMemory(const uint16_t Address, uint8_t* Data, const uint16_t Length)
{
	for(uint16_t i = 0x00; i < Length; i++)
	{
		W5100_SPI_CHIP_SELECT();
		W5100_SPI_TRANSMIT(W5100_CMD_READ);
		W5100_SPI_TRANSMIT(((Address + i) & 0xFF00) >> 0x08);
		W5100_SPI_TRANSMIT((Address + i) & 0xFF);
		Data[i] = W5100_SPI_TRANSMIT(0xFF);
		W5100_SPI_CHIP_DESELECT();
	}
}

/** @brief			Write a register of the Ethernet controller.
 *  @param Address	Register address
 *  @param Data		Register data
 */
static void W5100_WriteRegister(const uint16_t Address, const uint8_t Data)
{
	W5100_SPI_CHIP_SELECT();
	W5100_SPI_TRANSMIT(W5100_CMD_WRITE);
	W5100_SPI_TRANSMIT(Address >> 0x08);
	W5100_SPI_TRANSMIT(Address & 0xFF);
	W5100_SPI_TRANSMIT(Data);
	W5100_SPI_CHIP_DESELECT();
}

/** @brief			Read a register of the Ethernet controller.
 *  @param Address	Register address
 *  @param Data		Pointer to register data
 */
static void W5100_ReadRegister(const uint16_t Address, uint8_t* Data)
{
	W5100_SPI_CHIP_SELECT();
	W5100_SPI_TRANSMIT(W5100_CMD_READ);
	W5100_SPI_TRANSMIT(Address >> 0x08);
	W5100_SPI_TRANSMIT(Address & 0xFF);
	*Data = W5100_SPI_TRANSMIT(0xFF);
	W5100_SPI_CHIP_DESELECT();
}

/** @brief			Executes a command and wait until the command was executed or a timeout occurs.
 *  @param Socket	Socket index (must between 0 and 3)
 *  @param Command	Command code
 *  @param Timeout	Timeout in ms
 *  @return			Error code
 */
static W5100_ErrorCode_t W5100_ExecuteCommand(const uint8_t Socket, uint8_t const Command, const uint32_t Timeout)
{
	uint8_t Status;
	uint16_t SocketBase;
	uint32_t Timeout_Temp = Timeout;

	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}
	else if(Socket > 0x04)
	{
		return W5100_INVALID_PARAM;
	}

	SocketBase = W5100_SOCKET_ADDR(Socket);

	W5100_WriteRegister(SocketBase + W5100_OFFSET_CR, Command);

	do
	{
		W5100_ReadRegister(SocketBase + W5100_OFFSET_CR, &Status);

		if(Timeout_Temp-- == 0x00)
		{
			return W5100_TIMEOUT;
		}

		_delay_ms(1);
	} while(Status);
	
	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_Init(SPIM_Config_t* Config, W5100_Config_t* Eth_Config)
{
	if(Eth_Config == NULL)
	{
		return W5100_INVALID_PARAM;
	}

	// Initialize the CS Pin
	GPIO_SetDirection(GET_PERIPHERAL(W5100_SS), GET_INDEX(W5100_SS), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(W5100_SS), GET_INDEX(W5100_SS));

	// Initialize the reset pin and perform a hardware reset
	#ifdef W5100_USE_EXT_RESET
		GPIO_SetDirection(GET_PERIPHERAL(W5100_EXT_RESET), GET_INDEX(W5100_EXT_RESET), GPIO_DIRECTION_OUT);
		GPIO_Set(GET_PERIPHERAL(W5100_EXT_RESET), GET_INDEX(W5100_EXT_RESET));
	#endif

	if(Config != NULL)
	{
		W5100_SPI_INIT(Config);
	}

	_W5100_IsInitialized = true;

	W5100_SoftReset();

	// Perform a hardware reset
	#ifdef W5100_USE_EXT_RESET
		W5100_HardReset();
	#endif

	W5100_WriteRegister(W5100_REGISTER_MR, (Eth_Config->DisablePing << W5100_BIT_PB));

	W5100_SetGateway(Eth_Config->Gateway);
	W5100_SetMAC(Eth_Config->MAC);
	W5100_SetSubnetMask(Eth_Config->Submask);
	W5100_SetIP(Eth_Config->IP);

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_SoftReset(void)
{
	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}

	W5100_WriteRegister(W5100_REGISTER_MR, 0x01 << W5100_BIT_RESET);

	// Wait until the reset has finished
	for(uint16_t i = 0x00; i < 0xFFFF; i++);

	return W5100_NO_ERROR;
}

#ifdef W5100_USE_EXT_RESET
	void W5100_HardReset(void)
	{
		GPIO_Clear(GET_PERIPHERAL(W5100_EXT_RESET), GET_INDEX(W5100_EXT_RESET));
		for(uint16_t i = 0x00; i < 0xFFF; i++);
		GPIO_Set(GET_PERIPHERAL(W5100_EXT_RESET), GET_INDEX(W5100_EXT_RESET));

		// Wait until the reset has finished
		for(uint16_t i = 0x00; i < 0xFFFF; i++);
	}
#endif

W5100_ErrorCode_t W5100_SetGateway(IPAddress Address)
{
	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}

	W5100_WriteRegister(W5100_REGISTER_GAR0, Address[0]);
	W5100_WriteRegister(W5100_REGISTER_GAR1, Address[1]);
	W5100_WriteRegister(W5100_REGISTER_GAR2, Address[2]);
	W5100_WriteRegister(W5100_REGISTER_GAR3, Address[3]);

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_SetMAC(MACAddress Address)
{
	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}

	W5100_WriteRegister(W5100_REGISTER_SHAR0, Address[0]);
	W5100_WriteRegister(W5100_REGISTER_SHAR1, Address[1]);
	W5100_WriteRegister(W5100_REGISTER_SHAR2, Address[2]);
	W5100_WriteRegister(W5100_REGISTER_SHAR3, Address[3]);
	W5100_WriteRegister(W5100_REGISTER_SHAR4, Address[4]);
	W5100_WriteRegister(W5100_REGISTER_SHAR5, Address[5]);

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_SetSubnetMask(IPAddress Mask)
{
	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}

	W5100_WriteRegister(W5100_REGISTER_SUBR0, Mask[0]);
	W5100_WriteRegister(W5100_REGISTER_SUBR1, Mask[1]);
	W5100_WriteRegister(W5100_REGISTER_SUBR2, Mask[2]);
	W5100_WriteRegister(W5100_REGISTER_SUBR3, Mask[3]);

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_SetIP(IPAddress Address)
{
	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}

	W5100_WriteRegister(W5100_REGISTER_SIPR0, Address[0]);
	W5100_WriteRegister(W5100_REGISTER_SIPR1, Address[1]);
	W5100_WriteRegister(W5100_REGISTER_SIPR2, Address[2]);
	W5100_WriteRegister(W5100_REGISTER_SIPR3, Address[3]);

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_SetSocketSize(uint8_t Socket, W5100_Memory_t Memory)
{
	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}
	else if(Socket > 0x04)
	{
		return W5100_INVALID_PARAM;
	}

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_GetState(uint8_t Socket, W5100_Status_t* Status)
{
	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}
	else if((Socket > 0x04) || (Status == NULL))
	{
		return W5100_INVALID_PARAM;
	}

	W5100_ReadRegister(W5100_SOCKET_ADDR(Socket) + W5100_OFFSET_SR, Status);

	if(((uint8_t)*Status == 0x11) || ((uint8_t)*Status == 0x21) || ((uint8_t)*Status == 0x31))
	{
		*Status = W5100_SOCK_ARP;
	}

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_Open(uint8_t Socket, W5100_Protocol_t Protocol, uint16_t Port, W5100_Memory_t RxMemory, W5100_Memory_t TxMemory, uint32_t Timeout)
{
	uint8_t Temp;
	uint8_t Status;
	uint16_t SocketBase;

	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}
	else if(Socket > 0x04)
	{
		return W5100_INVALID_PARAM;
	}

	// Set the Rx memory for the socket
	W5100_ReadRegister(W5100_REGISTER_RMSR, &Temp);
	Temp &= ~(0x03 << (Socket << 0x01));
	Temp |= (RxMemory << (Socket << 0x01));
	W5100_WriteRegister(W5100_REGISTER_RMSR, Temp);

	// Set the Tx memory for the socket
	 W5100_ReadRegister(W5100_REGISTER_TMSR, &Temp);
	Temp &= ~(0x03 << (Socket << 0x01));
	Temp |= (TxMemory << (Socket << 0x01));
	W5100_WriteRegister(W5100_REGISTER_TMSR, Temp);

	SocketBase = W5100_SOCKET_ADDR(Socket);

	W5100_WriteRegister(SocketBase + W5100_OFFSET_MR, Protocol);
	W5100_WriteRegister(SocketBase + W5100_OFFSET_PORT0, ((Port & 0xFF00) >> 0x08));
	W5100_WriteRegister(SocketBase + W5100_OFFSET_PORT1, (Port & 0x00FF));

	W5100_ErrorCode_t ErrorCode = W5100_ExecuteCommand(Socket, W5100_CMD_OPEN, Timeout);
	if(ErrorCode != W5100_NO_ERROR)
	{
		return ErrorCode;
	}

	W5100_ReadRegister(SocketBase + W5100_OFFSET_SR, &Status);
	if(Status == W5100_SOCK_INIT)
	{
		return W5100_NO_ERROR;
	}

	return W5100_SOCKET_ERROR;
}

W5100_ErrorCode_t W5100_Disconnect(uint8_t Socket, uint32_t Timeout)
{
	return W5100_ExecuteCommand(Socket, W5100_CMD_DISCON, Timeout);
}

W5100_ErrorCode_t W5100_Close(uint8_t Socket, uint32_t Timeout)
{
	return W5100_ExecuteCommand(Socket, W5100_CMD_CLOSE, Timeout);
}

W5100_ErrorCode_t W5100_Listen(uint8_t Socket, uint32_t Timeout)
{
	uint8_t Status;
	uint16_t SocketBase;

	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}
	else if(Socket > 0x04)
	{
		return W5100_INVALID_PARAM;
	}

	SocketBase = W5100_SOCKET_ADDR(Socket);

	W5100_ReadRegister(SocketBase + W5100_OFFSET_SR, &Status);
	if(Status == W5100_SOCK_INIT)
	{
		W5100_ErrorCode_t ErrorCode = W5100_ExecuteCommand(Socket, W5100_CMD_LISTEN, Timeout);
		if(ErrorCode != W5100_NO_ERROR)
		{
			return ErrorCode;
		}

		W5100_ReadRegister(SocketBase + W5100_OFFSET_SR, &Status);
		if(Status == W5100_SOCK_LISTEN)
		{
			return W5100_NO_ERROR;
		}
	}

	W5100_Close(Socket, Timeout);

	return W5100_SOCKET_ERROR;
}

W5100_ErrorCode_t W5100_GetBytes(uint8_t Socket, uint16_t* Bytes)
{
	uint8_t Temp[2];
	uint16_t SocketBase;

	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}
	else if((Socket > 0x04) || (Bytes == NULL))
	{
		return W5100_INVALID_PARAM;
	}

	SocketBase = W5100_SOCKET_ADDR(Socket);

	W5100_ReadRegister(SocketBase + W5100_OFFSET_RX_RSR0, &Temp[0]);
	W5100_ReadRegister(SocketBase + W5100_OFFSET_RX_RSR1, &Temp[1]);

	*Bytes = ((uint16_t)(Temp[0] << 0x08)) | Temp[1];

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_Receive(uint8_t Socket, uint8_t* Buffer, uint16_t Length)
{
	uint8_t Temp[2];
	uint8_t Mask;
	uint16_t Offset;
	uint16_t RdPointer;
	uint16_t SocketBase;
	uint16_t SocketBufAddr;
	uint16_t SocketMemory = 0x00;

	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}
	else if((Socket > 0x04) || (Buffer == NULL) || (Length == 0x00))
	{
		return W5100_INVALID_PARAM;
	}

	SocketBufAddr = W5100_RX_BUFFER_BASE;
	SocketBase = W5100_SOCKET_ADDR(Socket);

	W5100_ReadRegister(W5100_REGISTER_RMSR, &Mask);
	for(uint8_t i = 0x00; i <= Socket ; i++)
	{
		SocketBufAddr += SocketMemory;
		SocketMemory = 1024 << ((Mask >> (i << 0x01)) & 0x03);
	}

	// Get the read pointer offset
	W5100_ReadRegister(SocketBase + W5100_OFFSET_RX_RSR0, &Temp[0]);
	W5100_ReadRegister(SocketBase + W5100_OFFSET_RX_RSR1, &Temp[1]);
	RdPointer = (((uint16_t)(Temp[0] << 0x08)) | Temp[1]);

	Offset = RdPointer & (SocketMemory - 0x01);

	// Check for an overflow
	if((Offset + Length) > SocketMemory)
	{
		uint16_t Upper;
		uint16_t Left;

		Upper = SocketMemory - Offset;
		Left = Length - Upper;

		W5100_ReadMemory(SocketBufAddr + Offset, Buffer, Upper);
		W5100_ReadMemory(SocketBufAddr, &Buffer[Upper], Left);
	}
	else
	{
		W5100_ReadMemory(SocketBufAddr + Offset, Buffer, Length);
	}

	return W5100_NO_ERROR;
}

W5100_ErrorCode_t W5100_Send(uint8_t Socket, uint8_t* Buffer, uint16_t Length, uint32_t Timeout)
{
	uint8_t Temp[2];
	uint8_t Mask;
	uint16_t Offset;
	uint16_t WrPointer;
	uint16_t SocketBase;
	uint16_t SocketBufAddr;
	uint16_t SocketMemory = 0x00;

	if(!_W5100_IsInitialized)
	{
		return W5100_NOT_INITIALIZED;
	}
	else if((Socket > 0x04) || (Buffer == NULL) || (Length == 0x00))
	{
		return W5100_INVALID_PARAM;
	}

	SocketBufAddr = W5100_TX_BUFFER_BASE;
	SocketBase = W5100_SOCKET_ADDR(Socket);

	W5100_ReadRegister(W5100_REGISTER_TMSR, &Mask);
	for(uint8_t i = 0x00; i <= Socket ; i++)
	{
		SocketBufAddr += SocketMemory;
		SocketMemory = 1024 << ((Mask >> (i << 0x01)) & 0x03);
	}

	// Get the write pointer address
	W5100_ReadRegister(SocketBase + W5100_OFFSET_TX_WR0, &Temp[0]);
	W5100_ReadRegister(SocketBase + W5100_OFFSET_TX_WR1, &Temp[1]);
	WrPointer = (((uint16_t)(Temp[0] << 0x08)) | Temp[1]);

	Offset = WrPointer & (SocketMemory - 0x01);

	// Check for an overflow
	if(Offset + Length > SocketMemory)
	{
		uint16_t Upper;
		uint16_t Left;

		Upper = SocketMemory - Offset;
		Left = Length - Upper;

		W5100_WriteMemory(SocketBufAddr + Offset, Buffer, Upper);
		W5100_WriteMemory(SocketBufAddr, &Buffer[Upper], Left);
	}
	else
	{
		W5100_WriteMemory(SocketBufAddr + Offset, Buffer, Length);
	}

	WrPointer += Length;

	// Write the new pointer back
	W5100_WriteRegister(SocketBase + W5100_OFFSET_TX_WR0, WrPointer >> 0x08);
	W5100_WriteRegister(SocketBase + W5100_OFFSET_TX_WR1, WrPointer & 0xFF);

	return W5100_ExecuteCommand(Socket, W5100_CMD_SEND, Timeout);
}