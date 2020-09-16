/*
 * DisplayManager.c
 *
 *  Copyright (C) Daniel Kampert, 2018
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

extern SPIM_Config_t _DisplayManagerConfig;

static uint8_t _DisplayMgrBuffer[DISPLAYMANAGER_LCD_FRAMEBUFFER_SIZE];

/** @brief			Write a single byte to the display.
 *  @param Page		Display page
 *  @param Column	Display column
 *  @param Data		Display data
 */
static void DisplayManager_WriteByte(const uint8_t Page, const uint8_t Column, const uint8_t Data)
{
	FrameBuffer_WriteByte(Page, Column, Data);

	Display_SetPage(Page);
	Display_SetColumn(Column);
	Display_WriteData(Data);
}

/** @brief			Read a single byte from the display.
 *  @param Page		Display page
 *  @param Column	Display column
 *  @return			Display data
 */
static uint8_t DisplayManager_ReadByte(const uint8_t Page, const uint8_t Column)
{
	return FrameBuffer_ReadByte(Page, Column);
}

void DisplayManager_Init(void)
{
	Display_Init(&_DisplayManagerConfig);

	// Initialize the frame buffer
	FrameBuffer_Init(DISPLAYMANAGER_LCD_WIDTH, DISPLAYMANAGER_LCD_HEIGHT / DISPLAYMANAGER_LCD_PIXEL_PER_BYTE, DISPLAYMANAGER_LCD_PAGES, _DisplayMgrBuffer);

	Display_SetStartLine(0);

	DisplayManager_Clear();
}

void DisplayManager_SwitchBacklight(const bool Enable)
{
	Display_SwitchBacklight(Enable);	
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

void DisplayManager_DrawPixel(const uint8_t x, const uint8_t y, const PixelMask_t Mask)
{
	// Check if the coordinates are outside of the screen
	if((x > (DISPLAYMANAGER_LCD_WIDTH - 1)) || (y > (DISPLAYMANAGER_LCD_HEIGHT - 1)))
	{
		return;
	}

	// Get the byte for the chosen pixel
	uint8_t Page = y / DISPLAYMANAGER_LCD_PIXEL_PER_BYTE;
	uint8_t ByteMask = (0x01 << (y - (Page * DISPLAYMANAGER_LCD_PIXEL_PER_BYTE)));

	// Read the byte from the frame buffer
	uint8_t Byte = DisplayManager_ReadByte(Page, x);

	// Set the pixel
	if(Mask == PIXELMASK_SET)
	{
		Byte |= ByteMask;
	}
	else
	{
		Byte &= ~ByteMask;
	}

	// Write the new value to the display
	DisplayManager_WriteByte(Page, x, Byte);
}