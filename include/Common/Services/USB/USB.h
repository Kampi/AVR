/*
 * USB.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB service.

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

/** @file Common/Services/USB/USB.h
 *  @brief USB device service implementation.
 *
 *  This file contains the prototypes and definitions for the USB service.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USB_H_
#define USB_H_
 
 #include "Common/Common.h"

 #if(MCU_ARCH == MCU_ARCH_AVR8)
	 #if(MCU_NAME == MCU_NAME_AT90USB1287)
		#include "Arch/AVR8/AT90/USB/USB_Controller.h"
	 #else
		 #error "Unknown CPU!"
	 #endif
 #elif(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/USB/USB_Controller.h"
 #else
	 #error "Unknown CPU architecture!"
 #endif
 
 #include "Common/Services/USB/Core/StandardDescriptor.h"
 #include "Common/Services/USB/Core/USB_DeviceStdRequest.h"
 #include "Common/Services/USB/Class/Class.h"

 /** @brief			Initialize the USB service.
  *  @param Config	Pointer to USB configuration object
  */
 void USB_Init(const USB_Config_t* Config);
 
 /** @brief	Polling function. 
  */
 void USB_Poll(void);
 
 /** @brief		Get the state of the USB state machine.
  *  @return	State of the state machine
  */
 volatile USB_State_t USB_GetState(void);

#endif /* USB_H_ */ 