/*
 * Config_DS1307.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Dallas DS1307 I2C RTC.

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

/** @file config/Config_DS1307.h
 *  @brief Configuration example for the Dallas DS1307 I2C RTC.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_DS1307_H_
#define CONFIG_DS1307_H_

 #include "Common/Common.h"
 
 /*
	 TWI configuration
 */
 #define TWI_BUFFER_SIZE						32							/**< Size of TWI buffer in bytes. */

 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #define DS1307_INTERFACE					TWIC						/**< I2C interface used by the RTC. */
 #endif

 #define DS1307_CLOCK							100000						/**< Clock rate used by the interface. */
 
 #define DS1307_USE_IRQ														/**< Use interrupt support. */
 #undef DS1307_INT_PULLUP													/**< Enable the internal MCU pull up. */

 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #define DS1307_INT							PORTR, 0					/**< DS1307 interrupt pin. */
	 #define DS1307_INT_CHANNEL					GPIO_INTERRUPT_0			/**< Interrupt channel used for the DS1307. */
	 #define DS1307_INT_SENSE					GPIO_SENSE_FALLING			/**< Interrupt sensing. */
	 #define DS1307_INT_LEVEL					INT_LVL_LO					/**< Interrupt priority. \n
																				  NOTE: Only needed by XMega architecture. */
 #else
	 #define DS1307_INT							PORTD, 2					/**< DS1307 interrupt pin. */
	 #define DS1307_INT_CHANNEL					GPIO_INTERRUPT_0			/**< Interrupt channel used for the DS1307. */
	 #define DS1307_INT_SENSE					GPIO_SENSE_FALLING			/**< Interrupt sensing. */
 #endif

#endif /* CONFIG_DS1307_H_ */