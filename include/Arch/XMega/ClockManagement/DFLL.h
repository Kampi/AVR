/*
 * DFLL.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega DFLL module.

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

/** @file Arch/XMega/ClockManagement/DFLL.h
 *  @brief Driver for Atmel AVR XMega DFLL module. 
 *
 *  This file contains the prototypes and definitions for the Atmel AVR  XMega DFLL driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef DFL_H_
#define DFL_H_

 #include "Arch/XMega/NVM/NVM.h"
 #include "Arch/XMega/CPU/CPU.h"
 #include "Arch/XMega/ClockManagement/SysClock.h"

 /** @brief DFLL calibration references.
  */
 typedef enum
 {
	 DFLL_REFERENCE_32KHZ_INT = 0x00,			/**< 32 kHz internal oscillator */ 
	 DFLL_REFERENCE_32KHZ_EXT = 0x01,			/**< 32 kHz external oscillator */ 
	 DFFL_REFERENCE_USBOF = 0x03,				/**< USB start of frame */ 
 } DFLL_CalibReference_t;

 /** @brief DFLL configuration object.
  */
 typedef struct
 {
	 DFLL_t* Device;							/**< Pointer to Timer1 device object */ 
	 DFLL_CalibReference_t Reference;			/**< Calibration reference */
 } DFLL_Config_t;

 /** @brief			Enable a DFLL device.
  *  @param Device	Pointer to DFLL device object
  */
 static inline void DFLL_Enable(DFLL_t* Device) __attribute__((always_inline));
 static inline void DFLL_Enable(DFLL_t* Device)
 {
	 Device->CTRL = 0x01;
 }

 /** @brief			Disable a DFLL device.
  *  @param Device	Pointer to DFLL device object
  */
 static inline void DFLL_Disable(DFLL_t* Device) __attribute__((always_inline));
 static inline void DFLL_Disable(DFLL_t* Device)
 {
	 Device->CTRL = 0x00;
 }
 
 /** @brief				Set the calibration reference for the DFLL device.
  *  @param Reference	Calibration reference
  */
 static inline void DFLL_SetCalibrationRef(const DFLL_CalibReference_t Reference) __attribute__((always_inline));
 static inline void DFLL_SetCalibrationRef(const DFLL_CalibReference_t Reference)
 {
	 OSC.DFLLCTRL = (OSC.DFLLCTRL & (~(0x03 << 0x01))) | (Reference << 0x01);
 }

 /** @brief			Set the compare value for a DFLL device.
  *  @param Compare	Compare value
  */
 static inline void DFLL_SetCompare(DFLL_t* Device, const uint16_t Compare) __attribute__((always_inline));
 static inline void DFLL_SetCompare(DFLL_t* Device, const uint16_t Compare)
 {
	 Device->COMP1 = Compare & 0xFF;
	 Device->COMP2 = (Compare >> 0x08) & 0xFF;
 }
 
 /** @brief		Get the compare value for a DFLL device.
  *  @return	Compare value
  */
 static inline uint32_t DFLL_GetCompare(DFLL_t* Device) __attribute__((always_inline));
 static inline uint32_t DFLL_GetCompare(DFLL_t* Device)
 {
	 return ((uint32_t)Device->COMP2 << 0x10) + (Device->COMP1 << 0x08) + Device->COMP0;
 }
 
 /** @brief			Calibrate a DFLL device.
  *  @param Device	Pointer to DFLL device object
  */
 static inline void DFLL_Calibrate(DFLL_t* Device) __attribute__((always_inline));
 static inline void DFLL_Calibrate(DFLL_t* Device)
 {
	 NVM.CMD  = NVM_CMD_READ_CALIB_ROW_gc;
	 Device->CALB = NVM_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, USBRCOSC));
 }

 /** @brief			Initialize the DFLL device.
  *  @param Config	Pointer to DFLL configuration struct
  */
 static inline void DFLL_Init(DFLL_Config_t* Config) __attribute__((always_inline));
 static inline void DFLL_Init(DFLL_Config_t* Config)
 {
	 if(Config->Reference == DFLL_REFERENCE_32KHZ_INT)
	 {
		 SysClock_EnableClock(CLOCK_SOURCE_INT2MHZ);
	 }
	 else if(Config->Reference == DFLL_REFERENCE_32KHZ_EXT)
	 {
		 SysClock_EnableClock(CLOCK_SOURCE_EXT32KHZ);
	 }
	 
	 DFLL_Enable(Config->Device);
	 DFLL_SetCalibrationRef(Config->Reference);
 }

 void DFLL_Calibrate(DFLL_t* Device);
 uint32_t DFLL_GetCompare(DFLL_t* Device);

#endif /* DFL_H_ */