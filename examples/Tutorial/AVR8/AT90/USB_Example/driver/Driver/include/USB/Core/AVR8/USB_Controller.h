/*
 * USB_Controller.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: USB controller for Atmel AVR8 AT90 MCUs.

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
 *  @brief USB controller for Atmel AVR8 AT90 MCUs.
 *
 *  This file contains the prototypes and definitions for the AT90USB1287 USB controller.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef USB_CONTROLLER_H_
#define USB_CONTROLLER_H_

 #include "Common.h"
 
 #include "Endpoint.h"
 #include "USB_Device.h"
 #include "USB_Interrupt.h"

 /** @brief	USB controller device state.
  */
 extern volatile USB_State_t _DeviceState;
 
 /** @brief	USB controller configuration.
  */
 extern uint8_t _Configuration;
 
 /** @brief	USB events.
 */
 extern USB_DeviceCallbacks_t _USBEvents;

 /** @brief	Enable the PLL for the USB module.
  */
 static inline void USB_Controller_EnablePLL(void) __attribute__ ((always_inline));
 static inline void USB_Controller_EnablePLL(void)
 {
	 PLLCSR = (((0x01 << PLLP1) | (0x01 << PLLP0)) | (0x01 << PLLE));
	 while(!(PLLCSR & (0x01 << PLOCK)));
 }

 /** @brief	Disable the PLL for the USB module.
  */
 static inline void USB_Controller_DisablePLL(void) __attribute__ ((always_inline));
 static inline void USB_Controller_DisablePLL(void)
 {
	 PLLCSR = 0x00;
 }

 /** @brief	Enable the VBUS pad of the USB module.
  */
 static inline void USB_Controller_EnableVBUSPad(void) __attribute__ ((always_inline));
 static inline void USB_Controller_EnableVBUSPad(void)
 {
	 USBCON |= (0x01 << OTGPADE);
 }

 /** @brief	Disable the VBUS pad of the USB module.
  */
 static inline void USB_Controller_DisableVBUSPad(void) __attribute__ ((always_inline));
 static inline void USB_Controller_DisableVBUSPad(void)
 {
	 USBCON &= ~(0x01 << OTGPADE);
 }

 /** @brief	Enable the USB pad regulator.
  */
 static inline void USB_Controller_EnableReg(void) __attribute__ ((always_inline));
 static inline void USB_Controller_EnableReg(void)
 {
	 UHWCON |= (0x01 << UVREGE);
 }

 /** @brief	Disable the USB pad regulator.
  */
 static inline void USB_Controller_DisableReg(void) __attribute__ ((always_inline));
 static inline void USB_Controller_DisableReg(void)
 {
	 UHWCON &= ~(0x01 << UVREGE);
 }

 /** @brief	Unfreeze the USB clock.
  */
 static inline void USB_Controller_EnableClk(void) __attribute__ ((always_inline));
 static inline void USB_Controller_EnableClk(void)
 {
	 USBCON &= ~(0x01 << FRZCLK);
 }

 /** @brief	Freeze the USB clock.
  */
 static inline void USB_Controller_DisableClk(void) __attribute__ ((always_inline));
 static inline void USB_Controller_DisableClk(void)
 {
	 USBCON |= (0x01 << FRZCLK);
 }

 /** @brief		Return the state of the USB connection.
  *  @return	1 if the device is connected with VBUS.
  */
 static inline uint8_t USB_Controller_CheckVBUS(void) __attribute__ ((always_inline));
 static inline uint8_t USB_Controller_CheckVBUS(void)
 {
	 if(USBSTA & (0x01 << VBUS))
	 {
		 return 0x01;
	 }
	 
	 return 0x00;
 }

 /** @brief	Reset the USB controller.
  */
 static inline void USB_Controller_Reset(void) __attribute__ ((always_inline));
 static inline void USB_Controller_Reset(void)
 {
	 USBCON &= ~(0x01 << USBE);
	 USBCON |= (0x01 << USBE);
 }

 /** @brief			Set the USB device mode.
  *  @param Mode	USB device mode
  */
 static inline void USB_Controller_SetMode(const USB_Mode_t Mode) __attribute__ ((always_inline));
 static inline void USB_Controller_SetMode(const USB_Mode_t Mode)
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
  *  @param Mode	USB device mode
  *  @param Speed	Device speed option
  */
 void USB_Controller_Init(const USB_Mode_t Mode, const USB_Speed_t Speed);

 /** @brief			Set the operation mode of the USB controller.
  *  @param Mode	USB device mode
  */
 void USB_Controller_SetMode(const USB_Mode_t Mode);

 /** @brief	Disable the USB interface.
  */
 void USB_Controller_Disable(void);
 
 /** @brief	Reset the USB interface.
  */
 void USB_Controller_ResetInterface(void);
 
 /** @brief	Initialize the USB controller as USB device.
  */
 void USB_Controller_InitDevice(void);

#endif /* USB_CONTROLLER_H_  */ 