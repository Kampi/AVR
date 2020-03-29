/*
 * I2C.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega TWI

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

/** @file Arch/XMega/I2C/I2C.h
 *  @brief Driver for XMega TWI module. 
 *
 *  This file contains the prototypes and definitions for the XMega I2C driver.
 *
 *  @author Daniel Kampert
 *  @bug - No external driver interface support
 *		 - No smart mode support
 */

#ifndef I2C_H_
#define I2C_H_
 
 #include "Common/Common.h"
 
 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/ClockManagement/SysClock.h"

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

 /** @brief	ID declaration for the different MCU types.
  */
 #define TWIC_ID			0					/**< TWI C ID */
 #define TWIE_ID			1					/**< TWI E ID */

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
  * I2C SDA hold times
  */
 typedef enum
 {
	 I2C_HOLD_NONE = 0x00,						/**< No hold time */
	 I2C_HOLD_50NS = 0x01,						/**< 50 ns hold time */
	 I2C_HOLD_300NS = 0x02,						/**< 300 ns hold time */
	 I2C_HOLD_400NS = 0x03,	 					/**< 400 ns hold time */
 } I2C_HoldTime_t;

 /** 
  * I2C timeout times
  */
 typedef enum
 {
	 I2C_TIMEOUT_DISABLE = 0x00,				/**< No timeout */
	 I2C_TIMEOUT_50US = 0x00,					/**< 50 us timeout time */
	 I2C_TIMEOUT_100US = 0x00,					/**< 100 us timeout time */
	 I2C_TIMEOUT_200US = 0x00,	 				/**< 200 us timeout time */
 } I2C_Timeout_t;
 
 /** 
  * I2C bus states
  */
 typedef enum
 {
	 I2C_STATE_UNKNWON = 0x00,					/**< Unknown bus state */
	 I2C_STATE_IDLE = 0x01,						/**< Idle state */
	 I2C_STATE_OWNER = 0x02,					/**< Owner state */
	 I2C_STATE_BUSY = 0x03,	 					/**< Busy state */
 } I2C_BusState_t;

 /** 
  * I2C master configuration object
  */
 typedef struct
 {
	 TWI_t* Device;								/**< Target TWI */
	 Interrupt_Level_t InterruptLevel;			/**< Interrupt level for interrupt support
													 NOTE: Only needed if you use interrupt driven transmissions */  
	 I2C_DeviceMode_t DeviceMode;				/**< Device mode */
	 I2C_Timeout_t Timeout;						/**< Bus inactive timeout */
	 uint32_t Bitrate;							/**< I2C bus frequency */
	 bool EnableQuickCommand;					/**< Set to #true to enable quick commands */
	 bool EnableSmartMode;					/**< Set to #true to enable smart mode */
	 bool EnableInterruptSupport;				/**< Set to #true to enable interrupt support in master mode */
 } I2CM_Config_t;

 /** 
  * I2C slave configuration object
  */
 typedef struct
 {
	 TWI_t* Device;								/**< Target TWI */
	 Interrupt_Level_t InterruptLevel;			/**< Interrupt level for interrupt support */	 
	 bool EnableSmartMode;					/**< Set to #true to enable smart mode */
	 uint8_t Address;							/**< Device address */
	 uint8_t Mask;								/**< Device address mask */
	 uint8_t* Buffer;							/**< Pointer to data buffer */
	 bool EnablePromiscuousMode;				/**< Enable promiscuous mode */
 } I2CS_Config_t;

 /** @brief I2C interrupt configuration object.
  */
 typedef struct
 {
	 TWI_t* Device;								/**< Pointer to TWI device object */
	 Interrupt_Level_t InterruptLevel;			/**< Interrupt level */	 
	 I2C_CallbackType_t Source;					/**< I2C interrupt type */
	 I2C_Callback_t Callback;					/**< Function pointer to I2C callback */
 } I2C_InterruptConfig_t;

 /** @brief I2C buffer object for slave mode.
  */
 typedef struct 
 {
	 TWI_t* Device;								/**< Target TWI */
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
	TWI_t* Device;								/**< Target TWI */
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

 /*
	Common functions
 */

 /** @brief			Set the hold time.
  *  @param Device	Pointer to TWI device object
  *  @param Hold	Hold time with respect to the negative edge of SCL
  */
 static inline void I2C_SetHold(TWI_t* Device, const I2C_HoldTime_t Hold) __attribute__((always_inline));
 static inline void I2C_SetHold(TWI_t* Device, const I2C_HoldTime_t Hold)
 {
	 Device->CTRL |= (Device->CTRL & (~(0x03 << 0x01))) | (Hold << 0x01);
 }

 /** @brief			Get the hold time.
  *  @param Device	Pointer to TWI device object
  *  @return		Hold time with respect to the negative edge of SCL
  */
 static inline I2C_HoldTime_t I2C_GetHold(TWI_t* Device) __attribute__((always_inline));
 static inline I2C_HoldTime_t I2C_GetHold(TWI_t* Device)
 {
	 return Device->CTRL & (0x03 << 0x01) >> 0x01;
 }

 /** @brief			Enable/Disable the external TWI interface.
  *  @param Device	Pointer to TWI device object
  *  @param Enable	Enable/Disable
  */
 static inline void I2C_SwitchExternalInterface(TWI_t* Device, const bool Enable) __attribute__((always_inline));
 static inline void I2C_SwitchExternalInterface(TWI_t* Device, const bool Enable)
 {
	 if(Enable == true)
	 {
		 Device->CTRL |= TWI_EDIEN_bm;
	 }
	 else
	 {
		 Device->CTRL &= ~TWI_EDIEN_bm;
	 }	
 }

 /** @brief			Install a new callback for the interrupt driven transmissions.
  *  @param Config	Pointer to interrupt configuration structure.
  */
 void I2C_InstallCallback(const I2C_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Device		Pointer to TWI object
  *  @param Callback	Callback which should be removed
  */
 void I2C_RemoveCallback(TWI_t* Device, const I2C_CallbackType_t Callback);
 
 /** @brief					Change the interrupt level for an installed master or slave interrupt.
  *  @param Device			Pointer to TWI object
  *  @param Callback		Callback which should be changed
  *  @param InterruptLevel	New interrupt level.
  */
 void I2C_ChangeInterruptLevel(TWI_t* Device, const I2C_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel);

 /*
	Master functions
 */

 /** @brief			Enable the TWI in master mode.
  *  @param Device	Pointer to TWI device object
  */
 static inline void I2C_MasterEnable(TWI_t* Device) __attribute__((always_inline));
 static inline void I2C_MasterEnable(TWI_t* Device)
 {
	 Device->MASTER.CTRLA |= TWI_MASTER_ENABLE_bm;
 }

 /** @brief			Disable the TWI master mode.
  *  @param Device	Pointer to TWI device object
  */
 static inline void I2C_MasterDisable(TWI_t* Device) __attribute__((always_inline));
 static inline void I2C_MasterDisable(TWI_t* Device)
 {
	 Device->MASTER.CTRLA &= ~TWI_MASTER_ENABLE_bm;
 }

 /** @brief			Read the status of the TWI master module.
  *  @param Device	Pointer to TWI device object
  *  @return		Status
  */
 static inline uint8_t I2CM_ReadStatus(const TWI_t* Device) __attribute__((always_inline));
 static inline uint8_t I2CM_ReadStatus(const TWI_t* Device)
 {
	 return Device->MASTER.STATUS;
 }

 /** @brief			Reset the TWI master state machine and set it into idle mode.
  *  @param Device	Pointer to TWI device object
  */
 static inline void I2CM_Reset(TWI_t* Device) __attribute__((always_inline));
 static inline void I2CM_Reset(TWI_t* Device)
 {
	 Device->MASTER.STATUS |= I2C_STATE_IDLE;
 }

 /** @brief			Set the inactive bus timeout for the master mode.
  *  @param Device	Pointer to TWI device object
  *  @param Timeout	Inactive bus timeout
  */
 static inline void I2CM_SetTimeout(TWI_t* Device, const I2C_Timeout_t Timeout) __attribute__((always_inline));
 static inline void I2CM_SetTimeout(TWI_t* Device, const I2C_Timeout_t Timeout)
 {
	 Device->MASTER.CTRLB = (Device->MASTER.CTRLB & (~(0x03 << 0x02))) | (Timeout << 0x02);
 }
 
 /** @brief			Get the inactive bus timeout for the master mode.
  *  @param Device	Pointer to TWI device object
  *  @return		Inactive bus timeout
  */
 static inline I2C_Timeout_t I2CM_GetTimeout(TWI_t* Device) __attribute__((always_inline));
 static inline I2C_Timeout_t I2CM_GetTimeout(TWI_t* Device)
 {
	 return ((Device->MASTER.CTRLB >> 0x02) & 0x03);
 }

 /** @brief			Enable/Disable the quick command mode for the master mode.
  *  @param Device	Pointer to TWI device object
  *  @param Enable	Enable/Disable
  */
 static inline void I2CM_SwitchQuickCommand(TWI_t* Device, const bool Enable) __attribute__((always_inline));
 static inline void I2CM_SwitchQuickCommand(TWI_t* Device, const bool Enable)
 {
	 Device->MASTER.CTRLB = (Device->MASTER.CTRLB & (~(0x01 << 0x01))) | (Enable << 0x01);
 }

 /** @brief			Enable/Disable the smart mode.
  *  @param Device	Pointer to TWI device object
  *  @param Enable	Enable/Disable
  */
 static inline void I2CM_SwitchSmartMode(TWI_t* Device, const bool Enable) __attribute__((always_inline));
 static inline void I2CM_SwitchSmartMode(TWI_t* Device, const bool Enable)
 {
	 Device->MASTER.CTRLB = (Device->MASTER.CTRLB & (~0x01)) | Enable;
 }
 
 /** @brief			Initialize a TWI master interface.
  *  @param Config	Pointer to TWI master configuration object
  */
 void I2CM_Init(I2CM_Config_t* Config);

 /** @brief			Enable interrupt support for master mode.
  *  @param Device	Pointer to TWI object
  *  @param Level	Interrupt level
  */
 void I2CM_EnableInterruptSupport(TWI_t* Device, const Interrupt_Level_t Level);

 /** @brief			Disable interrupt support for master mode.
  *  @param Device	Pointer to TWI object
  */
 void I2CM_DisableInterruptSupport(TWI_t* Device);

 /** @brief				Set the baud rate of a master mode TWI.
  *  @param Device		Pointer to TWI object
  *  @param Baudrate	Baud rate for the interface
  *  @param Clock		TWI module clock
  */
 void I2CM_SetBaudrate(TWI_t* Device, const uint32_t Baudrate, const uint32_t Clock);

 /** @brief					Transmit data to an I2C slave by using interrupt support.
							NOTE: You have to enable global interrupts to use it!
  *  @param Device			Pointer to TWI object
  *  @param DeviceAddress	I2C address of slave device
  *  @param Bytes			Byte count
  *  @param Data			Pointer to transmit buffer
  */
 void I2CM_TransmitBytes(TWI_t* Device, const uint8_t DeviceAddress, const uint8_t Bytes, uint8_t* Data);
 
 /** @brief					Transmit data to an I2C slave by using interrupt support.
							NOTE: You have to enable global interrupts to use it!
  *  @param Device			Pointer to TWI object
  *  @param DeviceAddress	I2C address of slave device
  *  @param Command			Device register or device command
  *  @param Bytes			Byte count
  *  @param Data			Pointer to transmit buffer
  */
 void I2CM_Transmit(TWI_t* Device, const uint8_t DeviceAddress, const uint8_t Command, const uint8_t Bytes, uint8_t* Data);
 
 /** @brief					Receive data from an I2C slave by using interrupt support.
							NOTE: You have to enable global interrupts to use it!
  *  @param Device			Pointer to TWI object
  *  @param DeviceAddress	I2C address of slave device
  *  @param Bytes			Byte count
  *  @param Data			Pointer to transmit buffer
  */
 void I2CM_ReceiveBytes(TWI_t* Device, const uint8_t DeviceAddress, const uint8_t Bytes, uint8_t* Data);
 
 /** @brief					Receive data from an I2C slave by using interrupt support.
							NOTE: You have to enable global interrupts to use it!
  *  @param Device			Pointer to TWI object
  *  @param DeviceAddress	I2C address of slave device
  *  @param Command			Device register or device command
  *  @param Bytes			Byte count
  *  @param Data			Pointer to transmit buffer
  */
 void I2CM_Receive(TWI_t* Device, const uint8_t DeviceAddress, const uint8_t Command, const uint8_t Bytes, uint8_t* Data);

 /** @brief			Get the status of an ongoing transaction.
  *  @param Device	Pointer to TWI object
  *  @return		Master device status
  */
 const I2C_MasterStatus_t I2CM_Status(const TWI_t* Device);

 /** @brief				Write a single byte with the TWI.
  *  @param Device		Pointer to TWI object
  *  @param Address		Slave address
  *  @param Data		Pointer to data
  *  @param Stop		Set to #false to send a repeated start instead of stop
  *  @return			I2C error
  */
 I2C_Error_t I2CM_WriteByte(TWI_t* Device, const uint8_t Address, const uint8_t Data, const bool Stop);

 /** @brief				Write n bytes with the TWI.
  *  @param Device		Pointer to TWI object
  *  @param Address		Slave address
  *  @param Length		Byte count
  *  @param Data		Pointer to data
  *  @param Stop		Set to #false to send a repeated start instead of stop
  *  @return			I2C error
  */
 I2C_Error_t I2CM_WriteBytes(TWI_t* Device, const uint8_t Address, const uint8_t Length, const uint8_t* Data, const bool Stop);

 /** @brief				Read a single byte from the TWI.
  *  @param Device		Pointer to TWI object
  *  @param Address		Slave address
  *  @param Data		Pointer to data
  *  @param Stop		Set to #false to send a repeated start instead of stop
  *  @return			I2C error
  */
 I2C_Error_t I2CM_ReadByte(TWI_t* Device, const uint8_t Address, uint8_t* Data, const bool Stop);

 /** @brief				Read n bytes from the TWI.
  *  @param Device		Pointer to TWI object
  *  @param Address		Slave address
  *  @param Length		Byte count
  *  @param Data		Pointer to data
  *  @param Stop		Set to #false to send a repeated start instead of stop
  *  @return			I2C error
  */
 I2C_Error_t I2CM_ReadBytes(TWI_t* Device, const uint8_t Address, const uint8_t Length, uint8_t* Data, const bool Stop);

 /*
	Slave functions
 */

 /** @brief			Enable the TWI in slave mode.
  *  @param Device	Pointer to TWI device object
  */
 static inline void I2C_SlaveEnable(TWI_t* Device) __attribute__((always_inline));
 static inline void I2C_SlaveEnable(TWI_t* Device)
 {
	 Device->SLAVE.CTRLA |= TWI_SLAVE_ENABLE_bm;
 }

 /** @brief			Disable the TWI slave mode.
  *  @param Device	Pointer to TWI device object
  */
 static inline void I2C_SlaveDisable(TWI_t* Device) __attribute__((always_inline));
 static inline void I2C_SlaveDisable(TWI_t* Device)
 {
	 Device->SLAVE.CTRLA &= ~TWI_SLAVE_ENABLE_bm;
 }

 /** @brief			Read the status of the TWI slave module.
  *  @param Device	Pointer to TWI device object
  *  @return		Status
  */
 static inline uint8_t I2CS_ReadStatus(const TWI_t* Device) __attribute__((always_inline));
 static inline uint8_t I2CS_ReadStatus(const TWI_t* Device)
 {
	 return Device->SLAVE.STATUS;
 }

 /** @brief			Enable/Disable the smart mode.
  *  @param Device	Pointer to TWI device object
  *  @param Enable	Enable/Disable
  */
 static inline void I2CS_SwitchSmartMode(TWI_t* Device, const bool Enable) __attribute__((always_inline));
 static inline void I2CS_SwitchSmartMode(TWI_t* Device, const bool Enable)
 {
	 Device->MASTER.CTRLB = (Device->MASTER.CTRLB & (~0x01)) | Enable;
 }

 /** @brief			Enable/Disable promiscuous mode.
  *  @param Device	Pointer to TWI device object
  *  @param Enable	Enable/Disable
  */
 static inline void I2CS_SwitchPromiscuousMode(TWI_t* Device, const bool Enable) __attribute__((always_inline));
 static inline void I2CS_SwitchPromiscuousMode(TWI_t* Device, const bool Enable)
 {
	 Device->SLAVE.CTRLA |= (Device->SLAVE.CTRLB & (~(0x01 << 0x01))) | (Enable << 0x01);
 }
 
 /** @brief			Enable/Disable second slave address.
  *  @param Device	Pointer to TWI device object
  *  @param Enable	Enable/Disable
  */
 static inline void I2CS_SwitchSecondAddress(TWI_t* Device, const bool Enable) __attribute__((always_inline));
 static inline void I2CS_SwitchSecondAddress(TWI_t* Device, const bool Enable)
 {
	 Device->SLAVE.ADDRMASK |= (Device->SLAVE.CTRLB & (~0x01)) | Enable;
 }
 
 /** @brief			Enable/Disable general call recognition.
  *  @param Device	Pointer to TWI device object
  *  @param Enable	Enable/Disable
  */
 static inline void I2CS_SwitchGeneralCall(TWI_t* Device, const bool Enable) __attribute__((always_inline));
 static inline void I2CS_SwitchGeneralCall(TWI_t* Device, const bool Enable)
 {
	 Device->SLAVE.ADDR |= (Device->SLAVE.ADDR & (~0x01)) | Enable;
 }

 /** @brief			Set the address (including R/W bit) of the TWI slave mode.
  *  @param Device	Pointer to TWI device object
  *  @param Address	Slave address
  */
 static inline void I2CS_SetAddress(TWI_t* Device, const uint8_t Address) __attribute__((always_inline));
 static inline void I2CS_SetAddress(TWI_t* Device, const uint8_t Address)
 {
	 Device->SLAVE.ADDR = ((Address & 0x7F) << 0x01);
 }

 /** @brief			Set the address mask of the TWI slave mode.
  *  @param Device	Pointer to TWI device object
  *  @param Mask	Slave address mask
  */
 static inline void I2CS_SetMask(TWI_t* Device, const uint8_t Mask) __attribute__((always_inline));
 static inline void I2CS_SetMask(TWI_t* Device, const uint8_t Mask)
 {
	 Device->SLAVE.ADDRMASK = Mask;
 }

 /** @brief			Initialize a TWI slave interface.
  *					NOTE: The slave interface needs interrupts to work properly!
  *  @param Config	Pointer to TWI configuration object
  */
 void I2CS_Init(I2CS_Config_t* Config);
 
 /** @brief			Get the status of the receive buffer in slave mode.
  *  @param Device	Pointer to TWI object
  */
 I2C_SlaveStatus_t I2CS_Status(TWI_t* Device);

#endif /* I2C_H_ */