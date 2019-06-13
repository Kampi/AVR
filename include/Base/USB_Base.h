/*
 * USB_Base.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB definitions for Atmel AVR MCUs

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

#ifndef USB_BASE_H_
#define USB_BASE_H_
 
 #include "Common/Common.h"

 /** @brief	USB device modes.
  */
 typedef enum
 {
	 USB_MODE_DEVICE = 0x00,			/**< Device mode */ 
	 USB_MODE_HOST = 0x01,				/**< Host mode */ 
 } USB_Mode_t;
 
 /** @brief	USB bus speed options.
  */
 typedef enum
 {
	 USB_SPEED_LOW = 0x00,				/**< Low speed bus */ 
	 USB_SPEED_HIGH = 0x01,				/**< High speed bus */ 
 } USB_Speed_t;
 
 /** @brief	States for USB state machine.
  */
 typedef enum
 {
	 USB_STATE_UNATTACHED = 0x00,		/**< Device unattached */ 
	 USB_STATE_POWERED = 0x01,			/**< Device powered */ 
	 USB_STATE_SUSPEND = 0x02,			/**< Device suspend */ 
	 USB_STATE_RESET = 0x03,			/**< Reset state */
	 USB_STATE_ADDRESSED = 0x04,		/**< Device addressed */
	 USB_STATE_CONFIGURED = 0x06,		/**< Device configured */
 } USB_State_t;

 /** @brief	Events for USB enumeration.
  */
 typedef struct
 {
	 /** @brief					USB configuration changed event.
								NOTE: This event is needed for the USB enumeration. This event
								is also time critical.
	  *  @param Configuration	Configuration number
	  */
	 void (*ConfigurationChanged)(const uint8_t Configuration);
	 
	 /** @brief					Device control request event
								NOTE: This event is needed for the USB enumeration. This event
								is also time critical.
	  *  @param bRequest		USB request
	  *  @param bmRequestType	Request type
	  *  @param wValue			Request value
	  */
	 void (*ControlRequest)(const uint8_t bRequest, const uint8_t bmRequestType, const uint16_t wValue);
	 void (*Suspend)();
	 void (*Wake)();
	 void (*ConnectedWithBus)();
	 void (*DisconnectFromBus)();
	 void (*StartOfFrame)();
	 void (*Error)();
 } USB_DeviceCallbacks_t;
 
#endif /* USB_BASE_H_ */