/*
 * I2C.h
 *
 *  Copyright (C) Daniel Kampert, 2018
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

/** @file include/Arch/AVR8/I2C/I2C.h
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
 
 #include "Arch/AVR8/CPU/CPU.h"
 #include "Arch/AVR8/GPIO/GPIO.h"
 
 #include "Base/I2C_Base.h"

 /** 
  * I2C master configuration object
  */
 typedef struct
 {
	 I2C_ClockPrescaler_t Prescaler;			/**< Clock prescaler */ 
	 uint32_t BitRate;							/**< I2C bus frequency */
	 Bool_t EnableInterruptSupport;				/**< Set to *TRUE to enable interrupt support in master mode */
 } I2CM_Config_t;

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
 void I2C_SwitchPullUp(const Bool_t Enable);

 /** @brief	Send a stop condition.
  */
 void I2CM_SendStop(void);
	
 /** @brief	Send a start condition.
  *  @return	I2C error code
  */
 I2C_Status_t I2CM_SendStart(void);

 /** @brief			Send a single byte.
  *  @param Data	Data byte
  *  @return		I2C error code
  */
 I2C_Status_t I2CM_SendByte(const uint8_t Data);

 /** @brief			Read a single byte.
  *  @param NACK	*TRUE if a NACK should be sended instead of an ACK
  *  @return		Data byte
  */
 uint8_t I2CM_ReadByte(const Bool_t NACK);

 /** @brief			Enable interrupt support for master mode.
  *  @param Device	Pointer to TWI object
  *  @param Level	Interrupt level for transmissions
  */
 void I2CM_EnableInterruptSupport(void);

 /** @brief			Disable interrupt support for master mode.
  *  @param Device	Pointer to TWI object
  */
 void I2CM_DisableInterruptSupport(void);

 /** @brief				Write n bytes with the TWI.
  *  @param Device		Pointer to TWI object
  *  @param Address		Slave address
  *  @param Register	Register address
  *  @param Length		Byte count
  *  @param Data		Pointer to data
  *  @param Repeat		Set to #TRUE to send a repeated start instead of stop
  *  @return			I2C error
  */
 I2C_Error_t I2CM_WriteBytes(const uint8_t Address, const uint8_t Register, const uint8_t Length, const uint8_t* Data, const Bool_t Repeat);

 /** @brief				Read n bytes from the TWI.
  *  @param Device		Pointer to TWI object
  *  @param Address		Slave address
  *  @param Register	Register address
  *  @param Length		Byte count
  *  @param Data		Pointer to data
  *  @param Repeat		Set to #TRUE to send a repeated start instead of stop
  *  @return			I2C error
  */
 I2C_Error_t I2CM_ReadBytes(const uint8_t Address, const uint8_t Register, const uint8_t Length, uint8_t* Data, const Bool_t Repeat);

#endif /* I2C_H_ */