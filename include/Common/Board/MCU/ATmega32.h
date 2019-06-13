/*
 * ATmega32.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: MCU definitions for Atmel Mega32

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

/** @file ATmega32.h
 *  @brief MCU specific definitions for the ATmega32.
 *
 *  @author Daniel Kampert
 */

#ifndef ATMEGA32__H_
#define ATMEGA32__H_

 #include "Common/Common.h"

 /*
	 Main clock
 */
 #define F_CPU											16000000UL					/**< MCU clock frequency */

 /*
	USART
 */
 #define USART_DEVICES									1
 #define USART_BUFFER_SIZE								32
 #define USART_DEFAULT_ECHO								TRUE

 /*
	ADC
 */
 #define ADC_DEVICES									1
 #define ADC_0											0
 #define ADC_1											1
 #define ADC_2											2
 #define ADC_3											3
 #define ADC_4											4
 #define ADC_5											5
 #define ADC_6											6
 #define ADC_7											7
 #define ADC_PORT										&PORTA
 
 /*
	I2C
 */
 #define I2C_DEVICES									1
 #define I2C_SCL										0
 #define I2C_SDA										1
 #define I2C_PORT										&PORTC
 
 /*
	SPI
 */
 #define SPI_DEVICES									1
 #define SPI_BUFFERSIZE									16
 #define SPI_SS_PIN										4
 #define SPI_SCK_PIN									7
 #define SPI_MOSI_PIN									5
 #define SPI_MISO_PIN									6
 #define SPI_PORT										&PORTB
 
 /*
	Timer 0
 */
 #define TIMER0_DEVICES									1

 /*
	Timer 1
 */
 #define TIMER1_DEVICES									1
 
 /*
	Timer 2
 */
 #define TIMER2_DEVICES									1

#endif /* ATMEGA32__H_ */