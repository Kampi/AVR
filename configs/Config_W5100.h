/*
 * Config_W5100.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the WIZnet W5100 Ethernet controller.

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

/** @file configs/Config_W5100.h
 *  @brief Configuration example for the WIZnet W5100 Ethernet controller
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_W5100_H_
#define CONFIG_W5100_H_

 #include "Common/Common.h"

 /*
	Ethernet configuration
 */
 #define ETHERNET_PORT							80
 #define ETHERNET_TIMEOUT						65535

 /*
	 SPI configuration
 */
 #define SPI_BUFFER_SIZE						32							/**< Size of SPI buffer in bytes. */

 #define W5100_INTERFACE_TYPE					INTERFACE_SPI				/**< Interface type for the W5100. \n
																				 NOTE: Only needed for devices with an USART-SPI interface */
 #define W5100_INTERFACE						SPIC						/**< SPI interface used by the W5100. \n
																				 NOTE: Only needed for XMega architecture */
 #undef W5100_USE_INT														/**< Set to use interrupts instead of polling. */
 #undef W5100_INT															/**< W5100 interrupt pin. \n
																				 NOTE: Only used when #W5100_USE_INT is set. */
 #undef W5100_INT_CHANNEL													/**< Interrupt channel used for the W5100. \n
																				 NOTE: Only used when #W5100_USE_INT is set. */
 #undef W5100_INT_LEVEL														/**< Interrupt priority. \n
																				 NOTE: Only used when #W5100_USE_INT is set.
																				 NOTE: Only used by XMega architecture. */
 #define W5100_SS								PORTE, 0					/**< W5100 SS pin. */

 #define W5100_USE_EXT_RESET												/**< Set to use an GPIO as an additional reset source for the W5100. */
 #define W5100_EXT_RESET						PORTB, 1					/**< Pin used for external reset. \n
																				 NOTE: Only used when #W5100_USE_EXT_RESET is set. */
#endif /* CONFIG_W5100_H_ */