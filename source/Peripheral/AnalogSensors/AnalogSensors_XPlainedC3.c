/*
 * AnalogSensors_XPlainedC3.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Sensor data for XPlainedC3 development board.

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

/** @file Peripheral/AnalogSensors/AnalogSensors_XPlainedC3.c
 *  @brief Sensor data for XPlainedC3 development board.
 *
 *  @author Daniel Kampert
 */

#include "Common/Board/AnalogSensors/AnalogSensors_XPlainedC3.h"

/** @brief	ICA of the ambient light sensor in uA.
 */
const uint16_t TEMT6000X01_ICA = 50;

/** @brief	Series resistor of the ambient light sensor in ohms.
 */
const uint16_t TEMT6000X01_R = 4700;

/** @brief	NTC temperature codes. The temperature is calculated by the formula
			Temperature = Index - 10
			Configure the ADC for single ended measurement with Vcc / 1.6 V reference
 */
const uint16_t TemperatureCodes[] = {
	1875, 1828, 1781, 1734, 1687, 1640, 1594, 1548, 1503, 1458, 1414, 1370,
	1327, 1285, 1243, 1202, 1162, 1123, 1084, 1047, 1010, 975, 940, 907, 874,
	842, 811, 781, 752, 724, 697, 671, 645, 621, 597, 575, 553, 531, 511, 491,
	472, 454, 437, 420, 404, 388, 373, 359, 345, 332, 319, 307, 295, 283, 273,
	262, 252, 243, 233, 225
};

const uint16_t Size_TemperatureCodes = sizeof(TemperatureCodes);