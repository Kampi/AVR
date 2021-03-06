/*
 * AnalogSensors_USBKey.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the VL53L0XV2 Time-of-Flight Range Sensor.

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

/** @file Peripheral/AnalogSensors/AnalogSensors_USBKey.c
 *  @brief Sensor data for AT90USBKey development board.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/AnalogSensors/AnalogSensors_USBKey.h"

/** @brief	NTC temperature codes. The temperature is calculated by the formula
			Temperature = Index - 10
			Configure the ADC for single ended measurement with Vcc / 1.6 V reference
 */
const uint16_t TemperatureCodes[] = {

};