/*
 * MCP2515.c
 *
 *  Copyright (C) Daniel Kampert, 2020
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
		#define MCP2515_CMD_WRITE				0x02
		#define MCP2515_CMD_READ				0x03
		#define MCP2515_CMD_BITMODIFY			0x05
		#define MCP2515_CMD_RTS(Buffer)			(0x80 | Buffer)
		#define MCP2515_CMD_READ_RX(Buffer)		(0x90 | (Buffer << 0x02))
		#define MCP2515_CMD_READ_STATUS			0xA0
		#define MCP2515_CMD_READ_RX_STATUS		0xB0
		#define MCP2515_CMD_RESET				0xC0
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
			#define MCP2515_REGISTER_TXRTSCTRL	0x0D
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
	
 	/** @defgroup MCP2515-Control-Bits
	 *  MCP2515 control bits.
	 *  @{
	 */
		#define MCP2515_BTLMODE					0x07
		#define MCP2515_MSG_RX_1				0x07
		#define MCP2515_SAM						0x06
		#define MCP2515_WAKFIL					0x06
		#define MCP2515_MSG_RX_0				0x06
		#define MCP2515_ABTF					0x06
		#define MCP2515_RTR						0x06
		#define MCP2515_RXM1					0x06
		#define MCP2515_RXM0					0x05
		#define MCP2515_MLOA					0x05
		#define MCP2515_ABAT					0x04
		#define MCP2515_TXERR					0x04
		#define MCP2515_MSG_TYPE1				0x04
		#define MCP2515_MSG_TYPE0				0x03
		#define MCP2515_EXIDE					0x03
		#define MCP2515_OSM						0x03
		#define MCP2515_TXREQ					0x03
		#define MCP2515_CLKEN					0x02
		#define MCP2515_BUKT					0x02
		#define MCP2515_B2RTSM					0x02
		#define MCP2515_B1RTSM					0x01
		#define MCP2515_TXP1					0x01
		#define MCP2515_TXP0					0x00
		#define MCP2515_B0RTSM					0x00
	/** @} */ // end of MCP2515-Control-Flags
/** @} */ // end of MCP2515

#if(MCU_ARCH == MCU_ARCH_AVR8)
	#define MCP2515_SPI_INIT(Config)							SPIM_Init(Config)
	#define MCP2515_SPI_TRANSMIT(Command)						SPIM_SendData(Command)
	#define MCP2515_SPI_CHIP_SELECT()							SPIM_SelectDevice(GET_PERIPHERAL(MCP2515_SS), GET_INDEX(MCP2515_SS))
	#define MCP2515_SPI_CHIP_DESELECT()							SPIM_DeselectDevice(GET_PERIPHERAL(MCP2515_SS), GET_INDEX(MCP2515_SS))
#elif(MCU_ARCH == MCU_ARCH_XMEGA)
	#if(MCP2515_INTERFACE_TYPE == INTERFACE_USART_SPI)
		#define MCP2515_SPI_INIT(Config)						USART_SPI_Init(Config)
		#define MCP2515_SPI_TRANSMIT(Command)					USART_SPI_SendData(&MCP2515_INTERFACE, Command)
		#define MCP2515_SPI_CHIP_SELECT()						USART_SPI_SelectDevice(GET_PERIPHERAL(MCP2515_SS), GET_INDEX(MCP2515_SS))
		#define MCP2515_SPI_CHIP_DESELECT()						USART_SPI_DeselectDevice(GET_PERIPHERAL(MCP2515_SS), GET_INDEX(MCP2515_SS))

	 #elif(MCP2515_INTERFACE_TYPE == INTERFACE_SPI) 
		#define MCP2515_SPI_INIT(Config)						SPIM_Init(Config)
		#define MCP2515_SPI_TRANSMIT(Command)					SPIM_SendData(&MCP2515_INTERFACE, Command)
		#define MCP2515_SPI_CHIP_SELECT()						SPIM_SelectDevice(GET_PERIPHERAL(MCP2515_SS), GET_INDEX(MCP2515_SS))
		#define MCP2515_SPI_CHIP_DESELECT()						SPIM_DeselectDevice(GET_PERIPHERAL(MCP2515_SS), GET_INDEX(MCP2515_SS))
	 #else
		 #error "Interface not supported for MCP2515!"
	 #endif
 #else
	#error "Architecture not supported for MCP2515!"
#endif

#define MCP2515_CLEAR_IF(Flag)									MCP2515_BitModify(MCP2515_REGISTER_CANINTF, Flag, 0x00);

#ifndef DOXYGEN
	static struct
	{
		MCP2515_Callback_t RxCallback;
		MCP2515_Callback_t TxCallback;
		MCP2515_Callback_t WakeCallback;
		MCP2515_ErrorCallback_t ErrorCallback;
	} __MCP2515_Callbacks;
#endif

/** @brief			Send a bit modify to the CAN controller.
 *  @param Address	Register address
 *  @param Mask		Register mask
 *  @param Value	Register value
 */
static void MCP2515_BitModify(const uint8_t Address, const uint8_t Mask, const uint8_t Value)
{
	MCP2515_SPI_CHIP_SELECT();

	MCP2515_SPI_TRANSMIT(MCP2515_CMD_BITMODIFY);
	MCP2515_SPI_TRANSMIT(Address);
	MCP2515_SPI_TRANSMIT(Mask);
	MCP2515_SPI_TRANSMIT(Value);

	MCP2515_SPI_CHIP_DESELECT();
}

/** @brief			Write into a register of the CAN controller.
 *  @param Address	Register Address
 *  @param Value	Register data
 */
static void MCP2515_WriteRegister(const uint8_t Address, const uint8_t Data)
{
	MCP2515_SPI_CHIP_SELECT();

	MCP2515_SPI_TRANSMIT(MCP2515_CMD_WRITE);
	MCP2515_SPI_TRANSMIT(Address);
	MCP2515_SPI_TRANSMIT(Data);

	MCP2515_SPI_CHIP_DESELECT();
}

/** @brief			Read a register of the CAN controller.
 *  @param Address	Register Address
 *  @return			Register data
 */
static uint8_t MCP2515_ReadRegister(const uint8_t Address)
{
	uint8_t Data = 0x00;

	MCP2515_SPI_CHIP_SELECT();

	MCP2515_SPI_TRANSMIT(MCP2515_CMD_READ);
	MCP2515_SPI_TRANSMIT(Address);
	Data = MCP2515_SPI_TRANSMIT(0xFF);

	MCP2515_SPI_CHIP_DESELECT();
	
	return Data;
}

/** @brief	Read the Rx status.
 *  @return	Rx status
 */
static uint8_t MCP2515_ReadRxStatus(void)
{
	uint8_t Data = 0x00;

	MCP2515_SPI_CHIP_SELECT();
	
	MCP2515_SPI_TRANSMIT(MCP2515_CMD_READ_RX_STATUS);
	Data = MCP2515_SPI_TRANSMIT(0xFF);
	MCP2515_SPI_TRANSMIT(0xFF);

	MCP2515_SPI_CHIP_DESELECT();

	return Data;
}

/** @brief	MCP2515 interrupt handler.
 */
static inline void MCP2515_Interrupthandler(void)
{
	// Get all active interrupt flags
	uint8_t Status = MCP2515_ReadRegister(MCP2515_REGISTER_CANINTF);

	// Message error interrupt
	if(Status & (0x01 << MCP2515_IF_MER))
	{
		MCP2515_CLEAR_IF(0x01 << MCP2515_IF_MER);
	}

	// Wakeup interrupt
	if(Status & (0x01 << MCP2515_IF_WAK))
	{
		MCP2515_CLEAR_IF(0x01 << MCP2515_IF_WAK);

		if(__MCP2515_Callbacks.WakeCallback != NULL)
		{
			__MCP2515_Callbacks.WakeCallback();
		}
	}

	// Error interrupt
	if(Status & (0x01 << MCP2515_IF_ERR))
	{
		MCP2515_CLEAR_IF(0x01 << MCP2515_IF_ERR);

		// Read the error flags
		// ToDo: How to clear error flags in callback?
		uint8_t ErrorFlags = MCP2515_ReadRegister(MCP2515_REGISTER_EFLG);
		
		if(__MCP2515_Callbacks.ErrorCallback != NULL)
		{
			__MCP2515_Callbacks.ErrorCallback(ErrorFlags);
		}
	}

	// Transmit buffer n full interrupt
	if((Status & (0x01 << MCP2515_IF_TX2)) || (Status & (0x01 << MCP2515_IF_TX1)) || (Status & (0x01 << MCP2515_IF_TX0)))
	{
		MCP2515_CLEAR_IF((0x01 << MCP2515_IF_TX2) | (0x01 << MCP2515_IF_TX1) | (0x01 << MCP2515_IF_TX0));
	}

	// Receive buffer n full interrupt
	if((Status & (0x01 << MCP2515_IF_RX1)) || (Status & (0x01 << MCP2515_IF_RX0)))
	{
		// General message callback for all messages
		if(__MCP2515_Callbacks.RxCallback != NULL)
		{
			__MCP2515_Callbacks.RxCallback();
		}
	}
}

/** @brief			Enable interrupt support for the MCU.
 *  @param Config	Pointer to MCP2515 configuration object
 */
static void MCP2515_EnableInterrupts(const MCP2515_Config_t* Config)
{
	// Initialize interrupt pin
	GPIO_SetDirection(Config->Port, Config->Pin, GPIO_DIRECTION_IN);
	
	GPIO_InterruptConfig_t ConfigGPIO = {
		.Channel = Config->Channel,
		.Sense = GPIO_SENSE_LOWLEVEL,
		#if(MCU_ARCH == MCU_ARCH_XMEGA)
			.Pin = Config->Pin,
			.Port = Config->Port,
			.InterruptLevel = Config->InterruptLevel,
		#endif
		.Callback = MCP2515_Interrupthandler
	};
	
	GPIO_InstallCallback(&ConfigGPIO);

	// Save the callbacks	
	if(Config->WakeCallback != NULL)
	{
		__MCP2515_Callbacks.WakeCallback = Config->WakeCallback;
	}

	if(Config->TxCallback != NULL)
	{
		__MCP2515_Callbacks.TxCallback = Config->TxCallback;
	}

	if(Config->RxCallback != NULL)
	{
		__MCP2515_Callbacks.RxCallback = Config->RxCallback;
	}
	
	if(Config->ErrorCallback != NULL)
	{
		__MCP2515_Callbacks.ErrorCallback = Config->ErrorCallback;
	}

	// Enable device interrupts
	MCP2515_WriteRegister(MCP2515_REGISTER_CANINTE, (0x01 << MCP2515_IF_MER) | (0x01 << MCP2515_IF_WAK) | (0x01 << MCP2515_IF_ERR) | (0x01 << MCP2515_IF_TX2) | (0x01 << MCP2515_IF_TX1) | (0x01 << MCP2515_IF_TX0) | (0x01 << MCP2515_IF_RX1) | (0x01 << MCP2515_IF_RX0));
}

void MCP2515_Init(SPIM_Config_t* Config, MCP2515_Config_t* DeviceConfig, MCP2515_ErrorCallback_t Bla)
{
	// Initialize the CS Pin
	GPIO_SetDirection(GET_PERIPHERAL(MCP2515_SS), GET_INDEX(MCP2515_SS), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(MCP2515_SS), GET_INDEX(MCP2515_SS));

	// Initialize the external reset I/O
	#if(defined MCP2515_USE_EXT_RESET)
		GPIO_SetDirection(GET_PERIPHERAL(MCP2515_EXT_RESET), GET_INDEX(MCP2515_EXT_RESET), GPIO_DIRECTION_OUT);
		GPIO_Set(GET_PERIPHERAL(MCP2515_EXT_RESET), GET_INDEX(MCP2515_EXT_RESET));
		
		// Wait a bit
		for(uint8_t i = 0x00; i < 0xFF; i++);
	#endif

	// Initialize the external request to send I/O
	#if(defined MCP2515_USE_EXT_TX)
		GPIO_SetDirection(GET_PERIPHERAL(MCP2515_EXT_TX0), GET_INDEX(MCP2515_EXT_TX0), GPIO_DIRECTION_OUT);
		GPIO_Set(GET_PERIPHERAL(MCP2515_EXT_TX0), GET_INDEX(MCP2515_EXT_TX0));
		GPIO_SetDirection(GET_PERIPHERAL(MCP2515_EXT_TX1), GET_INDEX(MCP2515_EXT_TX1), GPIO_DIRECTION_OUT);
		GPIO_Set(GET_PERIPHERAL(MCP2515_EXT_TX1), GET_INDEX(MCP2515_EXT_TX1));
		GPIO_SetDirection(GET_PERIPHERAL(MCP2515_EXT_TX2), GET_INDEX(MCP2515_EXT_TX2), GPIO_DIRECTION_OUT);
		GPIO_Set(GET_PERIPHERAL(MCP2515_EXT_TX2), GET_INDEX(MCP2515_EXT_TX2));
		
		// Enable TxnRTS pins in the device
		MCP2515_WriteRegister(MCP2515_REGISTER_TXRTSCTRL, (0x01 << MCP2515_B2RTSM) | (0x01 << MCP2515_B1RTSM) | (0x01 << MCP2515_B0RTSM));
	#endif

	// Initialize the SPI-Interface
	if(Config != NULL)
	{
		MCP2515_SPI_INIT(Config);
	}

	// Initialize MCP2515
	MCP2515_Reset();

	// Setup Interrupts
	MCP2515_EnableInterrupts(DeviceConfig);

	// Switch the device into configuration mode
	MCP2515_SetDeviceMode(MCP2515_CONFIG_MODE);

	// Set the double shot bit
	MCP2515_BitModify(MCP2515_REGISTER_CANCTRL, (0x01 << MCP2515_OSM), (DeviceConfig->EnableOneShot << MCP2515_OSM));

	// Set the rollover bit
	MCP2515_BitModify(MCP2515_REGISTER_RXB0CTRL, (0x01 << MCP2515_BUKT), (DeviceConfig->EnableRollover << MCP2515_BUKT));

	// Set the Wake-up filter
	MCP2515_BitModify(MCP2515_REGISTER_CNF3, (0x01 << MCP2515_WAKFIL), (DeviceConfig->EnableWakeUpFilter << MCP2515_WAKFIL));

	// Use default timing
	if(DeviceConfig->pTiming == NULL)
	{
		MCP2515_WriteRegister(MCP2515_REGISTER_CNF1, 0x03);
		MCP2515_WriteRegister(MCP2515_REGISTER_CNF2, 0xA0);
		MCP2515_WriteRegister(MCP2515_REGISTER_CNF3, 0x02);
	}
	else
	{
		MCP2515_ConfigureTiming(DeviceConfig->pTiming);
	}

	// Disable all mask and filter
	MCP2515_BitModify(MCP2515_REGISTER_RXB0CTRL, (0x01 << MCP2515_RXM1) | (0x01 << MCP2515_RXM0), (0x01 << MCP2515_RXM1) | (0x01 << MCP2515_RXM0));
	MCP2515_BitModify(MCP2515_REGISTER_RXB1CTRL, (0x01 << MCP2515_RXM1) | (0x01 << MCP2515_RXM0), (0x01 << MCP2515_RXM1) | (0x01 << MCP2515_RXM0));

	// Clear all interrupt flags
	MCP2515_WriteRegister(MCP2515_REGISTER_CANINTF, 0x00);

	// Switch the device mode back to normal mode
	MCP2515_SetDeviceMode(MCP2515_NORMAL_MODE);
	
	// Switch the device into loop back mode
	if(DeviceConfig->EnableLoopBack)
	{
		MCP2515_SetDeviceMode(MCP2515_LOOPBACK_MODE);
	}
}

void MCP2515_ConfigureTiming(const MCP2515_TimingConfig_t* Config)
{
	MCP2515_BitModify(MCP2515_REGISTER_CNF1, 0xFF, (Config->SJW << 0x06) | (Config->Prescaler & 0x3F));
	MCP2515_BitModify(MCP2515_REGISTER_CNF2, 0xFF, (Config->PS2UseCNF3 << MCP2515_BTLMODE) | (Config->EnableTrippeSample << MCP2515_SAM) | ((Config->PHSEG1 & 0x07) << 0x03) | (Config->PRSEG & 0x07));
	MCP2515_BitModify(MCP2515_REGISTER_CNF3, 0x07, (Config->PHSEG2 & 0x07));
}

void MCP2515_Reset(void)
{
	#if(defined MCP2515_USE_EXT_RESET)
		GPIO_Clear(GET_PERIPHERAL(MCP2515_EXT_RESET), GET_INDEX(MCP2515_EXT_RESET));
		for(uint8_t i = 0x00; i < 0xFF; i++);
		GPIO_Set(GET_PERIPHERAL(MCP2515_EXT_RESET), GET_INDEX(MCP2515_EXT_RESET));
	#else
		MCP2515_SPI_CHIP_SELECT();
		MCP2515_SPI_TRANSMIT(MCP2515_CMD_RESET);
		MCP2515_SPI_CHIP_DESELECT();
	#endif

	// Wait until the reset has finished
	for(uint8_t i = 0x00; i < 0xFF; i++);
}

void MPC2515_EnableClockOut(const MCP2515_ClockOutPrescaler_t Prescaler)
{
	MCP2515_BitModify(MCP2515_REGISTER_CANCTRL, (0x01 << MCP2515_CLKEN) | 0x03, Prescaler & 0x03);
}

void MPC2515_DisableClockOut(void)
{
	MCP2515_BitModify(MCP2515_REGISTER_CANCTRL, (0x01 << MCP2515_CLKEN), 0x00);
}

void MCP2515_InstallCallback(const MCP2515_Config_t* Config)
{
	if(Config->Source & MCP2515_RX_INTERRUPT)
	{		
		if(Config->RxCallback != NULL)
		{
			__MCP2515_Callbacks.RxCallback = Config->RxCallback;
		}
	}

	if(Config->Source & MCP2515_TX_INTERRUPT)
	{
		if(Config->TxCallback != NULL)
		{
			__MCP2515_Callbacks.RxCallback = Config->TxCallback;
		}
	}

	if(Config->Source & MCP2515_WAKE_INTERRUPT)
	{
		if(Config->WakeCallback != NULL)
		{
			__MCP2515_Callbacks.WakeCallback = Config->WakeCallback;
		}
	}
}

void MCP2515_RemoveCallback(const MCP2515_CallbackType_t Callback)
{
	if(Callback & MCP2515_RX_INTERRUPT)
	{
		__MCP2515_Callbacks.RxCallback = NULL;
	}

	if(Callback & MCP2515_TX_INTERRUPT)
	{
		__MCP2515_Callbacks.TxCallback = NULL;
	}

	if(Callback & MCP2515_WAKE_INTERRUPT)
	{
		__MCP2515_Callbacks.WakeCallback = NULL;
	}
}

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	void MCP2515_ChangeInterruptLevel(const MCP2515_Config_t* Config)
	{
		GPIO_ChangeInterruptLevel(Config->Port, Config->Pin, Config->Channel, Config->InterruptLevel);
	}
#endif

void MCP2515_SetDeviceMode(const MCP2515_DeviceMode_t Mode)
{
	MCP2515_BitModify(MCP2515_REGISTER_CANCTRL, 0xE0, (Mode << 0x05));
}

MCP2515_DeviceMode_t MCP2515_GetDeviceMode(void)
{
	return ((MCP2515_ReadRegister(MCP2515_REGISTER_CANSTAT) & 0xE0) >> 0x05);
}

void MCP2515_SwitchFilter(const MCP2515_ReceiveBuffer_t Buffer, const MCP2515_FilterRule_t Filter)
{
	MCP2515_BitModify(MCP2515_REGISTER_RXB0CTRL + ((Buffer & 0x01) << 0x04), (0x01 << MCP2515_RXM1) | (0x01 << MCP2515_RXM0), Filter << MCP2515_RXM0);
}

void MCP2515_ConfigFilter(const MCP2515_FilterConfig_t* Config)
{
	uint8_t IdentifierBaseAddress = 0x00;

	// Get the current device mode
	MCP2515_DeviceMode_t Mode = MCP2515_GetDeviceMode();

	// Change mode to configuration mode to change the filter setup
	MCP2515_SetDeviceMode(MCP2515_CONFIG_MODE);

	uint8_t Mask_Low = (Config->Mask & 0x07) << 0x05;
	uint8_t Mask_High = Config->Mask >> 0x03;

	switch(Config->Filter)
	{
		case MCP2515_RXF0:
		{
			IdentifierBaseAddress = MCP2515_REGISTER_RXF0SIDH;
			break;
		}
		case MCP2515_RXF1:
		{
			IdentifierBaseAddress = MCP2515_REGISTER_RXF1SIDH;
			break;
		}
		case MCP2515_RXF2:
		{
			IdentifierBaseAddress = MCP2515_REGISTER_RXF2SIDH;
			break;
		}
		case MCP2515_RXF3:
		{
			IdentifierBaseAddress = MCP2515_REGISTER_RXF3SIDH;
			break;
		}
		case MCP2515_RXF4:
		{
			IdentifierBaseAddress = MCP2515_REGISTER_RXF4SIDH;
			break;
		}
		case MCP2515_RXF5:
		{
			IdentifierBaseAddress = MCP2515_REGISTER_RXF5SIDH;
			break;
		}
	}

	// Set the identifier
	if(Config->ID > 0x00)
	{
		uint8_t Identifier_Low = (Config->ID & 0x07) << 0x05;
		uint8_t Identifier_High = Config->ID >> 0x03;
		
		MCP2515_WriteRegister(IdentifierBaseAddress, Identifier_High);
		MCP2515_WriteRegister(IdentifierBaseAddress + 0x01, Identifier_Low);
	}

	if((Config->Filter == MCP2515_RXF0) || (Config->Filter == MCP2515_RXF1))
	{
		if(!Config->UseExtended)
		{
			MCP2515_WriteRegister(MCP2515_REGISTER_RXM0SIDH, Mask_High);
			MCP2515_WriteRegister(MCP2515_REGISTER_RXM0SIDL, Mask_Low);
		}
	}
	else if((Config->Filter == MCP2515_RXF2) | (Config->Filter == MCP2515_RXF3) | (Config->Filter == MCP2515_RXF4) | (Config->Filter == MCP2515_RXF5))
	{
		if(!Config->UseExtended)
		{
			MCP2515_WriteRegister(MCP2515_REGISTER_RXM1SIDH, Mask_High);
			MCP2515_WriteRegister(MCP2515_REGISTER_RXM1SIDL, Mask_Low);
		}
	}
	
	// Restore device mode
	MCP2515_SetDeviceMode(Mode);
}

void MCP2515_RequestTransmission(const MCP2515_TransmitBuffer_t Buffer)
{
	uint8_t Buffer_Int = Buffer & 0x07;

	#if(defined MCP2515_USE_EXT_TX)
		if(Buffer == MCP2515_TX0)
		{
			GPIO_Clear(GET_PERIPHERAL(MCP2515_EXT_TX0), GET_INDEX(MCP2515_EXT_TX0));
			GPIO_Set(GET_PERIPHERAL(MCP2515_EXT_TX0), GET_INDEX(MCP2515_EXT_TX0));
		}
		else if(Buffer == MCP2515_TX1)
		{
			GPIO_Clear(GET_PERIPHERAL(MCP2515_EXT_TX1), GET_INDEX(MCP2515_EXT_TX1));
			GPIO_Set(GET_PERIPHERAL(MCP2515_EXT_TX1), GET_INDEX(MCP2515_EXT_TX1));
		}
		else
		{
			GPIO_Clear(GET_PERIPHERAL(MCP2515_EXT_TX2), GET_INDEX(MCP2515_EXT_TX2));
			GPIO_Set(GET_PERIPHERAL(MCP2515_EXT_TX2), GET_INDEX(MCP2515_EXT_TX2));
		}
	#else
		MCP2515_SPI_CHIP_SELECT();
		MCP2515_SPI_TRANSMIT(MCP2515_CMD_RTS(Buffer_Int));
		MCP2515_SPI_CHIP_DESELECT();
	#endif
}

void MPC2515_AbortTransmission(const MCP2515_TransmitBuffer_t Buffer)
{
	// Get the register address
	uint8_t Address = MCP2515_REGISTER_TXB0CTRL + (Buffer << 0x04);

	// Delete TXREQ Bit
	MCP2515_BitModify(Address, (0x01 << MCP2515_TXREQ), 0x00);
}

void MPC2515_AbortAllTransmission(void)
{
	MCP2515_BitModify(MCP2515_REGISTER_CANCTRL, (0x01 << MCP2515_ABAT), (0x01 << MCP2515_ABAT));
}

void MCP2515_ReadMessage(CAN_Message_t* Message)
{
	uint8_t Length = 0x00;
	uint8_t ReceiveCommand = 0x00;

	// Get the receive status
	uint8_t RxStatus = MCP2515_ReadRxStatus();

	// Return when no message received
	if(!(RxStatus & ((0x01 << MCP2515_MSG_RX_1) | (0x01 << MCP2515_MSG_RX_0))))
	{
		return;
	}
	else if(RxStatus & (0x01 << MCP2515_MSG_RX_1))
	{
		ReceiveCommand = MCP2515_CMD_READ_RX(MCP2515_RX1);
	}
	else
	{
		ReceiveCommand = MCP2515_CMD_READ_RX(MCP2515_RX0);
	}

	// Get the message type
	MCP2515_MessageType_t MessageType = (RxStatus & ((0x01 << MCP2515_MSG_TYPE1) | (0x01 << MCP2515_MSG_TYPE0))) >> MCP2515_MSG_TYPE0;

	MCP2515_SPI_CHIP_SELECT();
	MCP2515_SPI_TRANSMIT(ReceiveCommand);
	
	// Get the message ID
	if((MessageType == MCP2515_STANDARD_REMOTE) || (MessageType == MCP2515_STANDARD_DATA))
	{
		Message->ID = (MCP2515_SPI_TRANSMIT(0xFF)) << 0x03 | (MCP2515_SPI_TRANSMIT(0xFF)) >> 0x05;

		// Skip the extended identifier
		MCP2515_SPI_TRANSMIT(0xFF);
		MCP2515_SPI_TRANSMIT(0xFF);
	}
	else
	{
		// Skip the first byte of the standard identifier
		MCP2515_SPI_TRANSMIT(0xFF);
		Message->ID = ((((uint32_t)MCP2515_SPI_TRANSMIT(0xFF)) & 0x03) << 0x18) | (MCP2515_SPI_TRANSMIT(0xFF) << 0x08) | MCP2515_SPI_TRANSMIT(0xFF);
	}

	// Get the message length
	Length = MCP2515_SPI_TRANSMIT(0xFF);

	switch(MessageType)
	{
		case MCP2515_STANDARD_DATA:
		case MCP2515_EXTENDED_DATA:
		{
			Message->Length = Length;
			Message->Type = MessageType;

			for(uint8_t i = 0; i < Message->Length; i++)
			{
				*(Message->pData++) = MCP2515_SPI_TRANSMIT(0xFF);
			}
				
			break;
		}
		case MCP2515_STANDARD_REMOTE:
		case MCP2515_EXTENDED_REMOTE:
		{
			Message->Length = Length & (~(0x01 << MCP2515_RTR));
			Message->Type = MessageType;

			break;
		}
	}
	
	MCP2515_SPI_CHIP_DESELECT();
}

MCP2515_ErrorCode_t MCP2515_PrepareMessage(CAN_Message_t* Message, const MCP2515_MessagePriority_t Priority, const MCP2515_TransmitBuffer_t Buffer)
{
	if(Message->ID > 0x3FFFF)
	{
		return MCP2515_INVALID_IDENTIFIER;
	}
	
	// Check for an empty transmit buffer. First check the internal bitmap and then check 
	// the TXREQ bit in the second step
	uint8_t BufferBase = 0x00;
	uint8_t Status = 0x00;
	if(Buffer == MCP2515_TX0)
	{
		Status = MCP2515_ReadRegister(MCP2515_REGISTER_TXB0CTRL);
		if(!(Status & (0x01 << MCP2515_TXREQ)))
		{
			BufferBase = MCP2515_REGISTER_TXB0CTRL;
		}
	}
	else if(Buffer == MCP2515_TX1)
	{
		Status = MCP2515_ReadRegister(MCP2515_REGISTER_TXB1CTRL);
		if(!(Status & (0x01 << MCP2515_TXREQ)))
		{
			BufferBase = MCP2515_REGISTER_TXB1CTRL;
		}
	}
	else if(Buffer == MCP2515_TX2)
	{
		Status = MCP2515_ReadRegister(MCP2515_REGISTER_TXB2CTRL);
		if(!(Status & (0x01 << MCP2515_TXREQ)))
		{
			BufferBase = MCP2515_REGISTER_TXB2CTRL;
		}
	}

	// All Tx buffers are full - cancel transmission
	if(BufferBase == 0x00)
	{
		return MCP2515_TX_BUFFER_FULL;
	}

	// Delete TXREQ Bit
	MCP2515_BitModify(BufferBase, (0x01 << MCP2515_TXREQ), 0x00);

	// Set the message ID
	if((Message->Type == MCP2515_STANDARD_DATA) || (Message->Type == MCP2515_STANDARD_REMOTE))
	{
		MCP2515_WriteRegister(BufferBase + 0x02, (Message->ID & 0x07) << 0x05);
		MCP2515_WriteRegister(BufferBase + 0x01, Message->ID >> 0x03);
	}
	else
	{
		MCP2515_WriteRegister(BufferBase + 0x02, (0x01 << MCP2515_EXIDE) | ((Message->ID >> 0x10) & 0x03));
		MCP2515_WriteRegister(BufferBase + 0x03, (Message->ID >> 0x08) & 0xFF);
		MCP2515_WriteRegister(BufferBase + 0x04, Message->ID & 0xFF);
	}

	// Set the message priority
	MCP2515_BitModify(BufferBase, 0x03, Priority & 0x03);

	switch(Message->Type)
	{
		case MCP2515_EXTENDED_DATA:
		case MCP2515_STANDARD_DATA:
		{
			MCP2515_WriteRegister(BufferBase + 0x05, (Message->Length & 0x0F));

			for(uint8_t DataByte = 0x00; DataByte < MCP2515_MAX_DATABYTES; DataByte++)
			{
				MCP2515_WriteRegister(BufferBase + 0x06 + DataByte, *(Message->pData++));
			}

			break;
		}
		case MCP2515_STANDARD_REMOTE:
		case MCP2515_EXTENDED_REMOTE:
		{
			MCP2515_WriteRegister(BufferBase + 0x05, (Message->Length & 0x0F) + (0x01 << MCP2515_RTR));

			break;
		}
	}

	return MCP2515_NO_ERROR;
}