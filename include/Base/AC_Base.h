/*
 * AC_Base.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Base definitions for Atmel AVR AC

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

/** @file AC_Base.h
 *  @brief Common definitions for AVR AC.
 *
 *  @author Daniel Kampert
 */

#ifndef AC_BASE_H_
#define AC_BASE_H_

 #include "Common/types.h"

 /** @brief	AC callback definition.
 */
 typedef void (*AC_Callback_t)(void);

 /** @brief	ADC comparator enumeration.
  */
 typedef enum
 {
	 AC_COMPARATOR_0 = 0x01,				/**< AC comparator 0 */
	 AC_COMPARATOR_1 = 0x02,				/**< AC comparator 1 */
 } AC_Comparator_t;

 /** @brief	AC callback types.
  */
 typedef enum
 {
	 AC_COMP0_INTERRUPT = 0x01,				/**< AC comparator 0 interrupt */ 
	 AC_COMP1_INTERRUPT = 0x02,				/**< AC comparator 1 interrupt */ 
	 AC_WINDOW_INTERRUPT = 0x04,			/**< AC window interrupt */ 
 } AC_CallbackType_t;

#endif /* AC_BASE_H_ */