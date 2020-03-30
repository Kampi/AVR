/*
 * USB_Interrupt.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Interrupt functions for Atmel AVR AT90 USB interface.

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

/** @file Arch/AVR8/AT90/AT90/USB/USB_Interrupt.h
 *  @brief Interrupt functions for Atmel AVR AT90 USB interface.
 *
 *  This file contains the implementation of the Atmel AVR USB driver interrupts.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USB_INTERRUPT_H_
#define USB_INTERRUPT_H_

 #include "Common/Common.h"

 /** @brief	USB interrupt types.
  */
 typedef enum
 {
	 USB_VBUS_INTERRUPT = 0x00,											/**< VBus interrupt */ 
	 USB_WAKE_INTERRUPT = 0x01,											/**< Wake up interrupt */ 
	 USB_SUSPEND_INTERRUPT = 0x02,										/**< Suspend interrupt */ 
	 USB_EOR_INTERRUPT = 0x03,											/**< End of reset interrupt */ 
	 USB_SOF_INTERRUPT = 0x04,											/**< Start of frame interrupt */ 
	 USB_RXSTP_INTERRUPT = 0x05,										/**< Setup interrupt flag */ 
 } USB_InterruptType_t;

 /** @brief			Enable a specific USB interrupt.
  *  @param Source	Interrupt source
  */
 void USB_Controller_EnableInterrupt(const USB_InterruptType_t Source);
 
 /** @brief			Check if the given interrupt is enabled.
  *  @param Source	Interrupt source
  *  @return		#true if the interrupt source is enabled
  */
 bool USB_Controller_IsInterruptEnabled(const USB_InterruptType_t Interrupt);

 /** @brief			Check if the given interrupt is triggered.
  *  @param Source	Interrupt source
  *  @return		#true if the interrupt source is enabled
  */
 bool USB_Controller_CheckForInterrupt(const USB_InterruptType_t Interrupt);

 /** @brief			Clear a given interrupt flag.
  *  @param Source	Interrupt source
  */
 void USB_Controller_ClearInterruptFlag(const USB_InterruptType_t Interrupt);

 /** @brief			Disable a specific USB interrupt.
  *  @param Source	Interrupt source
  */
 void USB_Controller_DisableInterrupt(const USB_InterruptType_t Source);

 /** @brief	Disable all USB interrupts.
  */
 void USB_Controller_DisableAllInterrupts(void);

 /** @brief	Disable all USB interrupt flags.
  */
 void USB_Controller_ClearInterrupts(void);

#endif /* USB_INTERRUPT_H_ */