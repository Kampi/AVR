/*
 * I2C.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 I2C module.

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

/** @file Arch/AVR8/megaAVR/I2C/I2C.h
 *  @brief Driver for AVR8 I2C module.
 *
 *  This contains the prototypes and definitions for the AVR8 I2C driver.
 *  NOTE: Only master mode supported.
 *
 *  @author Daniel Kampert
 *  @bug - No interrupt support included
 *		 - No slave mode implemented
 */

#ifndef I2C_H_
#define I2C_H_

 #include "Common/Common.h"
 
 #include "Arch/AVR8/megaAVR/mega32/CPU/CPU.h"
 #include "Arch/AVR8/megaAVR/mega32/GPIO/GPIO.h"

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
		 I2C_INVALID_PARAM = 0x20,				/**< Invalid parameter */
	 } I2C_Error_t;
	/** @} */ // end of I2C-Errors
 /** @} */ // end of I2C

 /** @brief	TWI callback definition.
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

 /** 
  * I2C status codes
  */
 typedef enum
 {
	I2C_START_SEND = 0x08,
	I2C_REP_START_SEND = 0x10,
	I2C_ADDR_ACK_W_RECEIVED = 0x18,
	I2C_ADDR_NACK_W_RECEIVED = 0x20,
	I2C_DATA_ACK_W_RECEIVED = 0x28,
	I2C_DATA_NACK_W_RECEIVED = 0x30,
	I2C_ARBITRATION_LOST = 0x38,
	I2C_ADDR_ACK_R_RECEIVED = 0x40,
	I2C_ADDR_NACK_R_RECEIVED = 0x48,
	I2C_DATA_ACK_R_RECEIVED = 0x50,
	I2C_DATA_NACK_R_RECEIVED = 0x58,
 } I2C_Status_t;

 /** 
  * I2C master configuration object
  */
 typedef struct
 {
	 I2C_ClockPrescaler_t Prescaler;				/**< Clock prescaler */ 
	 uint32_t Bitrate;								/**< I2C bus frequency */
	 bool EnableInterruptSupport;					/**< Set to #true to enable interrupt support */
 } I2CM_Config_t;

 /** 
  * I2C slave configuration object
  */
 typedef struct
 {
	 uint8_t Address;								/**< Device address */
	 bool EnableInterruptSupport;					/**< Set to #true to enable interrupt support in slave mode */
 } I2CS_Config_t;

 /** @brief I2C interrupt configuration object.
  */
 typedef struct
 {	 
	 I2C_CallbackType_t Source;						/**< I2C interrupt type */
	 I2C_Callback_t Callback;						/**< Function pointer to I2C callback */
 } I2C_InterruptConfig_t;

 /** @brief I2C buffer object for slave mode.
  */
 typedef struct 
 {
	 uint8_t* Buffer;								/**< Pointer to data buffer */
	 volatile uint8_t BytesReceived;				/**< Received bytes */
	 volatile uint8_t IndexWrite;					/**< Write index */
	 volatile uint8_t BytesSend;					/**< Transmitted bytes */
	 volatile uint8_t IndexRead;					/**< Read index */
	 volatile I2C_SlaveStatus_t Status;				/**< Device status */
 } I2C_Buffer_t;

 /** @brief I2C message object for interrupt driven transmission in master mode.
  */
 typedef struct
 {
	 uint8_t DeviceAddress;							/**< Slave address */
	 uint8_t Register;								/**< Slave register address */
	 uint8_t BytesToWrite;							/**< Bytes to write */
	 volatile uint8_t IndexWrite;					/**< Write index */
	 uint8_t BytesToRead;							/**< Bytes to read */
	 volatile uint8_t IndexRead;					/**< Read index */
	 uint8_t* BufferWrite;							/**< Pointer to write buffer */
	 uint8_t* BufferRead;							/**< Pointer to read buffer */
	 volatile I2C_MasterStatus_t Status;			/**< Device status */
 } I2C_Message_t;

 /*
	Common functions
 */

 /** @brief	Enable the device.
  */
 static inline void I2C_Enable(void) __attribute__ ((always_inline));
 static inline void I2C_Enable(void)
 {
	TWCR |= (0x01 << TWEN);
 }

 /** @brief	Reset the device.
  */
 static inline void I2C_Reset(void) __attribute__ ((always_inline));
 static inline void I2C_Reset(void)
 {
	 TWCR &= ~((0x01 << TWSTO) | (0x01 << TWEN));
	 TWCR |= (0x01 << TWEN);
 }

 /** @brief	Disable the device.
  */
 static inline void I2C_Disable(void) __attribute__ ((always_inline));
 static inline void I2C_Disable(void)
 {
	 TWCR &= ~(0x01 << TWEN);
 }

 /** @brief		Read the status register.
  *  @return	Status
  */
 static inline I2C_Status_t I2C_ReadStatus(void) __attribute__ ((always_inline));
 static inline I2C_Status_t I2C_ReadStatus(void)
 {
	 return (TWSR & 0xF8);
 }

 /** @brief			Install a new I2C callback
  *  @param Config	Pointer to I2C interrupt configuration object
  */
 void I2C_InstallCallback(const I2C_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Callback	Callback which should be removed.
  */
 void I2C_RemoveCallback(const I2C_CallbackType_t Callback);

 /*
	Master functions
 */
 
 /** @brief				Set the prescaler for the TWI.
  *  @param Prescaler	Prescaler
  */
 static inline void I2CM_SetPrescaler(const I2C_ClockPrescaler_t Prescaler) __attribute__ ((always_inline));
 static inline void I2CM_SetPrescaler(const I2C_ClockPrescaler_t Prescaler)
 {
	 TWSR = (TWSR & (~((0x01 << TWPS0) | (0x01 << TWPS1)))) | (Prescaler << TWPS0);
 }

 /** @brief			Initialize the TWI interface in master mode.
  *  @param Config	Pointer to I2C master configuration object
  */
 void I2CM_Init(I2CM_Config_t* Config);

 /** @brief				Set the bit rate for the TWI.
  *  @param BitRate		Bit rate
  *  @param Prescaler	Prescaler
  *  @param Clock		TWI module clock
  */
 void I2CM_SetBaudrate(const uint32_t BitRate, const uint8_t Prescaler, const uint32_t Clock);

 /** @brief			Enable/Disable internal pull ups for the TWI.
  *  @param Enable	Enable/Disable
  */
 void I2C_SwitchPullUp(const bool Enable);

 /*
	Master functions
 */

 /** @brief			Write a single byte with the TWI.
  *  @param Address	Slave address
  *  @param Data	Pointer to data
  *  @param Stop	Set to #false to send a repeated start instead of stop
  *  @return		I2C error
  */
 I2C_Error_t I2CM_WriteByte(const uint8_t Address, const uint8_t Data, const bool Stop);

 /** @brief			Read a single byte from the TWI.
  *  @param Address	Slave address
  *  @param Data	Pointer to data
  *  @param Stop	Set to #false to send a repeated start instead of stop
  *  @return		I2C error
  */
 I2C_Error_t I2CM_ReadByte(const uint8_t Address, uint8_t* Data, const bool Stop);

 /** @brief			Enable interrupt support for master mode.
  *  @param Device	Pointer to TWI object
  *  @param Level	Interrupt level for transmissions
  */
 void I2CM_EnableInterruptSupport(void);

 /** @brief			Disable interrupt support for master mode.
  *  @param Device	Pointer to TWI object
  */
 void I2CM_DisableInterruptSupport(void);

 /** @brief			Write n bytes with the TWI.
  *  @param Device	Pointer to TWI object
  *  @param Address	Slave address
  *  @param Length	Byte count
  *  @param Data	Pointer to data
  *  @param Stop	Set to #true to send a repeated start instead of stop
  *  @return		I2C error
  */
 I2C_Error_t I2CM_WriteBytes(const uint8_t Address, const uint8_t Length, const uint8_t* Data, const bool Stop);

 /** @brief			Read n bytes from the TWI.
  *  @param Device	Pointer to TWI object
  *  @param Address	Slave address
  *  @param Length	Byte count
  *  @param Data	Pointer to data
  *  @param Stop	Set to #true to send a repeated start instead of stop
  *  @return		I2C error
  */
 I2C_Error_t I2CM_ReadBytes(const uint8_t Address, const uint8_t Length, uint8_t* Data, const bool Stop);

 /*
	Slave functions
 */

 /** @brief			Initialize a TWI slave interface.
  *					NOTE: The slave interface needs interrupts to work properly!
  *  @param Config	Pointer to TWI configuration object
  */
 void I2CS_Init(I2CS_Config_t* Config);

 /** @brief			Set the address (including R/W bit) of the TWI slave mode.
  *  @param Address	Slave address
  */
 static inline void I2CS_SetAddress(const uint8_t Address) __attribute__((always_inline));
 static inline void I2CS_SetAddress(const uint8_t Address)
 {
	 TWAR = ((Address & 0x7F) << 0x01);
 }

 /** @brief			Enable/Disable general call recognition.
  *  @param Enable	Enable/Disable
  */
 static inline void I2CS_SwitchGeneralCall(const bool Enable) __attribute__((always_inline));
 static inline void I2CS_SwitchGeneralCall(const bool Enable)
 {
	 TWAR |= (TWAR & (~0x01)) | Enable;
 }

#endif /* I2C_H_ */