/*
 * types.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Type definitions for AVR microcontroller.

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

/** @file types.h
 *  @brief Type definitions for AVR microcontroller.
 *
 *  @author Daniel Kampert
 */

#ifndef TYPES_H_
#define TYPES_H_

 #include <avr/io.h>
 #include "stddef.h"
 
 /** 
  * Definition of a simple boolean data type
  */
 typedef enum
 {
	FALSE = 0x00,		/**< Boolean FALSE */ 
	TRUE = 0x01,		/**< Boolean TRUE */ 
 } Bool_t;

 /** 
  * Definition of a complex number
  */
 typedef struct
 {
	 uint32_t Real;		/**< Real part */ 
	 uint32_t Imag;		/**< Imaginary part */ 
 } ComplexNumber_t;

 /** 
  * Memory locations for static objects
  */
 typedef enum
 {
	 MEMORY_RAM = 0x00,							/**< Object stored in RAM */
	 MEMORY_PROGMEM = 0x01						/**< Object stored in program memory
													 NOTE: Use this for large objects */
 } MemoryType_t;

 /** @brief			Convert a decimal value to a BCD value.
  *  @param Decimal	Decimal value
  *  @return		Input value as BCD value
  */
 static inline uint8_t Dec2BCD(const uint8_t Decimal) __attribute__ ((always_inline));
 static inline uint8_t Dec2BCD(const uint8_t Decimal)
 {
	 return (Decimal / 0x0A) << 0x04 | (Decimal % 0x0A);
 }

 /** @brief			Convert a BCD value to a decimal value.
  *  @param BCD		BCD value
  *  @return		Input value as decimal value
  */
 static inline uint8_t BCD2Dec(const uint8_t BCD) __attribute__ ((always_inline));
 static inline uint8_t BCD2Dec(const uint8_t BCD)
 {
	 return 0x0A * (BCD >> 0x04) + (BCD % 0x10);
 }

 /** @brief			Compare two arrays with the same length.
  *  @param Array1	Pointer to array 1
  *  @param Array2	Pointer to array 2
  *  @param Length	Length of the arrays
  *  @return		TRUE if both arrays are identical
  */
 static inline Bool_t CompareArray(const uint8_t* Array1, const uint8_t* Array2, const uint32_t Length) __attribute__ ((always_inline));
 static inline Bool_t CompareArray(const uint8_t* Array1, const uint8_t* Array2, const uint32_t Length)
 {
	 for(uint32_t i = 0x00; i < Length; i++)
	 {
		 if(*(Array1++) != *(Array2++))
		 {
			return FALSE;
		 }
	 }
	
	 return TRUE;
 }

#endif /* TYPES_H_ */