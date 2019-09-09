/*
 * I2CM.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: I2C master driver for XMega TWI

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file Arch/XMega/I2C/I2CM.c
 *  @brief Driver for XMega I2C master mode.
 *
 *  This file contains the implementation of the XMega I2C master driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/I2C/I2C.h"
#include "Arch/XMega/PowerManagement/PowerManagement.h"

#ifndef DOXYGEN
	extern I2C_Message_t __I2CM_Messages[TWI_DEVICES];
#endif

/** @brief			Send the address (including R/W bit) of a bus slave in master mode.
 *  @param Device	Pointer to TWI device object
 *  @param Address	Slave device address
 */
static inline void I2CM_SendAddress(TWI_t* Device, const uint8_t Address) __attribute__((always_inline));
static inline void I2CM_SendAddress(TWI_t* Device, const uint8_t Address)
{
	Device->MASTER.ADDR = Address;
	while(!((Device->MASTER.STATUS & TWI_MASTER_WIF_bm) || (Device->MASTER.STATUS & TWI_MASTER_RIF_bm)));
}

/** @brief			Write a data byte in master mode to the I2C.
 *  @param Device	Pointer to TWI device object
 *  @param Data		Data byte
 */
static inline void I2CM_SendData(TWI_t* Device, const uint8_t Data) __attribute__((always_inline));
static inline void I2CM_SendData(TWI_t* Device, const uint8_t Data)
{
	Device->MASTER.DATA = Data;
	while(!(Device->MASTER.STATUS & TWI_MASTER_WIF_bm));
}

/** @brief			Write a stop condition in master mode.
 *  @param Device	Pointer to TWI device object
 *  @param NACK		#TRUE if NACK should generated
 */
static inline void I2CM_SendStop(TWI_t* Device, const Bool_t NACK) __attribute__((always_inline));
static inline void I2CM_SendStop(TWI_t* Device, const Bool_t NACK)
{
	if(NACK)
	{
		Device->MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;
	}
	else
	{
		Device->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
	}
}

/** @brief			Get a data byte in master mode from the bus.
 *  @param Device	Pointer to TWI object
 *  @param NACK		#TRUE if NACK should generated
 *  @return			Data byte
 */
static uint8_t I2CM_ReadData(TWI_t* Device, const Bool_t NACK)
{
	uint8_t Data = 0x00;
	
	while(!(Device->MASTER.STATUS & TWI_MASTER_RIF_bm));
	Data = Device->MASTER.DATA;
	
	if(!NACK)
	{
		Device->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
	}
	
	return Data;
}

void I2CM_Init(I2CM_Config_t* Config)
{
	I2C_PowerEnable(Config->Device);

	I2CM_SetTimeout(Config->Device, Config->Timeout);
	I2CM_SetBaudrate(Config->Device, Config->BitRate, SysClock_GetClock());
	I2CM_SwitchQuickCommand(Config->Device, Config->EnableQuickCommand);
	I2CM_SwitchSmartMode(Config->Device, Config->EnableSmartMode);
	I2C_MasterEnable(Config->Device);
	
	I2CM_Reset(Config->Device);
	
	if(Config->EnableInterruptSupport == TRUE)
	{
		I2CM_EnableInterruptSupport(Config->Device, Config->InterruptLevel);
	}
}

const I2C_MasterStatus_t I2CM_Status(const TWI_t* Device)
{
	uint8_t ID = 0x00;
	
	if(Device == &TWIC)
	{
		ID = TWIC_ID;
	}
	else if(Device == &TWIE)
	{
		ID = TWIE_ID;
	}
	
	return __I2CM_Messages[ID].Status;
}

I2C_Error_t I2CM_WriteByte(TWI_t* Device, const uint8_t Address, const uint8_t Data, const Bool_t Stop)
{
	I2CM_SendAddress(Device, I2C_WRITE(Address));
	if(I2CM_ReadStatus(Device) & TWI_MASTER_BUSERR_bm)
	{
		return I2C_WRITE_ADDR_ERROR;
	}
	
	I2CM_SendData(Device, Data);
	if(I2CM_ReadStatus(Device) & (TWI_MASTER_BUSERR_bm | TWI_MASTER_RXACK_bm | TWI_MASTER_ARBLOST_bm))
	{
		I2CM_SendStop(Device, FALSE);

		return I2C_BUS_ERROR;
	}
	
	if(Stop == TRUE)
	{
		I2CM_SendStop(Device, FALSE);
	}
	
	return I2C_NO_ERROR;
}

I2C_Error_t I2CM_WriteBytes(TWI_t* Device, const uint8_t Address, const uint8_t Length, const uint8_t* Data, const Bool_t Stop)
{
	I2CM_SendAddress(Device, I2C_WRITE(Address));
	if(I2CM_ReadStatus(Device) & TWI_MASTER_BUSERR_bm)
	{
		return I2C_WRITE_ADDR_ERROR;
	}
	
	for(uint8_t i = 0x00; i < Length; i++)
	{
		I2CM_SendData(Device, *Data++);
		if(I2CM_ReadStatus(Device) & (TWI_MASTER_BUSERR_bm | TWI_MASTER_RXACK_bm | TWI_MASTER_ARBLOST_bm))
		{
			I2CM_SendStop(Device, FALSE);

			return I2C_BUS_ERROR;
		}
	}

	if(Stop == TRUE)
	{
		I2CM_SendStop(Device, FALSE);
	}

	return I2C_NO_ERROR;
}

I2C_Error_t I2CM_ReadByte(TWI_t* Device, const uint8_t Address, uint8_t* Data, const Bool_t Stop)
{
	I2CM_SendAddress(Device, I2C_READ(Address));
	if(I2CM_ReadStatus(Device) & TWI_MASTER_BUSERR_bm)
	{
		return I2C_READ_ADDR_ERROR;
	}
	
	*Data = I2CM_ReadData(Device, TRUE);
	
	if(Stop == TRUE)
	{
		I2CM_SendStop(Device, TRUE);
	}
	
	return I2C_NO_ERROR;
}

I2C_Error_t I2CM_ReadBytes(TWI_t* Device, const uint8_t Address, const uint8_t Length, uint8_t* Data, const Bool_t Stop)
{
	I2CM_SendAddress(Device, I2C_READ(Address));
	if(I2CM_ReadStatus(Device) & TWI_MASTER_BUSERR_bm)
	{
		return I2C_READ_ADDR_ERROR;
	}
	
	for(uint8_t i = 0x00; i < (Length - 1); i++)
	{
		*Data++ = I2CM_ReadData(Device, FALSE);
		if(I2CM_ReadStatus(Device) & TWI_MASTER_BUSERR_bm)
		{
			I2CM_SendStop(Device, TRUE);

			return I2C_BUS_ERROR;
		}
	}
	
	*Data++ = I2CM_ReadData(Device, TRUE);
	
	if(Stop == TRUE)
	{
		I2CM_SendStop(Device, TRUE);
	}
	
	return I2C_NO_ERROR;
}

void I2CM_SetBaudrate(TWI_t* Device, const uint32_t Baudrate, const uint32_t Clock)
{
	uint32_t BaudTemp = Baudrate;
	
	if((BaudTemp < 100000) || (BaudTemp > 400000))
	{
		BaudTemp = 100000;
	}
	
	BaudTemp = (Clock / (BaudTemp << 0x01)) - 5;
	
	Device->MASTER.BAUD = BaudTemp;
}

void I2CM_TransmitBytes(TWI_t* Device, const uint8_t DeviceAddress, const uint8_t Bytes, uint8_t* Data)
{
	uint8_t ID = 0x00;
	
	if(Device == &TWIC)
	{
		ID = TWIC_ID;
	}
	else if(Device == &TWIE)
	{
		ID = TWIE_ID;
	}
	
	__I2CM_Messages[ID].BufferRead = 0x00;
	__I2CM_Messages[ID].BytesToRead = 0x00;
	
	__I2CM_Messages[ID].IndexWrite = 0;
	__I2CM_Messages[ID].BufferWrite = Data;
	__I2CM_Messages[ID].BytesToWrite = Bytes;
	__I2CM_Messages[ID].Device = Device;
	__I2CM_Messages[ID].DeviceAddress = DeviceAddress;
	__I2CM_Messages[ID].Register = 0x00;
	__I2CM_Messages[ID].Status = I2C_MASTER_WRITE;
	
	// Start the transmission by writing the address
	__I2CM_Messages[ID].Device->MASTER.ADDR = I2C_WRITE(DeviceAddress);
}

void I2CM_Transmit(TWI_t* Device, const uint8_t DeviceAddress, const uint8_t Command, const uint8_t Bytes, uint8_t* Data)
{
	uint8_t ID = 0x00;
	
	if(Device == &TWIC)
	{
		ID = TWIC_ID;
	}
	else if(Device == &TWIE)
	{
		ID = TWIE_ID;
	}
	
	__I2CM_Messages[ID].BufferRead = 0x00;
	__I2CM_Messages[ID].BytesToRead = 0x00;
	
	__I2CM_Messages[ID].IndexWrite = 0x00;
	__I2CM_Messages[ID].BufferWrite = Data;
	__I2CM_Messages[ID].BytesToWrite = Bytes;
	__I2CM_Messages[ID].Device = Device;
	__I2CM_Messages[ID].DeviceAddress = DeviceAddress;
	__I2CM_Messages[ID].Register = Command;
	__I2CM_Messages[ID].Status = I2C_MASTER_REGISTER;
	
	// Start the transmission by writing the address
	__I2CM_Messages[ID].Device->MASTER.ADDR = I2C_WRITE(__I2CM_Messages[ID].DeviceAddress);
}

void I2CM_ReceiveBytes(TWI_t* Device, const uint8_t DeviceAddress, const uint8_t Bytes, uint8_t* Data)
{
	uint8_t ID = 0x00;
	
	if(Device == &TWIC)
	{
		ID = TWIC_ID;
	}
	else if(Device == &TWIE)
	{
		ID = TWIE_ID;
	}

	__I2CM_Messages[ID].BufferWrite = 0x00;
	__I2CM_Messages[ID].BytesToWrite = 0x00;

	__I2CM_Messages[ID].IndexRead = 0x00;
	__I2CM_Messages[ID].BufferRead = Data;
	__I2CM_Messages[ID].BytesToRead = Bytes;
	__I2CM_Messages[ID].Device = Device;
	__I2CM_Messages[ID].DeviceAddress = DeviceAddress;
	__I2CM_Messages[ID].Register = 0x00;
	__I2CM_Messages[ID].Status = I2C_MASTER_READ;
	
	// Start the transmission by writing the address
	__I2CM_Messages[ID].Device->MASTER.ADDR = I2C_READ(DeviceAddress);
}

void I2CM_Receive(TWI_t* Device, const uint8_t DeviceAddress, const uint8_t Command, const uint8_t Bytes, uint8_t* Data)
{
	uint8_t ID = 0x00;
	
	if(Device == &TWIC)
	{
		ID = TWIC_ID;
	}
	else if(Device == &TWIE)
	{
		ID = TWIE_ID;
	}
	
	__I2CM_Messages[ID].BufferWrite = 0x00;
	__I2CM_Messages[ID].BytesToWrite = 0x00;
	
	__I2CM_Messages[ID].IndexRead = 0x00;
	__I2CM_Messages[ID].BufferRead = Data;
	__I2CM_Messages[ID].BytesToRead = Bytes;
	__I2CM_Messages[ID].Device = Device;
	__I2CM_Messages[ID].DeviceAddress = DeviceAddress;
	__I2CM_Messages[ID].Register = Command;
	__I2CM_Messages[ID].Status = I2C_MASTER_REGISTER;
	
	// Start the transmission by writing the address
	__I2CM_Messages[ID].Device->MASTER.ADDR = I2C_WRITE(__I2CM_Messages[ID].DeviceAddress);
}