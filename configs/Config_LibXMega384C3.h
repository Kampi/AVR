/*
 * Config_LibXMega384C3.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: XMega384C3 configuration file for library builder.

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

/** @file configs/Config.h
 *  @brief XMega384C3 configuration file for library builder.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG__LIBXMEHA384C3_H_
#define CONFIG__LIBXMEHA384C3_H_

 #include "Common/Common.h"

 #define F_CPU										2000000UL					/**< Define this symbol so the compiler 
																					 won't exclude the clock selection part. */

 #define USART_BUFFER_SIZE							32							/**< Size of USART buffer in bytes. */
 #define DMA_BUFFER_SIZE							32							/**< Size of the DMA buffer. */
 #define SPI_BUFFER_SIZE							32							/**< Size of SPI buffer in bytes. */
 #define TWI_BUFFER_SIZE							32							/**< Size of TWI buffer in bytes. */

#endif /* CONFIG__LIBXMEHA384C3_H_ */