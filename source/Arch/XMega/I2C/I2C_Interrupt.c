/*
 * I2C_Interrupt.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega I2C module.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file Arch/XMega/I2C/I2C_Interrupt.c
 *  @brief Driver for XMega I2C module.
 *
 *  This file contains the implementation of the interrupt functions for the XMega I2C driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/I2C/I2C.h"

#ifndef DOXYGEN
	struct
	{
		I2C_Callback_t TxCompleteInterrupt;
		I2C_Callback_t RxCompleteInterrupt;
		I2C_Callback_t ErrorInterrupt;
		I2C_Callback_t DataRdyInterrupt;
	} _I2C_Callbacks[TWI_DEVICES];

	I2C_Message_t _I2CM_Messages[TWI_DEVICES];
	I2C_Buffer_t _I2CS_Buffer[TWI_DEVICES];
#endif

/** @brief			I2C error handler for master mode.
 *  @param Device	Device ID
 */
static void _I2CM_ErrorHandler(const uint8_t Device)
{
	_I2CM_Messages[Device].Status = I2C_MASTER_ERROR;
	
	_I2CM_Messages[Device].Device->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
	
	if(_I2C_Callbacks[Device].ErrorInterrupt)
	{
		_I2C_Callbacks[Device].ErrorInterrupt();
	}
}

/** @brief			I2C arbitration lost handler.
 *  @param Device	Device ID
 */
static void _I2CM_ArbitrationLostHandler(const uint8_t Device)
{
	_I2CM_ErrorHandler(Device);
}

/** @brief			I2C bus data write handler
 *  @param Device	Device ID
 */
static void _I2CM_WriteHandler(const uint8_t Device)
{
	// Abort transmission of slave has send an NACK
	if(_I2CM_Messages[Device].Device->MASTER.STATUS & TWI_MASTER_RXACK_bm)
	{
		// Slave send NACK instead of ACK - abort transmission
		_I2CM_ErrorHandler(Device);
	}
	else
	{
		// Send the start address
		if(_I2CM_Messages[Device].Status == I2C_MASTER_REGISTER)
		{
			_I2CM_Messages[Device].Device->MASTER.DATA = _I2CM_Messages[Device].Register;

			if(_I2CM_Messages[Device].BytesToRead > 0x00)
			{
				_I2CM_Messages[Device].Status = I2C_MASTER_ADDRESS;
			}
			else
			{
				_I2CM_Messages[Device].Status = I2C_MASTER_WRITE;
			}
		}
		// Send repeated start after the address
		else if(_I2CM_Messages[Device].Status == I2C_MASTER_ADDRESS)
		{
			_I2CM_Messages[Device].Device->MASTER.ADDR = I2C_READ(_I2CM_Messages[Device].DeviceAddress);
			_I2CM_Messages[Device].Status = I2C_MASTER_READ;
		}
		// Send the data
		else if(_I2CM_Messages[Device].Status == I2C_MASTER_WRITE)
		{
			if(_I2CM_Messages[Device].IndexWrite < _I2CM_Messages[Device].BytesToWrite)
			{
				_I2CM_Messages[Device].Device->MASTER.DATA = _I2CM_Messages[Device].BufferWrite[_I2CM_Messages[Device].IndexWrite++];
			}
			else
			{
				_I2CM_Messages[Device].Device->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
				_I2CM_Messages[Device].Status = I2C_MASTER_SEND;
				
				if(_I2C_Callbacks[Device].TxCompleteInterrupt != NULL)
				{
					_I2C_Callbacks[Device].TxCompleteInterrupt();
				}
			}
		}
	}
}

/** @brief			I2C bus data read handler
 *  @param Device	Device ID
 */
static void _I2CM_ReadHandler(const uint8_t Device)
{
	if(_I2CM_Messages[Device].Status == I2C_MASTER_READ)
	{
		// Check for buffer overflow
		if(_I2CM_Messages[Device].IndexRead < TWI_BUFFER_SIZE)
		{
			_I2CM_Messages[Device].BufferRead[_I2CM_Messages[Device].IndexRead++] = _I2CM_Messages[Device].Device->MASTER.DATA;
		}
		else
		{
			_I2CM_Messages[Device].Device->MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;
			_I2CM_Messages[Device].Status = I2C_MASTER_BUFFEROVERFLOW;
		}
		
		if(_I2CM_Messages[Device].IndexRead < _I2CM_Messages[Device].BytesToRead)
		{
			_I2CM_Messages[Device].Device->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
		}
		else
		{
			_I2CM_Messages[Device].Device->MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;
			_I2CM_Messages[Device].Status = I2C_MASTER_RECEIVED;
			
			if(_I2C_Callbacks[Device].RxCompleteInterrupt != NULL)
			{
				_I2C_Callbacks[Device].RxCompleteInterrupt();
			}
		}
	}
	else
	{
		_I2CM_ErrorHandler(Device);
	}
}

/** @brief			I2C master interrupt handler.
 *  @param Device	Device ID
 */
static void _I2CM_InterruptHandler(const uint8_t Device)
{
	uint8_t Status = I2CM_ReadStatus(_I2CM_Messages[Device].Device);
	
	// Check the interface status
	if(Status & TWI_MASTER_ARBLOST_bm)
	{
		_I2CM_ArbitrationLostHandler(Device);
	}
	else if(Status & TWI_MASTER_WIF_bm)
	{
		_I2CM_WriteHandler(Device);
	}
	else if(Status & TWI_MASTER_RIF_bm)
	{
		_I2CM_ReadHandler(Device);
	}
	else
	{
		_I2CM_ErrorHandler(Device);
	}
}

void I2CM_EnableInterruptSupport(TWI_t* Device, const Interrupt_Level_t Level)
{
	Device->MASTER.CTRLA |= TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm;
	Device->MASTER.CTRLA |= (Device->SLAVE.CTRLA & (~(0x03 << 0x06))) | (Level << 0x06);
}

void I2CM_DisableInterruptSupport(TWI_t* Device)
{
	Device->MASTER.CTRLA &= ~(TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm);
	Device->MASTER.CTRLA &= ~(0x03 << 0x06);
}

/** @brief			I2C error handler for slave mode.
 *  @param Device	Device ID
 */
static void _I2CS_ErrorHandler(const uint8_t Device)
{
	_I2CS_Buffer[Device].Status = I2C_SLAVE_ERROR;
}

/** @brief			I2C address match handler for slave mode.
 *  @param Device	Device ID
 */
static void _I2CS_AddressHandler(const uint8_t Device)
{
	_I2CS_Buffer[Device].BytesReceived = 0x00;
	_I2CS_Buffer[Device].BytesSend = 0x00;
	
	_I2CS_Buffer[Device].Device->SLAVE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
}

/** @brief			I2C stop handler for slave mode.
 *  @param Device	Device ID
 */
static void _I2CS_StopHandler(const uint8_t Device)
{
	_I2CS_Buffer[Device].Device->SLAVE.STATUS |= TWI_SLAVE_APIF_bm;
}

/** @brief			I2C data handler for slave mode.
 *  @param Device	Device ID
 */
static void _I2CS_DataHandler(const uint8_t Device)
{
	uint8_t Status = _I2CS_Buffer[Device].Device->SLAVE.STATUS;

	// Transmit data to the master (Read from master)
	if(Status & TWI_SLAVE_DIR_bm)
	{
		// Check if NACK
		if(_I2CS_Buffer[Device].Device->SLAVE.STATUS & TWI_SLAVE_RXACK_bm)
		{
			_I2CS_Buffer[Device].Device->SLAVE.CTRLB = TWI_SLAVE_CMD_COMPTRANS_gc;
		}
		else
		{
			_I2CS_Buffer[Device].BytesSend++;
			_I2CS_Buffer[Device].Device->SLAVE.DATA = _I2CS_Buffer[Device].Buffer[_I2CS_Buffer[Device].IndexRead++];
			
			if(_I2CS_Buffer[Device].IndexRead > TWI_BUFFER_SIZE)
			{
				_I2CS_Buffer[Device].IndexRead = 0x00;
				_I2CS_Buffer[Device].Status = I2C_SLAVE_BUFFEROVERFLOW;
			}

			_I2CS_Buffer[Device].Device->SLAVE.CTRLB |= TWI_SLAVE_CMD_RESPONSE_gc;
		}
	}
	// Receive data from master (Write from master)
	else
	{
		_I2CS_Buffer[Device].BytesReceived++;
		_I2CS_Buffer[Device].Buffer[_I2CS_Buffer[Device].IndexWrite++] = _I2CS_Buffer[Device].Device->SLAVE.DATA;
		
		if(_I2CS_Buffer[Device].IndexWrite > TWI_BUFFER_SIZE)
		{
			_I2CS_Buffer[Device].Device->SLAVE.CTRLB = TWI_SLAVE_ACKACT_bm;
			_I2CS_Buffer[Device].Status = I2C_SLAVE_BUFFEROVERFLOW;
		}

		_I2CS_Buffer[Device].Device->SLAVE.CTRLB |= TWI_SLAVE_CMD_RESPONSE_gc;
	}
}

/** @brief			I2C slave mode interrupt handler.
 *  @param Device	Device ID
 */
static void _I2CS_InterruptHandler(const uint8_t Device)
{
	uint8_t Status = I2CS_ReadStatus(_I2CS_Buffer[Device].Device);
	
	if(Status & TWI_SLAVE_BUSERR_bm)
	{
		_I2CS_ErrorHandler(Device);
	}
	else if(Status & TWI_SLAVE_COLL_bm)
	{
		_I2CS_ErrorHandler(Device);
	}
	else if((Status & TWI_SLAVE_APIF_bm) && (Status & TWI_SLAVE_AP_bm))
	{
		_I2CS_AddressHandler(Device);
	}
   else if(Status & TWI_SLAVE_APIF_bm)
   {
	   _I2CS_StopHandler(Device);
   }
   else if(Status & TWI_SLAVE_DIF_bm) 
   {
	   _I2CS_DataHandler(Device);
   }
   else 
   {
	   _I2CS_ErrorHandler(Device);
   }
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(TWIC_TWIM_vect)
	{
		_I2CM_InterruptHandler(TWIC_ID);
	}

	ISR(TWIC_TWIS_vect)
	{
		_I2CS_InterruptHandler(TWIC_ID);
	}

	ISR(TWIE_TWIM_vect)
	{
		_I2CM_InterruptHandler(TWIE_ID);
	}
	
	ISR(TWIE_TWIS_vect)
	{
		_I2CS_InterruptHandler(TWIE_ID);
	}
#endif