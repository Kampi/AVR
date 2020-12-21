/*
 * AT90USBKey.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Hardware driver for the Atmel AT90USBKey development board.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */ 

/** @file Common/Board/AT90USBKey.h
 *  @brief Hardware driver and on-board peripherals for the Atmel AVR AT90USBKey development board.
 *
 *  @author Daniel Kampert
 */

#ifndef AT90USBKEY_H_
#define AT90USBKEY_H_

 #include "Common/Board/MCU/AT90USB1287.h"					

 /*
	 Board specific configurations
 */

 /*
	 Clock frequency generated by the external oscillator
 */
 #define F_CPU										8000000UL

 /*
	Peripheral definitions
 */
 // LED 1
 #define LED0_RED									PORTD, 4
 #define LED0_GREEN									PORTD, 5
 
 // LED 2
 #define LED1_RED									PORTD, 7
 #define LED1_GREEN									PORTD, 6
 
 /*
	 Analog inputs
 */
 // Temperature sensor
 #define TEMPERATURE_SENSOR							0
 
 // VBat monitor
 #define VBAT_MONITOR								3
 
 /*
	 Reference voltage [mV]
 */
 #define REFERENCE_VOLTAGE							3300	

 /*
	Hardware driver
 */
 #include "Arch/AVR8/AT90/GPIO/GPIO.h"

 /*
	Peripheral driver
 */
 #include "Peripheral/Joystick.h"
 #include "Peripheral/AnalogSensors.h"
 
#endif /* AT90USBKEY_H_ */