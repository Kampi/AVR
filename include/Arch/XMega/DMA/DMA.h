/*
 * DMA.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega DMA controller.

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

/** @file Arch/XMega/DMA/DMA.h
 *  @brief Driver for Atmel AVR XMega DMA controller.
 *
 *  This file contains the prototypes and definitions for the Atmel AVR XMega DMA driver.
 *
 *  @author Daniel Kampert
 *  @bug - Event mode
 */

#ifndef DMA_H_
#define DMA_H_

 #include "Common/Common.h"

 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/PowerManagement/PowerManagement.h"

/** @brief			DMA interrupt handler
 *  @param Channel	DMA channel
 */
 typedef void (*DMA_Callback_t)(uint8_t Channel);

 /** @brief DMA source buffer.
  */
 typedef volatile uint8_t DMA_SourceBuffer[DMA_BUFFER_SIZE];
 
 /** @brief DMA destination buffer.
  */
 typedef volatile uint8_t DMA_DestinationBuffer[DMA_BUFFER_SIZE];

 /** @brief DMA callback types.
  */
 typedef enum
 {
 	DMA_TRANSACTION_INTERRUPT = 0x01,		/**< DMA transaction complete interrupt */ 
	DMA_ERROR_INTERRUPT = 0x02,				/**< DMA error interrupt */ 
 } DMA_CallbackType_t;

 /** @brief DMA burst length.
  */
 typedef enum
 {
	 DMA_BURSTLENGTH_1 = 0x00,				/**< Burst length 1 byte */ 
	 DMA_BURSTLENGTH_2 = 0x01,				/**< Burst length 2 byte */ 
	 DMA_BURSTLENGTH_4 = 0x02,				/**< Burst length 4 byte */ 
	 DMA_BURSTLENGTH_8 = 0x03,				/**< Burst length 8 byte */ 
 } DMA_BurstLength_t;

 /** @brief DMA double buffer setting.
  */
 typedef enum
 {
	 DMA_DOUBLEBUFFER_DISABLE = 0x00,		/**< Double buffer disabled */ 
	 DMA_DOUBLEBUFFER_CH01 = 0x01,			/**< Double buffer enabled for channel 0/1 */ 
	 DMA_DOUBLEBUFFER_CH23 = 0x02,			/**< Double buffer enabled for channel 2/3 */ 
	 DMA_DOUBLEBUFFER_BOOTH = 0x03,			/**< Double buffer enabled for channel 0/1 and 2/3  */ 
 } DMA_BufferMode_t;

 /** @brief DMA channel priority.
  */
 typedef enum
 {
	 DMA_PRIORITY_RR = 0x00,				/**< Round robin */ 
	 DMA_PRIORITY_CH0RR = 0x01,				/**< Channel 0 > Round robin 1, 2, 3 */ 
	 DMA_PRIORITY_CH01RR = 0x02,			/**< Channel 0 > Channel 1 > round robin 2, 3 */ 
	 DMA_PRIORITY_CH0123 = 0x03,			/**< Channel 0 > Channel 1 > Channel 2 > Channel 3 */ 
 } DMA_Priority_t;
 
 /** @brief DMA trigger sources.
  */
 typedef enum
 {	
	DMA_TRIGGER_SOFTWARE = 0x00,			/**<  */ 
	DMA_TRIGGER_EVENTSYSTEM_CH0 = 0x01,		/**<  */ 
	DMA_TRIGGER_EVENTSYSTEM_CH1 = 0x02,		/**<  */ 
	DMA_TRIGGER_EVENTSYSTEM_CH2 = 0x03,		/**<  */ 
	DMA_TRIGGER_AES = 0x04,					/**<  */ 
	DMA_TRIGGER_ADCA_CH0 = 0x10,			/**<  */ 
	DMA_TRIGGER_ADCA_CH1 = 0x11,			/**<  */ 
	DMA_TRIGGER_ADCA_CH2 = 0x12,			/**<  */ 
	DMA_TRIGGER_ADCA_CH3 = 0x13,			/**<  */ 
	DMA_TRIGGER_ADCA_ALL = 0x14,			/**<  */
	DMA_TRIGGER_DACA_CH0 = 0x15,			/**<  */
	DMA_TRIGGER_DACA_CH1 = 0x16,			/**<  */
	DMA_TRIGGER_DACA_CH2 = 0x17,			/**<  */ 
	DMA_TRIGGER_DACA_CH3 = 0x18,			/**<  */ 
	DMA_TRIGGER_DACA_ALL = 0x19,			/**<  */ 
	DMA_TRIGGER_ADCB_CH0 = 0x20,			/**<  */ 
	DMA_TRIGGER_ADCB_CH1 = 0x21,			/**<  */ 
	DMA_TRIGGER_ADCB_CH2 = 0x22,			/**<  */ 
	DMA_TRIGGER_ADCB_CH3 = 0x23,			/**<  */ 
	DMA_TRIGGER_ADCB_ALL = 0x24,			/**<  */ 
	DMA_TRIGGER_DACB_CH0 = 0x25,			/**<  */ 
	DMA_TRIGGER_DACB_CH1 = 0x26,			/**<  */ 
	DMA_TRIGGER_DACB_CH2 = 0x27,			/**<  */ 
	DMA_TRIGGER_DACB_CH3 = 0x28,			/**<  */ 
	DMA_TRIGGER_DACB_ALL = 0x29,			/**<  */ 
	DMA_TRIGGER_TCC0_OVF = 0x40,			/**<  */ 
	DMA_TRIGGER_TCC0_ERR = 0x41,			/**<  */ 
	DMA_TRIGGER_TCC0_CCA = 0x42,			/**<  */ 
	DMA_TRIGGER_TCC0_CCB = 0x43,			/**<  */ 
	DMA_TRIGGER_TCC0_CCC = 0x44,			/**<  */ 
	DMA_TRIGGER_TCC0_CCD = 0x45,			/**<  */ 
	DMA_TRIGGER_TCC1_OVF = 0x46,			/**<  */ 
	DMA_TRIGGER_TCC1_ERR = 0x47,			/**<  */ 
	DMA_TRIGGER_TCC1_CCA = 0x48,			/**<  */ 
	DMA_TRIGGER_TCC1_CCB = 0x49,			/**<  */ 
	DMA_TRIGGER_SPIC = 0x4A,				/**<  */ 
	DMA_TRIGGER_USARTC0_RXC = 0x4B,			/**<  */ 
	DMA_TRIGGER_USARTC0_DRE = 0x4C,			/**<  */ 
	DMA_TRIGGER_USARTC1_RXC = 0x4E,			/**<  */ 
	DMA_TRIGGER_USARTC1_DRE = 0x4F,			/**<  */ 
	DMA_TRIGGER_TCD0_OVF = 0x60,			/**<  */ 
	DMA_TRIGGER_TCD0_ERR = 0x61,			/**<  */ 
	DMA_TRIGGER_TCD0_CCA = 0x62,			/**<  */ 
	DMA_TRIGGER_TCD0_CCB = 0x63,			/**<  */ 
	DMA_TRIGGER_TCD0_CCC = 0x64,			/**<  */ 
	DMA_TRIGGER_TCD0_CCD = 0x65,			/**<  */ 
	DMA_TRIGGER_TCD1_OVF = 0x66,			/**<  */ 
	DMA_TRIGGER_TCD1_ERR = 0x67,			/**<  */ 
	DMA_TRIGGER_TCD1_CCA = 0x68,			/**<  */ 
	DMA_TRIGGER_TCD1_CCB = 0x69,			/**<  */ 
	DMA_TRIGGER_SPID = 0x6A,				/**<  */ 
	DMA_TRIGGER_USARTD0_RXC = 0x6B,			/**<  */ 
	DMA_TRIGGER_USARTD0_DRE = 0x6C,			/**<  */ 
	DMA_TRIGGER_USARTD1_RXC = 0x6E,			/**<  */ 
	DMA_TRIGGER_USARTD1_DRE = 0x6F,			/**<  */ 
	DMA_TRIGGER_TCE0_OVF = 0x80,			/**<  */ 
	DMA_TRIGGER_TCE0_ERR = 0x81,			/**<  */ 
	DMA_TRIGGER_TCE0_CCA = 0x82,			/**<  */ 
	DMA_TRIGGER_TCE0_CCB = 0x83,			/**<  */ 
	DMA_TRIGGER_TCE0_CCC = 0x84,			/**<  */ 
	DMA_TRIGGER_TCE0_CCD = 0x85,			/**<  */ 
	DMA_TRIGGER_TCE1_OVF = 0x86,			/**<  */ 
	DMA_TRIGGER_TCE1_ERR = 0x87,			/**<  */ 
	DMA_TRIGGER_TCE1_CCA = 0x88,			/**<  */ 
	DMA_TRIGGER_TCE1_CCB = 0x89,			/**<  */ 
	DMA_TRIGGER_SPIE = 0x8A,				/**<  */ 
	DMA_TRIGGER_USARTE0_RXC = 0x8B,			/**<  */ 
	DMA_TRIGGER_USARTE0_DRE = 0x8C,			/**<  */ 
	DMA_TRIGGER_USARTE1_RXC = 0x8E,			/**<  */ 
	DMA_TRIGGER_USARTE1_DRE = 0x8F,			/**<  */ 
	DMA_TRIGGER_TCF0_OVF = 0xA0,			/**<  */ 
	DMA_TRIGGER_TCF0_ERR = 0xA1,			/**<  */ 
	DMA_TRIGGER_TCF0_CCA = 0xA2,			/**<  */ 
	DMA_TRIGGER_TCF0_CCB = 0xA3,			/**<  */ 
	DMA_TRIGGER_TCF0_CCC = 0xA4,			/**<  */ 
	DMA_TRIGGER_TCF0_CCD = 0xA5,			/**<  */ 
	DMA_TRIGGER_TCF1_OVF = 0xA6,			/**<  */ 
	DMA_TRIGGER_TCF1_ERR = 0xA7,			/**<  */ 
	DMA_TRIGGER_TCF1_CCA = 0xA8,			/**<  */ 
	DMA_TRIGGER_TCF1_CCB = 0xA9,			/**<  */ 
	DMA_TRIGGER_SPIF = 0xAA,				/**<  */ 
	DMA_TRIGGER_USARTF0_RXC = 0xAB,			/**<  */ 
	DMA_TRIGGER_USARTF0_DRE = 0xAC,			/**<  */ 
	DMA_TRIGGER_USARTF1_RXC = 0xAE,			/**<  */ 
	DMA_TRIGGER_USARTF1_DRE = 0xAF,			/**<  */ 
 } DMA_TriggerSource_t;

 /** @brief DMA address reload modes.
  */
 typedef enum
 {
	 DMA_ADDRESS_RELOAD_NONE = 0x00,		/**< No reload */ 
	 DMA_ADDRESS_RELOAD_BLOCK = 0x01,		/**< Reload after block transfer */ 
	 DMA_ADDRESS_RELOAD_BURST = 0x02,		/**< Reload after burst transfer */ 
	 DMA_ADDRESS_RELOAD_TRANSACTION = 0x03, /**< Reload after transaction */ 
 } DMA_AddressReload_t;
 
 /** @brief DMA address modes.
  */
 typedef enum
 {
	DMA_ADDRESS_MODE_FIXED = 0x00,			/**< No address change */ 
	DMA_ADDRESS_MODE_INC = 0x01,			/**< Increase address */ 
	DMA_ADDRESS_MODE_DEC = 0x02,			/**< Decrease address */ 
 } DMA_AddressMode_t;

 /** @brief DMA controller configuration object.
  */
 typedef struct
 {
	DMA_BufferMode_t BufferMode;			/**< DMA controller buffer mode */ 
	DMA_Priority_t Priority;				/**< DMA controller channel priority */ 
 } DMA_Config_t;

 /** @brief DMA interrupt configuration object.
  */
 typedef struct
 {
	 DMA_CH_t* Channel;						/**< Pointer to DMA channel object */ 
	 DMA_CallbackType_t Source;				/**< DMA interrupt type */
	 Interrupt_Level_t InterruptLevel;		/**< Interrupt level */
	 DMA_Callback_t Callback;				/**< Function pointer to DMA callback */
 } DMA_InterruptConfig_t;

 /** @brief DMA channel configuration object.
  */
 typedef struct
 {
	 DMA_CH_t* Channel;						/**< Pointer to DMA channel object */ 
	 bool EnableSingleShot;				/**< Set to #true  to enable single shot */
	 bool EnableRepeatMode;				/**< Set to #true to enable repeat */ 
	 DMA_BurstLength_t BurstLength;			/**< Burst length */ 
	 DMA_AddressReload_t SrcReload;			/**< Reload mode for source address */ 
	 DMA_AddressReload_t DstReload;			/**< Reload mode for destination address */ 
	 DMA_AddressMode_t SrcAddrMode;			/**< Address mode for source address */ 
	 DMA_AddressMode_t DstAddrMode;			/**< Address mode for destination address */ 
	 DMA_TriggerSource_t TriggerSource;		/**< Trigger source */ 
	 uint16_t TransferCount;				/**< Bytes to transfer */ 
	 uint8_t RepeatCount;					/**< Repeat count. Only needed if #EnableRepeatMode is #true */ 
	 uintptr_t SrcAddress;					/**< Source address */ 
	 uintptr_t DstAddress;					/**< Destination address */ 
 } DMA_TransferConfig_t;

 /** @brief	Enable the DMA controller.
  */
 static inline void DMA_Enable(void) __attribute__((always_inline)); 
 static inline void DMA_Enable(void)
 {
	 DMA.CTRL |= DMA_ENABLE_bm;
 }

 /** @brief	Disable the DMA controller.
  */
 static inline void DMA_Disable(void) __attribute__((always_inline)); 
 static inline void DMA_Disable(void)
 {
	 DMA.CTRL &= ~DMA_ENABLE_bm;
 }

 /** @brief	Reset the DMA controller.
  */
 static inline void DMA_Reset(void) __attribute__((always_inline)); 
 static inline void DMA_Reset(void)
 {
	 DMA_Disable();
	
	 DMA.CTRL |= DMA_RESET_bm;

	 DMA_Enable();
 }

 /** @brief		Read the status register.
  *  @return	Content of the status register
  */
 static inline uint8_t DMA_ReadStatus(void) __attribute__((always_inline)); 
 static inline uint8_t DMA_ReadStatus(void)
 {
	 return DMA.INTFLAGS;
 }

 /** @brief			Write the status register.
  *  @param Status	Content for the status register
  */
 static inline void DMA_WriteStatus(const uint8_t Status) __attribute__((always_inline)); 
 static inline void DMA_WriteStatus(const uint8_t Status)
 {
	 DMA.INTFLAGS = Status;
 }

 /** @brief			Set the buffer mode of the DMA controller.
  *  @param Status	Double buffer mode
  */
 static inline void DMA_SetBufferMode(const DMA_BufferMode_t DoubleBuffer) __attribute__((always_inline)); 
 static inline void DMA_SetBufferMode(const DMA_BufferMode_t DoubleBuffer)
 {
	 DMA.CTRL = (DMA.CTRL & (~(DoubleBuffer << 0x02))) | (DoubleBuffer << 0x02);
 }

 /** @brief		Get the buffer mode of the DMA controller.
  *  @return	Double buffer mode
  */
 static inline const DMA_BufferMode_t DMA_GetDBufferMode(void) __attribute__((always_inline)); 
 static inline const DMA_BufferMode_t DMA_GetDBufferMode(void)
 {
	 return ((DMA.CTRL & 0x0C) >> 0x02);
 }

 /** @brief				Set the channel priority of the DMA controller.
  *  @param Priority	Channel priority
  */
 static inline void DMA_SetChannelPriorityMode(const DMA_Priority_t Priority) __attribute__((always_inline)); 
 static inline void DMA_SetChannelPriorityMode(const DMA_Priority_t Priority)
 {
	 DMA.CTRL = (DMA.CTRL & (~Priority)) | Priority;
 }

 /** @brief		Get the channel priority of the DMA controller.
  *  @return	Channel priority
  */
 static inline const DMA_Priority_t DMA_GetChannelPriorityMode(void) __attribute__((always_inline)); 
 static inline const DMA_Priority_t DMA_GetChannelPriorityMode(void)
 {
	 return (DMA.CTRL & 0x03);
 }

 /** @brief			Initialize the DMA controller.
  *  @param Config	Pointer to configuration struct
  */
 static inline void DMA_Init(DMA_Config_t* Config) __attribute__((always_inline));
 static inline void DMA_Init(DMA_Config_t* Config)
 {
	 DMA_PowerEnable();
	 DMA_Reset();
	 DMA_SetBufferMode(Config->BufferMode);
	 DMA_SetChannelPriorityMode(Config->Priority);
	 DMA_Enable();
 }

 /** @brief			Get the configuration of the DMA controller.
  *  param Config	Pointer to configuration struct
  */
 static inline void DMA_GetConfig(DMA_Config_t* Config) __attribute__((always_inline));
 static inline void DMA_GetConfig(DMA_Config_t* Config)
 {
	 Config->BufferMode = DMA_GetDBufferMode();
	 Config->Priority = DMA_GetChannelPriorityMode();
 }
 
 /** @brief			Get the configuration of the DMA controller.
  *  param Config	Pointer to configuration struct
  */
 void DMA_Channel_Reset(DMA_CH_t* Channel);
 
 /** @brief			Get the configuration of the DMA controller.
  *  param Config	Pointer to configuration struct
  */
 void DMA_Channel_Enable(DMA_CH_t* Channel);
 
 /** @brief			Get the configuration of the DMA controller.
  *  param Config	Pointer to configuration struct
  */
 void DMA_Channel_SwitchRepeatMode(DMA_CH_t* Channel, bool RepeatMode);
 
 /** @brief			Get the configuration of the DMA controller.
  *  param Config	Pointer to configuration object
  */
 void DMA_Channel_Disable(DMA_CH_t* Channel);
 
 /** @brief					Change the interrupt level of a installed callback.
  *  @param Channel			Pointer to DMA channel object
  *  @param CallbackType	DMA callback type
  *  param InterruptLevel	Interrupt level
  */
 void DMA_Channel_ChangeInterruptLevel(DMA_CH_t* Channel, DMA_CallbackType_t Callback, Interrupt_Level_t InterruptLevel);
 
 /** @brief			Install a new DMA callback.
  *  @param Config	Pointer to interrupt configuration
  */
 void DMA_Channel_InstallCallback(DMA_InterruptConfig_t* Config);
 
 /** @brief					Remove an installed callback.
  *  @param Channel			Pointer to DMA channel object
  *  @param CallbackType	DMA callback type
  */
 void DMA_Channel_RemoveCallback(DMA_CH_t* Channel, DMA_CallbackType_t Callback);
 
 /** @brief			Configure a DMA channel.
  *  param Config	Pointer to channel configuration object
  */
 void DMA_Channel_Config(DMA_TransferConfig_t* Config);
 
 /** @brief				Set the burst length of a DMA channel.
  *  param Channel		Pointer to DMA channel object
  *  param Burstlength	Burst length
  */
 void DMA_Channel_SetBurstLength(DMA_CH_t* Channel, DMA_BurstLength_t Burstlength);
 
 /** @brief					Set the bytes to transfer for a DMA channel.
  *  param Channel			Pointer to DMA channel object
  *  param TransferCount	Bytes to transmit
  */
 void DMA_Channel_SetTransferCount(DMA_CH_t* Channel, uint16_t TransferCount);
 
 /** @brief				Set the repeat count of a DMA channel.
  *  param Channel		Pointer to DMA channel object
  *  param RepeatCount	Repeat count
  */
 void DMA_CHannel_SetRepeatCount(DMA_CH_t* Channel, uint8_t RepeatCount);
 
 /** @brief				Enable/Disable single shot mode of a channel.
  *  param Channel		Pointer to DMA channel object
  *  param SingleShot	Enable/disable
  */
 void DMA_Channel_SwitchSingleShot(DMA_CH_t* Channel, bool SingleShot);

 /** @brief			Set the source reload mode of a channel.
  *  param Channel	Pointer to DMA channel object
  *  param Mode		Reload mode
  */
 void DMA_Channel_SetSrcReloadMode(DMA_CH_t* Channel, DMA_AddressReload_t Mode);
 
 /** @brief			Get the destination reload mode of a channel.
  *  param Channel	Pointer to DMA channel object
  *  param Mode		Reload mode
  */
 void DMA_Channel_SetDestReloadMode(DMA_CH_t* Channel, DMA_AddressReload_t Mode);
 
 /** @brief			Set the source addressing mode of a channel.
  *  param Channel	Pointer to DMA channel object
  *  param Mode		Addressing mode
  */
 void DMA_Channel_SetDestAddressingMode(DMA_CH_t* Channel, DMA_AddressMode_t Mode);
 
 /** @brief			Set the destination addressing mode of a channel.
  *  param Channel	Pointer to DMA channel object
  *  param Mode		Addressing mode
  */
 void DMA_Channel_SetSrcAddressingMode(DMA_CH_t* Channel, DMA_AddressMode_t Mode);
 
 /** @brief			Set the source address of a channel.
  *  param Channel	Pointer to DMA channel object
  *  param Address	Source address
  */
 void DMA_Channel_SetSrcAddress(DMA_CH_t* Channel, uintptr_t Address);
 
 /** @brief			Set the destination address of a channel.
  *  param Channel	Pointer to DMA channel object
  *  param Address	Destination address
  */
 void DMA_Channel_SetDestAddress(DMA_CH_t* Channel, uintptr_t Address);
 
 /** @brief					Set the trigger source of a channel.
  *  param Channel			Pointer to DMA channel object
  *  param TriggerSource	Trigger source
  */
 void DMA_Channel_SetTriggerSource(DMA_CH_t* Channel, DMA_TriggerSource_t TriggerSource);
 
 /** @brief			Start a DMA transfer with the given channel.
  *  param Channel	Pointer to DMA channel object
  */
 void DMA_Channel_StartTransfer(DMA_CH_t* Channel);
 
 /** @brief			Repeat the last transfer.
  *  param Channel	Pointer to DMA channel object
  */
 void DMA_Channel_RepeatTransfer(DMA_CH_t* Channel);

#endif /* DMA_H_ */