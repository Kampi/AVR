/*
 * SSD1306.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for SSD1306 display controller.

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

/** @file Peripheral/SSD1306/SSD1306.h
 *  @brief Driver for SSD1306 display controller.
 *
 *  This contains the prototypes and definitions for the display driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef SSD1306_H_
#define SSD1306_H_

 #include "Common/Common.h"

 /*
	Architecture specific definitions
 */
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/ClockManagement/SysClock.h"
	 #if(SSD1306_INTERFACE_TYPE == INTERFACE_USART_SPI)
		 #include "Arch/XMega/USART/USART.h"
	 #elif(SSD1306_INTERFACE_TYPE == INTERFACE_SPI)
		 #include "Arch/XMega/SPI/SPI.h"
	 #else
		 #error "Interface not supported for SSD1306!"
	 #endif
 #elif(MCU_ARCH == MCU_ARCH_AVR8)
	 #include "Arch/AVR8/SPI/SPI.h"
 #else
	  #error "Architecture not supported for SSD1306!"
 #endif

 #if(!defined SSD1306_DATA)
	 #warning "Invalid configuration for the SSD1306 chip select. Use default."

	 #define SSD1306_DATA							PORTD, 0
 #endif

 #if(!defined SSD1306_RESET)
	 #warning "Invalid configuration for the SSD1306 register select. Use default."

	 #define SSD1306_RESET							PORTA, 3
 #endif
 
 #if(!defined SSD1306_SS)
	 #warning "Invalid configuration for the SSD1306 reset. Use default."

	 #define SSD1306_SS								PORTF, 3
 #endif

 /*
	Public API
 */

 /** @brief			Initialize the display controller.
  *  @param Config	Pointer to SPI master configuration object
  *					NOTE: Set to #NULL when the SPI is already initialized!
  */
 void Display_Init(SPIM_Config_t* Config);

 /** @brief			Write data to the display controller.
  *  @param Command	Display data
  */
 void Display_WriteData(const uint8_t Data);

 /** @brief			Set the page address of the display controller.
  *  @param Line	Display page
  */
 void Display_SetPage(const uint8_t Page);

 /** @brief			Set the column address of the display controller.
  *  @param Line	Column address
  */
 void Display_SetColumn(const uint8_t Column);

 /** @brief			Set the start line address of the display controller.
  *  @param Line	Display line
  */
 void Display_SetStartLine(const uint8_t Line);

 /** @brief	Perform a reset of the display controller.
  */
 void Display_Reset(void);

 /** @brief			Enable/Disable the display controller.
  *  @param Enable	Enable/Disable
  */
 void Display_SwitchDisplay(const Bool_t Enable);

 /** @brief			Enable/Disable the invert mode of the display controller.
  *  @param Enable	Enable/Disable
  */
 void Display_SwitchInvert(const Bool_t Enable);

 /** @brief				Set the contrast of the display controller.
  *  @param Contrast	Contrast value
  */
 void Display_SetContrast(const uint8_t Contrast);

#endif /* SSD1306_H_ */