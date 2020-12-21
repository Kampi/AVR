/*
 * AnalogSensors.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for analog sensors.

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

/** @file Peripheral/AnalogSensors.h
 *  @brief File info: Driver for analog sensors.
 *
 *  @author Daniel Kampert
 */

#ifndef ANALOGSENSORS_H_
#define ANALOGSENSORS_H_

 #include "Board.h"
 #include "Common/Common.h"

 #ifndef REFERENCE_VOLTAGE
	 #warning "No voltage reference defined!"
	 #define REFERENCE_VOLTAGE						1000
 #endif
 
 #ifndef ADC_RESOLUTION
	 #warning "No ADC resolution defined!"
	 #define ADC_RESOLUTION							10
 #endif

 #if(BOARD == BOARD_NONE)

 #elif(BOARD == BOARD_AT90USBKEY)
	 #include "Common/Board/AnalogSensors/AnalogSensors_USBKey.h"
 #elif(BOARD == BOARD_XMEGA384C3_XPLAINED)
	 #include "Common/Board/AnalogSensors/AnalogSensors_XPlainedC3.h"
 #endif

#endif /* ANALOGSENSORS_H_ */