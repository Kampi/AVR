/*
 * Config_DS1307.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Configuration file for the DS1307 I2C RTC.

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

/** @file Config/Config_DS1307.h
 *  @brief Configuration file for the Dallas DS1307 I2C RTC.
 *
 *  @author Daniel Kampert
 */

#ifndef CONFIG_DS1307_H_
#define CONFIG_DS1307_H_

 #include "Common/Common.h"
 
 #define DS1307_INTERFACE						TWIC 					/**< I2C interface used by the DS1307. */
 #define DS1307_CLOCK							100000					/**< Clock rate used by the interface. */
 
 #undef DS1307_USE_IRQ													/**< Use interrupt support. */
 
 #define DS1307_INT								PORTR, 0				/**< DS1307 interrupt pin. */
 #define DS1307_INT_CHANNEL												/**< Interrupt channel used for the DS1307. */

#endif /* CONFIG_DS1307_H_ */