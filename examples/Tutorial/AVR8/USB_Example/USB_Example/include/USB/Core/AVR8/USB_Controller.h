/*
 * USB_Controller.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: USB-Controller for AT90USB1287.

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

/** @file USB/Core/AVR8/USB_Controller.h
 *  @brief USB controller for AT90USB1287.
 *
 *  This file contains the prototypes and definitions for the AT90USB1287 USB controller.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USB_CONTROLLER_H_
#define USB_CONTROLLER_H_

 #include "USB/Core/Common.h"
 
 #include "Endpoint.h"
 #include "USB_Interrupt.h"
 #include "USB_DeviceController.h"

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
  *  @return	1 if the device is connected with VBUS.
  */
 static inline uint8_t USBController_CheckVBUS(void) __attribute__ ((always_inline));
 static inline uint8_t USBController_CheckVBUS(void)
 {
	 if(USBSTA & (0x01 << VBUS))
	 {
		 return 0x01;
	 }
	 
	 return 0x00;
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

 /** @brief	Attach the device to the USB bus.
  */
 static inline void USBController_Attach(void) __attribute__ ((always_inline));
 static inline void USBController_Attach(void)
 {
	 UDCON &= ~(0x01 << DETACH);
 }
 
 /** @brief	Detach the device from the USB bus.
  */
 static inline void USBController_Detach(void) __attribute__ ((always_inline));
 static inline void USBController_Detach(void)
 {
	 UDCON &= ~(0x01 << DETACH);
 }

 /** @brief			Initialize the USB interface.
  *  @param Mode	USB device mode
  *  @param Speed	Device speed option
  */
 void USBController_Init(const USB_Mode_t Mode, const USB_Speed_t Speed);

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
 
 /** @brief	Initialize the USB controller as USB device.
  */
 void USBController_InitDevice(void);

#endif /* USB_CONTROLLER_H_  */ 