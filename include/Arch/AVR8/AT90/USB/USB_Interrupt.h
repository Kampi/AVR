/*
 * USB_Interrupt.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Interrupt functions for Atmel AVR USB.

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
 *  @brief Interrupt functions for Atmel AVR USB.
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
	 USB_VBUS_INTERRUPT = 0x00,				/**< VBus interrupt */ 
	 USB_WAKE_INTERRUPT = 0x01,				/**< Wake up interrupt */ 
	 USB_SUSPEND_INTERRUPT = 0x02,			/**< Suspend interrupt */ 
	 USB_EOR_INTERRUPT = 0x03,				/**< End of reset interrupt */ 
	 USB_SOF_INTERRUPT = 0x04,				/**< Start of frame interrupt */ 
	 USB_RXSTP_INTERRUPT = 0x05,			/**< Setup interrupt flag */ 
 } USB_InterruptType_t;

 /** @brief			Enable a specific USB interrupt.
  *  @param Source	Interrupt source
  */
 static inline void USB_Controller_EnableInterrupt(const USB_InterruptType_t Source) __attribute__ ((always_inline));
 static inline void USB_Controller_EnableInterrupt(const USB_InterruptType_t Source)
 {
	 switch(Source)
	 {
		 /*
			Device & Host interrupts
		 */
		 case USB_VBUS_INTERRUPT:
		 {
			 USBCON |= (0x01 << VBUSTE);
			 break;
		 }
		 
		 /*
			Device interrupts
		 */
		 case USB_WAKE_INTERRUPT:
		 {
			 UDIEN |= (0x01 << WAKEUPE);
			 break;
		 }
		 case USB_SUSPEND_INTERRUPT:
		 {
			  UDIEN |= (0x01 << SUSPE);
			  break;
		 }
		 case USB_EOR_INTERRUPT:
		 {
			  UDIEN |= (0x01 << EORSTE);
			  break;
		 }
		 case USB_SOF_INTERRUPT:
		 {
			  UDIEN |= (0x01 << SOFE);
			  break;
		 }
		 case USB_RXSTP_INTERRUPT:
		 {
			  UEIENX |= (0x01 << RXSTPE);
			  break;
		 }
		 default:
		 {
			break;
		 }

		 /*
			Host interrupts
		 */
	 }
 }

 /** @brief			Check if the given interrupt is enabled.
  *  @param Source	Interrupt source
  *  @return		#TRUE if the interrupt source is enabled
  */
 static inline Bool_t USB_Controller_IsInterruptEnabled(const USB_InterruptType_t Interrupt) __attribute__ ((always_inline));
 static inline Bool_t USB_Controller_IsInterruptEnabled(const USB_InterruptType_t Interrupt)
 {
	 switch(Interrupt)
	 {
		 /*
			Device & Host interrupts
		 */
		 case USB_VBUS_INTERRUPT:
		 {
			return (USBCON & (0x01 << VBUSTE));
		 }
		 
		 /*
			Device interrupts
		 */
		 case USB_WAKE_INTERRUPT:
		 {
			 return ((UDIEN & (0x01 << WAKEUPE)) >> WAKEUPE);
		 }
		 case USB_SUSPEND_INTERRUPT:
		 {
			  return (UDIEN & (0x01 << SUSPE));
		 }
		 case USB_EOR_INTERRUPT:
		 {
			 return ((UDIEN & (0x01 << EORSTE)) >> EORSTE);
		 }
		 case USB_SOF_INTERRUPT:
		 {
			 return ((UDIEN & (0x01 << SOFE)) >> SOFE);
		 }
		 case USB_RXSTP_INTERRUPT:
		 {
			 return ((UEIENX & (0x01 << RXSTPE)) >> RXSTPE);
		 }

		 /*
			Host interrupts
		 */
	 }
	 
	 return FALSE;
 }

 /** @brief			Check if the given interrupt is triggered.
  *  @param Source	Interrupt source
  *  @return		#TRUE if the interrupt source is enabled
  */
 static inline Bool_t USB_Controller_CheckForInterrupt(const USB_InterruptType_t Interrupt) __attribute__ ((always_inline));
 static inline Bool_t USB_Controller_CheckForInterrupt(const USB_InterruptType_t Interrupt)
 {
	 switch(Interrupt)
	 {
		 /*
			Device & Host interrupts
		 */
		 case USB_VBUS_INTERRUPT:
		 {
			return (USBINT & (0x01 << VBUSTI));
		 }
		 
		 /*
			Device interrupts
		 */
		 case USB_WAKE_INTERRUPT:
		 {
			 return ((UDINT & (0x01 << WAKEUPI)) >> WAKEUPI);
		 }
		 case USB_SUSPEND_INTERRUPT:
		 {
			  return (UDINT & (0x01 << SUSPI));
		 }
		 case USB_EOR_INTERRUPT:
		 {
			 return ((UDINT & (0x01 << EORSTI)) >> EORSTI);
		 }
		 case USB_SOF_INTERRUPT:
		 {
			 return ((UDINT & (0x01 << SOFI)) >> SOFI);
		 }
		 case USB_RXSTP_INTERRUPT:
		 {
			 return ((UEINTX & (0x01 << RXSTPI)) >> RXSTPI);
		 }

		 /*
			Host interrupts
		 */
	 }
	 
	 return FALSE;
 }

 /** @brief			Clear a given interrupt flag.
  *  @param Source	Interrupt source
  */
 static inline void USB_Controller_ClearInterruptFlag(const USB_InterruptType_t Interrupt) __attribute__ ((always_inline));
 static inline void USB_Controller_ClearInterruptFlag(const USB_InterruptType_t Interrupt)
 {
	 switch(Interrupt)
	 {
		 /*
			Device & Host interrupts
		 */
		 case USB_VBUS_INTERRUPT:
		 {
			 USBINT &= ~(0x01 << VBUSTI);
			 break;
		 }
		 
		 /*
			Device interrupts
		 */
		 case USB_WAKE_INTERRUPT:
		 {
			 UDINT &= ~(0x01 << WAKEUPI);
			 break;
		 }
		 case USB_SUSPEND_INTERRUPT:
		 {
			 UDINT &= ~(0x01 << SUSPI);
			 break;
		 }
		 case USB_EOR_INTERRUPT:
		 {
			 UDINT &= ~(0x01 << EORSTI);
			 break;
		 }
		 case USB_SOF_INTERRUPT:
		 {
			 UDINT &= ~(0x01 << SOFI);
			 break;
		 }
		 case USB_RXSTP_INTERRUPT:
		 {
			 UEINTX &= ~(0x01 << RXSTPI);
			 break;
		 }

		 /*
			Host interrupts
		 */
	 }
 }

 /** @brief			Disable a specific USB interrupt.
  *  @param Source	Interrupt source
  */
 static inline void USB_Controller_DisableInterrupt(const USB_InterruptType_t Source) __attribute__ ((always_inline));
 static inline void USB_Controller_DisableInterrupt(const USB_InterruptType_t Source)
 {
	 switch(Source)
	 {
		 /*
			Device & Host interrupts
		 */
		 case USB_VBUS_INTERRUPT:
		 {
			 USBCON &= ~(0x01 << VBUSTE);
			 break;
		 }
		 
		 /*
			Device interrupts
		 */
		 case USB_WAKE_INTERRUPT:
		 {
			 UDIEN &= ~(0x01 << WAKEUPE);
			 break;
		 }
		 case USB_SUSPEND_INTERRUPT:
		 {
			  UDIEN &= ~(0x01 << SUSPE);
			  break;
		 }
		 case USB_EOR_INTERRUPT:
		 {
			  UDIEN &= ~(0x01 << EORSTE);
			  break;
		 }
		 case USB_SOF_INTERRUPT:
		 {
			  UDIEN &= ~(0x01 << SOFE);
			  break;
		 }
		 case USB_RXSTP_INTERRUPT:
		 {
			  UEIENX &= ~(0x01 << RXSTPE);
			  break;
		 }

		 /*
			Host interrupts
		 */
	 } 
 }

 /** @brief	Disable all USB interrupts.
  */
 static inline void USB_Controller_DisableAllInterrupts(void) __attribute__ ((always_inline));
 static inline void USB_Controller_DisableAllInterrupts(void)
 {
	 USBCON &= ~((0x01 << VBUSTE) | (0x01 << IDTE));

	 // Clear all device mode interrupt enable bits
	 UDIEN = 0x00;
	 
	 // Clear all host mode interrupt enable bits
	 UHIEN = 0x00;

	 // Clear all OTG mode interrupt enable bits
	 OTGIEN = 0x00;
 }

 /** @brief	Disable all USB interrupt flags.
  */
 static inline void USB_Controller_ClearInterrupts(void) __attribute__ ((always_inline));
 static inline void USB_Controller_ClearInterrupts(void)
 {
	 // Clear all interrupt flags of the USB controller
	 USBINT = 0x00;
	 
	 // Clear all device mode interrupt flags
	 UDINT = 0x00;
	 
	 // Clear all host mode interrupt flags
	 UHINT = 0x00;
	 
	 // Clear all OTG mode interrupt flags
	 OTGINT = 0x00;
 }

#endif /* USB_INTERRUPT_H_ */