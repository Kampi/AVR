/*
 * I2C_Base.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Common Definitions for I2C Interface of all AVR8 MCUs

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

/** @file I2C_Base.h
 *  @brief Common definitions for AVR TWI.
 *
 *  @author Daniel Kampert
 */

#ifndef I2C_BASE_H_
#define I2C_BASE_H_

 #include "Common/types.h"

 /** @brief	TWI callback definition
 */
 typedef void (*I2C_Callback_t)(void);

 /** @brief Macro to calculate the I2C write address.
  */
 #define I2C_WRITE(Address)						(Address << 0x01)
 
 /** @brief Macro to calculate the I2C read address.
  */
 #define I2C_READ(Address)						((Address << 0x01) + 0x01)

 /** @brief I2C device modes.
  */
 typedef enum
 {
	 I2C_SLAVE = 0x00,							/**< I2C slave mode */ 
	 I2C_MASTER = 0x01,							/**< I2C master mode */ 
 } I2C_DeviceMode_t;

 /** @brief I2C clock prescaler.
  */
 typedef enum
 {
	 I2C_PRESCALER_1 = 0x00,					/**< Clock prescaler 1  */ 
	 I2C_PRESCALER_4 = 0x01,					/**< Clock prescaler 4  */ 
	 I2C_PRESCALER_16 = 0x02,					/**< Clock prescaler 16 */ 
	 I2C_PRESCALER_64 = 0x03,					/**< Clock prescaler 64 */ 
 } I2C_ClockPrescaler_t;

 /** @brief I2C callback types.
  */
 typedef enum
 {
	 I2C_TXCOMPLETE_INTERRUPT = 0x01,			/**< Transmission complete interrupt (Master) */
	 I2C_RXCOMPLETE_INTERRUPT = 0x02,			/**< Receive complete interrupt (Master) */
	 I2C_ERROR_INTERRUPT = 0x04,				/**< Error interrupt (Master) */
	 I2C_DATA_RDY_INTERRUPT = 0x08,				/**< New data ready interrupt (Slave) */
 } I2C_CallbackType_t;

 /** @brief I2C master status codes.
  */
 typedef enum
 {
	 I2C_MASTER_REGISTER = 0x00,				/**< Write register address phase */
	 I2C_MASTER_ADDRESS = 0x01,					/**< Device address (read) phase */
	 I2C_MASTER_WRITE = 0x02,					/**< Write phase */
	 I2C_MASTER_READ = 0x03,					/**< Read phase */
	 I2C_MASTER_SEND = 0x04,					/**< Data successfully transmitted */
	 I2C_MASTER_RECEIVED = 0x05,				/**< Data successfully received */
	 I2C_MASTER_BUFFEROVERFLOW = 0x06,			/**< Buffer overflow */
	 I2C_MASTER_ERROR = 0xFF,					/**< Transmission error */
 } I2C_MasterStatus_t;

 /** @brief I2C slave status codes.
  */
 typedef enum
 {
	 I2C_SLAVE_IDLE = 0x00,						/**< Slave idle */
	 I2C_SLAVE_BUFFEROVERFLOW = 0x01,			/**< Buffer overflow */
	 I2C_SLAVE_ERROR = 0xFF,					/**< Transmission error */
 } I2C_SlaveStatus_t;
 
 /** @defgroup I2C
  *  @{
  */
	 /** @defgroup I2C-Errors
	  *  I2C error codes.
	  *  @{
	  */
	 typedef enum
	 {
		 I2C_NO_ERROR = 0x00,					/**< No error */
		 I2C_START_ERROR = 0x01,				/**< Error with start condition */
		 I2C_READ_ADDR_ERROR = 0x02,			/**< Wrong read address */
		 I2C_WRITE_ADDR_ERROR = 0x04,			/**< Wrong write address */
		 I2C_BUS_ERROR = 0x08,					/**< I2C bus error */
		 I2C_NO_DEVICE = 0x10,					/**< I2C device not found */
	 } I2C_Error_t;
	/** @} */ // end of I2C-Errors
 /** @} */ // end of I2C
 
 /** @brief I2C interrupt configuration object.
  */
 typedef struct
 {
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		 TWI_t* Device;							/**< Pointer to TWI device object */
		 Interrupt_Level_t InterruptLevel;		/**< Interrupt level */
	 #endif
	 
	 I2C_CallbackType_t Source;					/**< I2C interrupt type */
	 I2C_Callback_t Callback;					/**< Function pointer to I2C callback */
 } I2C_InterruptConfig_t;
 
 /** @brief I2C buffer object for slave mode.
  */
 typedef struct 
 {
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		TWI_t* Device;							/**< Target TWI */
	 #endif

	 uint8_t* Buffer;							/**< Pointer to data buffer */
	 volatile uint8_t BytesReceived;			/**< Received bytes */
	 volatile uint8_t IndexWrite;				/**< Write index */
	 volatile uint8_t BytesSend;				/**< Transmitted bytes */
	 volatile uint8_t IndexRead;				/**< Read index */
	 volatile I2C_SlaveStatus_t Status;			/**< Device status */
 } I2C_Buffer_t;

 /** @brief I2C message object for interrupt driven transmission in master mode.
  */
 typedef struct
 {
	 #if(MCU_ARCH == MCU_ARCH_XMEGA)
		TWI_t* Device;							/**< Target TWI */
	 #endif

	 uint8_t DeviceAddress;						/**< Slave address */
	 uint8_t Register;							/**< Slave register address */
	 uint8_t BytesToWrite;						/**< Bytes to write */
	 volatile uint8_t IndexWrite;				/**< Write index */
	 uint8_t BytesToRead;						/**< Bytes to read */
	 volatile uint8_t IndexRead;				/**< Read index */
	 uint8_t* BufferWrite;						/**< Pointer to write buffer */
	 uint8_t* BufferRead;						/**< Pointer to read buffer */
	 volatile I2C_MasterStatus_t Status;		/**< Device status */
 } I2C_Message_t;

#endif /* I2C_BASE_H_ */