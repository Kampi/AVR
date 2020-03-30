/*
 * TestSignals.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *  Website: www.kampis-elektroecke.de
 *  File info: Test patterns for analog signals.

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

/** @file Common/TestSignals/TestSignals.h
 *  @brief Test patterns for analog signals.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef TESTSIGNALS_H_
#define TESTSIGNALS_H_

 #include <stdint.h>

 /** @brief	Pattern length.
  */
 #define TESTSIGNAL_LENGTH							400

 uint16_t Sine_120Hz[TESTSIGNAL_LENGTH];
 uint16_t Sine_480Hz[TESTSIGNAL_LENGTH];

#endif /* TESTSIGNALS_H_ */
