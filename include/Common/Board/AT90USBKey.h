/*
 * at90usbkey.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Board definitions for the AT90USBKey

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

/** @file at90usbkey.h
 *  @brief Hardware definitions for the AT90USBKey.
 *
 *  @author Daniel Kampert
 */

#ifndef AT90USBKEY_H_
#define AT90USBKEY_H_

 #include "Definitions.h"
 
 #include "Common/Board/MCU/AT90USB1287.h"
 
 #define F_CPU										8000000UL
 
 /*
	Peripheral definitions
 */
 // LED 1
 #define LED0_PORT									&PORTD
 #define LED0_RED_PIN								4
 #define LED0_GREEN_PIN								5
 
 // LED 2
 #define LED1_PORT									&PORTD
 #define LED1_RED_PIN								7
 #define LED1_GREEN_PIN								6
 
 /*
	Analog sensors
 */
 // Temperature sensor
 #define TEMPERATURE_SENSOR_PIN						0
 
 // VBat monitor
 #define VBAT_MONITOR_PIN							3
 
 /*
	Reference voltage [mV]
 */
 #define REFERENCE_VOLTAGE							3300

 /*
	Peripheral includes
 */
 #include "Arch/AVR8/GPIO/GPIO.h"
 #include "Arch/AVR8/SPI/SPI.h"
 
 #include "Peripheral/Joystick.h"
 #include "Peripheral/AnalogSensors.h"
 
 #include "Common/Services/USB/USB.h"
 //#include "Common/Services/DataFlashManager/DataFlashManager.h"
 
#endif /* AT90USBKEY_H_ */