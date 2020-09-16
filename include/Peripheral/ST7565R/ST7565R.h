/*
 * ST7565R.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for ST7565R Graphic LCD.

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

/** @file Peripheral/ST7565R/ST7565R.h
 *  @brief Driver for ST7565R Graphic LCD.
 *
 *  This contains the prototypes and definitions for the display driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef ST7565R_H_
#define ST7565R_H_

 #include "Common/Common.h"

 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/ClockManagement/SysClock.h"
	 #if(ST7565R_INTERFACE_TYPE == INTERFACE_USART_SPI)
		 #include "Arch/XMega/USART/USART.h"
	 #elif(ST7565R_INTERFACE_TYPE == INTERFACE_SPI)
		 #include "Arch/XMega/SPI/SPI.h"
	 #else
		 #error "Interface not supported for ST7565R!"
	 #endif
 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/SPI/SPI.h"
 #else
	  #error "Architecture not supported for ST7565R!"
 #endif

 #if(!defined ST7565R_INTERFACE)
	 #error "Invalid interface for the ST7565R!"
 #endif

 #if(!defined ST7565R_SS)
	 #error "Invalid configuration for the ST7565R chip select!"
 #endif

 #if(!defined ST7565R_REGISTER_SELECT)
	 #error "Invalid configuration for the ST7565R register select!"
 #endif
 
 #if(!defined ST7565R_RESET)
	 #error "Invalid configuration for the ST7565R reset!"
 #endif
 
 #if(!defined ST7565R_BACKLIGHT)
	 #error "Invalid configuration for the ST7565R backlight!"
 #endif

 /** @brief			Initialize the display controller.
  *  @param Config	Pointer to SPI master configuration object
  */
 void Display_Init(SPIM_Config_t* Config);
 
 /** @brief	Reset the display.
  */
 void Display_Reset(void);
 
 /** @brief			Enable/Disable the display back light.
  *  @param Enable	#true to enable the back light.
  */
 void Display_SwitchBacklight(const bool Enable);
 
 /** @brief			Write data to the display.
  *  @param Command	Display data
  */
 void Display_WriteData(const uint8_t Data);
 
 /** @brief			Set the page address of the display.
  *  @param Page	Page address
  */
 void Display_SetPage(const uint8_t Page);

 /** @brief			Set the column address of the display.
  *  @param Column	Column address
  */
 void Display_SetColumn(const uint8_t Column);

 /** @brief			Set the start line of the display.
  *  @param Line	Line number
  */
 void Display_SetStartLine(const uint8_t Line);

 /** @brief			Enable/Disable the display controller.
  *  @param Enable	Enable/Disable
  */
 void Display_SwitchDisplay(const bool Enable);

 /** @brief			Enable/Disable the invert mode of the display controller.
  *  @param Enable	Enable/Disable
  */
 void Display_SwitchInvert(const bool Enable);

 /** @brief				Set the contrast of the display controller.
  *  @param Contrast	Contrast value
  */
 void Display_SetContrast(const uint8_t Contrast);

#endif /* ST7565R_H_ */