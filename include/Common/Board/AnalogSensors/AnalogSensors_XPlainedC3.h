/*
 * AnalogSensors_XPlainedC3.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Analog sensors for the XPlained C3 development board.

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

/** @file Peripheral/AnalogSensors/AnalogSensors_XPlainedC3.h
 *  @brief Analog sensors for the XPlained C3 development board.
 *
 *  This contains the prototypes and definitions for the XPlained C3 driver for analog sensors.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef ANALOGSENSORS_XPLAINEDC3_H_
#define ANALOGSENSORS_XPLAINEDC3_H_

 #include "Board.h"
 #include "Common/Common.h"

 extern const uint16_t TEMT6000X01_ICA;
 extern const uint16_t TEMT6000X01_R;
 extern const uint16_t TemperatureCodes[];
 extern const uint16_t AmbientCodes[];
 extern const uint16_t Size_TemperatureCodes;

 /** @brief			Convert an ADC result into a temperature [°C].
  *  @param Result	ADC result
  *  @return		Temperature in °C
  */
 static inline uint16_t ADC2Temp(const uint16_t Result) __attribute__ ((always_inline));
 static inline uint16_t ADC2Temp(const uint16_t Result)
 {
	 if(Result < TemperatureCodes[0]) 
	 {
		 uint16_t i = 0x00;
		 for(i = 0x00; i < (Size_TemperatureCodes / 2); i++)
		 {
			 if(TemperatureCodes[i] < Result)
			 {
				 break;
			 }
		 }

		 return i - 10;
	 }
	 
	 return -10;
 }
 
 /** @brief			Convert an ADC result into a ambient light value [lux].
  *  @param Result	ADC result
  *  @return		Ambient light in lux
  */
 static inline float ADC2Light(const uint16_t Result) __attribute__ ((always_inline));
 static inline float ADC2Light(const uint16_t Result)
 {
	 // Ev = 100 * U / (R * ICA)
	 return  100 * (Result << 0x01) * ((REFERENCE_VOLTAGE / 1000) / ((float)(1 << ADC_RESOLUTION))) * (1.0 / (float)((1 << ADC_RESOLUTION)) * 1.0 / ((float)TEMT6000X01_R)) * 10E6 / TEMT6000X01_ICA;
 }

#endif /* ANALOGSENSORS_XPLAINEDC3_H_ */