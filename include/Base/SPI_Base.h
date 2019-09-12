/*
 * SPI_Base.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for AVR SPI interface

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

/** @file SPI_Base.h
 *  @brief Common definitions for AVR SPI interface.
 *
 *  @author Daniel Kampert
 */

#ifndef SPI_BASE_H_
#define SPI_BASE_H_

 #if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
	#include "Common/Board/MCU/XMega256A3BU.h"
 #elif MCU_NAME == MCU_NAME_ATMEGA32
	#include "Common/Board/MCU/ATmega32.h"
 #endif

 #include "Common/types.h"
 #include "Common/RingBuffer/RingBuffer.h"

 /** @brief	SPI callback definition
 */
 typedef void (*SPI_Callback_t)(void);

 /** @brief SPI device modes.
  */
 typedef enum
 {
	 SPI_SLAVE = 0x00,											/**< SPI slave mode */ 
	 SPI_MASTER = 0x01,											/**< SPI master mode */ 
 } SPI_DeviceMode_t;

 /** @brief SPI data transmission modes.
  */
 typedef enum
 {
	SPI_DATAORDER_MSB_FIRST = 0x00,								/**< MSB first */ 
	SPI_DATAORDER_LSB_FIRST = 0x01,								/**< LSB first */ 
 } SPI_DataOrder_t;

 /** @brief SPI clock polarity settings.
  */
 typedef enum
 {
	SPI_CLOCK_LOW = 0x00,										/**< Clock polarity low */ 
	SPI_CLOCK_HIGH = 0x01,										/**< Clock polarity high */ 
 } SPI_ClockPolarity_t;

 /** @brief SPI clock phase settings.
  */
 typedef enum
 {
	SPI_PHASE_LOW = 0x00,										/**< Clock phase low */ 
	SPI_PHASE_HIGH = 0x01,										/**< Clock phase low */ 
 } SPI_ClockPhase_t;

 /** @brief SPI clock modes.
  */
 typedef enum
 {
	 SPI_MODE_0 = (SPI_CLOCK_LOW << 0x01) | SPI_PHASE_LOW,		/**< SPI mode 0 */ 
	 SPI_MODE_1 = (SPI_CLOCK_LOW << 0x01) | SPI_PHASE_HIGH,		/**< SPI mode 1 */ 
	 SPI_MODE_2 = (SPI_CLOCK_HIGH << 0x01) | SPI_PHASE_LOW,		/**< SPI mode 2*/ 
	 SPI_MODE_3 = (SPI_CLOCK_HIGH << 0x01) | SPI_PHASE_HIGH,	/**< SPI mode 3 */ 
 } SPI_Mode_t;

 /** @brief SPI clock prescaler.
  */
 typedef enum
 {
	SPI_PRESCALER_4 = 0x00,										/**< Clock prescaler 4 */ 
	SPI_PRESCALER_16 = 0x01,									/**< Clock prescaler 16 */ 
	SPI_PRESCALER_64 = 0x02,									/**< Clock prescaler 64 */ 
	SPI_PRESCALER_128 = 0x03,									/**< Clock prescaler 128 */ 
	SPI_PRESCALER_2 = 0x04,										/**< Clock prescaler 2 */ 
	SPI_PRESCALER_8 = 0x05,										/**< Clock prescaler 8 */ 
	SPI_PRESCALER_32 = 0x06,									/**< Clock prescaler 32 */ 
 } SPI_ClockPrescaler_t;

 /** @brief SPI communication status codes.
  */
 typedef enum
 {
	 SPI_MESSAGE_COMPLETE = 0x00,								/**< Message transmission complete */
	 SPI_MESSAGE_PENDING = 0x01,								/**< Message transmission in progress */
	 SPI_MESSAGE_ERROR = 0x02,									/**< Transmission error */
	 SPI_BUFFER_OVERFLOW = 0x02,								/**< SPI slave buffer overflow */
 } SPI_Status_t;

 /** @brief SPI callback types.
  */
 typedef enum
 {
	 SPI_COMPLETE_INTERRUPT = 0x01,								/**< SPI transmit complete interrupt */
	 SPI_ERROR_INTERRUPT = 0x02,								/**< Error interrupt */
 } SPI_CallbackType_t;

 /** @brief SPI master configuration object.
  */
 typedef struct
 {
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		 void* Device;											/**< Pointer to USART-SPI or SPI device object */
		 uint32_t SPIClock;										/**< SPI clock frequency \n
																	 NOTE: You only need this when you use the USART-SPI interface. */
	 #endif

	 SPI_Mode_t Mode;											/**< SPI mode */
	 SPI_DataOrder_t DataOrder;									/**< Data order */
	 Bool_t EnableDoubleSpeed;									/**< Set #TRUE to enable double speed */
	 SPI_ClockPrescaler_t Prescaler;							/**< Clock prescaler for SPI module 
																	 NOTE: You only need this when you use the SPI interface. */
 } SPIM_Config_t;

 /** @brief SPI slave configuration object.
  */
 typedef struct
 {
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		void* Device;											/**< Pointer to USART-SPI or SPI device object */
	 #endif
	 
	 SPI_Mode_t Mode;											/**< SPI mode */
	 SPI_DataOrder_t DataOrder;									/**< Data order */
 } SPIS_Config_t;

 /** @brief SPI interrupt configuration object.
  */
 typedef struct
 {
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		void* Device;											/**< Pointer to USART-SPI or SPI device object */
		Interrupt_Level_t InterruptLevel;						/**< Interrupt level */
	 #endif
	 
	 SPI_CallbackType_t Source;									/**< SPI interrupt type */
	 SPI_Callback_t Callback;									/**< Function pointer to SPI callback */
 } SPI_InterruptConfig_t;

 /** @brief SPI message object.
  */
 typedef struct
 {
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		void* Device;											/**< Pointer to target USART-SPI or SPI device object */
		PORT_t* Port;											/**< Port for slave select */
	 #else
		volatile uint8_t* Port;
	 #endif
	 
	 uint8_t Pin;												/**< Pin for slave select */
	 uint8_t* BufferOut;										/**< Pointer to output buffer */
	 uint8_t* BufferIn;											/**< Pointer to input buffer */
	 volatile uint8_t BytesProcessed;							/**< Counter for processed bytes */
	 uint8_t Length;											/**< Message length */
	 volatile SPI_Status_t Status;								/**< Transmission status */
 } SPI_Message_t;

 /** @brief SPI buffer object.
  */
 typedef struct
 {
	 uint8_t* RxBuffer;											/**< Pointer to receive buffer */
	 uint8_t* TxBuffer;											/**< Pointer to transmit buffer */
	 volatile uint8_t BytesProcessed;							/**< Counter for processed bytes */
	 volatile SPI_Status_t Status;								/**< Transmission status */
 } SPI_Buffer_t;

#endif /* SPI_BASE_H_ */