/*
 * Socket.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Ethernet service for AVR devices.

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

/** @file Services/Ethernet/Socket.h
 *  @brief AVR Ethernet service.
 *
 *  This contains the prototypes and definitions for the Ethernet service.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef SOCKET_H_
#define SOCKET_H_

 #include "Common/Common.h"

 #if(defined USE_W5100)
	 #include "Peripheral/W5100/W5100.h"

	 typedef W5100_Config_t EthernetConfig_t;
	 typedef W5100_ErrorCode_t EthernetError_t;
 #endif

 /** @brief		Initialize the sockets and the underlaying Ethernet controller.
  *  @return	Error code
  */
 EthernetError_t Socket_Init(void);

#endif /* SOCKET_H_ */