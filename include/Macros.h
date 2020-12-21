/*
 * Macros.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Common GCC macros. 

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

/** @file Macros.h
 *  @brief Common GCC macros.
 *
 *  @author Daniel Kampert
 */

#ifndef MACROS_H_
#define MACROS_H_

 #define _CONCAT(A, B)														A ## B
 #define _FIRST(A, B)														A
 #define _SECOND(A, B)														B
 
 #define FIRST_ARG(A)														_FIRST(A)
 #define SECOND_ARG(A)														_SECOND(A)
 #define CONCAT(A)															_CONCAT(A)

 #define GET_PERIPHERAL(Arg)												&_FIRST(Arg)
 #define GET_INDEX(Arg)														_SECOND(Arg)

 #define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)		2 * N
 #define VA_NARGS(...)														VA_NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

 #define _STR(x) #x
 #define STR(x) _STR(x)

#endif /* MACROS_H_ */