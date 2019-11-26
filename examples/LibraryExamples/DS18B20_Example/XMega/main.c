/*
 * main.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 * File info: DS18B20 example for XMega.

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

/** @file main.c
 *  @brief DS18B20 example for XMega.
 *
 *  Software for the XMega DS18B20 example from
 *  https://www.kampis-elektroecke.de/2019/11/1-wire-fuer-den-avr/
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/DS18B20/DS18B20.h"
#include "Common/Services/ConsoleService/ConsoleService.h"

#include <stdlib.h>

#define DS18B20_ALARM_UPPER_LIMIT					75
#define DS18B20_ALARM_LOWER_LIMIT					0
#define DS18B20_BUS_DEVICES							0x03

OneWire_Error_t ErrorCode;

uint8_t Devices;
OneWire_ROM_t DS18B20_ROM[DS18B20_BUS_DEVICES];
DS18B20_Scratchpad_t Scratchpad;
OneWire_PowerState_t PowerState;
double Temperature;

uint8_t AlarmDevices;
OneWire_ROM_t DS18B20_AlarmROM[DS18B20_BUS_DEVICES];

int main(void)
{	
	/*
		Initialize the console service.
	*/
	ConsoleService_Init();

	printf("---DS18B20 Library example---\n\r");

	if(DS18B20_Init() == ONEWIRE_NO_ERROR)
	{
		// Get the devices
		printf("[INFO] Looking for sensors...");
		if(DS18B20_GetDevices(&Devices, DS18B20_BUS_DEVICES, DS18B20_ROM) == ONEWIRE_NO_ERROR)		
		{
			printf(" %d sensors found!\n\r", Devices);

			// Loop through each sensor
			for(uint8_t i = 0x00; i < Devices; i++)
			{
				printf("[INFO] Sensor: %d\n\r", i + 1);
				printf("	Serial: 0x");
				
				for(uint8_t j = 0x00; j < 0x06; j++)
				{
					printf("%x", DS18B20_ROM[i].SerialNumber[j]);
				}
				
				printf("\n\r");
	
				// Set the alarm limit and start a new measurement
				if((DS18B20_SetAlarm(&DS18B20_ROM[i], DS18B20_ALARM_UPPER_LIMIT, DS18B20_ALARM_LOWER_LIMIT) | DS18B20_Measure(&DS18B20_ROM[i], DS18B20_RESOLUTION_11, &Temperature)) == ONEWIRE_NO_ERROR)
				{
					char Buffer[8];
					dtostrf(Temperature, 4,2, Buffer);
					printf("	Temperature: %s degree Celsius\n\r", Buffer);
				}
			}

			// Get the alarm devices
			printf("[INFO] Looking for alarm devices...");
			if(DS18B20_GetAlarmDevices(&AlarmDevices, DS18B20_BUS_DEVICES, DS18B20_AlarmROM) == ONEWIRE_NO_ERROR)
			{
				printf(" %d alarm devices found!\n\r", AlarmDevices);
				
				for(uint8_t i = 0x00; i < Devices; i++)
				{
					printf("[INFO] Sensor: %d\n\r", i + 1);
					printf("	Serial: 0x");
					
					for(uint8_t j = 0x00; j < 0x06; j++)
					{
						printf("%x", DS18B20_ROM[i].SerialNumber[j]);
					}
					
					printf("\n\r");
				}
			}
			
			if(AlarmDevices == 0x00)
			{
				printf(" no alarm devices found!\n\r");
			}
		}
		else
		{
			printf("[ERROR] No devices found!");
		}
	}
	else
	{
		printf("[ERROR] 1-Wire error!");
	}

    while(1)
    {
    }
}