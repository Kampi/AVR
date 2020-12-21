/*
 * MassStorage.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: USB mass storage class.

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

/** @file Services/USB/Class/MassStorage/MassStorage.h
 *  @brief USB mass storage class.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef MASSSTORAGE_H_
#define MASSSTORAGE_H_

 #include "Services/USB/USB.h"
 
 #include "MassStorage_Common.h"

 /** @brief		Read the command block from the host from the data OUT endpoint.
  *				NOTE: You have so use #Endpoint_Select first to select the right endpoint!
  *  @param CBW	Pointer to #USB_MassStorage_CommandBlockWrapper_t object
  *  @return	#true when a valid command block has been read
  */
 bool USB_MassStorage_ReadCommandBlock(USB_MassStorage_CommandBlockWrapper_t* CBW);

#endif /* MASSSTORAGE_H_  */