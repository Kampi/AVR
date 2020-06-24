/*
 * Config_MCP2515.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration example for the Microchip MCP2515 CAN controller.

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

/** @file configs/Config_MCP2515.h
 *  @brief Configuration example for the Microchip MCP2515 CAN controller.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_MCP2515_H_
#define CONFIG_MCP2515_H_

 #include "Common/Common.h"
 
 /*
	 SPI configuration
 */
 #define SPI_BUFFER_SIZE						32							/**< Size of SPI buffer in bytes. */
 
 #undef MCP2515_INTERFACE_TYPE												/**< Interface type for the MCP2515. \n
																				 NOTE: Only needed for devices with an USART-SPI interface */
 #undef MCP2515_INTERFACE													/**< SPI interface used by the MCP2515. \n
																				 NOTE: Only needed for XMega architecture */
 #define MCP2515_SS								PORTB, 0					/**< MCP2515 SS pin. */
 #define MCP2515_INT							PORTD, 3					/**< MCP2515 interrupt pin. */
 #define MCP2515_INT_CHANNEL					GPIO_INTERRUPT_1			/**< Interrupt channel used for the MCP2515. */
 #undef MCP2515_INT_LEVEL													/**< Interrupt priority. \n
																				 NOTE: Only used by XMega architecture. */
 #define MCP2515_USE_EXT_RESET												/**< Set to use an GPIO instead of the SPI as reset for the MCP2515. */
 #define MCP2515_EXT_RESET						PORTB, 1					/**< Pin used for external reset. \n
																				 NOTE: Only used when #MCP2515_USE_EXT_RESET is set */
 #undef MCP2515_USE_EXT_TX													/**< Set to use GPIO instead of the SPI to request a transmission. */
 #undef MCP2515_EXT_TX0														/**< Pin used for external transmission request for buffer 0. \n
																				 NOTE: Only used when #MCP2515_USE_EXT_TX is set */
 #undef MCP2515_EXT_TX1														/**< Pin used for external transmission request for buffer 1. \n
																				 NOTE: Only used when #MCP2515_USE_EXT_TX is set */
 #undef MCP2515_EXT_TX2														/**< Pin used for external transmission request for buffer 2. \n
																				 NOTE: Only used when #MCP2515_USE_EXT_TX is set */

#endif /* CONFIG_MCP2515_H_ */