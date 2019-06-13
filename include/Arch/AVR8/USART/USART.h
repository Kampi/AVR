/*
 * USART.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 USART Interface

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

/** @file include/Arch/AVR8/ATmega/USART/USART.h
 *  @brief Driver for AVR8 USART module.
 *
 *  This contains the prototypes and definitions for the AVR8 USART driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USART_H_
#define USART_H_
 
 #if(MCU_NAME == MCU_NAME_ATMEGA32)
	#include "Common/Board/MCU/ATmega32.h"
 #else
	#error "No valid mcu"
 #endif
 
 #include "Arch/AVR8/ATmega/CPU/CPU.h"
 
 #include "Base/USART_Base.h"

 /** 
  * USART interrupt configuration object
  */
 typedef struct
 {
	 USART_CallbackType_t Source;				/**< USART interrupt type */
	 USART_Callback_t Callback;					/**< Function pointer to USART callback */ 	
 } USART_InterruptConfig_t;

 /** 
  * USART configuration object
  */
 typedef struct
 {
	USART_DeviceMode_t Mode;					/**< USART device mode */ 	
	USART_Direction_t Direction;				/**< Direction mode */ 	
	uint32_t Baudrate;							/**< Baudrate */ 
	USART_Parity_t Parity;						/**< Parity settings */ 
	USART_Stop_t Stop;							/**< Stop bit settings */ 
	USART_Size_t Size;							/**< Data bit settings */ 	
	int8_t BSCALE;								/**< BSCALE value for baud rate */
	Bool_t DoubleSpeed;							/**< Set *TRUE for double speed */ 	
	USART_Polarity_t ClockPolarity;				/**< Clock polarity. Only needed in synchronous mode */ 	
 } USART_Config_t;

 /** @brief				Set the direction of the USART interface.
  *  @param Direction	USART direction
  */
 static inline void USART_SetDirection(const USART_Direction_t Direction) __attribute__ ((always_inline));
 static inline void USART_SetDirection(const USART_Direction_t Direction)
 {
	 UCSRB = (UCSRB & (~(0x03 << 0x03))) | (Direction << 0x03);
 }

 /** @brief		Get the direction of the USART interface.
  *  @return	USART direction
  */
 static inline USART_Direction_t USART_GetDirection(void) __attribute__ ((always_inline));
 static inline USART_Direction_t USART_GetDirection(void)
 {
	 return (UCSRB >> 0x03) & 0x03;
 }

 /** @brief			Initialize the USART interface.
  *  @param Config	Pointer to USART device configuration struct
  */
 void USART_Init(USART_Config_t* Config);
 
 /** @brief			Install a new callback for the USART interface.
  *  @param Config	Pointer to configuration structure
  */
 void USART_InstallCallback(USART_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Callback	Callback which should be removed
  */
 void USART_RemoveCallback(const USART_CallbackType_t Callback);
 
 /** @brief		Get a single byte from the USART interface.
  *  @return	Data byte
  */
 static inline char USART_GetChar(void) __attribute__ ((always_inline));
 static inline char USART_GetChar(void)
 {
	 return UDR;
 }

 /** @brief			Transmit a single byte with the USART interface.
  *  @param Device	Pointer to USART object
  *  @param Data	Data byte
  */
 static inline void USART_SendChar(const char Data) __attribute__ ((always_inline));
 static inline void USART_SendChar(const char Data)
 {
	 if(!(UCSRB & (0x01 << UCSZ2)))
	 {
		 UDR = Data;
		 while(!(UCSRA & (0x01 << UDRE)));
	 }
	 else
	 {
	 }
 }

 /** @brief			Flush the USART transmit buffer.
  */
 void USART_Flush(void);
 
 /** @brief			Interrupt driven transmit function for the USART.
					NOTE: You have to enable global interrupts to use it!
  *  @param Data	Pointer to data array
  */
 void USART_Print(char* Data);
 
 /** @brief			Transmit a char array with the USART interface.
  *  @param Data	Pointer to data array
  */
 void USART_Write(char* Data);
 
 /** @brief			Transmit a complete line (including CR + LF) with the USART interface.
  *  @param Data	Pointer to data array
  */
 void USART_WriteLine(char* Data);
 
 /** @brief			Transmit a decimal number with the USART interface.
  *  @param Value	Decimal number
  */
 void USART_WriteDecimal(const uint32_t Value);
 
 /** @brief			Enable/Disable the USART echo.
  *  @param Enable	Enable/Disable the echo
  */
 void USART_SwitchEcho(const Bool_t Enable);
 
 /** @brief				Set the baud rate of a USART interface.
  *  @param Baudrate	Baudrate for the interface
  *  @param Clock		USART module clock
  *  @param DoubleSpeed	Double speed enabled/disabled
  */
 void USART_SetBaudrate(const uint32_t BaudRate, const uint32_t Clock, const Bool_t DoubleSpeed);

#endif /* USART_H_ */