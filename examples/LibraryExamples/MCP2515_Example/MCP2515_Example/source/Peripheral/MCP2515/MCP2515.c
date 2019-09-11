/*
 * MCP2515.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 * File info: Driver for MCP2515 CAN controller.

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

/** @file Peripheral/MCP2515/MCP2515.c
 *  @brief Driver for Microchip MCP2515 CAN controller.
 *
 *  This file contains the implementation of the CAN controller driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/MCP2515/MCP2515.h"

/** @defgroup MCP2515
 *  @{
 */
	/** @defgroup MCP2515-Commands
	 *  MCP2515 commands.
	 *  @{
	 */
		#define MCP2515_CMD_RESET		0xC0
		#define MCP2515_CMD_READ		0x03
		#define MCP2515_CMD_READRX0		0x90
		#define MCP2515_CMD_READRX1		0x96
		#define MCP2515_CMD_WRITE		0x02
		#define MCP2515_CMD_RXSTATUS	0xB0
		#define MCP2515_CMD_BITMODIFY	0x05
		#define MCP2515_CMD_RTS0		0x81
		#define MCP2515_CMD_RTS1		0x82
		#define MCP2515_CMD_RTS2		0x84
	/** @} */ // end of MCP2515-Commands
	 
	/** @defgroup MCP2515-Errors
	 *  MCP2515 error codes.
	 *  @{
	 */
		#define MCP2515_REGISTER_TEC	0x1C
		#define MCP2515_REGISTER_REC	0x1D
		#define MCP2515_REGISTER_EFLG	0x2D
	/** @} */ // end of MCP2515-MCP2515
	 
	/** @defgroup MCP2515-Register
	 *  MCP2515 register addresses.
	 *  @{
	 */
		/** @defgroup MCP2515-Register-Control
		 *  MCP2515 control register.
		 *  @{
		 */
			#define MCP2515_REGISTER_CANSTAT	0x0E
			#define MCP2515_REGISTER_CANCTRL	0x0F
			#define MCP2515_REGISTER_CNF3		0x28
			#define MCP2515_REGISTER_CNF2		0x29
			#define MCP2515_REGISTER_CNF1		0x2A
			#define MCP2515_REGISTER_CANINTE	0x2B
			#define MCP2515_REGISTER_CANINTF	0x2C
		/** @} */ // end of MCP2515-Register-Control

		/** @defgroup MCP2515-Register-Transmit
		 *  MCP2515 Transmit buffer register.
		 *  @{
		 */
			#define MCP2515_REGISTER_TXB0CTRL	0x30
			#define MCP2515_REGISTER_TXB0SIDH	0x31
			#define MCP2515_REGISTER_TXB0SIDL	0x32
			#define MCP2515_REGISTER_TXB0EID8	0x33
			#define MCP2515_REGISTER_TXB0EID0	0x34
			#define MCP2515_REGISTER_TXB0DLC	0x35
			#define MCP2515_REGISTER_TXB0D0		0x36
			#define MCP2515_REGISTER_TXB0D1		0x37
			#define MCP2515_REGISTER_TXB0D2		0x38
			#define MCP2515_REGISTER_TXB0D3		0x39
			#define MCP2515_REGISTER_TXB0D4		0x3A
			#define MCP2515_REGISTER_TXB0D5		0x3B
			#define MCP2515_REGISTER_TXB0D6		0x3C
			#define MCP2515_REGISTER_TXB0D7		0x3D

			#define MCP2515_REGISTER_TXB1CTRL	0x40
			#define MCP2515_REGISTER_TXB1SIDH	0x41
			#define MCP2515_REGISTER_TXB1SIDL	0x42
			#define MCP2515_REGISTER_TXB1EID8	0x43
			#define MCP2515_REGISTER_TXB1EID0	0x44
			#define MCP2515_REGISTER_TXB1DLC	0x45
			#define MCP2515_REGISTER_TXB1D0		0x46
			#define MCP2515_REGISTER_TXB1D1		0x47
			#define MCP2515_REGISTER_TXB1D2		0x48
			#define MCP2515_REGISTER_TXB1D3		0x49
			#define MCP2515_REGISTER_TXB1D4		0x4A
			#define MCP2515_REGISTER_TXB1D5		0x4B
			#define MCP2515_REGISTER_TXB1D6		0x4C
			#define MCP2515_REGISTER_TXB1D7		0x4D

			#define MCP2515_REGISTER_TXB2CTRL	0x50
			#define MCP2515_REGISTER_TXB2SIDH	0x51
			#define MCP2515_REGISTER_TXB2SIDL	0x52
			#define MCP2515_REGISTER_TXB2EID8	0x53
			#define MCP2515_REGISTER_TXB2EID0	0x54
			#define MCP2515_REGISTER_TXB2DLC	0x55
			#define MCP2515_REGISTER_TXB2D0		0x56
			#define MCP2515_REGISTER_TXB2D1		0x57
			#define MCP2515_REGISTER_TXB2D2		0x58
			#define MCP2515_REGISTER_TXB2D3		0x59
			#define MCP2515_REGISTER_TXB2D4		0x5A
			#define MCP2515_REGISTER_TXB2D5		0x5B
			#define MCP2515_REGISTER_TXB2D6		0x5C
			#define MCP2515_REGISTER_TXB2D7		0x5D
		/** @} */ // end of MCP2515-Register-Transmit

		/** @defgroup MCP2515-Register-Receive
		  *  MCP2515 Receive buffer register.
		  *  @{
		  */
			#define MCP2515_REGISTER_RXB0CTRL	0x60
			#define MCP2515_REGISTER_RXB0SIDH	0x61
			#define MCP2515_REGISTER_RXB0SIDL	0x62
			#define MCP2515_REGISTER_RXB0EID8	0x63
			#define MCP2515_REGISTER_RXB0EID0	0x64
			#define MCP2515_REGISTER_RXB0DLC	0x65
			#define MCP2515_REGISTER_RXB0D0		0x66
			#define MCP2515_REGISTER_RXB0D1		0x67
			#define MCP2515_REGISTER_RXB0D2		0x68
			#define MCP2515_REGISTER_RXB0D3		0x69
			#define MCP2515_REGISTER_RXB0D4		0x6A
			#define MCP2515_REGISTER_RXB0D5		0x6B
			#define MCP2515_REGISTER_RXB0D6		0x6C
			#define MCP2515_REGISTER_RXB0D7		0x6D

			#define MCP2515_REGISTER_RXB1CTRL	0x70
			#define MCP2515_REGISTER_RXB1SIDH	0x71
			#define MCP2515_REGISTER_RXB1SIDL	0x72
			#define MCP2515_REGISTER_RXB1EID8	0x73
			#define MCP2515_REGISTER_RXB1EID0	0x74
			#define MCP2515_REGISTER_RXB1DLC	0x75
			#define MCP2515_REGISTER_RXB1D0		0x76
			#define MCP2515_REGISTER_RXB1D1		0x77
			#define MCP2515_REGISTER_RXB1D2		0x78
			#define MCP2515_REGISTER_RXB1D3		0x79
			#define MCP2515_REGISTER_RXB1D4		0x7A
			#define MCP2515_REGISTER_RXB1D5		0x7B
			#define MCP2515_REGISTER_RXB1D6		0x7C
			#define MCP2515_REGISTER_RXB1D7		0x7D
		/** @} */ // end of MCP2515-Register-Receive

		/** @defgroup MCP2515-Register-Filter
		 *  MCP2515 Receive filter register.
		 *  @{
		 */
			#define MCP2515_REGISTER_RXF0SIDH	0x00
			#define MCP2515_REGISTER_RXF0SIDL	0x01
			#define MCP2515_REGISTER_RXF0EID8	0x02
			#define MCP2515_REGISTER_RXF0EID0	0x03
			#define MCP2515_REGISTER_RXF1SIDH	0x04
			#define MCP2515_REGISTER_RXF1SIDL	0x05
			#define MCP2515_REGISTER_RXF1EID8	0x06
			#define MCP2515_REGISTER_RXF1EID0	0x07
			#define MCP2515_REGISTER_RXF2SIDH	0x08
			#define MCP2515_REGISTER_RXF2SIDL	0x09
			#define MCP2515_REGISTER_RXF2EID8	0x0A
			#define MCP2515_REGISTER_RXF2EID0	0x0B
			#define MCP2515_REGISTER_RXF3SIDH	0x10
			#define MCP2515_REGISTER_RXF3SIDL	0x11
			#define MCP2515_REGISTER_RXF3SID8	0x12
			#define MCP2515_REGISTER_RXF3EID0	0x13
			#define MCP2515_REGISTER_RXF4SIDH	0x14
			#define MCP2515_REGISTER_RXF4SIDL	0x15
			#define MCP2515_REGISTER_RXF4EID8	0x16
			#define MCP2515_REGISTER_RXF4EID0	0x17
			#define MCP2515_REGISTER_RXF5SIDH	0x18
			#define MCP2515_REGISTER_RXF5SIDL	0x19
			#define MCP2515_REGISTER_RXF5SID8	0x1A
			#define MCP2515_REGISTER_RXF5EID0	0x1B
		/** @} */ // end of MCP2515-Register-Filter

		/** @defgroup MCP2515-Register-Mask
		 *  MCP2515 Receive mask register.
		 *  @{
		 */
			#define MCP2515_REGISTER_RXM0SIDH	0x20
			#define MCP2515_REGISTER_RXM0SIDL	0x21
			#define MCP2515_REGISTER_RXM0EIDH	0x22
			#define MCP2515_REGISTER_RXM0EIDL	0x23
			#define MCP2515_REGISTER_RXM1SIDH	0x24
			#define MCP2515_REGISTER_RXM1SIDL	0x25
			#define MCP2515_REGISTER_RXM1EIDH	0x26
			#define MCP2515_REGISTER_RXM1EIDL	0x27
		/** @} */ // end of MCP2515-Register-Mask
	/** @} */ // end of MCP2515-Register
	
 	/** @defgroup MCP2515-Flags
	 *  MCP2515 status flags.
	 *  @{
	 */
		/** @defgroup MCP2515-Flags-Interrupt
		 *  MCP2515 interrupt flags.
		 *  @{
		 */
			#define MCP2515_IF_RX0				0x00
			#define MCP2515_IF_RX1				0x01
			#define MCP2515_IF_TX0				0x02
			#define MCP2515_IF_TX1				0x03
			#define MCP2515_IF_TX2				0x04
			#define MCP2515_IF_ERR				0x05
			#define MCP2515_IF_WAK				0x06
			#define MCP2515_IF_MER				0x07
		/** @} */ // end of MCP2515-Flags-Interrupt
		
		/** @defgroup MCP2515-Flags-Error
		 *  MCP2515 error flags.
		 *  @{
		 */
			#define MCP2515_WEARN				0x00
			#define MCP2515_RXWAR				0x01
			#define MCP2515_TWWAR				0x02
			#define MCP2515_RXEP				0x03
			#define MCP2515_TXEP				0x04
			#define MCP2515_TXBO				0x05
			#define MCP2515_RX0OVR				0x06
			#define MCP2515_RX1OVR				0x07
		/** @} */ // end of MCP2515-Error-Interrupt
	/** @} */ // end of MCP2515-Flags
/** @} */ // end of MCP2515

#if(MCU_ARCH == MCU_ARCH_AVR8)
	#define MCP2515_SPI_INIT(Config)							SPIM_Init(Config)												/**< Generic SPI initialization macro for the flash interface */
	#define MCP2515_SPI_TRANSMIT(Command)						SPIM_SendData(Command)											/**< Generic SPI transmission macro for the flash interface  */
	#define MCP2515_SPI_CHIP_SELECT()							SPIM_SelectDevice(MCP2515_SS_PORT, MCP2515_SS_PIN)				/**< Generic chip select macro for the flash interface */
	#define MCP2515_SPI_CHIP_DESELECT()							SPIM_DeselectDevice(MCP2515_SS_PORT, MCP2515_SS_PIN)			/**< Generic chip deselect macro for the display interface */
#elif(MCU_ARCH == MCU_ARCH_XMEGA)
	#if(MCP2515_INTERFACE_TYPE == INTERFACE_USART_SPI)
		#define MCP2515_SPI_INIT(Config)						USART_SPI_Init(Config)											/**< Generic SPI initialization macro for the flash interface */
		#define MCP2515_SPI_TRANSMIT(Command)					USART_SPI_SendData(MCP2515_INTERFACE, Command)					/**< Generic SPI transmission macro for the flash interface  */
		#define MCP2515_SPI_CHIP_SELECT()						USART_SPI_SelectDevice(MCP2515_SS_PORT, MCP2515_SS_PIN)			/**< Generic chip select macro for the flash interface */
		#define MCP2515_SPI_CHIP_DESELECT()						USART_SPI_DeselectDevice(MCP2515_SS_PORT, MCP2515_SS_PIN)		/**< Generic chip deselect macro for the display interface */

	 #elif(MCP2515_INTERFACE_TYPE == INTERFACE_SPI) 
		#define MCP2515_SPI_INIT(Config)						SPIM_Init(Config)												/**< Generic SPI initialization macro for the flash interface */
		#define MCP2515_SPI_TRANSMIT(Command)					SPIM_SendData(MCP2515_INTERFACE, Command)						/**< Generic SPI transmission macro for the flash interface  */
		#define MCP2515_SPI_CHIP_SELECT()						SPIM_SelectDevice(MCP2515_SS_PORT, MCP2515_SS_PIN)				/**< Generic chip select macro for the flash interface */
		#define MCP2515_SPI_CHIP_DESELECT()						SPIM_DeselectDevice(MCP2515_SS_PORT, MCP2515_SS_PIN)			/**< Generic chip deselect macro for the display interface */
	 #else
		 #error "Invalid interface for MCP2515!!"
	 #endif
 #else
	#error "Architecture not supported!"
 #endif

#ifndef DOXYGEN
	static struct
	{
		MCP2515_MessageCallback_t MessageCallback;
		MCP2515_MessageCallback_t StandardDataCallback;
		MCP2515_MessageCallback_t ExtendetDataCallback;
		MCP2515_MessageCallback_t StandardRemoteCallback;
		MCP2515_MessageCallback_t ExtendedRemoteCallback;
	} MCP2515_Callbacks;
#endif

/** @brief	MCP2515 interrupt handler.
 */
static inline void MCP2515_Interrupthandler(void)
{
	static CAN_Message_t Message;

	uint8_t Status = MCP2515_ReadRegister(MCP2515_REGISTER_CANINTF);

	if((Status & (0x01 << MCP2515_IF_RX0)) || (Status & (0x01 << MCP2515_IF_RX1)))
	{
		MCP2515_ReadMessage(&Message);

		// General message callback for all messages
		if(MCP2515_Callbacks.MessageCallback != NULL)
		{
			MCP2515_Callbacks.MessageCallback(Message);
		}

		// Message specific callbacks
		if(Message.MessageType == MCP2515_STANDARD_DATA)
		{
			if(MCP2515_Callbacks.StandardDataCallback != NULL)
			{
				MCP2515_Callbacks.StandardDataCallback(Message);
			}
		}
		else if(Message.MessageType == MCP2515_EXTENDED_DATA)
		{
			if(MCP2515_Callbacks.ExtendetDataCallback != NULL)
			{
				MCP2515_Callbacks.ExtendetDataCallback(Message);
			}
		}
		else if(Message.MessageType == MCP2515_STANDARD_REMOTE)
		{
			if(MCP2515_Callbacks.StandardRemoteCallback != NULL)
			{
				MCP2515_Callbacks.StandardRemoteCallback(Message);
			}
		}
		else if(Message.MessageType == MCP2515_EXTENDED_REMOTE)
		{
			if(MCP2515_Callbacks.ExtendedRemoteCallback != NULL)
			{
				MCP2515_Callbacks.ExtendedRemoteCallback(Message);
			}			
		}
	}
}

void MCP2515_Init(SPIM_Config_t* Config, const MCP2515_InterruptConfig_t* ConfigInterrupt)
{
	// Initialize CS Pin
	GPIO_SetDirection(MCP2515_SS_PORT, MCP2515_SS_PIN, GPIO_DIRECTION_OUT);
	GPIO_Set(MCP2515_SS_PORT, MCP2515_SS_PIN);

	// Initialize SPI-Interface
	if(Config != NULL)
	{
		MCP2515_SPI_INIT(Config);
	}

	// Setup Interrupts
	MCP2515_EnableInterrupts(ConfigInterrupt);

	// Initialize MCP2515
	MCP2515_Reset();
	MCP2515_SetDeviceMode(MCP2515_CONFIG_MODE);
	MCP2515_WriteRegister(MCP2515_REGISTER_CANCTRL, 0x08);
	
	MCP2515_SetTxPriority(MCP2515_TX0, MCP2515_PRIO_HIGHEST);

	MCP2515_WriteRegister(MCP2515_REGISTER_CNF1, 0x03);
	MCP2515_WriteRegister(MCP2515_REGISTER_CNF2, 0xA0);
	MCP2515_WriteRegister(MCP2515_REGISTER_CNF3, 0x02);

	// Disable receive filter
	MCP2515_WriteRegister(MCP2515_REGISTER_RXB0CTRL, 0x60);
	MCP2515_WriteRegister(MCP2515_REGISTER_RXB1CTRL, 0x60);
	
	// Clear all interrupts
	MCP2515_WriteRegister(MCP2515_REGISTER_CANINTF, 0x00);

	// Switch the device back into normal mode
	MCP2515_SetDeviceMode(MCP2515_NORMAL_MODE);
}

void MCP2515_Reset(void)
{
	MCP2515_SPI_CHIP_SELECT();

	MCP2515_SPI_TRANSMIT(MCP2515_CMD_RESET);

	// Wait until the reset has finished
	for(uint8_t i = 0x00; i < 0xFF; i++);

	MCP2515_SPI_CHIP_DESELECT();
}

void MCP2515_EnableInterrupts(const MCP2515_InterruptConfig_t* Config)
{
	// Initialize interrupt pin
	GPIO_SetDirection(Config->Port, Config->Pin, GPIO_DIRECTION_IN);
	if(Config->InternalPullUp == TRUE)
	{
		GPIO_Set(Config->Port, Config->Pin);
	}
	
	GPIO_InterruptConfig_t ConfigGPIO = {
		.Channel = Config->IOChannel,
		.Type = GPIO_SENSE_FALLING,
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			.Pin = Config->Pin,
			.Port = Config->Port,
			.InterruptLevel = Config->InterruptLevel,
		#endif
		.Callback = MCP2515_Interrupthandler
	};
    
    GPIO_InstallCallback(&ConfigGPIO);
	
	// Enable device interrupts
	MCP2515_WriteRegister(MCP2515_REGISTER_CANINTE, 0x41);
}

void MCP2515_DisableInterrupts(const MCP2515_InterruptConfig_t* Config)
{
	#if(MCU_ARCH == MCU_ARCH_AVR8)
		GPIO_RemoveCallback(Config->IOChannel);
	#elif(MCU_ARCH == MCU_ARCH_XMEGA)
		GPIO_RemoveCallback(Config->Port, Config->IOChannel);
	#endif
}

void MCP2515_InstallCallback(const MCP2515_InterruptConfig_t* Config)
{
	if(Config->Source & MCP2515_MESSAGE_INTERRUPT)
	{		
		if(Config->MessageCallback != NULL)
		{
			MCP2515_Callbacks.MessageCallback = Config->MessageCallback;
		}
	}
	
	if(Config->Source & MCP2515_STANDARD_DATA_INTERRUPT)
	{
		if(Config->MessageCallback != NULL)
		{
			MCP2515_Callbacks.StandardDataCallback = Config->MessageCallback;
		}		
	}
	
	if(Config->Source & MCP2515_EXTENDED_DATA_INTERRUPT)
	{
		if(Config->MessageCallback != NULL)
		{
			MCP2515_Callbacks.ExtendetDataCallback = Config->MessageCallback;
		}
	}
	
	if(Config->Source & MCP2515_STANDARD_REMOTE_INTERRUPT)
	{
		if(Config->MessageCallback != NULL)
		{
			MCP2515_Callbacks.StandardRemoteCallback = Config->MessageCallback;
		}
	}
	
	if(Config->Source & MCP2515_EXTENDED_REMOTE_INTERRUPT)
	{
		if(Config->MessageCallback != NULL)
		{
			MCP2515_Callbacks.ExtendedRemoteCallback = Config->MessageCallback;
		}
	}
}

void MCP2515_RemoveCallback(const MCP2515_CallbackType_t Callback)
{
	if(Callback & MCP2515_MESSAGE_INTERRUPT)
	{
		MCP2515_Callbacks.MessageCallback = NULL;
	}
	
	if(Callback & MCP2515_STANDARD_DATA_INTERRUPT)
	{
		MCP2515_Callbacks.StandardDataCallback = NULL;
	}
	
	if(Callback & MCP2515_EXTENDED_DATA_INTERRUPT)
	{
		MCP2515_Callbacks.ExtendetDataCallback = NULL;
	}
	
	if(Callback & MCP2515_STANDARD_REMOTE_INTERRUPT)
	{
		MCP2515_Callbacks.StandardRemoteCallback = NULL;
	}
	
	if(Callback & MCP2515_EXTENDED_REMOTE_INTERRUPT)
	{
		MCP2515_Callbacks.ExtendedRemoteCallback = NULL;
	}
}

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	void MCP2515_ChangeInterruptLevel(const MCP2515_InterruptConfig_t* Config)
	{
		GPIO_ChangeInterruptLevel(Config->Port, Config->Pin, Config->IOChannel, Config->InterruptLevel);
	}
#endif

void MCP2515_BitModify(const uint8_t Address, const uint8_t Mask, const uint8_t Value)
{
	MCP2515_SPI_CHIP_SELECT();

	MCP2515_SPI_TRANSMIT(MCP2515_CMD_BITMODIFY);
	MCP2515_SPI_TRANSMIT(Address);
	MCP2515_SPI_TRANSMIT(Mask);
	MCP2515_SPI_TRANSMIT(Value);

	MCP2515_SPI_CHIP_DESELECT();
}

void MCP2515_WriteRegister(const uint8_t Address, const uint8_t Data)
{
	MCP2515_SPI_CHIP_SELECT();

	MCP2515_SPI_TRANSMIT(MCP2515_CMD_WRITE);
	MCP2515_SPI_TRANSMIT(Address);
	MCP2515_SPI_TRANSMIT(Data);

	MCP2515_SPI_CHIP_DESELECT();
}

uint8_t MCP2515_ReadRegister(const uint8_t Address)
{
	uint8_t Data = 0x00;

	MCP2515_SPI_CHIP_SELECT();

	MCP2515_SPI_TRANSMIT(MCP2515_CMD_READ);
	MCP2515_SPI_TRANSMIT(Address);
	Data = MCP2515_SPI_TRANSMIT(0xFF);

	MCP2515_SPI_CHIP_DESELECT();
	
	return Data;
}

void MCP2515_SetDeviceMode(const MCP2515_DeviceMode_t Mode)
{
	MCP2515_BitModify(MCP2515_REGISTER_CANCTRL, 0xE0, (Mode << 0x05));
}

MCP2515_DeviceMode_t MCP2515_GetDeviceMode(void)
{
	return ((MCP2515_ReadRegister(MCP2515_REGISTER_CANSTAT) & 0xE0) >> 0x05);
}

void MCP2515_SetTxPriority(const MCP2515_TransmitBuffer_t Buffer, const MCP2515_MessagePriority_t Priority)
{
	if(Buffer == MCP2515_TX0)
	{
		MCP2515_BitModify(MCP2515_REGISTER_TXB0CTRL, 0x03, Priority);
	}
	else if(Buffer == MCP2515_TX1)
	{
		MCP2515_BitModify(MCP2515_REGISTER_TXB1CTRL, 0x03, Priority);
	}
	else if(Buffer == MCP2515_TX2)
	{
		MCP2515_BitModify(MCP2515_REGISTER_TXB2CTRL, 0x03, Priority);
	}
}

void MCP2515_SetRxFilter(const MCP2515_ReceiveBuffer_t Buffer, const MCP2515_Filter_t Filter)
{
	if(Buffer == MCP2515_RX0)
	{
		MCP2515_BitModify(MCP2515_REGISTER_RXB0CTRL, 0x60, Filter << 0x05);
	}
	else if(Buffer == MCP2515_RX1)
	{
		MCP2515_BitModify(MCP2515_REGISTER_RXB1CTRL, 0x60, Filter << 0x05);
	}
}

void MCP2515_ConfigFilter(const MCP2515_FilterConfig_t* Config)
{
	// Get the current device mode
	MCP2515_DeviceMode_t Mode = MCP2515_GetDeviceMode();

	// Change mode to configuration mode to change the filter setup
	MCP2515_SetDeviceMode(MCP2515_CONFIG_MODE);

	uint8_t Identifier_Low = (Config->FilterID & 0x07) << 0x05;
	uint8_t Identifier_High = Config->FilterID >> 0x03;

	uint8_t Mask_Low = (Config->FilterMask & 0x07) << 0x05;
	uint8_t Mask_High = Config->FilterMask >> 0x03;

	MCP2515_WriteRegister(Config->Filter, Identifier_High);
	MCP2515_WriteRegister(Config->Filter + 0x01, Identifier_Low);

	if((Config->Filter == MCP2515_RXF_0) | (Config->Filter == MCP2515_RXF_1))
	{
		MCP2515_WriteRegister(MCP2515_REGISTER_RXM0SIDH, Mask_High);
		MCP2515_WriteRegister(MCP2515_REGISTER_RXM0SIDL, Mask_Low);
	}
	else if((Config->Filter == MCP2515_RXF_2) | (Config->Filter == MCP2515_RXF_3) | (Config->Filter == MCP2515_RXF_4) | (Config->Filter == MCP2515_RXF_5))
	{
		MCP2515_WriteRegister(MCP2515_REGISTER_RXM1SIDH, Mask_High);
		MCP2515_WriteRegister(MCP2515_REGISTER_RXM1SIDL, Mask_Low);
	}

	// Restore device mode
	MCP2515_SetDeviceMode(Mode);
}

void MCP2515_StartTransmission(const MCP2515_TransmitBuffer_t Buffer)
{
	MCP2515_SPI_CHIP_SELECT();

	uint8_t Command = 0x00;

	if(Buffer == MCP2515_TX0)
	{
		Command = MCP2515_CMD_RTS0;
	}
	else if(Buffer == MCP2515_TX1)
	{
		Command = MCP2515_CMD_RTS1;
	}
	else if(Buffer == MCP2515_TX2)
	{
		Command = MCP2515_CMD_RTS2;
	}

	MCP2515_SPI_TRANSMIT(Command);

	MCP2515_SPI_CHIP_DESELECT();
}

void MCP2515_ReadMessage(CAN_Message_t* Message)
{
	uint8_t ID_High = 0x00;
	uint8_t ID_Low = 0x00;
	uint8_t Length = 0x00;
	uint8_t ReceiveBufferAddress = 0x00;

	// Get the receive status
	uint8_t RxStatus = MCP2515_ReadRxStatus();

	// Check witch receive buffer contains the message
	if(RxStatus & 0x40)
	{
		ReceiveBufferAddress = MCP2515_CMD_READRX0;
	}
	else if(RxStatus & 0x80)
	{
		ReceiveBufferAddress = MCP2515_CMD_READRX1;
	}

	MCP2515_SPI_CHIP_SELECT();
	
	MCP2515_SPI_TRANSMIT(ReceiveBufferAddress);
	
	// Get the message ID
	ID_High = (MCP2515_SPI_TRANSMIT(0xFF)) << 0x03;
	ID_Low = (MCP2515_SPI_TRANSMIT(0xFF)) >> 0x05;
	Message->MessageID = ID_High | ID_Low;

	// Read the two empty bytes
	MCP2515_SPI_TRANSMIT(0xFF);
	MCP2515_SPI_TRANSMIT(0xFF);

	// Get the message length
	Length = MCP2515_SPI_TRANSMIT(0xFF);

	// Check message type
	uint8_t MessageType = (RxStatus & 0x18) >> 0x03;
	
	switch(MessageType)
	{
		case(MCP2515_STANDARD_DATA):
		{
			Message->MessageLength = Length;
			Message->MessageType = MessageType;

			for(uint8_t i = 0; i < Message->MessageLength; i++)
			{
				*(Message->Buffer++) = MCP2515_SPI_TRANSMIT(0xFF);
			}

			break;
		}
		case(MCP2515_STANDARD_REMOTE):
		{
			// Remove the remote bit
			Message->MessageLength = Length - 0x40;

			Message->MessageType = MessageType;

			break;
		}
	}
	
	MCP2515_SPI_CHIP_DESELECT();
}

MCP2515_ErrorCodes_t MCP2515_SendMessage(CAN_Message_t* Message)
{
	if(Message->MessageID > 0x7FF)
	{
		return MCP2515_INVALID_IDENTIFIER;
	}
	
	// Delete TXREQ Bit
	MCP2515_BitModify(MCP2515_REGISTER_TXB0CTRL, 0x08, 0x00);

	// Set the message id
	MCP2515_SetIdentifier(MCP2515_TX0, Message->MessageID);

	// Set the message priority
	MCP2515_SetTxPriority(MCP2515_TX0, Message->MessagePriority);

	if(Message->MessageType == MCP2515_STANDARD_DATA)
	{
		MCP2515_WriteRegister(MCP2515_REGISTER_TXB0DLC, Message->MessageLength);

		for(uint8_t DataByte = 0x00; DataByte < MCP2515_MAX_DATABYTES; DataByte++)
		{
			MCP2515_WriteRegister(MCP2515_REGISTER_TXB0D0 + DataByte, *(Message->Buffer++));
		}
	}
	else if(Message->MessageType == MCP2515_STANDARD_REMOTE)
	{
		MCP2515_WriteRegister(MCP2515_REGISTER_TXB0DLC,  Message->MessageLength + 0x40);
	}

	MCP2515_StartTransmission(MCP2515_TX0);

	return MCP2515_NO_ERROR;
}

void MCP2515_SetIdentifier(const MCP2515_TransmitBuffer_t Buffer, const uint32_t Identifier)
{
	uint8_t Identifier_Low = (Identifier & 0x07) << 0x05;
	uint8_t Identifier_High = Identifier >> 0x03;

	switch(Buffer)
	{
		case(MCP2515_TX0):
		{
			MCP2515_WriteRegister(MCP2515_REGISTER_TXB0SIDL, Identifier_Low);
			MCP2515_WriteRegister(MCP2515_REGISTER_TXB0SIDH, Identifier_High);
		}
		case(MCP2515_TX1):
		{
			MCP2515_WriteRegister(MCP2515_REGISTER_TXB1SIDL, Identifier_Low);
			MCP2515_WriteRegister(MCP2515_REGISTER_TXB1SIDH, Identifier_High);
		}
		case(MCP2515_TX2):
		{
			MCP2515_WriteRegister(MCP2515_REGISTER_TXB2SIDL, Identifier_Low);
			MCP2515_WriteRegister(MCP2515_REGISTER_TXB2SIDH, Identifier_High);
		}
	}
}

uint8_t MCP2515_ReadRxStatus(void)
{
	uint8_t Data;

	MCP2515_SPI_CHIP_SELECT();
	 
	MCP2515_SPI_TRANSMIT(MCP2515_CMD_RXSTATUS);
	Data = MCP2515_SPI_TRANSMIT(0xFF);

	// Get repeat data
	MCP2515_SPI_TRANSMIT(0xFF);

	MCP2515_SPI_CHIP_DESELECT();

	return Data;
}