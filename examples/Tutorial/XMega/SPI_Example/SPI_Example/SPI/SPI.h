/*
 * SPI.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega SPI.

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

#ifndef SPI_H_
#define SPI_H_

 #include <avr/io.h>
 #include <avr/interrupt.h>

 #define SPI_BUFFER_SIZE						8
 
 typedef struct
 {
	 uint8_t* BufferOut;
	 uint8_t* BufferIn;
	 uint8_t BytesProcessed;
	 uint8_t Length;
	 uint8_t Status;
	 PORT_t* Port;
	 uint8_t Pin;
 } SPI_Message_t;
 
 typedef struct 
 {
	 uint8_t* RxBuffer;
	 uint8_t* TxBuffer;
	 uint8_t BytesProcessed;
	 uint8_t Status;
 } SPI_Buffer_t;

 void USART_SPI_Init(void);
 void USART_SPI_InitInterrupt(void);
 uint8_t USART_SPI_SendData(uint8_t Data);
 void USART_SPI_Transmit(SPI_Message_t* Message);
 uint8_t USART_SPI_Status(void);
 void USART_SPI_Select(PORT_t* Port, uint8_t Pin);
 void USART_SPI_Deselect(PORT_t* Port, uint8_t Pin);
 
 void SPIM_Init(void);
 void SPIM_InitInterrupt(void);
 uint8_t SPIM_SendData(uint8_t Data);
 void SPIM_Transmit(SPI_Message_t* Message);
 uint8_t SPIM_Status(void); 
 void SPIM_Select(PORT_t* Port, uint8_t Pin);
 void SPIM_Deselect(PORT_t* Port, uint8_t Pin);
 
 void SPIS_Init(uint8_t* ReceiveBuffer, uint8_t* TransmitBuffer);
 void SPIS_InitBuffer(uint8_t* TransmitBuffer);
 uint8_t SPIS_ActiveTransmission(void);

#endif /* SPI_H_ */