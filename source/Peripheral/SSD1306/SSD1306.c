/*
 * SSD1306.c
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

/** @file Peripheral/SSD1306/SSD1306.c
 *  @brief Driver for SSD1306 display controller.
 *
 *  This contains the implementation of the display driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/SSD1306/SSD1306.h"

/** @defgroup SSD1306
 *  @{
 */
 	/** @defgroup SSD1306-Commands
	 *  SD card commands.
	 *  @{
	 */
		#define SSD1306_CMD_LOW_COL(Column)					(0x00 | (Column))
		#define SSD1306_CMD_HIGH_COL(Column)				(0x10 | (Column))
		#define SSD1306_CMD_START_LINE(Line)				(0x40 | (Line))
		#define SSD1306_CMD_CONTRAST						0x81
		#define SSD1306_CMD_CHARGE_PUMP						0x8D
		#define SSD1306_CMD_REMAP_0							0xA0
		#define SSD1306_CMD_REMAP_127						0xA1
		#define SSD1306_CMD_ENTIRE_DISPLAY_ON(Ignore)		(0xA4 | (Ignore))
		#define SSD1306_CMD_INVERT_OFF						0xA6
		#define SSD1306_CMD_INVERT_ON						0xA7
		#define SSD1306_CMD_MULTIPLEX_RATIO					0xA8
		#define SSD1306_CMD_DISPLAY_ON						0xAF
		#define SSD1306_CMD_DISPLAY_OFF						0xAE
		#define SSD1306_CMD_PAGE_ADDRESS(Page)				(0xB0 | (Page & 0x07))
		#define SSD1306_CMD_COM_SCAN_NORM					0xC0
		#define SSD1306_CMD_COM_SCAN_REMAPPED				0xC8
		#define SSD1306_CMD_DISPLAYOFFSET					0xD3
		#define SSD1306_CMD_DISPLAYCLOCK					0xD5
		#define SSD1306_CMD_PRECHARGE						0xD9
		#define SSD1306_CMD_COM_HARDWARE					0xDA
		#define SSD1306_CMD_VCOMH							0xDB
	/** @} */ // end of SSD1306-Commands
/** @} */ // end of SSD1306

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#if(SSD1306_INTERFACE_TYPE == INTERFACE_USART_SPI)
		#define SSD1306_SPIM_INIT(Config)											USART_SPI_Init(Config)
		#define SSD1306_SPIM_TRANSMIT(Data)											USART_SPI_SendData(&CONCAT(DISPLAY_INTERFACE), Data)
		#define SSD1306_SPIM_CHIP_SELECT(Port, Pin)									USART_SPI_SelectDevice(Port, Pin)
		#define SSD1306_SPIM_CHIP_DESELECT(Port, Pin)								USART_SPI_DeselectDevice(Port, Pin)
		#elif(SSD1306_INTERFACE_TYPE == INTERFACE_SPI)
	#define SSD1306_SPIM_INIT(Config)												SPIM_Init(Config)
		#define SSD1306_SPIM_TRANSMIT(Data)											SPIM_SendData(&CONCAT(DISPLAY_INTERFACE), Data)
		#define SSD1306_SPIM_CHIP_SELECT(Port, Pin)									SPIM_SelectDevice(Port, Pin)
		#define SSD1306_SPIM_CHIP_DESELECT(Port, Pin)								SPIM_DeselectDevice(Port, Pin)
	#else
		#error "Invalid interface for SSD1306 display!"
	#endif
#elif(MCU_ARCH == MCU_ARCH_AVR8)
	#define SSD1306_SPIM_INIT(Config)												SPIM_Init(Config)
	#define SSD1306_SPIM_TRANSMIT(Data)												SPIM_SendData(Data)
	#define SSD1306_SPIM_CHIP_SELECT(Port, Pin)										SPIM_SelectDevice(Port, Pin)
	#define SSD1306_SPIM_CHIP_DESELECT(Port, Pin)									SPIM_DeselectDevice(Port, Pin)
#else
	#error "Architecture not supported for SSD1306 display!"
#endif

/** 
 * Display initialization commands
 */
static uint8_t DisplayInitialization[] = 
{
	SSD1306_CMD_MULTIPLEX_RATIO,
	0x1F,
	SSD1306_CMD_DISPLAYOFFSET,
	0x00,
	SSD1306_CMD_START_LINE(0x00),
	SSD1306_CMD_REMAP_127,
	SSD1306_CMD_COM_SCAN_REMAPPED,
	SSD1306_CMD_COM_HARDWARE,
	0x02,
	SSD1306_CMD_CONTRAST,
	0x8F,
	SSD1306_CMD_ENTIRE_DISPLAY_ON(0),
	SSD1306_CMD_INVERT_OFF,
	SSD1306_CMD_DISPLAYCLOCK,
	0x80,
	SSD1306_CMD_CHARGE_PUMP,
	0x14,
	SSD1306_CMD_VCOMH,
	0x40,
	SSD1306_CMD_PRECHARGE,
	0xF1,
	SSD1306_CMD_DISPLAY_ON
};

/** @brief			Write a command to the display controller.
 *  @param Command	Display command
 */
static void SSD1306_WriteCommand(const uint8_t Data)
{
	SSD1306_SPIM_CHIP_SELECT(GET_PERIPHERAL(SSD1306_SS), GET_INDEX(SSD1306_SS));
	GPIO_Clear(GET_PERIPHERAL(SSD1306_DATA), GET_INDEX(SSD1306_DATA));
	SSD1306_SPIM_TRANSMIT(Data);
	SSD1306_SPIM_CHIP_DESELECT(GET_PERIPHERAL(SSD1306_SS), GET_INDEX(SSD1306_SS));
}

/** @brief			Write multiple commands to the display controller.
 *  @param Data		Display commands
 *  @param Length	Length of commands (including parameters)
 */
static void SSD1306_WriteCommandBytes(const uint8_t* Data, const uint8_t Length)
{
	SSD1306_SPIM_CHIP_SELECT(GET_PERIPHERAL(SSD1306_SS), GET_INDEX(SSD1306_SS));
	GPIO_Clear(GET_PERIPHERAL(SSD1306_DATA), GET_INDEX(SSD1306_DATA));
	
	for(uint8_t i = 0x00; i < Length; i++)
	{
		SSD1306_SPIM_TRANSMIT(*Data++);
	}
	
	SSD1306_SPIM_CHIP_DESELECT(GET_PERIPHERAL(SSD1306_SS), GET_INDEX(SSD1306_SS));
}

void Display_Init(SPIM_Config_t* Config)
{
	// Set Data/Command pin as output in low state
	GPIO_SetDirection(GET_PERIPHERAL(SSD1306_DATA), GET_INDEX(SSD1306_DATA), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(SSD1306_DATA), GET_INDEX(SSD1306_DATA));
	
	// Set display reset as output in high state
	GPIO_SetDirection(GET_PERIPHERAL(SSD1306_RESET), GET_INDEX(SSD1306_RESET), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(SSD1306_RESET), GET_INDEX(SSD1306_RESET));
	
	// Set display SS as output in high state
	GPIO_SetDirection(GET_PERIPHERAL(SSD1306_SS), GET_INDEX(SSD1306_SS), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(SSD1306_SS), GET_INDEX(SSD1306_SS));
	
	Display_Reset();
	
	// Initialize SPI-Interface
	if(Config != NULL)
	{
		SSD1306_SPIM_INIT(Config);
	}
	
	SSD1306_WriteCommandBytes(DisplayInitialization, sizeof(DisplayInitialization));	
}

void Display_WriteData(const uint8_t Data)
{
	SSD1306_SPIM_CHIP_SELECT(GET_PERIPHERAL(SSD1306_SS), GET_INDEX(SSD1306_SS));
	GPIO_Set(GET_PERIPHERAL(SSD1306_DATA), GET_INDEX(SSD1306_DATA));
	SSD1306_SPIM_TRANSMIT(Data);
	GPIO_Clear(GET_PERIPHERAL(SSD1306_DATA), GET_INDEX(SSD1306_DATA));
	SSD1306_SPIM_CHIP_DESELECT(GET_PERIPHERAL(SSD1306_SS), GET_INDEX(SSD1306_SS));
}

void Display_SetPage(const uint8_t Page)
{
	SSD1306_WriteCommand(SSD1306_CMD_PAGE_ADDRESS(Page & 0x0F));
}

void Display_SetColumn(const uint8_t Column)
{
	// Use only the lower 4 bits, because the controller supports only 128 columns
	uint8_t Temp = Column & 0x7F;
	SSD1306_WriteCommand(SSD1306_CMD_HIGH_COL(Temp >> 0x04));
	SSD1306_WriteCommand(SSD1306_CMD_LOW_COL(Temp & 0x0F));
}

void Display_SetStartLine(const uint8_t Line)
{	
	// Use only the lower 6 bits, because the controller supports only 64 lines
	SSD1306_WriteCommand(SSD1306_CMD_START_LINE(Line & 0x3F));
}

void Display_Reset(void)
{
	GPIO_Clear(GET_PERIPHERAL(SSD1306_DATA), GET_INDEX(SSD1306_DATA));
	for(uint16_t i = 0x00; i < 0xFFFF; i++);
	GPIO_Set(GET_PERIPHERAL(SSD1306_DATA), GET_INDEX(SSD1306_DATA));
}

void Display_SwitchDisplay(const Bool_t Enable)
{
	if(Enable == TRUE)
	{
		SSD1306_WriteCommand(SSD1306_CMD_DISPLAY_ON);
	}
	else
	{
		SSD1306_WriteCommand(SSD1306_CMD_DISPLAY_OFF);
	}
}

void Display_SwitchInvert(const Bool_t Enable)
{
	if(Enable == TRUE)
	{
		SSD1306_WriteCommand(SSD1306_CMD_INVERT_ON);
	}
	else
	{
		SSD1306_WriteCommand(SSD1306_CMD_INVERT_OFF);
	}
}

void Display_SetContrast(const uint8_t Contrast)
{
	SSD1306_WriteCommand(SSD1306_CMD_CONTRAST);
	SSD1306_WriteCommand(Contrast);
}