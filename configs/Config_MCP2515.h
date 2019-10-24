/*
 * Config_MCP2515.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
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

/** @file Config_MCP2515.h
 *  @brief Configuration example for the Microchip MCP2515 CAN controller.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_MCP2515_H_
#define CONFIG_MCP2515_H_

 #include "Common/Common.h"
 
 #define MCP2515_INTERFACE_TYPE					INTERFACE_USART_SPI		/**< Interface type for the MCP2515. */
 #define MCP2515_INTERFACE						SPIC					/**< SPI interface for the MCP2515. */
 
 #define MCP2515_SS_PORT						&PORTB					/**< MCP2515 SS port. */
 #define MCP2515_SS_PIN							0						/**< MCP2515 SS pin. */
 #define MCP2515_INT_PORT						&PORTB					/**< MCP2515 interrupt port. */
 #define MCP2515_INT_PIN						1						/**< MCP2515 interrupt pin. */

#endif /* CONFIG_MCP2515_H_ */