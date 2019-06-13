/*
 * time_avr.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Common time definitions for AVR microcontroller

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file time_avr.h
 *  @brief Common time definitions for AVR library.
 *
 *  @author Daniel Kampert
 */

#ifndef TIME_AVR_H_
#define TIME_AVR_H_

 /** 
  * Time hour modes
  */
 typedef enum
 {
	MODE_24_HOUR = 0x00,			/**< 24 hour mode */ 
	MODE_12_HOUR = 0x01,			/**< 12 hour mode */ 
 } HourMode_t;

 /** 
  * Time meridiem modes
  */
 typedef enum
 {
	MERIDIEM_AM = 0x00,				/**< AM mode */ 
	MERIDIEM_PM = 0x01,				/**< PM mode */ 
 } Meridiem_t;

 /** 
  * Weekdays
  */
 typedef enum
 {
	SUNDAY = 0x01,					/**< Sunday */ 
	MONDAY = 0x02,					/**< Monday */ 
	TUESDAY = 0x03,					/**< Tuesday */ 
	WEDNESDAY = 0x04,				/**< Wednesday */ 
	THURSDAY = 0x05,				/**< Thursday */ 
	FRIDAY = 0x06,					/**< Friday */ 
	SATURDAY = 0x07					/**< Saturday */ 
 } WeekDays_t;

 /** 
  * Month of a year
  */
 typedef enum 
 {
	JANUARY = 0x01,					/**< January */
	FEBRUARY = 0x02,				/**< February */
	MARCH = 0x03,					/**< March */
	APRIL = 0x04,					/**< April */
	MAY = 0x05,						/**< May */
	JUNE = 0x06,					/**< June */
	JULY = 0x07,					/**< July */
	AUGUST = 0x08,					/**< August */
	SEPTEMBER = 0x09,				/**< September */
	OCTOBER = 0x0A,					/**< October */
	NOVEMBER = 0x0B,				/**< November */
	DECEMBER = 0x0C					/**< December */
 } Months_t;

 /** 
  * Generic time variable
  */
 typedef struct
 {
	 uint8_t Second;				/**< Seconds */
	 uint8_t Minute;				/**< MInutes */
	 uint8_t Hour;					/**< Hours */
	 WeekDays_t DayOfWeek;			/**< Days of the week */
	 uint8_t Day;					/**< Day */
	 Months_t Month;				/**< Month */
	 uint8_t Year;					/**< Year */
	 HourMode_t HourMode;			/**< Hour mode */
	 Meridiem_t MeridiemMode;		/**< Meridiem mode */
 } __attribute__((packed)) Time_t;

#endif /* TIME_AVR_H_ */