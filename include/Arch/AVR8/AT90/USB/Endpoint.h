/*
 * Endpoint.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Endpoint definition for the Atmel AVR8 AT90 USB interface.

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

/** @file Arch/AVR8/AT90/USB/Endpoint.h
 *  @brief Endpoint definition for the Atmel AVR8 AT90 USB interface.
 *
 *  This file contains the prototypes and definitions for Atmel AVR endpoints.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef ENDPOINT_H_
#define ENDPOINT_H_

 #include "Arch/AVR8/AT90/USB/USB_Controller.h"
 #include "Services/USB/Core/StandardRequest.h"

 /** @brief	Endpoint types.
  */
 typedef enum
 {
	 ENDPOINT_TYPE_CONTROL = 0x00,										/**< Control endpoint */ 
	 ENDPOINT_TYPE_ISOCHRONOUS = 0x01,									/**< Isochronous endpoint */ 
	 ENDPOINT_TYPE_BULK = 0x02,											/**< Bulk endpoint */ 
	 ENDPOINT_TYPE_INTERRUPT = 0x03										/**< Interrupt endpoint */ 
 } Endpoint_Type_t;

 /** @brief	Endpoint size in bytes.
  */
 typedef enum
 {
	 ENDPOINT_SIZE_8 = 0x00,											/**< Endpoint size 8 bytes */ 
	 ENDPOINT_SIZE_16 = 0x01,											/**< Endpoint size 16 bytes */ 
	 ENDPOINT_SIZE_32 = 0x02,											/**< Endpoint size 32 bytes */ 
	 ENDPOINT_SIZE_64 = 0x03,											/**< Endpoint size 64 bytes */ 
	 ENDPOINT_SIZE_128 = 0x04,											/**< Endpoint size 128 bytes \n
																			 NOTE: Only for endpoint 1! */ 
	 ENDPOINT_SIZE_256 = 0x05,											/**< Endpoint size 256 bytes \n
																			 NOTE: Only for endpoint 1! */ 
 } Endpoint_Size_t;

 /** @brief	Endpoint directions.
  */
 typedef enum
 {
	 ENDPOINT_DIRECTION_OUT = 0x00,										/**< Endpoint direction OUT */ 
	 ENDPOINT_DIRECTION_IN = 0x80,										/**< Endpoint direction IN */ 
 } Endpoint_Direction_t;

 /** @brief	Control endpoint size in bytes.
  */
 #if(!defined(ENDPOINT_CONTROL_SIZE))
	 #define ENDPOINT_CONTROL_SIZE					ENDPOINT_CONTROL_DEFAULT_SIZE
 #endif
 
 /** @brief	Control endpoint address.
  */
 #if(!defined(ENDPOINT_CONTROL_ADDRESS))
	 #define ENDPOINT_CONTROL_ADDRESS				ENDPOINT_CONTROL_DEFAULT_ADDRESS
 #endif

 /** @brief			Select an endpoint.
  *  @param Address	Endpoint address
  */
 static inline void Endpoint_Select(const uint8_t Address) __attribute__ ((always_inline));
 static inline void Endpoint_Select(const uint8_t Address)
 {
	 UENUM = (Address & 0x0F);
 }

 /** @brief			Reset the selected endpoint.
  *					NOTE: You have so use #Endpoint_Select first!
  *  @param Address	Endpoint address
  */
 static inline void Endpoint_Reset(const uint8_t Address) __attribute__ ((always_inline));
 static inline void Endpoint_Reset(const uint8_t Address)
 {
	 UERST = (0x01 << (Address & 0x0F));
	 UERST = 0x00;
 }

 /** @brief	Enable the selected endpoint.
  *			NOTE: You have so use #Endpoint_Select first!
  */
 static inline void Endpoint_Enable(void) __attribute__ ((always_inline));
 static inline void Endpoint_Enable(void)
 {
	 UECONX |= (0x01 << EPEN);
 }

 /** @brief	Disable the selected endpoint.
  *			NOTE: You have so use #Endpoint_Select first!
  */
 static inline void Endpoint_Disable(void) __attribute__ ((always_inline));
 static inline void Endpoint_Disable(void)
 {
	 UECONX &= ~(0x01 << EPEN);
 }

 /** @brief		Get the direction of the active endpoint.
  *				NOTE: You have so use #Endpoint_Select first!
  *  @return	Endpoint direction
  */
 static inline Endpoint_Direction_t Endpoint_GetDirection(void) __attribute__ ((always_inline));
 static inline Endpoint_Direction_t Endpoint_GetDirection(void)
 {
	 if(UECFG0X & (0x01 << EPDIR))
	 {
		 return ENDPOINT_DIRECTION_IN;
	 }
	 
	 return ENDPOINT_DIRECTION_OUT;
 }

 /** @brief		Get the current active endpoint.
  *  @return	Endpoint address
  */
 static inline uint8_t Endpoint_GetCurrent(void) __attribute__ ((always_inline));
 static inline uint8_t Endpoint_GetCurrent(void)
 {
	 return (UENUM & 0x0F);
 }

 /** @brief		Read one data byte from the current active endpoint.
  *				NOTE: You have so use #Endpoint_Select first!
  *  @return	Endpoint data
  */
 static inline uint8_t Endpoint_ReadByte(void) __attribute__ ((always_inline));
 static inline uint8_t Endpoint_ReadByte(void)
 {
	 return UEDATX;
 }

 /** @brief		Acknowledge the packet and clear the endpoint bank.
  *				NOTE: You have so use #Endpoint_Select first!
  */
 static inline void Endpoint_ClearSETUP(void) __attribute__ ((always_inline));
 static inline void Endpoint_ClearSETUP(void)
 {
	 UEINTX &= ~(0x01 << RXSTPI);
 }

 /** @brief		Acknowledge OUT data and clear the endpoint bank.
  *				NOTE: You have so use #Endpoint_Select first!
  */
 static inline void Endpoint_AckOUT(void) __attribute__ ((always_inline));
 static inline void Endpoint_AckOUT(void)
 {
	 UEINTX &= ~((0x01 << RXOUTI) | (0x01 << FIFOCON));
 }

 /** @brief		Send the packet and clear the endpoint bank.
  *				NOTE: You have so use #Endpoint_Select first!
  */
 static inline void Endpoint_FlushIN(void) __attribute__ ((always_inline));
 static inline void Endpoint_FlushIN(void)
 {
	 UEINTX &= ~((0x01 << TXINI) | (0x01 << FIFOCON));
 }

 /** @brief		Test if the control endpoint received an setup packed.
  *				NOTE: You have so use #Endpoint_Select first!
  *  @return	#true when a setup packed was received
  */
 static inline uint8_t Endpoint_SETUPReceived(void) __attribute__ ((always_inline));
 static inline uint8_t Endpoint_SETUPReceived(void)
 {
	 return UEINTX & (0x01 << RXSTPI);
 }

 /** @brief		Test if new OUT data is received.
  *				NOTE: You have so use #Endpoint_Select first!
  *  @return	#true when OUT data received.
  */
 static inline uint8_t Endpoint_OUTReceived(void) __attribute__ ((always_inline));
 static inline uint8_t Endpoint_OUTReceived(void)
 {
	 return UEINTX & (0x01 << RXOUTI);
 }

 /** @brief		Test if the bank can accept new IN data.
  *				NOTE: You have so use #Endpoint_Select first!
  *  @return	#true when bank is ready
  */
 static inline uint8_t Endpoint_INReady(void) __attribute__ ((always_inline));
 static inline uint8_t Endpoint_INReady(void)
 {
	 return UEINTX & (0x01 << TXINI);
 }

 /** @brief	Stall the current endpoint.
  *			NOTE: You have so use #Endpoint_Select first!
  */
 static inline void Endpoint_STALLTransaction(void) __attribute__ ((always_inline));
 static inline void Endpoint_STALLTransaction(void)
 {
	 UECONX |= (0x01 << STALLRQ);
 }

 /** @brief	Clear the stall condition.
  *			NOTE: You have so use #Endpoint_Select first!
  */
 static inline void Endpoint_ClearSTALL(void) __attribute__ ((always_inline));
 static inline void Endpoint_ClearSTALL(void)
 {
	 UECONX |= (0x01 << STALLRQC);
 }

 /** @brief		Test if the endpoint is stalled.
  *				NOTE: You have so use #Endpoint_Select first!
  *  @return	#true when stalled
  */
 static inline uint8_t Endpoint_IsSTALL(void) __attribute__ ((always_inline));
 static inline uint8_t Endpoint_IsSTALL(void)
 { 
	 return UECONX & (0x01 << STALLRQ);
 }

 /** @brief		Test if the application can read data from the endpoint or can write data to the endpoint.
  *				NOTE: You have so use #Endpoint_Select first!
  *  @return	#true when IN endpoint and write is allowed, #true when OUT endpoint and read is allowed
  */
 static inline uint8_t Endpoint_IsReadWriteAllowed(void) __attribute__ ((always_inline));
 static inline uint8_t Endpoint_IsReadWriteAllowed(void)
 { 
	 return UEINTX & (0x01 << RWAL);
 }

 /** @brief		Get the number of bytes currently stored in the endpoint FIFO.
  *				NOTE: You have so use #Endpoint_Select first!
  *  @return	Byte count
  */
 static inline uint16_t Endpoint_GetBytes(void) __attribute__ ((always_inline));
 static inline uint16_t Endpoint_GetBytes(void)
 {
	 return UEBCX;
 }

 /** @brief	Write one data byte in the currently selected endpoint.
  *			NOTE: You have so use #Endpoint_Select first!
  *  @Data	Data byte
  */
 static inline void Endpoint_WriteByte(const uint8_t Data) __attribute__ ((always_inline));
 static inline void Endpoint_WriteByte(const uint8_t Data)
 {
	 UEDATX = Data;
 }

 /** @brief	Write two data bytes (one integer) in the currently selected endpoint.
  *			NOTE: You have so use #Endpoint_Select first!
  *  @Data	Data bytes
  */
 static inline void Endpoint_WriteInteger(const uint16_t Data) __attribute__ ((always_inline));
 static inline void Endpoint_WriteInteger(const uint16_t Data)
 {
	 UEDATX = (Data & 0xFF);
	 UEDATX = (Data >> 0x08);
 }

 /** @brief				Configure an endpoint.
  *  @param Address		Endpoint address
  *  @param Type		Endpoint type
  *  @param Size		Endpoint size in bytes
  *  @param DoubleBank	Set to #true to use a double bank for the endpoint
  *  @return			#true when successfully
  */
 uint8_t Endpoint_Configure(const uint8_t Address, const Endpoint_Type_t Type, const uint8_t Size, const uint8_t DoubleBank);

 /** @brief				Process the STATUS stage of the control transmission. The operation depends on the request direction.
  *  @param Direction	USB request direction
  */
 void Endpoint_HandleSTATUS(const USB_RequestDirection_t Direction);

#endif /* ENDPOINT_H_ */