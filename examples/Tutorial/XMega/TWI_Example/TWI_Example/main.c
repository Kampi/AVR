/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: TWI example for XMega.

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

/** @file main.c
 *  @brief TWI example for XMega. This example use a DS1307 RTC as a I2C slave.
 *  Some device registers get filled with random data. After that the registers 
 *  are read. The read/write is implemented as polled and as interrupt driven solution. 
 *
 *	This file contains several examples:
 *		a) I2C master mode. Set symbol I2C_MASTER = 1.
 *			1) Polled communication with I2C device (DS1307). Set symbol EXAMPLE = 1.
 *			   This example writes several bytes to the device and read them out.
 *			2) Interrupt communication with I2C device (DS1307). Set symbol EXAMPLE = 2.
 *			   Same scenario as example 1, but using interrupts only for communication.
 *			3) Mixed communication with I2C device (DS1307). Set symbol EXAMPLE = 3.
 *			   Same scenario as example 1, but using interrupts to write the data, polled communication
 *			   to write the register pointer and interrupt driven communication to read the data.
 *		b) I2C slave mode. Set symbol I2C_MASTER = 0.
 *			1) Interrupt driven communication between the I2C master (TWIC) and the I2C slave (TWIE). Set symbol EXAMPLE = 1.
 *			   Bridge both interfaces with a jumper cable. The example transmit 8 byte of data with the master device to the slave
 *			   and read them back.
 *			2) Same as example 1, but the master produce an buffer overflow by writing in the slave device. Set symbol EXAMPLE = 2.
 *			3) Same as example 1, but the master produce an buffer overflow by reading data from the slave device. Set symbol EXAMPLE = 3.
 *
 *  Software for the XMega TWI tutorial from
 *  https://www.kampis-elektroecke.de/avr/xmega-i2c/
 *
 *  @author Daniel Kampert
 */

#include "TWI/TWI.h"

#define TWI_DEVICE_ADDRESS						0x68

#define TARGET_REGISTER							0x03
#define BYTES_TO_SEND							0x03

uint8_t I2C_WriteBuffer[TWI_BUFFER_SIZE];
uint8_t I2C_ReadBuffer[TWI_BUFFER_SIZE];
uint8_t I2C_SlaveBuffer[TWI_BUFFER_SIZE];

int main(void)
{
#if(I2C_MASTER == 1)
	#if(EXAMPLE == 1)
		/*
			Polled TWI example	
		*/
	
		// Fill the write buffer
		for(uint8_t i = 0x00; i < BYTES_TO_SEND; i++)
		{
			I2C_WriteBuffer[i] = i + 0x05;
		}

		/*
			Initialize master mode TWI
				-> Normal TWI
				-> Master mode
				-> No Timeout
				-> No Interrupts
				-> 100 kHz @ 2 MHz
		*/
		TWIM_Init();
	
		// Write the data into the target register
		// Send the device address
		TWIM_SendAddress(TWI_WRITE(TWI_DEVICE_ADDRESS));
		
		// Send the register address
		TWIM_SendData(TARGET_REGISTER);

		// Send the data bytes
		for(uint8_t i = 0x00; i < BYTES_TO_SEND; i++)
		{
			TWIM_SendData(I2C_WriteBuffer[i]);
		}
	
		// Send stop condition
		TWIM_SendStop(TWI_ACK);

		// Read data from the target register
		for(uint8_t i = 0x00; i < 0x03; i++)
		{
			// Send the device address
			TWIM_SendAddress(TWI_WRITE(TWI_DEVICE_ADDRESS));
		
			// Send the register address
			TWIM_SendData(TARGET_REGISTER);
		
			// Send the device address
			TWIM_SendAddress(TWI_READ(TWI_DEVICE_ADDRESS));
			
			// Read the data bytes
			for(uint8_t i = 0x00; i < (BYTES_TO_SEND - 1); i++)
			{
				I2C_ReadBuffer[i] = TWIM_ReadData(TWI_ACK);
			}

			I2C_ReadBuffer[BYTES_TO_SEND - 1] = TWIM_ReadData(TWI_NACK);
		
			// Send stop condition
			TWIM_SendStop(TWI_NACK);
		}
	
	#elif(EXAMPLE == 2)
		/*
			Interrupt driven TWI example
		*/

		// Fill the write buffer
		for(uint8_t i = 0x00; i < BYTES_TO_SEND; i++)
		{
			I2C_WriteBuffer[i] = i + 0x03;
		}
	
		/*
			Initialize the TWI
				-> Normal TWI
				-> Master mode
				-> No Timeout
				-> Master interrupts (low level)
				-> 100 kHz @ 2 MHz
		*/
		TWIM_InitInterrupt();

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
	
		// Transmit the message
		TWIM_Transmit(TWI_DEVICE_ADDRESS, TARGET_REGISTER, BYTES_TO_SEND, I2C_WriteBuffer);
		
		// Wait until data got transmitted
		while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));

		// Read the device
		for(uint8_t i = 0x00; i < 0x03; i++)
		{
			TWIM_Receive(TWI_DEVICE_ADDRESS, TARGET_REGISTER, BYTES_TO_SEND, I2C_ReadBuffer);

			// Wait until data are received
			while(!((TWIM_Status() == TWI_MASTER_RECEIVED) || (TWIM_Status() == TWI_MASTER_ERROR)));
		}

	#elif(EXAMPLE == 3)
		// Fill the write buffer
		for(uint8_t i = 0x00; i < BYTES_TO_SEND; i++)
		{
			I2C_WriteBuffer[i] = i + 0x01;
		}

		/*
			Initialize master mode TWI
				-> Normal TWI
				-> Master mode
				-> No Timeout
				-> No Interrupts
				-> 100 kHz @ 2 MHz
		*/
		TWIM_InitInterrupt();
		
		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
			
		// Send some data bytes to the slave	
		TWIM_Transmit(TWI_DEVICE_ADDRESS, TARGET_REGISTER, BYTES_TO_SEND, I2C_WriteBuffer);
		while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));

		// Disable interrupts and set the address pointer of the device to address 0
		cli();
		TWIM_SendAddress(TWI_WRITE(TWI_DEVICE_ADDRESS));
		TWIM_SendData(TARGET_REGISTER);
		TWIM_SendStop(0);
		sei();

		// Read some bytes
		TWIM_ReceiveBytes(TWI_DEVICE_ADDRESS, BYTES_TO_SEND, I2C_ReadBuffer);
		while(TWIM_Status() != TWI_MASTER_RECEIVED);

	#endif
#else
	#if(EXAMPLE == 1)
		// Fill the write buffer
		for(uint8_t i = 0x00; i < TWI_BUFFER_SIZE; i++)
		{
			I2C_WriteBuffer[i] = i + 0x10;
		}

		/*
			Initialize the TWI
				-> TWIC
				-> Normal TWI
				-> Master mode
				-> No Timeout
				-> Master interrupts (low level)
				-> 100 kHz @ 2 MHz
		*/
		TWIM_InitInterrupt();

		/*
			Initialize slave mode TWI
				-> TWIE
				-> Normal TWI
				-> Slave mode
				-> No Timeout
				-> Slave interrupts (low level)
		*/
		TWIS_Init(I2C_SlaveBuffer);

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
			
		// Send some data bytes to the slave	
		TWIM_TransmitBytes(TWI_SLAVE_ADDRESS, TWI_BUFFER_SIZE, I2C_WriteBuffer);
		while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
		
		// Read the data from the slave
		TWIM_ReceiveBytes(TWI_SLAVE_ADDRESS, TWI_BUFFER_SIZE, I2C_ReadBuffer);
		while(!((TWIM_Status() == TWI_MASTER_RECEIVED) || (TWIM_Status() == TWI_MASTER_ERROR)));
		
	#elif(EXAMPLE == 2)
	
		// Fill the write buffer
		for(uint8_t i = 0x00; i < TWI_BUFFER_SIZE; i++)
		{
			I2C_WriteBuffer[i] = i + 0x10;
		}

		/*
			Initialize the TWI
				-> TWIC
				-> Normal TWI
				-> Master mode
				-> No Timeout
				-> Master interrupts (low level)
				-> 100 kHz @ 2 MHz
		*/
		TWIM_InitInterrupt();

		/*
			Initialize slave mode TWI
				-> TWIE
				-> Normal TWI
				-> Slave mode
				-> No Timeout
				-> Slave interrupts (low level)
		*/
		TWIS_Init(I2C_SlaveBuffer);

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
			
		// Send some data bytes to the slave	
		TWIM_TransmitBytes(TWI_SLAVE_ADDRESS, TWI_BUFFER_SIZE + 1, I2C_WriteBuffer);
		while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
		
	#elif(EXAMPLE == 3)
	
		// Fill the write buffer
		for(uint8_t i = 0x00; i < TWI_BUFFER_SIZE; i++)
		{
			I2C_WriteBuffer[i] = i + 0x10;
		}

		/*
			Initialize the TWI
				-> TWIC
				-> Normal TWI
				-> Master mode
				-> No Timeout
				-> Master interrupts (low level)
				-> 100 kHz @ 2 MHz
		*/
		TWIM_InitInterrupt();

		/*
			Initialize slave mode TWI
				-> TWIE
				-> Normal TWI
				-> Slave mode
				-> No Timeout
				-> Slave interrupts (low level)
		*/
		TWIS_Init(I2C_SlaveBuffer);

		/*
			Initialize the interrupts
				-> Enable low level interrupts
				-> Enable global interrupts
		*/
		PMIC.CTRL = PMIC_LOLVLEN_bm;
		sei();
			
		// Send some data bytes to the slave	
		TWIM_TransmitBytes(TWI_SLAVE_ADDRESS, TWI_BUFFER_SIZE, I2C_WriteBuffer);
		while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
		
		// Read the data from the slave
		TWIM_ReceiveBytes(TWI_SLAVE_ADDRESS, TWI_BUFFER_SIZE + 1, I2C_ReadBuffer);
		while(!((TWIM_Status() == TWI_MASTER_RECEIVED) || (TWIM_Status() == TWI_MASTER_ERROR) || (TWIM_Status() == TWI_MASTER_BUFFEROVERFLOW)));
		
	#endif
#endif

	// Get the state of the master and the slave device
	TWI_MasterStatus_t StatusMaster = TWIM_Status();
	TWI_SlaveStatus_t StatusSlave = TWIS_Status();

    while(1) 
    {
    }
}