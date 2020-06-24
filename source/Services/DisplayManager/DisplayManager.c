/*
 * DisplayManager.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Display manager service.

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

/** @file Services/DisplayManager/DisplayManager.c
 *  @brief Display manager service.
 *
 *  This contains the implementation of the display manager service.
 *
 *  @author Daniel Kampert
 */

#include "Services/DisplayManager/DisplayManager.h"

/*
	Interface configuration
*/
#if((defined USE_SSD1306) || (defined USE_ST7565R))
	static SPIM_Config_t DisplayConfig = {
		.SPIClock = DISPLAY_CLOCK,
		.DataOrder = SPI_DATAORDER_MSB_FIRST,
		.Mode = SPI_MODE_0,
		.Device = &CONCAT(DISPLAY_INTERFACE)
	};
#endif

static uint8_t __DisplayMgrBuffer[DISPLAYMANAGER_LCD_FRAMEBUFFER_SIZE];

void DisplayManager_Init(void)
{
	Display_Init(&DisplayConfig);

	// Initialize the frame buffer
	FrameBuffer_Init(DISPLAYMANAGER_LCD_WIDTH, DISPLAYMANAGER_LCD_HEIGHT / DISPLAYMANAGER_LCD_PIXEL_PER_BYTE, DISPLAYMANAGER_LCD_PAGES, __DisplayMgrBuffer);

	Display_SetStartLine(0);

	DisplayManager_Clear();
}

void DisplayManager_Clear(void)
{
	for(uint8_t Page = 0x00; Page < DISPLAYMANAGER_LCD_PAGES; Page++)
	{
		for(uint8_t Column = 0x00; Column < DISPLAYMANAGER_LCD_WIDTH; Column++)
		{
			DisplayManager_WriteByte(Page, Column, 0x00);
		}
	}
}

void DisplayManager_ClearLine(const uint8_t Line)
{
	uint8_t Page = Line & (DISPLAYMANAGER_LCD_PAGES - 0x01);

	for(uint8_t Column = 0x00; Column < DISPLAYMANAGER_LCD_WIDTH; Column++)
	{
		DisplayManager_WriteByte(Page, Column, 0x00);
	}
}

void DisplayManager_ClearColumn(const uint8_t Column)
{
	uint8_t Column_Temp = Column & (DISPLAYMANAGER_LCD_WIDTH - 0x01);

	for(uint8_t Page = 0x00; Page < DISPLAYMANAGER_LCD_PAGES; Page++)
	{
		DisplayManager_WriteByte(Page, Column_Temp, 0x00);
	}
}

void DisplayManager_WriteByte(const uint8_t Page, const uint8_t Column, const uint8_t Data)
{
	FrameBuffer_WriteByte(Page, Column, Data);

	Display_SetPage(Page);
	Display_SetColumn(Column);
	Display_WriteData(Data);
}

uint8_t DisplayManager_ReadByte(const uint8_t Page, const uint8_t Column)
{
	return FrameBuffer_ReadByte(Page, Column);
}