/*
 * DisplayManager_Drawing.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Drawing functions for AVR display manager service.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file Services/DisplayManager/DisplayManager_Drawing.c
 *  @brief Drawing functions for AVR display manager service.
 *
 *  @author Daniel Kampert
 */

#include <stdlib.h>
#include "Services/DisplayManager/DisplayManager.h"

/*
	Using Bresenham algorithm to draw the line
*/
void DisplayManager_DrawLine(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const PixelMask_t Mask)
{
	int8_t e = 0;										// Error variable
	int8_t incr_x = 1;									// Increment direction x axis
	int8_t incr_y = 1;									// Increment direction y axis
	int8_t pdx = 0;										// Parallel step x axis
	int8_t pdy = 0;										// Parallel step y axis
	int8_t ddx = 0;										// Diagonal step x axis
	int8_t ddy = 0;										// Diagonal step y axis
	int8_t ds = 0;										// Delta fast direction
	int8_t df = 0;										// Delta slow direction
	int8_t x = x1;										// Current point x axis
	int8_t y = y1;										// Current point y axis
	
	// Get the distance between both points
	int8_t dx = x2 - x1;
	int8_t dy = y2 - y1;
	
	// Get increment direction of x
	if(dx < 0)
	{
		incr_x = -1;
		dx = -dx;
	}
	
	// Get increment direction of y
	if(dy < 0)
	{
		incr_y = -1;
		dy = -dy;
	}
	
	// Chose the biggest distance
	if(dx > dy)
	{
		pdx = incr_x;
		ddx = incr_x;
		ddy = incr_y;
		ds = dy;
		df = dx;		
	}
	else
	{
		pdy = incr_y;
		ddx = incr_x;
		ddy = incr_y;
		ds = dx;
		df = dy;
	}
	
	e = df >> 0x01;
	DisplayManager_DrawPixel(x, y, Mask);
	
	for(uint8_t i = 0x00; i < df; i++)
	{
		e -= ds;
		
		if(e < 0)
		{
			e += df;
			x += ddx;
			y += ddy;
		}
		else
		{
			x += pdx;
			y += pdy;
		}
		
		DisplayManager_DrawPixel(x, y, Mask);
	}
}

void DisplayManager_DrawHorizontalLine(const uint8_t x, const uint8_t y, const uint8_t Length, const uint8_t Width, const PixelMask_t Mask)
{
	if(((x + Length) > (DISPLAYMANAGER_LCD_WIDTH - 1)) || (y > (DISPLAYMANAGER_LCD_HEIGHT - 1)))
	{
		return;
	}

	for(uint8_t i = 0x00; i < Length; i++)
	{
		for(uint8_t j = 0x00; j < Width; j++)
		{
			DisplayManager_DrawPixel(x + i, y + j, Mask);
		}
	}
}

void DisplayManager_DrawVerticalLine(const uint8_t x, const uint8_t y, const uint8_t Length, const uint8_t Width, const PixelMask_t Mask)
{
	if(((y + Length) > (DISPLAYMANAGER_LCD_HEIGHT - 1)) || (x > (DISPLAYMANAGER_LCD_WIDTH - 1)))
	{
		return;
	}
	
	for(uint8_t i = 0x00; i < Length; i++)
	{
		for(uint8_t j = 0x00; j < Width; j++)
		{
			DisplayManager_DrawPixel(x + j, y + i, Mask);
		}
	}
}

void DisplayManager_DrawRect(const uint8_t x, const uint8_t y, const uint8_t Width, const uint8_t Height, const FillOptions_t Fill, const uint8_t LineWidth, const PixelMask_t Mask)
{
	// Check if the rectangle fits on the screen
	if(((x + Width) > (DISPLAYMANAGER_LCD_WIDTH - 1)) || ((y + Height) > (DISPLAYMANAGER_LCD_HEIGHT - 1)))
	{
		return;
	}

	DisplayManager_DrawHorizontalLine(x, y, Width, LineWidth, Mask);
	DisplayManager_DrawVerticalLine(x + Width - LineWidth, y, Height, LineWidth, Mask);
	DisplayManager_DrawHorizontalLine(x, y + Height - LineWidth, Width, LineWidth, Mask);
	DisplayManager_DrawVerticalLine(x, y, Height, LineWidth, Mask);
	
	// Fill the rectangle
	if(Fill == FILL_SOLID)
	{
		for(uint8_t i = 0x00; i < Height; i += LineWidth)
		{
			DisplayManager_DrawHorizontalLine(x, y + i, Width, LineWidth, Mask);
		}
	}
}

void DisplayManager_DrawCircle(const uint8_t x, const uint8_t y, const uint8_t Radius, const FillOptions_t Fill, const PixelMask_t Mask)
{
	// Check if the rectangle fits on the screen
	if(((x + Radius) > (DISPLAYMANAGER_LCD_WIDTH - 1)) || ((y + Radius) > (DISPLAYMANAGER_LCD_HEIGHT - 1)))
	{
		return;
	}
	
	// Skip drawing if radius is zero
	if(Radius == 0) 
	{
		DisplayManager_DrawPixel(x, y, Mask);
		return;
	}

	uint8_t OffsetX;
	uint8_t OffsetY;
	int16_t Error;

	OffsetX = 0;
	OffsetY = Radius;
	Error = 3 - (Radius << 0x01);

	while(OffsetX <= OffsetY)
	{
		if(Fill == FILL_NO)
		{
			DisplayManager_DrawPixel(x + OffsetY, y - OffsetX, Mask);
			DisplayManager_DrawPixel(x + OffsetX, y - OffsetY, Mask);
			DisplayManager_DrawPixel(x - OffsetX, y - OffsetY, Mask);
			DisplayManager_DrawPixel(x - OffsetY, y - OffsetX, Mask);
			DisplayManager_DrawPixel(x - OffsetY, y + OffsetX, Mask);
			DisplayManager_DrawPixel(x - OffsetX, y + OffsetY, Mask);
			DisplayManager_DrawPixel(x + OffsetX, y + OffsetY, Mask);
			DisplayManager_DrawPixel(x + OffsetY, y + OffsetX, Mask);
		}
		else
		{
			DisplayManager_DrawVerticalLine(x + OffsetY, y - OffsetX, OffsetX + 1, 1, Mask);
			DisplayManager_DrawVerticalLine(x + OffsetX, y - OffsetY, OffsetY + 1, 1, Mask);
			DisplayManager_DrawVerticalLine(x - OffsetY, y - OffsetX, OffsetX + 1, 1, Mask);
			DisplayManager_DrawVerticalLine(x - OffsetX, y - OffsetY, OffsetY + 1, 1, Mask);
			DisplayManager_DrawVerticalLine(x - OffsetY, y, OffsetX + 1, 1, Mask);
			DisplayManager_DrawVerticalLine(x - OffsetX, y, OffsetY + 1, 1, Mask);
			DisplayManager_DrawVerticalLine(x + OffsetY, y, OffsetX + 1, 1, Mask);
			DisplayManager_DrawVerticalLine(x + OffsetX, y, OffsetY + 1, 1, Mask);
		}

		if(Error < 0x00) 
		{
			Error += ((OffsetX << 0x02) + 0x06);
		} 
		else 
		{
			Error += (((OffsetX - OffsetY) << 0x02) + 0x0A);
			--OffsetY;
		}

		++OffsetX;
	}
}

void DisplayManager_DrawCircleSegment(const uint8_t x, const uint8_t y, const uint8_t Radius, const CircleSegment_t Segment, const FillOptions_t Fill, const PixelMask_t Mask)
{
	// Check if the rectangle fits on the screen
	if(((x + Radius) > (DISPLAYMANAGER_LCD_WIDTH - 1)) || ((y + Radius) > (DISPLAYMANAGER_LCD_HEIGHT - 1)))
	{
		return;
	}
	
	// Skip drawing if radius is zero
	if(Radius == 0)
	{
		DisplayManager_DrawPixel(x, y, Mask);
		return;
	}

	uint8_t OffsetX;
	uint8_t OffsetY;
	int16_t Error;

	OffsetX = 0x00;
	OffsetY = Radius;
	Error = 3 - (Radius << 0x01);

	// Draw the outlines
	if(Fill == FILL_NO)
	{
		if(Segment & CIRCLE_SEGMENT_QUADRANT1)
		{
			DisplayManager_DrawLine(x, y, x + Radius, y, Mask);
			DisplayManager_DrawLine(x, y, x, y - Radius, Mask);
		}
		
		if(Segment & CIRCLE_SEGMENT_QUADRANT2)
		{
			DisplayManager_DrawLine(x, y, x - Radius, y, Mask);
			DisplayManager_DrawLine(x, y, x, y - Radius, Mask);
		}
		
		if(Segment & CIRCLE_SEGMENT_QUADRANT3)
		{
			DisplayManager_DrawLine(x, y, x, y + Radius, Mask);
			DisplayManager_DrawLine(x, y, x - Radius, y, Mask);
		}
		
		if(Segment & CIRCLE_SEGMENT_QUADRANT4)
		{
			DisplayManager_DrawLine(x, y, x, y + Radius, Mask);
			DisplayManager_DrawLine(x, y, x + Radius, y, Mask);
		}
	}

	while(OffsetX <= OffsetY)
	{
		if(Fill == FILL_NO)
		{
			if(Segment & CIRCLE_SEGMENT_QUADRANT1)
			{
				DisplayManager_DrawPixel(x + OffsetY, y - OffsetX, Mask);
				DisplayManager_DrawPixel(x + OffsetX, y - OffsetY, Mask);
			}
			
			if(Segment & CIRCLE_SEGMENT_QUADRANT2)
			{
				DisplayManager_DrawPixel(x - OffsetX, y - OffsetY, Mask);
				DisplayManager_DrawPixel(x - OffsetY, y - OffsetX, Mask);
			}
			
			if(Segment & CIRCLE_SEGMENT_QUADRANT3)
			{
				DisplayManager_DrawPixel(x - OffsetY, y + OffsetX, Mask);
				DisplayManager_DrawPixel(x - OffsetX, y + OffsetY, Mask);
			}
			
			if(Segment & CIRCLE_SEGMENT_QUADRANT4)
			{
				DisplayManager_DrawPixel(x + OffsetX, y + OffsetY, Mask);
				DisplayManager_DrawPixel(x + OffsetY, y + OffsetX, Mask);
			}
		}
		else
		{
			if(Segment & CIRCLE_SEGMENT_QUADRANT1)
			{
				DisplayManager_DrawVerticalLine(x + OffsetY, y - OffsetX, OffsetX + 1, 1, Mask);
				DisplayManager_DrawVerticalLine(x + OffsetX, y - OffsetY, OffsetY + 1, 1, Mask);
			}
				
			if(Segment & CIRCLE_SEGMENT_QUADRANT2)
			{
				DisplayManager_DrawVerticalLine(x - OffsetY, y - OffsetX, OffsetX + 1, 1, Mask);
				DisplayManager_DrawVerticalLine(x - OffsetX, y - OffsetY, OffsetY + 1, 1, Mask);
			}
			
			if(Segment & CIRCLE_SEGMENT_QUADRANT3)
			{
				DisplayManager_DrawVerticalLine(x - OffsetY, y, OffsetX + 1, 1, Mask);
				DisplayManager_DrawVerticalLine(x - OffsetX, y, OffsetY + 1, 1, Mask);
			}
			
			if(Segment & CIRCLE_SEGMENT_QUADRANT4)
			{
				DisplayManager_DrawVerticalLine(x + OffsetY, y, OffsetX + 1, 1, Mask);
				DisplayManager_DrawVerticalLine(x + OffsetX, y, OffsetY + 1, 1, Mask);
			}
		}

		if(Error < 0x00)
		{
			Error += ((OffsetX << 0x02) + 0x06);
		}
		else
		{
			Error += (((OffsetX - OffsetY) << 0x02) + 0x0A);
			--OffsetY;
		}

		++OffsetX;
	}
}

void DisplayManager_DrawBitmap(const uint8_t x, const uint8_t y, const Bitmap_t* Bitmap)
{
	uint8_t i = 0x00;
	uint8_t Height = 0x00;

	if(((x + Bitmap->Width) > (DISPLAYMANAGER_LCD_WIDTH - 1)) || ((y + Bitmap->Height) > (DISPLAYMANAGER_LCD_HEIGHT - 1)))
	{
		return;
	}

	while(Height < Bitmap->Height)
	{
		for(uint8_t j = 0x00; j < Bitmap->Width; j++)
		{
			uint8_t Byte = 0x00;
			
			if(Bitmap->Type == MEMORY_PROGMEM)
			{
				Byte = pgm_read_byte(Bitmap->Data.Flash + (i * Bitmap->Width) + j);
			}
			else if(Bitmap->Type == MEMORY_RAM)
			{
				Byte = *(Bitmap->Data.RAM + (i * Bitmap->Width) + j);
			}
				
			for(uint8_t k = 0x00; k < 0x08; k++)
			{
				// Draw a pixel if the current LSB is set
				if(Byte & 0x01)
				{
					DisplayManager_DrawPixel(x + j, y + i * DISPLAYMANAGER_LCD_PIXEL_PER_BYTE + k, PIXELMASK_SET);
				}
						
				// Shift the mask
				Byte = Byte >> 0x01;
			}
		}
		
		if((Height + DISPLAYMANAGER_LCD_PIXEL_PER_BYTE) < Bitmap->Height)
		{
			Height += DISPLAYMANAGER_LCD_PIXEL_PER_BYTE;
		}
		else
		{
			Height += Bitmap->Height - Height;
		}
		
		i++;
	}
}

uint8_t DisplayManager_DrawString(const uint8_t x, const uint8_t y, const char* String)
{
	uint8_t x_temp = x;
	uint8_t y_temp = y;
	
	while(*String)
	{
		// Get the character from string
		uint8_t ASCII = *String++;
		
		// Check if character is valid
		if((ASCII > 31) && (ASCII < 127))
		{
			// Get the font layout for the character
			const uint8_t* Layout = FontTable[ASCII - 32];
			
			uint8_t Columns = *Layout++;

			// Add line break
			if((x_temp + Columns) > DISPLAYMANAGER_LCD_WIDTH)
			{
				x_temp = 0x00;
				y_temp += FONT_HEIGHT;
			}

			// Loop over the columns layout (first item is the column count)
			for(uint8_t i = 0x00; i < Columns; i++)
			{
				// Get the bit mask
				uint8_t Mask = *Layout++;
				
				for(uint8_t j = 0x00; j < 0x08; j++)
				{
					// Draw a pixel if the current LSB is set
					if(Mask & 0x01)
					{
						DisplayManager_DrawPixel(x_temp, y_temp + j, PIXELMASK_SET);
					}
					
					// Shift the mask
					Mask = Mask >> 0x01;
				}
				
				// Next column
				x_temp++;
			}
			
			// Add one pixel spacing between the characters
			x_temp++;
		}
	}
	
	return x_temp;
}

void DisplayManager_InitGraph(const uint8_t x, const uint8_t y, const char* Title, const uint16_t Max, Graph_t* Graph)
{
	Graph->x = x;
	Graph->y = y;
	Graph->MaxValue = Max;
	Graph->Start = DisplayManager_DrawString(Graph->x, Graph->y, Title) + 3;
	DisplayManager_DrawVerticalLine(Graph->Start, Graph->y, 8, 1, PIXELMASK_SET);
	Graph->Start += 3;
	Graph->MaxLength = DISPLAYMANAGER_LCD_WIDTH - Graph->Start - 1;
	Graph->Scale = (float)Graph->MaxLength / (float)Graph->MaxValue;
}

void DisplayManager_UpdateGraph(Graph_t* Graph, const uint16_t Value)
{	
	// Get the length of the new graph in pixel
	uint8_t NewLength = Graph->Scale * Value;
	
	if(Graph->Lenght == 0)
	{
		Graph->Lenght = NewLength;
		DisplayManager_DrawHorizontalLine(Graph->Start, Graph->y + 2, Graph->Lenght, 3, PIXELMASK_SET);
	}
	else
	{
		int8_t Difference = NewLength - Graph->Lenght;
		
		if(Difference >= 0)
		{
			DisplayManager_DrawHorizontalLine(Graph->Start + Graph->Lenght, Graph->y + 2, Difference, 3, PIXELMASK_SET);
		}
		else
		{
			DisplayManager_DrawHorizontalLine(Difference + Graph->Start + Graph->Lenght, Graph->y + 2, abs(Difference), 3, PIXELMASK_CLEAR);
		}
		
		Graph->Lenght = NewLength;
	}
}