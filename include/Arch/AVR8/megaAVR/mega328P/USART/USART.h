/*
 * USART.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 mega328P USART module.

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

/** @file Arch/AVR8/megaAVR/mega328P/USART/USART.h
 *  @brief Driver for Atmel AVR8 megaAVR USART module.
 *
 *  This contains the prototypes and definitions for the mega328P USART driver.
 *
 *  @author Daniel Kampert
 *  @bug 	- No synchronous mode
 */

#ifndef USART_H_
#define USART_H_
 
 #include "Common/Common.h"

 #include "Arch/AVR8/megaAVR/mega328P/CPU/CPU.h"
 #include "Arch/AVR8/megaAVR/mega328P/GPIO/GPIO.h"
 
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

 /** @brief	USART callback definition.
 */
 typedef void (*USART_Callback_t)(void);

 /** @brief USART parity modes.
  */
 typedef enum
 {
	USART_PARITY_NONE = 0x00,					/**< No parity */
	USART_PARITY_EVEN = 0x01,					/**< Even parity */
	USART_PARITY_ODD = 0x02,					/**< Odd parity */
 } USART_Parity_t;

 /** @brief USART stop bit settings.
  */
 typedef enum
 {
	USART_STOP_1 = 0x00,						/**< One stop bit */
	USART_STOP_2 = 0x01,						/**< Two stop bits */
 } USART_Stop_t;

 /** @brief USART data length settings.
  */
 typedef enum
 {
	USART_SIZE_5 = 0x00,						/**< 5 data bits */
	USART_SIZE_6 = 0x01,						/**< 6 data bits */
	USART_SIZE_7 = 0x02,						/**< 7 data bits */
	USART_SIZE_8 = 0x03,						/**< 8 data bits */
	USART_SIZE_9 = 0x07,						/**< 9 data bits */
 } USART_Size_t;

 /** @brief USART interface operation mode.
  */
 typedef enum
 {
	USART_MODE_ASYNCH = 0x00,					/**< UART mode */
	USART_MODE_SYNCH = 0x01,					/**< USART mode */
	USART_MODE_IRCOM = 0x02,					/**< IRCOM mode */
 } USART_DeviceMode_t;

 /** @brief USART transmitter/receiver settings.
  */
 typedef enum
 {
	USART_DIRECTION_NONE = 0x00,				/**< No transmitter and receiver enabled */
	USART_DIRECTION_TX = 0x01,					/**< Transmitter only */
	USART_DIRECTION_RX = 0x02,					/**< Receiver only */
	USART_DIRECTION_BOTH = 0x03,				/**< Receiver and transmitter */
 } USART_Direction_t;
 
 /** @brief USART-SPI clock polarity.
  */
 typedef enum
 {
	 USART_POL_RISING = 0x00,					/**< Clock polarity rising edge */
	 USART_POL_FALLING = 0x01,					/**< Clock polarity falling edge */
 } USART_Polarity_t;

 /** @brief USART callback types.
  */
 typedef enum
 {
	 USART_DRE_INTERRUPT = 0x01,				/**< Data register empty interrupt */
	 USART_TXC_INTERRUPT = 0x02,				/**< Transmit complete interrupt */
	 USART_RXC_INTERRUPT = 0x04,				/**< Receive complete interrupt */
	 USART_BUFFER_OVERFLOW = 0x08,				/**< Buffer overflow interrupt */
 } USART_CallbackType_t;

 /** @brief USART interrupt configuration object.
  */
 typedef struct
 {
	 USART_CallbackType_t Source;				/**< USART interrupt type */
	 USART_Callback_t Callback;					/**< Function pointer to USART callback */
 } USART_InterruptConfig_t;

 /** @brief USART configuration object.
  */
 typedef struct
 {
	USART_DeviceMode_t DeviceMode;				/**< USART device mode */
	USART_Direction_t Direction;				/**< Direction mode */
	uint32_t Baudrate;							/**< Baudrate */
	USART_Parity_t Parity;						/**< Parity settings */
	USART_Stop_t Stop;							/**< Stop bit settings */
	USART_Size_t Size;							/**< Data bit settings */
	USART_Polarity_t ClockPolarity;				/**< Clock polarity. Only needed in synchronous mode */
	Bool_t EnableDoubleSpeed;					/**< Set #TRUE to enable double speed */
	Bool_t EnableInterruptSupport;				/**< Set to #TRUE to enable interrupt support */
	Bool_t EnableEcho;							/**< Set to #TRUE to enable the USART echo */
 } USART_Config_t;

 /** @brief				Set the direction of the USART interface.
  *  @param Direction	USART direction
  */
 static inline void USART_SetDirection(const USART_Direction_t Direction) __attribute__ ((always_inline));
 static inline void USART_SetDirection(const USART_Direction_t Direction)
 {
	 UCSR0B = (UCSR0B & (~(0x03 << 0x03))) | (Direction << 0x03);
 }

 /** @brief		Get the direction of the USART interface.
  *  @return	USART direction
  */
 static inline USART_Direction_t USART_GetDirection(void) __attribute__ ((always_inline));
 static inline USART_Direction_t USART_GetDirection(void)
 {
	 return (UCSR0B >> 0x03) & 0x03;
 }

 /** @brief		Get a single byte from the USART interface.
  *  @return	Data byte
  */
 static inline char USART_GetChar(void) __attribute__ ((always_inline));
 static inline char USART_GetChar(void)
 {
	 return UDR0;
 }

 /** @brief			Transmit a single byte with the USART interface.
  *  @param Device	Pointer to USART object
  *  @param Data	Data byte
  */
 static inline void USART_SendChar(const char Data) __attribute__ ((always_inline));
 static inline void USART_SendChar(const char Data)
 {
	 if(!(UCSR0B & (0x01 << UCSZ02)))
	 {
		 while(!(UCSR0A & (0x01 << UDRE0)));
		 UDR0 = Data;
	 }
	 else
	 {
	 }
 }

 /** @brief			Initialize the USART interface.
  *  @param Config	Pointer to USART device configuration struct
  */
 void USART_Init(USART_Config_t* Config);

 /** @brief			Transmit a char array with the USART interface.
  *  @param Data	Pointer to data array
  */
 void USART_Write(char* Data);

 /** @brief				Set the baud rate of a USART interface.
  *  @param Baudrate	Baudrate for the interface
  *  @param Clock		USART module clock
  *  @param DoubleSpeed	Double speed enabled/disabled
  */
 void USART_SetBaudrate(const uint32_t BaudRate, const uint32_t Clock, const Bool_t DoubleSpeed);

#endif /* USART_H_ */