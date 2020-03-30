/*
 * EventSystem.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega event System.

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

/** @file Arch/XMega/EventSystem/EventSystem.h
 *  @brief Driver for XMega event system. 
 *
 *  This file contains the prototypes and definitions for the XMega event system.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef EVENTSYSTEM_H_
#define EVENTSYSTEM_H_

 #include "Common/Common.h"

 #include "Arch/XMega/CPU/CPU.h"

 /** @brief	Event channel.
  */
 typedef enum
 {
	 EVENT_CHANNEL_0 = 0x00,		/**< Event channel 0 */ 
	 EVENT_CHANNEL_1 = 0x01,		/**< Event channel 1 */ 
	 EVENT_CHANNEL_2 = 0x02,		/**< Event channel 2 */ 
	 EVENT_CHANNEL_3 = 0x03,		/**< Event channel 3 */ 
	 EVENT_CHANNEL_4 = 0x04,		/**< Event channel 4 */ 
	 EVENT_CHANNEL_5 = 0x05,		/**< Event channel 5 */ 
	 EVENT_CHANNEL_6 = 0x06,		/**< Event channel 6 */ 
	 EVENT_CHANNEL_7 = 0x07,		/**< Event channel 7 */ 
 } Event_Channel_t;

 /** @brief	Event filter coefficients.
  */
 typedef enum
 {
	 EVENT_COEF_1 = 0x00,			/**< Coefficient 1 sample */ 
	 EVENT_COEF_2 = 0x01,			/**< Coefficient 2 samples */ 
	 EVENT_COEF_3 = 0x02,			/**< Coefficient 3 samples */ 
	 EVENT_COEF_4 = 0x03,			/**< Coefficient 4 samples */ 
	 EVENT_COEF_5 = 0x04,			/**< Coefficient 5 samples */ 
	 EVENT_COEF_6 = 0x05,			/**< Coefficient 6 samples */ 
	 EVENT_COEF_7 = 0x06,			/**< Coefficient 7 samples */ 
	 EVENT_COEF_8 = 0x07,	 		/**< Coefficient 8 samples */ 
 } EventSystem_FilterCoef_t;

 /** @brief			Set the filter coefficient for an event channel.
  *  @param Channel	Event channel
  *  @param Coef	Filter coefficient
  */
 static inline void Event_SetFilterCoef(const Event_Channel_t Channel, const EventSystem_FilterCoef_t Coef) __attribute__ ((always_inline));
 static inline void Event_SetFilterCoef(const Event_Channel_t Channel, const EventSystem_FilterCoef_t Coef)
 {
	 *(&EVSYS.CH0CTRL + Channel) = (*(&EVSYS.CH0CTRL + Channel) & (~0x03)) | Coef;
 }

 /** @brief			Set an I/O as event source.
  *  @param Channel	Event channel
  *  @param Port	Pointer to I/O port
  *  @param Pin		Pin number
  */
 static inline void Event_SetPinSource(const Event_Channel_t Channel, const PORT_t* Port, const uint8_t Pin) __attribute__ ((always_inline));
 static inline void Event_SetPinSource(const Event_Channel_t Channel, const PORT_t* Port, const uint8_t Pin)
 {
	 volatile uint8_t* Mux;
	 uint8_t MuxValue = 0x00;
	 
	 if(Port == &PORTA)
	 {
		 MuxValue = (0x0A << 0x03) | Pin;
	 }
	 else if(Port == &PORTB)
	 {
		 MuxValue = (0x0B << 0x03) | Pin;
	 }
	 else if(Port == &PORTC)
	 {
		 MuxValue = (0x0C << 0x03) | Pin;
	 }
	 else if(Port == &PORTD)
	 {
		 MuxValue = (0x0D << 0x03) | Pin;
	 }
	 else if(Port == &PORTE)
	 {
		 MuxValue = (0x0E << 0x03) | Pin;
	 }
	 else if(Port == &PORTF)
	 {
		 MuxValue = (0x0F << 0x03) | Pin;
	 }
	 
	if(Channel < 0x08)
	{
		Mux = &EVSYS.CH0MUX + Channel;
		*Mux = MuxValue;
	}
 }

 /** @brief			Trigger one channel of the event network.
  *  @param Channel	Event channel
  */
 static inline void Event_Trigger(const Event_Channel_t Channel) __attribute__ ((always_inline));
 static inline void Event_Trigger(const Event_Channel_t Channel)
 {
	 EVSYS.DATA |= (0x01 << Channel);
	 EVSYS.STROBE |= (0x01 << Channel);;
 }

 /** @brief			Lock the event system.
  *  @param Lock	Enable/Disable the lock
  */
 static inline void Event_SwitchLock(const bool Lock) __attribute__ ((always_inline));
 static inline void Event_SwitchLock(const bool Lock)
 {
	 uint8_t Flags = CPU_IRQSave();

	 asm volatile(	"movw r30,  %0"		"\n\t"
					"ldi  r16,  %2"     "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::	"r" (&MCU.EVSYSLOCK), 
						"r" (Lock & 0x01), 
						"M" (CCP_IOREG_gc), 
						"i" (&CCP) 
					: "r16", "r30"
				);

	 CPU_IRQRestore(Flags);
 }

#endif /* EVENTSYSTEM_H_ */