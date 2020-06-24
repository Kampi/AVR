/*
 * GPIO.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AT90 GPIO interface.

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

/** @file Arch/AVR8/AT90/GPIO/GPIO.h
 *  @brief Driver for AT90 GPIO interface.
 *
 *  This file contains the prototypes and definitions for the AVR8 GPIO driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef GPIO_H_
#define GPIO_H_

 #include "Common/Common.h"

 #include "Arch/AVR8/AT90/CPU/CPU.h"

 /** @brief GPIO port definitions.
  */
 typedef struct
 {
	 uint8_t IN;										/* I/O port Input */
	 uint8_t DIR;										/* I/O Port Data Direction */
	 uint8_t OUT;										/* I/O Port Output */
 } PORT_t;

 /** @brief	GPIO callback definition.
 */
 typedef void (*GPIO_Callback_t)(void);

 /** @brief GPIO output directions.
  */
 typedef enum
 {
	 GPIO_DIRECTION_IN = 0x00,							/**< Direction input */ 
	 GPIO_DIRECTION_OUT = 0x01,							/**< Direction output */ 
 } GPIO_Direction_t;

 /** @brief GPIO interrupt channels
  */
 typedef enum
 {
	GPIO_INTERRUPT_0 = 0x01,							/**< GPIO interrupt channel 0 */ 
	GPIO_INTERRUPT_1 = 0x02,							/**< GPIO interrupt channel 1 */ 
	GPIO_INTERRUPT_2 = 0x04,							/**< GPIO interrupt channel 2 */ 
 } GPIO_InterruptChannel_t;
 
 /** @brief GPIO input sense configurations
  */
 typedef enum 
 {
	GPIO_SENSE_LOWLEVEL = 0x00,							/**< Sense low level */
	GPIO_SENSE_BOTH = 0x01,								/**< Sense rising and falling edge */
	GPIO_SENSE_FALLING = 0x02,							/**< Sense falling edge only */
	GPIO_SENSE_RISING = 0x03,							/**< Sense rising edge only */ 
 } GPIO_InputSense_t;

 #undef PORTA
 #define PORTA                 (*(PORT_t *) 0x20)

 #undef PORTB
 #define PORTB                 (*(PORT_t *) 0x23)

 #undef PORTC
 #define PORTC                 (*(PORT_t *) 0x26)

 #undef PORTD
 #define PORTD                 (*(PORT_t *) 0x29)

 #undef PORTE
 #define PORTE                 (*(PORT_t *) 0x2C)
	 
 #undef PORTF
 #define PORTF                 (*(PORT_t *) 0x2F)

 /** 
  * GPIO interrupt configuration object
  */
 typedef struct
 {
	 GPIO_InterruptChannel_t Channel;					/**< Interrupt channel */
	 GPIO_InputSense_t Sense;							/**< GPIO interrupt type */
	 GPIO_Callback_t Callback;							/**< Function pointer to GPIO callback */
 } GPIO_InterruptConfig_t;

 /** @brief				Set the direction of an I/O port.
  *  @param Port		Pointer to port object
  *  @param Mask		Port mask
  *  @param Direction	I/O Direction
  */
 static inline void GPIO_SetPortDirection(PORT_t* Port, const uint8_t Mask, const GPIO_Direction_t Direction) __attribute__ ((always_inline));
 static inline void GPIO_SetPortDirection(PORT_t* Port, const uint8_t Mask, const GPIO_Direction_t Direction)
 {
	if(Direction == GPIO_DIRECTION_IN)
	{
		Port->DIR &= ~Mask;
	}
	else if(Direction == GPIO_DIRECTION_OUT)
	{
		Port->DIR |= Mask;
	}
 }

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


 /** @brief			Set an I/O as output.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_SetDir(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_SetDir(PORT_t* Port, const uint8_t Pin)
 {	
	Port->DIR |= (0x01 << Pin);
 }

 /** @brief			Set an I/O as input.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_ClearDir(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_ClearDir(PORT_t* Port, const uint8_t Pin)
 {
	Port->DIR &= ~(0x01 << Pin);
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

 /** @brief			Read the value of an I/O port.
  *  @param Port	Pointer to port object
  *  @return		I/O state of the port
  */
 static inline uint8_t GPIO_ReadPort(PORT_t* Port) __attribute__ ((always_inline));
 static inline uint8_t GPIO_ReadPort(PORT_t* Port)
 {
	 return Port->IN;
 }

 /** @brief			Read the value of an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  *  @return		I/O state
  */
 static inline bool GPIO_Read(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline bool GPIO_Read(PORT_t* Port, const uint8_t Pin)
 {
	 return (Port->IN & (0x01 << Pin)) >> Pin;
 }

 /** @brief			Apply a data mask to set the complete port.
  *  @param Port	Pointer to port object
  *  @param Mask	Data mask
  */
 static inline void GPIO_SetPort(PORT_t* Port, const uint8_t Mask) __attribute__ ((always_inline));
 static inline void GPIO_SetPort(PORT_t* Port, const uint8_t Mask)
 {	
	Port->OUT |= Mask;
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

 /** @brief			Apply a data mask to clear the complete port.
  *  @param Port	Pointer to port object
  *  @param Mask	Data mask
  */
 static inline void GPIO_ClearPort(PORT_t* Port, const uint8_t Mask) __attribute__ ((always_inline));
 static inline void GPIO_ClearPort(PORT_t* Port, const uint8_t Mask)
 {
	Port->OUT &= ~Mask;
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