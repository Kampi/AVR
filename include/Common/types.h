/*
 * types.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Common type definitions for AVR microcontroller.

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

/** @file Common/types.h
 *  @brief Common type definitions for AVR microcontroller.
 *
 *  @author Daniel Kampert
 */

#ifndef TYPES_H_
#define TYPES_H_

 #include <avr/io.h>
 #include <stddef.h>
 #include <stdbool.h>

 /** @brief Definition of a complex number.
  */
 typedef struct
 {
	 uint32_t Real;								/**< Real part */ 
	 uint32_t Imag;								/**< Imaginary part */ 
 } ComplexNumber_t;

 /** @brief Memory locations for static objects.
  */
 typedef enum
 {
	 MEMORY_RAM = 0x00,							/**< Object stored in RAM */
	 MEMORY_PROGMEM = 0x01						/**< Object stored in program memory
													 NOTE: Use this for large objects */
 } MemoryType_t;

 /** @brief Time hour modes.
  */
 typedef enum
 {
	MODE_24_HOUR = 0x00,						/**< 24 hour mode */ 
	MODE_12_HOUR = 0x01,						/**< 12 hour mode */ 
 } HourMode_t;

 /** @brief Time meridiem modes.
  */
 typedef enum
 {
	MERIDIEM_AM = 0x00,							/**< AM mode */ 
	MERIDIEM_PM = 0x01,							/**< PM mode */ 
 } Meridiem_t;

 /** @brief Weekdays.
  */
 typedef enum
 {
	SUNDAY = 0x01,								/**< Sunday */ 
	MONDAY = 0x02,								/**< Monday */ 
	TUESDAY = 0x03,								/**< Tuesday */ 
	WEDNESDAY = 0x04,							/**< Wednesday */ 
	THURSDAY = 0x05,							/**< Thursday */ 
	FRIDAY = 0x06,								/**< Friday */ 
	SATURDAY = 0x07								/**< Saturday */ 
 } WeekDays_t;

 /** @brief Month of a year.
  */
 typedef enum 
 {
	JANUARY = 0x01,								/**< January */
	FEBRUARY = 0x02,							/**< February */
	MARCH = 0x03,								/**< March */
	APRIL = 0x04,								/**< April */
	MAY = 0x05,									/**< May */
	JUNE = 0x06,								/**< June */
	JULY = 0x07,								/**< July */
	AUGUST = 0x08,								/**< August */
	SEPTEMBER = 0x09,							/**< September */
	OCTOBER = 0x0A,								/**< October */
	NOVEMBER = 0x0B,							/**< November */
	DECEMBER = 0x0C								/**< December */
 } Months_t;

 /** @brief Time object.
  */
 typedef struct
 {
	 uint8_t Second;							/**< Seconds */
	 uint8_t Minute;							/**< MInutes */
	 uint8_t Hour;								/**< Hours */
	 WeekDays_t DayOfWeek;						/**< Days of the week */
	 uint8_t Day;								/**< Day */
	 Months_t Month;							/**< Month */
	 uint8_t Year;								/**< Year */
	 HourMode_t HourMode;						/**< Hour mode */
	 Meridiem_t MeridiemMode;					/**< Meridiem mode */
 } __attribute__((packed)) Time_t;

 /** @brief Representation string for number conversions.
  */
 const static char Representation[] = "0123456789ABCDEF";

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
  *  @return		#true if both arrays are identical
  */
 static inline bool CompareArray(const uint8_t* Array1, const uint8_t* Array2, const uint32_t Length) __attribute__ ((always_inline));
 static inline bool CompareArray(const uint8_t* Array1, const uint8_t* Array2, const uint32_t Length)
 {
	 for(uint32_t i = 0x00; i < Length; i++)
	 {
		 if(*(Array1++) != *(Array2++))
		 {
			return false;
		 }
	 }
	
	 return true;
 }

#endif /* TYPES_H_ */