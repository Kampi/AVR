/*
 * XMega256A3BU.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: MCU specific definitions and constants for the Atmel XMega256A3BU.

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

/** @file Common/Board/MCU/XMega256A3BU.h
 *  @brief MCU specific definitions and constants for the Atmel XMega256A3BU.
 *
 *  @author Daniel Kampert
 */

#ifndef XMEGA256A3BU_H_
#define XMEGA256A3BU_H_

 #include "Common/Common.h"

 /*
	Sleep modes
 */
 #define SLEEP_MODES									5							/**< Number of sleep modes for the device */ 

 /*
	Default reference voltage
 */
 #define DEFAULT_REFERENCE_VOLTAGE						1000						/**< Default reference voltage for analog parts */ 

 /*
	GPIO
 */
 #define GPIO_PORT_COUNT								7							/**< GPIO port count */ 
 
 /*
	DMA
 */
 #define DMA_CHANNEL									4							/**< DMA channel count for each controller */
 
 /*
	DAC
 */
 #define DAC_RESOLUTION									12							/**< Resolution of the DAC in bit */ 
 
 /*
	AES
 */
 #define AES_KEYSIZE									16							/**< Size of a AES key in bytes */ 
 #define AES_DATASIZE									16							/**< Size of a AES data block in bytes */ 
 
 /*
	ADC
 */
 #define ADC_DEVICES									2							/**< ADC device count in MCU */
 #define ADC_CHANNEL									4							/**< Channels per ADC device */
 #define ADC_RESOLUTION									12							/**< ADC resolution in bits */
 
 /*
	AC
*/
 #define AC_DEVICES										2							/**< AC device count in MCU */
 
 /*
	USART
 */
 #define USART_DEVICES									3							/**< USART device count in MCU */
 #define USART_CHANNEL									2							/**< Channels per USART device */
 #define USART_XCK0_PIN									1							/**< Pin number of USART XCK0 pin */
 #define USART_RX0_PIN									2							/**< Pin number of USART RxD0 pin */
 #define USART_TX0_PIN									3							/**< Pin number of USART TxD0 pin */
 #define USART_XCK1_PIN									3							/**< Pin number of USART XCK1 pin */
 #define USART_RX1_PIN									6							/**< Pin number of USART RxD1 pin */
 #define USART_TX1_PIN									7							/**< Pin number of USART TxD1 pin */
 
 /*
	Timer 0
 */
 #define TIMER0_DEVICES									4
 #define TIMER0_CC										4

 /*
	Timer 1
 */
 #define TIMER1_DEVICES									3
 #define TIMER1_CC										2
 
 /*
	Timer 2
 */
 #define TIMER2_DEVICES									4
 #define TIMER2_CC										4

 /*
	SPI
 */
 #define SPI_DEVICES									2							/**< SPI device count in MCU */
 #define SPI_SS_PIN										4							/**< Pin number of SPI SS pin */
 #define SPI_MOSI_PIN									3							/**< Pin number of SPI MOSI pin */
 #define SPI_MISO_PIN									2							/**< Pin number of SPI MISO pin */
 #define SPI_SCK_PIN									1							/**< Pin number of SPI SCK pin */

 /*
	TWI
 */
 #define TWI_DEVICES									2							/**< TWI device count in MCU */
 #define TWI_SDA_PIN									0							/**< Pin number of TWI SDA pin */
 #define TWI_SCL_PIN									1							/**< Pin number of TWI SCL pin */

#endif /* XMEGA256A3BU_H_ */