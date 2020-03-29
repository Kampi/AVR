/*
 * LogoUSB.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: USB logo as bitmap file.

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

/** @file Bitmaps/LogoUSB.c
 *  @brief USB logo as bitmap file.
 *
 *  @author Daniel Kampert
 */

#include "Common/common.h"

 /** 
  * USB logo
  *		Width: 24
  *		Height: 16
  */
const uint8_t PictureUSB[] __attribute__((__progmem__)) =  {
	0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x80, 0xc0, 0x60, 0x20, 0x20,
	0x70, 0x70, 0x20, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x07, 0x07, 0x07, 0x03, 0x01, 0x01, 0x01, 0x03, 0x07, 0x0d, 0x09, 0x1d,
	0x1d, 0x1d, 0x01, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00,
};