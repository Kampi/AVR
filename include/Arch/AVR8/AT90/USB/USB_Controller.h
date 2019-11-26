/*
 * USB_Controller.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB-Controller for Atmel AVR MCUs.

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

/** @file Arch/AVR8/USB/USB_Controller.h
 *  @brief USB-Controller for Atmel AVR MCUs.
 *
 *  This file contains the prototypes and definitions for the USB driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USB_CONTROLLER_H_
#define USB_CONTROLLER_H_

 #include "Common/Services/USB/USB_Types.h"

 #include "Arch/AVR8/AT90/USB/Endpoint.h"
 #include "Arch/AVR8/AT90/USB/USB_Interrupt.h"
 #include "Arch/AVR8/AT90/USB/USB_DeviceController.h"

 /** @brief	Enable the PLL for the USB module.
  */
 static inline void USBController_EnablePLL(void) __attribute__ ((always_inline));
 static inline void USBController_EnablePLL(void)
 {
	 PLLCSR = (((0x01 << PLLP1) | (0x01 << PLLP0)) | (0x01 << PLLE));
	 while(!(PLLCSR & (0x01 << PLOCK)));
 }

 /** @brief	Disable the PLL for the USB module.
  */
 static inline void USBController_DisablePLL(void) __attribute__ ((always_inline));
 static inline void USBController_DisablePLL(void)
 {
	 PLLCSR = 0x00;
 }

 /** @brief	Enable the VBUS pad of the USB module.
  */
 static inline void USBController_EnableVBUSPad(void) __attribute__ ((always_inline));
 static inline void USBController_EnableVBUSPad(void)
 {
	 USBCON |= (0x01 << OTGPADE);
 }

 /** @brief	Disable the VBUS pad of the USB module.
  */
 static inline void USBController_DisableVBUSPad(void) __attribute__ ((always_inline));
 static inline void USBController_DisableVBUSPad(void)
 {
	 USBCON &= ~(0x01 << OTGPADE);
 }

 /** @brief	Enable the USB pad regulator.
  */
 static inline void USBController_EnableReg(void) __attribute__ ((always_inline));
 static inline void USBController_EnableReg(void)
 {
	 UHWCON |= (0x01 << UVREGE);
 }

 /** @brief	Disable the USB pad regulator.
  */
 static inline void USBController_DisableReg(void) __attribute__ ((always_inline));
 static inline void USBController_DisableReg(void)
 {
	 UHWCON &= ~(0x01 << UVREGE);
 }

 /** @brief	Unfreeze the USB clock.
  */
 static inline void USBController_EnableClk(void) __attribute__ ((always_inline));
 static inline void USBController_EnableClk(void)
 {
	 USBCON &= ~(0x01 << FRZCLK);
 }

 /** @brief	Freeze the USB clock.
  */
 static inline void USBController_DisableClk(void) __attribute__ ((always_inline));
 static inline void USBController_DisableClk(void)
 {
	 USBCON |= (0x01 << FRZCLK);
 }

 /** @brief		Return the state of the USB connection.
  *  @return	#TRUE if the device is connected with VBUS.
  */
 static inline Bool_t USBController_CheckVBUS(void) __attribute__ ((always_inline));
 static inline Bool_t USBController_CheckVBUS(void)
 {
	 if(USBSTA & (0x01 << VBUS))
	 {
		 return TRUE;
	 }
	 
	 return FALSE;
 }

 /** @brief	Reset the USB controller.
  */
 static inline void USBController_Reset(void) __attribute__ ((always_inline));
 static inline void USBController_Reset(void)
 {
	 USBCON &= ~(0x01 << USBE);
	 USBCON |= (0x01 << USBE);
 }

 /** @brief			Set the USB device mode.
  *  @param Mode	USB device mode
  */
 static inline void USBController_SetMode(const USB_Mode_t Mode) __attribute__ ((always_inline));
 static inline void USBController_SetMode(const USB_Mode_t Mode)
 {
	 if(Mode == USB_MODE_DEVICE)
	 {
		 UHWCON |= (0x01 << UIMOD);
	 }
	 else
	 {
		 UHWCON &= ~(0x01 << UIMOD);
	 }
 }

 /** @brief			Initialize the USB interface.
  *  @param Config	Pointer to USB configuration object
  */
 void USBController_Init(const USB_Config_t* Config);

 /** @brief			Set the operation mode of the USB controller.
  *  @param Mode	USB device mode
  */
 void USBController_SetMode(const USB_Mode_t Mode);

 /** @brief	Disable the USB interface.
  */
 void USBController_Disable(void);
 
 /** @brief	Reset the USB interface.
  */
 void USBController_ResetInterface(void);

#endif /* USB_CONTROLLER_H_  */ 