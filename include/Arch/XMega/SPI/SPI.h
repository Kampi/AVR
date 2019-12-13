/*
 * SPI.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega SPI module.

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

/** @file Arch/XMega/SPI/SPI.h
 *  @brief Driver for XMega SPI module.
 *
 *  This contains the prototypes and definitions for the XMega SPI driver.
 *
 *  @author Daniel Kampert
 *  @bug - Not tested
 *		 - No SS pin interrupt in master mode
 */
 
#ifndef SPI_H_
#define SPI_H_
 
 #include "Common/Common.h"

 #include "Arch/XMega/GPIO/GPIO.h"
 #include "Arch/XMega/PMIC/PMIC.h"

 /** @brief	ID declaration for the different MCU types.
  */
 #define SPIC_ID			0									/**< SPI C ID */
 #define SPID_ID			1									/**< SPI D ID */
 #define SPIE_ID			2									/**< SPI E ID */
 #define SPIF_ID			3									/**< SPI F ID */

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
	 void* Device;												/**< Pointer to USART-SPI or SPI device object */
	 uint32_t SPIClock;											/**< SPI clock frequency \n
																	 NOTE: You only need this when you use the USART-SPI interface. */
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
	 void* Device;												/**< Pointer to USART-SPI or SPI device object */
	 SPI_Mode_t Mode;											/**< SPI mode */
	 SPI_DataOrder_t DataOrder;									/**< Data order */
 } SPIS_Config_t;

 /** @brief SPI interrupt configuration object.
  */
 typedef struct
 {
	 void* Device;												/**< Pointer to USART-SPI or SPI device object */
	 Interrupt_Level_t InterruptLevel;							/**< Interrupt level */
	 SPI_CallbackType_t Source;									/**< SPI interrupt type */
	 SPI_Callback_t Callback;									/**< Function pointer to SPI callback */
 } SPI_InterruptConfig_t;

 /** @brief SPI message object.
  */
 typedef struct
 {
	 void* Device;												/**< Pointer to target USART-SPI or SPI device object */
	 PORT_t* Port;												/**< Port for slave select */	 
	 uint8_t Pin;												/**< Pin for slave select */
	 uint8_t* BufferOut;										/**< Pointer to output buffer */
	 uint8_t* BufferIn;											/**< Pointer to input buffer */
	 volatile uint8_t BytesProcessed;							/**< Counter for processed bytes */
	 uint8_t Length;											/**< Message length */
	 volatile SPI_Status_t Status;								/**< Transmission status */
 } SPI_Message_t;

 /*
	Common functions
 */
 
 /** @brief			Enable interrupt support for master mode.
  *  @param Device	Pointer to SPI object
  *  @param Level	Interrupt level 
  */
 void SPI_EnableInterruptSupport(SPI_t* Device, const Interrupt_Level_t Level);

 /** @brief			Disable interrupt support for master mode.
  *  @param Device	Pointer to SPI object
  */
 void SPI_DisableInterruptSupport(SPI_t* Device);
 
 /** @brief			Install a new callback for the interrupt driven transmissions.
  *  @param Config	Pointer to interrupt configuration structure.
  */
 void SPI_InstallCallback(const SPI_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Device		Pointer to SPI object
  *  @param Callback	Callback which should be removed
  */
 void SPI_RemoveCallback(SPI_t* Device, const SPI_CallbackType_t Callback);
 
 /** @brief					Change the interrupt level for an installed interrupt.
  *  @param Device			Pointer to SPI object
  *  @param Callback		Callback which should be changed
  *  @param InterruptLevel	New interrupt level.
  */
 void SPI_ChangeInterruptLevel(SPI_t* Device, const SPI_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel);

 /** @brief			Enable the SPI interface.
  *  @param Device	Pointer to SPI device object
  */
 static inline void SPI_Enable(SPI_t* Device) __attribute__((always_inline));
 static inline void SPI_Enable(SPI_t* Device)
 {
	 Device->CTRL = (Device->CTRL & (~(0x01 << 0x06))) | (0x01 << 0x06);
 }

 /** @brief			Disable the SPI interface.
  *  @param Device	Pointer to SPI device object
  */
 static inline void SPI_Disable(SPI_t* Device) __attribute__((always_inline));
 static inline void SPI_Disable(SPI_t* Device)
 {
	 Device->CTRL &= ~(0x01 << 0x06);
 }

 /** @brief			Set the mode of the SPI interface.
  *  @param Device	Pointer to SPI device object
  *  @param Mode	SPI device mode
  */
 static inline void SPI_SetMode(SPI_t* Device, const SPI_Mode_t Mode) __attribute__((always_inline));
 static inline void SPI_SetMode(SPI_t* Device, const SPI_Mode_t Mode)
 {
	 Device->CTRL = (Device->CTRL & (~(0x01 << 0x02))) | (Mode << 0x02);
 }

 /** @brief			Set the data order of the SPI interface.
  *  @param Device	Pointer to SPI device object
  *  @param Mode	SPI device data order
  */
 static inline void SPI_SetDataOrder(SPI_t* Device, const SPI_DataOrder_t Order) __attribute__((always_inline));
 static inline void SPI_SetDataOrder(SPI_t* Device, const SPI_DataOrder_t Order)
 {
	 Device->CTRL = (Device->CTRL & (~(0x01 << 0x05))) | (Order << 0x05);
 }

 /*
	Master functions
 */

 /** @brief			Initialize a SPI interface in master mode.
  *  @param Config	Pointer to SPI master configuration object
  */
 void SPIM_Init(SPIM_Config_t* Config);

 /** @brief				Set the SPI transmission speed.
  *  @param Device		Pointer to SPI device object
  *  @param SPIClock	SPI clock frequency
  *  @param Clock		Clock frequency of the SPI module
  */
 void SPIM_SetClock(SPI_t* Device, const uint32_t SPIClock, const uint32_t Clock);
 
 /** @brief				Get the SPI transmission speed.
  *  @param Device		Pointer to SPI device object
  *  @param Clock		Clock frequency of the SPI module
  *  @return			SPI clock frequency
  */
 const uint32_t SPIM_GetClock(SPI_t* Device, const uint32_t Clock);

 /** @brief			Activate the double speed of the SPI interface.
  *  @param Device	Pointer to SPI device object
  *  @param Enable	Enable/Disable double speed
  */
 static inline void SPIM_SwitchDoubleSpeed(SPI_t* Device, const Bool_t Enable) __attribute__((always_inline));
 static inline void SPIM_SwitchDoubleSpeed(SPI_t* Device, const Bool_t Enable)
 {
	 if(Enable)
	 {
		 Device->CTRL |= 0x01 << 0x07;
	 }
	 else
	 {
		 Device->CTRL &= ~(0x01 << 0x07);
	 }
 }
 
 /** @brief				Set the prescaler of the SPI interface in master mode.
  *  @param Device		Pointer to SPI device object
  *  @param Prescaler	Prescaler value
  *						NOTE: Double speed is set automatically
  */
 static inline void SPIM_SetPrescaler(SPI_t* Device, const SPI_ClockPrescaler_t Prescaler) __attribute__((always_inline));
 static inline void SPIM_SetPrescaler(SPI_t* Device, const SPI_ClockPrescaler_t Prescaler)
 {
	 if((Prescaler == SPI_PRESCALER_2) || (Prescaler == SPI_PRESCALER_8) || (Prescaler == SPI_PRESCALER_32))
	 {
		 SPIM_SwitchDoubleSpeed(Device, TRUE);
	 }
	
	 Device->CTRL = (Device->CTRL & (~(0x01 << 0x01))) | (Prescaler & 0x03);
 }

 /** @brief			Get the prescaler of the SPI interface.
  *  @param Device	Pointer to SPI device object
  *  @return		Prescaler value
  */
 static inline const SPI_ClockPrescaler_t SPIM_GetPrescaler(SPI_t* Device) __attribute__((always_inline));
 static inline const SPI_ClockPrescaler_t SPIM_GetPrescaler(SPI_t* Device)
 {
	 return ((Device->CTRL & 0x80) >> 0x05) | (Device->CTRL & 0x03);
 }

 /** @brief			Select an SPI device.
  *  @param Port	Pointer to I/O port for the CS
  *  @param Pin		Pin for the CS signal
  */
 static inline void SPIM_SelectDevice(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void SPIM_SelectDevice(PORT_t* Port, const uint8_t Pin)
 {
	 GPIO_Clear(Port, Pin);
 }

 /** @brief			Deselect an SPI device.
  *  @param Port	Pointer to I/O port for the CS
  *  @param Pin		Pin for the CS signal
  */
 static inline void SPIM_DeselectDevice(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void SPIM_DeselectDevice(PORT_t* Port, const uint8_t Pin)
 {
	 GPIO_Set(Port, Pin);
 }

 /** @brief			Transmit a single byte with the SPI.
  *  @param Device	Pointer to SPI device object
  *  @param Data	Data byte
  *  @return		Received data byte
  */
 const uint8_t SPIM_SendData(SPI_t* Device, const uint8_t Data);

 /** @brief				Transmit data with the SPI by using interrupt support.
						NOTE: You have to enable global interrupts to use it!
  *  @param Device		Pointer to SPI object
  *  @param Bytes		Byte count
  *  @param WriteBuffer	Pointer to transmit buffer
  *  @param ReadBuffer	Pointer to receive buffer
  *  @param Port		Pointer to slave select port object
  *  @param Port		Slave select pin number
  *  @return			Status code
  */
 SPI_Status_t SPIM_Transmit(SPI_t* Device, const uint8_t Bytes, uint8_t* WriteBuffer, uint8_t* ReadBuffer, PORT_t* Port, const uint8_t Pin);

 /** @brief			Get the status of an ongoing transaction.
  *  @param Device	Pointer to SPI object
  *  @return		Status code
  */
 const SPI_Status_t SPIM_Status(const SPI_t* Device);

 /*
	Slave functions functions
 */
 
 /** @brief			Initialize a SPI interface in slave mode.
  *  @param Config	Pointer to SPI slave configuration object
  */
 void SPIS_Init(SPIS_Config_t* Config);
 
 /** @brief			Get the status of an SPI slave.
  *  @param Device	Pointer to SPI object
  *  @return		Status code
  */
 const SPI_Status_t SPIS_Status(const SPI_t* Device);

#endif /* SPI_H_ */