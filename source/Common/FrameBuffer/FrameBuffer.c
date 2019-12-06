/*
 * FrameBuffer.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Generic frame buffer for displays without readable display memory.

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

/** @file Common/FrameBuffer/FrameBuffer.c
 *  @brief Generic frame buffer for displays without readable display memory.
 *
 *  This contains the implementation of the frame buffer.
 *
 *  @author Daniel Kampert
 */

#include "Common/FrameBuffer/FrameBuffer.h"

static uint8_t __Width;
static uint8_t __Height;
static uint8_t __Pages;
static uint8_t* __Buffer;

void FrameBuffer_Init(const uint8_t Width, const uint8_t Height, const uint8_t Pages, uint8_t* Buffer)
{
	__Width = Width;
	__Height = Height;
	__Pages = Pages;
	__Buffer = Buffer;
}

void FrameBuffer_WriteByte(const uint8_t Page, const uint8_t Column, const uint8_t Data)
{
	*(__Buffer + (Page * __Width) + Column) = Data;
}

uint8_t FrameBuffer_ReadByte(const uint8_t Page, const uint8_t Column)
{
	return *(__Buffer + (Page * __Width) + Column);
}

void FrameBuffer_WritePage(const uint8_t Page, const uint8_t Column, const uint8_t Length, const uint8_t* Data)
{
	uint8_t* Start = __Buffer + ((Page * __Width) + Column);
	
	while(*Data)
	{
		*Start++ = *Data++;
	}
}

void FrameBuffer_ReadPage(const uint8_t Page, const uint8_t Column, const uint8_t Length, uint8_t* Data)
{
	uint8_t* Start = __Buffer + ((Page * __Width) + Column);
	
	while(*Data)
	{
		*Data++ = *Start++;
	}
}