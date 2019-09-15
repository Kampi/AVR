/*
 * GPIO.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 GPIO Interface

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

/** @file Arch/AVR8/GPIO/GPIO.h
 *  @brief Driver for AVR8 GPIO. 
 *
 *  This file contains the prototypes and definitions for the AVR8 GPIO driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef GPIO_H_
#define GPIO_H_

 #include "Definitions.h"

 #if(MCU_NAME == MCU_NAME_ATMEGA32)
	#include "Common/Board/MCU/ATmega32.h"
 #elif(MCU_NAME == MCU_NAME_AT90USB1287)
	 #include "Common/Board/MCU/AT90USB1287.h"
 #else
	#error "No valid mcu"
 #endif
 
 #include "Arch/AVR8/CPU/CPU.h"
 
 #include "Base/GPIO_Base.h"

 /** 
  * I/O Ports
  */
 typedef struct
 {
	 uint8_t IN;										/* I/O port Input */
	 uint8_t DIR;										/* I/O Port Data Direction */
	 uint8_t OUT;										/* I/O Port Output */
 } PORT_t;

 /** 
  * GPIO interrupt channels
  */
 typedef enum
 {
	GPIO_INTERRUPT_0 = 0x01,							/**< GPIO interrupt channel 0 */ 
	GPIO_INTERRUPT_1 = 0x02,							/**< GPIO interrupt channel 1 */ 
	GPIO_INTERRUPT_2 = 0x04,							/**< GPIO interrupt channel 2 */ 
 } GPIO_InterruptChannel_t;
 
 /** 
  * GPIO interrupt sense
  */
 typedef enum 
 {
	GPIO_SENSE_LOWLEVEL = 0x00,							/**< Sense low level */
	GPIO_SENSE_BOTH = 0x01,								/**< Sense rising and falling edge */
	GPIO_SENSE_FALLING = 0x02,							/**< Sense falling edge only */
	GPIO_SENSE_RISING = 0x03,							/**< Sense rising edge only */ 
 } GPIO_InputSense_t;

 #undef PORTA
 #define PORTA                 (*(PORT_t *) 0x39)

 #undef PORTB
 #define PORTB                 (*(PORT_t *) 0x36)

 #undef PORTC
 #define PORTC                 (*(PORT_t *) 0x33)

 #undef PORTD
 #define PORTD                 (*(PORT_t *) 0x30)

 /** 
  * GPIO interrupt configuration object
  */
 typedef struct
 {
	 GPIO_InterruptChannel_t Channel;					/**< Interrupt channel */
	 GPIO_InputSense_t Sense;							/**< GPIO interrupt type */
	 GPIO_Callback_t Callback;							/**< Function pointer to GPIO callback */
 } GPIO_InterruptConfig_t;

 /** @brief				Set the direction of an I/O pin.
  *  @param Port		Pointer to port object
  *  @param Pin			Pin number
  *  @param Direction	I/O Direction
  */
 static inline void GPIO_SetDirection(PORT_t* Port, const uint8_t Pin, const GPIO_Direction_t Direction) __attribute__ ((always_inline));
 static inline void GPIO_SetDirection(PORT_t* Port, const uint8_t Pin, const GPIO_Direction_t Direction)
 {
	 if(Direction == GPIO_DIRECTION_IN)
	 {
		 Port->DIR &= ~(0x01 << Pin);
	 }
	 else if(Direction == GPIO_DIRECTION_OUT)
	 {
		 Port->DIR |= 0x01 << Pin;
	 }
 }

 /** @brief			Toggle the direction of an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_ToggleDirection(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_ToggleDirection(PORT_t* Port, const uint8_t Pin)
 {
	 Port->DIR ^= 0x01 << Pin;
 }

 /** @brief			Read the value of an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  *  @return		I/O state
  */
 static inline Bool_t GPIO_Read(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline Bool_t GPIO_Read(PORT_t* Port, const uint8_t Pin)
 {
	 return (Port->IN & (0x01 << Pin)) >> Pin;
 }

 /** @brief			Set an I/O to high state or enable pull up if the I/O is configured as input.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Set(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_Set(PORT_t* Port, const uint8_t Pin)
 {
	 Port->OUT |= 0x01 << Pin;
 }

 /** @brief			Clear an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Clear(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_Clear(PORT_t* Port, const uint8_t Pin)
 {
	 Port->OUT &= ~(0x01 << Pin);
 }

 /** @brief			Toggle an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Toggle(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_Toggle(PORT_t* Port, const uint8_t Pin)
 {
	 Port->OUT ^= (0x01 << Pin);
 }

 /** @brief			Install a new GPIO interrupt callback.
  *  @param Config	Pointer to configuration structure.
  */
 void GPIO_InstallCallback(const GPIO_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Channel		Interrupt channel
  */
 void GPIO_RemoveCallback(const GPIO_InterruptChannel_t Channel);

#endif /* GPIO_H_ */