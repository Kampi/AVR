/*
 * FIFO.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Software FIFO service

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

/** @file Common/FIFO/FIFO.h
 *  @brief Software FIFO service
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef FIFO_H_
#define FIFO_H_

 #include <avr/io.h>

 #include "Common/types.h"

 /** 
  * FIFO error codes
  */
 typedef enum 
 {
	 FIFO_OK = 0x00,          			/**< No error */
	 FIFO_ERROR_OVERFLOW = 0x01,		/**< Overflow error */
	 FIFO_ERROR_UNDERFLOW = 0x02,		/**< Underflow error */
	 FIFO_ERROR = 0x03,					/**< Unknown error */
 } FIFO_Status_t;

 /** 
  * FIFO object declaration
  */
 typedef struct 
 {
	union
	{
		uint32_t* u32ptr;				/**< Pointer to unsigned-32 bits location */
		uint16_t* u16ptr;				/**< Pointer to unsigned-16 bits location */
		uint8_t* u8ptr;					/**< Pointer to unsigned-8 bit location */
	} Buffer;
	
	volatile uint8_t IndexRead;			/**< Read index */
	volatile uint8_t IndexWrite;		/**< Write index */
	uint8_t Size;						/**< Size of the FIFO in elements */
	uint8_t mask;                 //!< Mask used to speed up FIFO operation (wrapping)
 } FIFO_t;

 /** @brief		Test if the FIFO is empty.
  *  @return	#TRUE when empty
  */
 static inline const Bool_t FIFO_IsEmpty(FIFO_t* FIFO) __attribute__ ((always_inline));
 static inline const Bool_t FIFO_IsEmpty(FIFO_t* FIFO)
 {
	if(FIFO->IndexWrite == FIFO->IndexRead)
	{
		return TRUE;
	}
	
	return FALSE;
 }

 /** @brief			Initialize a new FIFO object.
  *  @param FIFO	Pointer to FIFO object
  *  @param Buffer	Pointer to data buffer
  *  @param Size	Size of the FIFO
  *  @return		Status code
  */
 FIFO_Status_t FIFO_Init(FIFO_t* FIFO, void* Buffer, uint8_t Size);

#endif /* FIFO_H_ */