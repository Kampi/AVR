/*
 * Config_AT90USBKey.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Project configuration example for the AT90USBKey development board.

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

/** @file configs/Config_AT90USBKey.h
 *  @brief Project configuration example for the AT90USBKey development board.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_AT90USBKEY_H_
#define CONFIG_AT90USBKEY_H_

 #include "Common/Common.h"

 #define BOARD										BOARD_AT90USBKEY		/**< Use the driver for the AT90USBKey development board. */

 /*
	 Main clock
 */
 #define F_CPU										8000000UL				/**< MCU clock frequency */

 /*
	LED
 */
 #define LED0_RED									PORTD, 4				/**< Red color of LED0. */
 #define LED0_GREEN									PORTD, 5				/**< Green color of LED0. */
 #define LED1_RED									PORTD, 7				/**< Red color of LED1. */
 #define LED1_GREEN									PORTD, 6				/**< Green color of LED1. */
 
 /*
	 Analog sensors
 */
 #define TEMPERATURE_SENSOR_PIN						0						/**< Analog temperature sensor. */
 #define VBAT_MONITOR_PIN							3						/**< Analog VBat sensor. */
 
 /*
	 Reference voltage [mV]
 */
 #define REFERENCE_VOLTAGE							3300	

 /*
	 External flash memory
 */
 #define USE_AT45DB642D
 #define DATAFLASH_COUNT							2	
 
#endif /* CONFIG_AT90USBKEY_H_ */