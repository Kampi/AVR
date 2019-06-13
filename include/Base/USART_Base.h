/*
 * USART_Base.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Common definitions for AVR USART interface

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

/** @file USART_Base.h
 *  @brief Common definitions for AVR USART interface.
 *
 *  @author Daniel Kampert
 */

#ifndef USART_BASE_H_
#define USART_BASE_H_ 

 #include "Common/types.h"
 #include "Common/RingBuffer/RingBuffer.h"

 /** @defgroup Serial
  *  @{
  */
 	 /** @defgroup Serial-Commands
	  *  Serial commands.
	  *  @{
	  */
		 #define LF											0x0A	/**< Line feed */ 
		 #define CR											0x0D	/**< Carriage return */ 
		 #define XON										0x11	/**< Software flow control on */ 
		 #define XOFF										0x13	/**< Software flow control off */
	 /** @} */ // end of Serial-Commands
 /** @} */ // end of Serial

 #ifndef USART_DEFAULT_ECHO
	#define USART_DEFAULT_ECHO						TRUE
	#warning "Using default echo"
 #endif

 /** @brief	USART callback definition
 */
 typedef void (*USART_Callback_t)(void);

 /** @brief USART parity modes.
  */
 typedef enum
 {
	USART_PARITY_NONE = 0x00,		/**< No parity */ 
	USART_PARITY_EVEN = 0x01,		/**< Even parity */ 
	USART_PARITY_ODD = 0x02,		/**< Odd parity */ 
 } USART_Parity_t;

 /** @brief USART stop bit settings.
  */
 typedef enum
 {
	USART_STOP_1 = 0x00,			/**< One stop bit */ 
	USART_STOP_2 = 0x01,			/**< Two stop bits */ 
 } USART_Stop_t;

 /** @brief USART data length settings.
  */
 typedef enum
 {
	USART_SIZE_5 = 0x00,			/**< 5 data bits */ 
	USART_SIZE_6 = 0x01,			/**< 6 data bits */ 
	USART_SIZE_7 = 0x02,			/**< 7 data bits */ 
	USART_SIZE_8 = 0x03,			/**< 8 data bits */ 
	USART_SIZE_9 = 0x07,			/**< 9 data bits */ 
 } USART_Size_t;

 /** @brief USART interface operation mode.
  */
 typedef enum
 {
	USART_MODE_ASYNCH = 0x00,		/**< UART mode */ 
	USART_MODE_SYNCH = 0x01,		/**< USART mode */ 
	USART_MODE_IRCOM = 0x02,		/**< IRCOM mode */ 
 } USART_DeviceMode_t;

 /** @brief USART transmitter/receiver settings.
  */
 typedef enum
 {
	USART_DIRECTION_NONE = 0x00,	/**< No transmitter and receiver enabled */ 
	USART_DIRECTION_TX = 0x01,		/**< Transmitter only */ 
	USART_DIRECTION_RX = 0x02,		/**< Receiver only */ 
	USART_DIRECTION_BOTH = 0x03,	/**< Receiver and transmitter */ 
 } USART_Direction_t;
 
 /** @brief USART-SPI clock polarity.
  */
 typedef enum
 {
	 USART_POL_RISING = 0x00,		/**< Clock polarity rising edge */ 
	 USART_POL_FALLING = 0x01,		/**< Clock polarity falling edge */ 
 } USART_Polarity_t;

 /** @brief USART callback types.
  */
 typedef enum
 {
	 USART_DRE_INTERRUPT = 0x01,	/**< Data register empty interrupt */ 
	 USART_TXC_INTERRUPT = 0x02,	/**< Transmit complete interrupt */ 
	 USART_RXC_INTERRUPT = 0x04,	/**< Receive complete interrupt */ 
	 USART_BUFFER_OVERFLOW = 0x08,	/**< Buffer overflow interrupt */ 
 } USART_CallbackType_t;

#endif /* USART_BASE_H_ */