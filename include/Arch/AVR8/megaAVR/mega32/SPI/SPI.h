/*
 * SPI.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 SPI Interface.

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

/** @file Arch/AVR8/megaAVR/SPI/SPI.h
 *  @brief Driver for AVR8 SPI module.
 *
 *  This contains the prototypes and definitions for the AVR8 SPI driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef SPI_H_
#define SPI_H_

 #include "Common/Common.h"

 #include "Arch/AVR8/megaAVR/CPU/CPU.h"
 #include "Arch/AVR8/megaAVR/GPIO/GPIO.h"

 /** @brief	SPI callback definition.
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

 /** @brief SPI buffer object.
  */
 typedef struct
 {
	 uint8_t* RxBuffer;											/**< Pointer to receive buffer */
	 uint8_t* TxBuffer;											/**< Pointer to transmit buffer */
	 volatile uint8_t BytesProcessed;							/**< Counter for processed bytes */
	 volatile SPI_Status_t Status;								/**< Transmission status */
 } SPI_Buffer_t;

 /** @brief SPI master configuration object.
  */
 typedef struct
 {
	 SPI_Mode_t Mode;												/**< SPI mode */
	 SPI_DataOrder_t DataOrder;										/**< Data order */
	 bool EnableDoubleSpeed;										/**< Set #true to enable double speed */
	 SPI_ClockPrescaler_t Prescaler;								/**< Clock prescaler for SPI module 
																		 NOTE: You only need this when you use the SPI interface. */
 } SPIM_Config_t;

 /** @brief SPI slave configuration object.
  */
 typedef struct
 {	 
	 SPI_Mode_t Mode;												/**< SPI mode */
	 SPI_DataOrder_t DataOrder;										/**< Data order */
 } SPIS_Config_t;

 /** @brief SPI interrupt configuration object.
  */
 typedef struct
 {	 
	 SPI_CallbackType_t Source;										/**< SPI interrupt type */
	 SPI_Callback_t Callback;										/**< Function pointer to SPI callback */
 } SPI_InterruptConfig_t;

 /** @brief SPI message object.
  */
 typedef struct
 {
	 PORT_t* Port;													/**< Port for slave select */
	 uint8_t Pin;													/**< Pin for slave select */
	 uint8_t* BufferOut;											/**< Pointer to output buffer */
	 uint8_t* BufferIn;												/**< Pointer to input buffer */
	 volatile uint8_t BytesProcessed;								/**< Counter for processed bytes */
	 uint8_t Length;												/**< Message length */
	 volatile SPI_Status_t Status;									/**< Transmission status */
 } SPI_Message_t;

 /** @brief	Enable the SPI.
  */
 static inline void SPI_Enable(void) __attribute__ ((always_inline));
 static inline void SPI_Enable(void)
 {
 	 SPCR |= (0x01 << SPE);
 }
 
 /** @brief	Disable the SPI.
  */
 static inline void SPI_Disable(void) __attribute__ ((always_inline));
 static inline void SPI_Disable(void)
 {
	 SPCR &= ~(0x01 << SPE);
 }

 /** @brief			Set the data order of the SPI.
  *  @param Mode	SPI device data order
  */
 static inline void SPI_SetDataOrder(const SPI_DataOrder_t DataOrder) __attribute__ ((always_inline));
 static inline void SPI_SetDataOrder(const SPI_DataOrder_t DataOrder)
 {
	 SPCR = (SPCR & (~(0x01 << DORD))) | (DataOrder << DORD);
 }
 
 /** @brief			Set the device mode of the SPI.
  *  @param Mode	SPI device mode
  */
 static inline void SPI_SetDeviceMode(const SPI_DeviceMode_t Mode) __attribute__ ((always_inline));
 static inline void SPI_SetDeviceMode(const SPI_DeviceMode_t Mode)
 {
	 SPCR = (SPCR & (~(0x01 << MSTR))) | (Mode << MSTR);
 }

 /*
	 Master functions
 */
 
 /** @brief				Initialize the SPI.
  *  @param Config		Pointer to SPI configuration object
  */
 void SPIM_Init(SPIM_Config_t* Config);

 /** @brief			Transmit a single byte with the SPI.
  *  @param Data	Data byte
  *  @return		Received data byte
  */
 uint8_t SPIM_SendData(const uint8_t Data);
 
 /** @brief			Select an SPI device.
  *  @param Port	Pointer to I/O port for the CS
  *  @param Pin		Pin for the CS signal
  */
 static inline void SPIM_SelectDevice(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void SPIM_SelectDevice(PORT_t* Port, const uint8_t Pin)
 {
	 GPIO_Clear(Port, Pin);
 }

 /** @brief			Deselect an SPI device.
  *  @param Port	Pointer to I/O port for the CS
  *  @param Pin		Pin for the CS signal
  */
 static inline void SPIM_DeselectDevice(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void SPIM_DeselectDevice(PORT_t* Port, const uint8_t Pin)
 {
	 GPIO_Set(Port, Pin);
 }
 
 /** @brief				Set the prescaler of the SPI interface.
  *  @param Prescaler	Prescaler value
  */
 static inline void SPIM_SetPrescaler(const SPI_ClockPrescaler_t Prescaler) __attribute__ ((always_inline));
 static inline void SPIM_SetPrescaler(const SPI_ClockPrescaler_t Prescaler)
 {
	 SPSR |= ((Prescaler & 0x04) >> 0x02) << SPSR;
	 SPCR |= (Prescaler & 0x03);
 }
 
 /** @brief			Set the mode of the SPI.
  *  @param Mode	SPI device mode
  */
 static inline void SPIM_SetMode(const SPI_Mode_t Mode) __attribute__ ((always_inline));
 static inline void SPIM_SetMode(const SPI_Mode_t Mode)
 {
	 if(Mode == SPI_MODE_0)
	 {
		 SPCR &= ~((0x01 << CPOL) | (0x01 << CPHA));
	 }
	 else if(Mode == SPI_MODE_1)
	 {
		 SPCR &= ~(0x01 << CPOL);
		 SPCR |= (0x01 << CPHA);
	 }
	 else if(Mode == SPI_MODE_2)
	 {
		 SPCR |= (0x01 << CPOL);
		 SPCR &= ~(0x01 << CPHA);
	 }
	 else
	 {
		 SPCR |= (0x01 << CPOL) | (0x01 << CPHA);
	 }
 }

#endif /* SPI_H_ */