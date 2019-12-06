/*
 * USART.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 tinyAVR USART module.

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

/** @file Arch/AVR8/tinyAVR/USART/USART.h
 *  @brief Driver for Atmel AVR8 megaAVR tinyAVR module.
 *
 *  This contains the prototypes and definitions for the tinyAVR USART driver.
 *
 *  @author Daniel Kampert
 *  @bug - Update code based on XMega driver
 *		 - No 9 bit support	
 *		 - No synchronous support
 *		 - No Multi-Processor Communication mode
 */

#ifndef USART_H_
#define USART_H_
 
 #include "Common/Common.h"
 #include "Common/Ringbuffer/RingBuffer.h"

 #include "Arch/AVR8/tinyAVR/GPIO/GPIO.h"
 
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
	 USART_RXS_INTERRUPT = 0x08,				/**< Receive start interrupt */
	 USART_ISF_INTERRUPT = 0x08,				/**< Inconsistent Sync Field interrupt \n
													 NOTE: This interrupt is only active when auto-baud is selected. */
 } USART_CallbackType_t;

 /** @brief USART interrupt configuration object.
  */
 typedef struct
 {
	 USART_t* Device;							/**< Pointer to USART device object */
	 USART_CallbackType_t Source;				/**< USART interrupt type */
	 USART_Callback_t Callback;					/**< Function pointer to USART callback */
 } USART_InterruptConfig_t;

 /** @brief USART configuration object.
  */
 typedef struct
 {
	USART_t* Device;							/**< Pointer to USART device object */
	USART_DeviceMode_t DeviceMode;				/**< USART device mode */ 	
	USART_Direction_t Direction;				/**< Direction mode */ 	
	uint32_t Baudrate;							/**< Baudrate */ 
	USART_Parity_t Parity;						/**< Parity settings */ 
	USART_Stop_t Stop;							/**< Stop bit settings */ 
	USART_Size_t Size;							/**< Data bit settings */ 	
	int8_t BSCALE;								/**< BSCALE value for baud rate */
	USART_Polarity_t ClockPolarity;				/**< Clock polarity. Only needed in synchronous mode */
	Bool_t EnableDoubleSpeed;					/**< Set #TRUE to enable double speed */
	Bool_t EnableInterruptSupport;				/**< Set to #TRUE to enable interrupt support */
	Bool_t EnableEcho;							/**< Set to #TRUE to enable the USART echo */
	Bool_t EnableOpenDrain;						/**< Set to #TRUE to enable the open drain mode for the Tx pin */
 } USART_Config_t;

 /** @brief USART message object for interrupt driven transmission.
  */
 typedef struct
 {
	 USART_t* Device;							/**< Target USART */
	 RingBuffer_t* Ptr_TxRingBuffer;			/**< Pointer to target ring buffer */				
 } USART_Message_t;

 /** @brief			Set the device mode for USART interface.
  *  @param Device	Pointer to USART object
  *  @param Mode	USART device mode
  */
 static inline void USART_SetDeviceMode(USART_t* Device, const USART_DeviceMode_t DeviceMode) __attribute__ ((always_inline));
 static inline void USART_SetDeviceMode(USART_t* Device, const USART_DeviceMode_t DeviceMode)
 {
     Device->CTRLC = (Device->CTRLC & (~(0x03 << 0x06))) | (DeviceMode << 0x06);
 }
 
 /** @brief			Get the device mode for USART interface.
  *  @param Device	Pointer to USART object
  *  @return		USART device mode
  */
 static inline const USART_DeviceMode_t USART_GetDeviceMode(USART_t* Device) __attribute__ ((always_inline));
 static inline const USART_DeviceMode_t USART_GetDeviceMode(USART_t* Device)
 {
     return Device->CTRLC >> 0x06;
 }

 /** @brief			Set the data size for USART interface.
  *  @param Device	Pointer to USART object
  *  @param Size	Data size
  */
 static inline void USART_SetDataSize(USART_t* Device, const USART_Size_t Size) __attribute__ ((always_inline));
 static inline void USART_SetDataSize(USART_t* Device, const USART_Size_t Size)
 {
     Device->CTRLC = (Device->CTRLC & ~0x07) | Size;
 }
 
 /** @brief			Get the data size for USART interface.
  *  @param Device	Pointer to USART object
  *  @return		Data size
  */
 static inline const USART_Size_t USART_GetDataSize(USART_t* Device) __attribute__ ((always_inline));
 static inline const USART_Size_t USART_GetDataSize(USART_t* Device)
 {
     return Device->CTRLC & 0x07;
 }
 
 /** @brief			Set the parity for USART interface.
  *  @param Device	Pointer to USART object
  *  @param Parity	Parity
  */
 static inline void USART_SetParity(USART_t* Device, const USART_Parity_t Parity) __attribute__ ((always_inline));
 static inline void USART_SetParity(USART_t* Device, const USART_Parity_t Parity)
 {
     Device->CTRLC = (Device->CTRLC & (~(0x03 << 0x04))) | (Parity << 0x04);
 }

 /** @brief			Get the parity for USART interface.
  *  @param Device	Pointer to USART object
  *  @return		Parity
  */
 static inline const USART_Parity_t USART_GetParity(USART_t* Device) __attribute__ ((always_inline));
 static inline const USART_Parity_t USART_GetParity(USART_t* Device)
 {
     return (Device->CTRLC >> 0x04) & 0x03;
 }
 
 /** @brief			Set the stop bits for USART interface.
  *  @param Device	Pointer to USART object
  *  @param Stop	Stop bit
  */
 static inline void USART_SetStopbits(USART_t* Device, const USART_Stop_t Stop) __attribute__ ((always_inline));
 static inline void USART_SetStopbits(USART_t* Device, const USART_Stop_t Stop)
 {
     Device->CTRLC = (Device->CTRLC & (~(0x01 << 0x03))) | (Stop << 0x03);
 }
 
 /** @brief			Get the stop bits for USART interface.
  *  @param Device	Pointer to USART object
  *  @return		Stop bit
  */
 static inline const USART_Stop_t USART_GetStopbits(USART_t* Device) __attribute__ ((always_inline));
 static inline const USART_Stop_t USART_GetStopbits(USART_t* Device)
 {
     return (Device->CTRLC >> 0x03) & 0x01;
 }

 /** @brief				Set the direction of the USART interface.
  *  @param Device		Pointer to USART object
  *  @param Direction	USART direction
  */
 static inline void USART_SetDirection(USART_t* Device, const USART_Direction_t Direction) __attribute__ ((always_inline));
 static inline void USART_SetDirection(USART_t* Device, const USART_Direction_t Direction)
 {
	 Device->CTRLB = (Device->CTRLB & (~(0x03 << 0x06))) | (Direction << 0x06);
 }

 /** @brief			Get the direction of the USART interface.
  *  @param Device	Pointer to USART object
  *  @return		USART direction
  */
 static inline const USART_Direction_t USART_GetDirection(USART_t* Device) __attribute__ ((always_inline));
 static inline const USART_Direction_t USART_GetDirection(USART_t* Device)
 {
	 return (Device->CTRLB >> 0x06) & 0x03;
 }

 /** @brief			Enable/Disable open drain mode for the Tx pin.
  *  @param Device	Pointer to USART object
  *  @param Enable	Enable/Disable
  */
 static inline void USART_SwitchOpenDrain(USART_t* Device, const Bool_t Enable) __attribute__ ((always_inline));
 static inline void USART_SwitchOpenDrain(USART_t* Device, const Bool_t Enable)
 {
	 Device->CTRLB = (Device->CTRLB & ~(0x01 << 0x03)) | ((Enable & 0x01) << 0x03);
 }


 /** @brief			Get a single byte from the USART interface.
  *  @param Device	Pointer to USART object
  *  @return		Data byte
  */
 static inline unsigned char USART_GetChar(USART_t* Device) __attribute__ ((always_inline));
 static inline unsigned char USART_GetChar(USART_t* Device)
 {
	 return Device->RXDATAL;
 }

 /** @brief			Transmit a single byte with the USART interface.
  *  @param Device	Pointer to USART object
  *  @param Data	Data byte
  */
 static inline void USART_SendChar(USART_t* Device, const unsigned char Data) __attribute__ ((always_inline));
 static inline void USART_SendChar(USART_t* Device, const unsigned char Data)
 {
	 if(((Device->CTRLC & 0x07) != USART_CHSIZE_9BITH_gc) || ((Device->CTRLC & 0x07) != USART_CHSIZE_9BITL_gc))
	 {
		 Device->TXDATAL = Data;
		 while(!(Device->STATUS & USART_DREIF_bm));
	 }
	 else
	 {
	 	// ToDo
	 }
 }

 /** @brief			Initialize the USART interface.
  *  @param Config	Pointer to USART device configuration struct
  */
 void USART_Init(USART_Config_t* Config);

 /** @brief			Install a new callback for a USART interface.
  *  @param Config	Pointer to interrupt configuration structure.
  */
 void USART_InstallCallback(const USART_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Device		Pointer to USART object
  *  @param Callback	Callback which should be removed
  */
 void USART_RemoveCallback(USART_t* Device, const USART_CallbackType_t Callback);

 /** @brief			Enable/Disable the USART echo.
  *  @param Device	Pointer to USART object
  *  @param Enable	Enable/Disable the echo
  */
 void USART_SwitchEcho(USART_t* Device, const Bool_t Enable);

 /** @brief				Set the baud rate of a USART interface.
  *  @param Device		Pointer to USART object
  *  @param Baudrate	Baudrate for the interface
  *  @param Clock		USART module clock
  *  @param DoubleSpeed	Enabled/Disable double speed
  */
 void USART_SetBaudrate(USART_t* Device, const uint32_t Baudrate, const uint32_t Clock, const Bool_t DoubleSpeed);

 /** @brief			Transmit a char array with the USART interface.
  *  @param Device	Pointer to USART object
  *  @param Data	Pointer to data array
  */
 void USART_Write(USART_t* Device, const char* data);

 /** @brief			Transmit a decimal number with the USART interface.
  *  @param Device	Pointer to USART object
  *  @param Value	Decimal value
  */
 void USART_WriteDecimal(USART_t* Device, const uint32_t Value);
 
 /** @brief			Transmit a complete line (including CR + LF) with the USART interface.
  *  @param Device	Pointer to USART object
  *  @param Data	Pointer to data array
  */
 void USART_WriteLine(USART_t* Device, const char* data);

#endif /* USART_H_ */