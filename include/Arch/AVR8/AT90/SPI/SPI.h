/*
 * SPI.h
 *
 *  Copyright (C) Daniel Kampert, 2018
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

/** @file Arch/AVR8/AT90/SPI/SPI.h
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

 #include "Arch/AVR8/AT90/CPU/CPU.h"
 #include "Arch/AVR8/AT90/GPIO/GPIO.h"

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