/*
 * GPIO.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 tinyAVR GPIO interface.

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

/** @file Arch/AVR8/tinyAVR/GPIO/GPIO.h
 *  @brief Driver for Atmel AVR8 tinyAVR GPIO interface.
 *
 *  This file contains the prototypes and definitions for the AVR8 tinyAVR GPIO driver.
 *
 *  @author Daniel Kampert
 *  @bug - Event mode
 */

#ifndef GPIO_H_
#define GPIO_H_

 #include "Common/Common.h"

 /** @brief	GPIO callback definition.
 */
 typedef void (*GPIO_Callback_t)(void);

 /** @brief GPIO output directions
  */
 typedef enum
 {
	 GPIO_DIRECTION_IN = 0x00,					/**< Direction input */ 
	 GPIO_DIRECTION_OUT = 0x01,					/**< Direction output */ 
 } GPIO_Direction_t;

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
		Port->DIRCLR = Mask;
	}
	else if(Direction == GPIO_DIRECTION_OUT)
	{
		Port->DIRSET = Mask;
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
		Port->DIRCLR = (0x01 << Pin);
	}
	else if(Direction == GPIO_DIRECTION_OUT)
	{
		Port->DIRSET = (0x01 << Pin);
	}
 }

 /** @brief			Set an I/O as output.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_SetDir(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_SetDir(PORT_t* Port, const uint8_t Pin)
 {	
	Port->DIRSET = 0x01 << Pin;
 }

 /** @brief			Set an I/O as input.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_ClearDir(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_ClearDir(PORT_t* Port, const uint8_t Pin)
 {
	Port->DIRCLR = 0x01 << Pin;
 }

 /** @brief			Toggle the direction of an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_ToggleDirection(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_ToggleDirection(PORT_t* Port, const uint8_t Pin)
 {
	Port->DIRTGL = 0x01 << Pin;
 }

 /** @brief			Read the value of an I/O port.
  *  @param Port	Pointer to port object
  *  @return		I/O state of the port
  */
 static inline const uint8_t GPIO_ReadPort(PORT_t* Port) __attribute__ ((always_inline));
 static inline const uint8_t GPIO_ReadPort(PORT_t* Port)
 {
	 return Port->IN;
 }

 /** @brief			Read the value of an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  *  @return		I/O state
  */
 static inline const Bool_t GPIO_Read(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline const Bool_t GPIO_Read(PORT_t* Port, const uint8_t Pin)
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

 /** @brief			Set an I/O to high state.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Set(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_Set(PORT_t* Port, const uint8_t Pin)
 {	
	Port->OUTSET = 0x01 << Pin;
 }

 /** @brief			Apply a data mask to clear the complete port.
  *  @param Port	Pointer to port object
  *  @param Mask	Data mask
  */
 static inline void GPIO_ClearPort(PORT_t* Port, const uint8_t Mask) __attribute__ ((always_inline));
 static inline void GPIO_ClearPort(PORT_t* Port, const uint8_t Mask)
 {
	Port->OUTCLR = Mask;
 }

 /** @brief			Clear an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Clear(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_Clear(PORT_t* Port, const uint8_t Pin)
 {
	Port->OUTCLR = 0x01 << Pin;
 }

 /** @brief			Toggle the output of an I/O pin.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Toggle(PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void GPIO_Toggle(PORT_t* Port, const uint8_t Pin)
 {
	Port->OUTTGL = 0x01 << Pin;
 }

 /** @brief			Enable the output of the peripheral clock on Pin 7.
  *  @param Enable	Enable/Disable
  */
 static inline void GPIO_SwitchClockOutput(const Bool_t Enable) __attribute__ ((always_inline));
 static inline void GPIO_SwitchClockOutput(const Bool_t Enable)
 {
	 uint8_t Temp = CLKCTRL.MCLKCTRLA;

	 if(Enable)
	 {
		 Temp |= CLKCTRL_CLKOUT_bm;
	 }
	 else
	 {
		 Temp &= ~CLKCTRL_CLKOUT_bm;
	 }

	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&CLKCTRL.MCLKCTRLB),
						"r" (Temp),
						"M" (CCP_IOREG_gc),
						"i" (&CCP)
					: "r16", "r30"
				 );
 }

#endif /* GPIO_H_ */