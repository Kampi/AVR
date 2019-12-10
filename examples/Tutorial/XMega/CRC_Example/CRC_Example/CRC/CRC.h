/*
 * CRC.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: CRC driver for XMega.

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

#ifndef CRC_H_
#define CRC_H_

 #include <avr/io.h>

 void CRC_Reset(void);
 void CRC_Enable(void);
 void CRC_Disable(void);
 uint32_t CRC_GetChecksum(void);
 uint32_t CRC_GetResult(void);
 uint32_t CRC_Data(uint8_t* Data, uint32_t Length, uint8_t CRC32);
 uint32_t CRC_Flash(uint32_t Start, uint32_t Length);
 void CRC_DMAStart(uint8_t Channel, uint8_t CRC32);
 uint32_t CRC_DMAStop(void);

#endif /* CRC_H_ */