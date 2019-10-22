/*
 * DisplayManager.h
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

/** @file Common/Services/DisplayManager/DisplayManager.h
 *  @brief AVR display manager service.
 *
 *  This contains the prototypes and definitions for the display manager.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef DISPLAYMANAGER_H_
#define DISPLAYMANAGER_H_
 
 #include "Common/Common.h"
 
 #if(defined USE_SSD1306)
	 #include "Peripheral/SSD1306/SSD1306.h"
	 typedef SPIM_Config_t DisplayInterface_t;
 #elif(defined USE_ST7565R)
	 #include "Peripheral/ST7565R/ST7565R.h"
	 
	 typedef SPIM_Config_t DisplayInterface_t;
 #endif

 #include "Font/Font.h"
 #include "Framebuffer/Framebuffer.h"

 #if((!defined DISPLAY_WIDTH) | (!defined DISPLAY_HEIGHT) | (!DISPLAY_PIXEL_PER_BYTE))
	#error "Invalid display configuration. Please check your configuration file!"
 #endif

 #define DISPLAYMANAGER_LCD_WIDTH					DISPLAY_WIDTH																					/**< Width of the display in pixel*/
 #define DISPLAYMANAGER_LCD_HEIGHT					DISPLAY_HEIGHT																					/**< Height of the display in pixel */
 #define DISPLAYMANAGER_LCD_PIXEL_PER_BYTE			DISPLAY_PIXEL_PER_BYTE																			/**< Bytes per pixel */
 #define DISPLAYMANAGER_LCD_PAGES					(DISPLAYMANAGER_LCD_HEIGHT / DISPLAYMANAGER_LCD_PIXEL_PER_BYTE)									/**< Page count of the display, based on the display height and the bytes per pixel */
 #define DISPLAYMANAGER_LCD_FRAMEBUFFER_SIZE		((DISPLAYMANAGER_LCD_WIDTH * DISPLAYMANAGER_LCD_HEIGHT) / DISPLAYMANAGER_LCD_PIXEL_PER_BYTE)	/**< Size of the display frame buffer */

 /*
	Function prototypes used by the display manager. The display driver must implement this functions.
 */

 /** @brief			Initialize the display and the interface.
  *  @param Config	Pointer to display configuration object.
  */
 void Display_Init(DisplayInterface_t* Config);
 
 /** @brief	Reset the display.
  */
 void Display_Reset(void);
 
 /** @brief			Enable/Disable the display back light.
  *  @param Enable	#TRUE to enable the back light.
  */
 void Display_SwitchDisplay(const Bool_t Enable);

 /** @brief			Write data to the display.
  *  @param Data	Data byte
  */
 void Display_WriteData(const uint8_t Data);
 
 /** @brief		Set the display page.
  *  @param Page Display page
  */
 void Display_SetPage(const uint8_t Page);
 
 /** @brief			Select the display column.
  *  @param Column	Display column
  */
 void Display_SetColumn(const uint8_t Column);
 
 /** @brief			Set the start line of the display.
  *  @param Line	Start line
  */
 void Display_SetStartLine(const uint8_t Line);

 /**
  * Fill options for drawing objects
  */
 typedef enum
 {
	 FILL_NO = 0x00,							/**< No filling */
	 FILL_SOLID = 0x01,							/**< Fill object */
 } FillOptions_t;

 /**
  * Pixel mask for drawing
  */
 typedef enum
 {
	 PIXELMASK_SET = 0x00,						/**< Set the pixel */
	 PIXELMASK_CLEAR = 0x01,					/**< Clear the pixel */
 } PixelMask_t;

 /**
  * Circle segments
  */
 typedef enum
 {
	 CIRCLE_SEGMENT_QUADRANT1 = 0x01,			/**< Quadrant 1 (positive x, positive y) */
	 CIRCLE_SEGMENT_QUADRANT2 = 0x02,			/**< Quadrant 2 (negative x, positive y) */
	 CIRCLE_SEGMENT_QUADRANT3 = 0x04,			/**< Quadrant 3 (negative x, negative y) */
	 CIRCLE_SEGMENT_QUADRANT4 = 0x08,			/**< Quadrant 4 (positive x, negative y) */
 } CircleSegment_t;

 /**
  * Graph object
  */
 typedef struct 
 {
	 uint8_t x;									/**< x coordinate of the graph */
	 uint8_t y;									/**< y coordinate of the graph */
	 uint8_t Start;								/**< Start coordinate of the graph bar */
	 uint16_t MaxValue;							/**< Max value of the graph */
	 uint8_t MaxLength;							/**< Max length of the graph */
	 float Scale;								/**< Graph scale in steps per pixel */
	 uint8_t Lenght;							/**< Length of the graph */
 } Graph_t;

 /**
  * Bitmap object
  */
 typedef struct 
 {
	 uint8_t Width;								/**< Width of the bitmap */
	 uint8_t Height;							/**< Height of the bitmap */
	 MemoryType_t Type;							/**< Type of the bitmap */
	 union
	 {
		 const uint8_t* RAM;
		 const uint8_t* Flash; 
	 } Data;									/**< Bitmap data */
 } Bitmap_t;

 /** @brief	Initialize the display manager service.
  */
 void DisplayManager_Init(void);

 /** @brief	Clear the display.
  */
 void DisplayManager_Clear(void);

 /** @brief			Clear a single display line.
  *  @param Line	Line number
  */
 void DisplayManager_ClearLine(const uint8_t Line);

 /** @brief			Clear a single display column.
  *  @param Column	Column number
  */
 void DisplayManager_ClearColumn(const uint8_t Column);

 /** @brief			Write a single byte to the display.
  *  @param Page	Display page
  *  @param Column	Display column
  *  @param Data	Display data
  */
 void DisplayManager_WriteByte(const uint8_t Page, const uint8_t Column, const uint8_t Data);

 /** @brief			Read a single byte from the display.
  *  @param Page	Display page
  *  @param Column	Display column
  *  @return		Display data
  */
 uint8_t DisplayManager_ReadByte(const uint8_t Page, const uint8_t Column);

 /** @brief		Draw a single pixel on the display.
  *  @param x		x coordinate
  *  @param y		y coordinate
  *  @param Mask	Pixel mask
  */
 void DisplayManager_DrawPixel(const uint8_t x, const uint8_t y, const PixelMask_t Mask);

 /** @brief			Draw a line between two points.
  *  @param x1		Start coordinate (x direction)
  *  @param y1		Start coordinate (y direction)
  *  @param x2		End coordinate (x direction)
  *  @param y2		End coordinate (y direction)
  *  @param Mask	Pixel mask
  */
 void DisplayManager_DrawLine(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const PixelMask_t Mask);

 /** @brief			Draw a horizontal line between two points.
  *  @param x		Start coordinate (x direction)
  *  @param y		Start coordinate (y direction)
  *  @param Length	Length of the line
  *  @param Width	Line width
  *  @param Mask	Pixel mask
  */
 void DisplayManager_DrawHorizontalLine(const uint8_t x, const uint8_t y, const uint8_t Length, const uint8_t Width, const PixelMask_t Mask);

 /** @brief			Draw a horizontal line between two points.
  *  @param x		Start coordinate (x direction)
  *  @param y		Start coordinate (y direction)
  *  @param Length	Length of the line
  *  @param Width	Line width
  *  @param Mask	Pixel mask
  */
 void DisplayManager_DrawVerticalLine(const uint8_t x, const uint8_t y, const uint8_t Length, const uint8_t Width, const PixelMask_t Mask);

 /** @brief				Draw a rectangle.
  *  @param x			x coordinate of the upper left corner
  *  @param y			y coordinate of the upper left corner
  *  @param Width		Width of the rectangle
  *  @param Height		Height of the rectangle
  *  @param Fill		Fill options
  *  @param LineWidth	Line width
  *  @param Mask		Pixel mask
  */
 void DisplayManager_DrawRect(const uint8_t x, const uint8_t y, const uint8_t Width, const uint8_t Height, const FillOptions_t Fill, const uint8_t LineWidth, const PixelMask_t Mask);

 /** @brief			Draw a circle.
  *  @param x		x coordinate of the center
  *  @param y		y coordinate of the center
  *  @param Radius	Width of the rectangle
  *  @param Fill	Fill options
  *  @param Mask	Pixel mask
  */
 void DisplayManager_DrawCircle(const uint8_t x, const uint8_t y, const uint8_t Radius, const FillOptions_t Fill, const PixelMask_t Mask);

 /** @brief				Draw a circle segment.
  *  @param x			x coordinate of the center
  *  @param y			y coordinate of the center
  *  @param Radius		Width of the rectangle
  *  @param Segment		Circle segment
  *  @param Fill		Fill options
  *  @param Mask		Pixel mask
  */
 void DisplayManager_DrawCircleSegment(const uint8_t x, const uint8_t y, const uint8_t Radius, const CircleSegment_t Segment, const FillOptions_t Fill, const PixelMask_t Mask);

 /** @brief			Draw a bitmap on the display.
  *					NOTE: Use the python script https://gitlab.com/Kampi/Python/blob/master/Bitmap2Array.py to generate the array
  *  @param x		x coordinate
  *  @param y		y coordinate
  *  @param Bitmap	Pointer to bitmap object
  */
 void DisplayManager_DrawBitmap(const uint8_t x, const uint8_t y, const Bitmap_t* Bitmap);

 /** @brief			Draw a string on the display.
  *  @param x		x coordinate
  *  @param y		y coordinate
  *  @param String	Pointer to string
  *  @return		New cursor position
  */
 uint8_t DisplayManager_DrawString(const uint8_t x, const uint8_t y, const char* String);

 /** @brief			Initialize a new horizontal graph.
  *  @param x		x coordinate
  *  @param y		y coordinate
  *  @param Title	Graph title
  *  @param Max		Max value for graph
  *  @param Graph	Pointer to #Graph_t object
  */
 void DisplayManager_InitGraph(const uint8_t x, const uint8_t y, const char* Title, const uint16_t Max, Graph_t* Graph);
 
 /** @brief			Update a graph with a new value.
  *  @param Graph	Pointer to #Graph_t object
  *  @param Value	New graph value
  */
 void DisplayManager_UpdateGraph(Graph_t* Graph, const uint16_t Value);

#endif /* DISPLAYMANAGER_H_ */