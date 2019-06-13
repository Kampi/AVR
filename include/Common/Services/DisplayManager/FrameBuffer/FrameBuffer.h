/*
 * FrameBuffer.h
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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */ 

/** @file Common/Services/DisplayManager/FrameBuffer/FrameBuffer.h
 *  @brief Generic frame buffer for displays without readable display memory.
 *
 *  @author Daniel Kampert
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

 #include "Common/Common.h"

 /** @brief			Initialize a new frame buffer.
  *  @param Width	Width of each page in pixel
  *  @param Height	Height of each page in pixel
  *  @param Pages	Page count
  *  @param Buffer	Pointer to memory location for data
  */
 void FrameBuffer_Init(const uint8_t Width, const uint8_t Height, const uint8_t Pages, uint8_t* Buffer);

 /** @brief			Store a byte in the frame buffer.
  *  @param Page	Display page
  *  @param Column	Display column
  *  @param Data	Data byte
  */
 void FrameBuffer_WriteByte(const uint8_t Page, const uint8_t Column, const uint8_t Data);

 /** @brief			Read a stored byte from the frame buffer.
  *  @param Page	Display page
  *  @param Column	Display column
  *  @return		Data byte
  */
 uint8_t FrameBuffer_ReadByte(const uint8_t Page, const uint8_t Column);

 /** @brief			Store a page in the frame buffer.
  *  @param Page	Display page
  *  @param Column	Display column
  *  @param Length	Byte count
  *  @param Data	Pointer to data
  */
 void FrameBuffer_WritePage(const uint8_t Page, const uint8_t Column, const uint8_t Length, const uint8_t* Data);

 /** @brief			Read a page from the frame buffer.
  *  @param Page	Display page
  *  @param Column	Display column
  *  @param Length	Byte count
  *  @param Data	Pointer to data
  */
 void FrameBuffer_ReadPage(const uint8_t Page, const uint8_t Column, const uint8_t Length, uint8_t* Data);

#endif /* FRAMEBUFFER_H_ */