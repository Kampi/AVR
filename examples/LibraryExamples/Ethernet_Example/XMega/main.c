/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: W5100 Ethernet example for XMega.

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

/** @file main.c
 *  @brief W5100 Ethernet example for XMega.
 *
 *  Software for the XMega W5100 example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include <string.h>
#include "Services/Ethernet/Socket.h"

#define ETHERNET_BUFFER						2048

uint8_t Rx_Buffer[ETHERNET_BUFFER];

const char HTTP[]  =	"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"
						"<html>\r\n"
							"<body>\r\n"
								"<title>Title</title>\r\n"
								"<p>Hello world</p>\r\n"
							"</body>\r\n"
						"</html>\r\n";

int main(void)
{
	Socket_Init();

    while(1) 
    {
		W5100_Status_t Status;
		W5100_GetState(0, &Status);

		switch(Status)
		{
			case W5100_SOCK_CLOSED:
			{
				if(W5100_Open(0, W5100_PROT_TCP, ETHERNET_PORT, W5100_MEM_2K, W5100_MEM_2K, ETHERNET_TIMEOUT) == W5100_NO_ERROR)
				{
					W5100_Listen(0, ETHERNET_TIMEOUT);
				}

				break;
			}
			case W5100_SOCK_ESTABLISHED:
			{
				uint16_t Rx_Bytes;

				if(W5100_GetBytes(0, &Rx_Bytes) == W5100_NO_ERROR)
				{
					if(Rx_Bytes)
					{
						W5100_Receive(0, Rx_Buffer, Rx_Bytes);
						W5100_Send(0, (uint8_t*)HTTP, strlen((char*)HTTP), ETHERNET_TIMEOUT);
					}
					else
					{
						
					}
				}

				W5100_Disconnect(0, ETHERNET_TIMEOUT);

				break;
			}
			case W5100_SOCK_FIN_WAIT:
			case W5100_SOCK_CLOSING:
			case W5100_SOCK_TIME_WAIT:
			case W5100_SOCK_CLOSE_WAIT:
			case W5100_SOCK_LAST_ACK:
			{
				W5100_Close(0, ETHERNET_TIMEOUT);

				break;
			}
		}
    }
}