#include "Peripheral/DS18B20/DS18B20.h"

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
	if(DS18B20_Init() == ONEWIRE_NO_ERROR)
	{
		if(DS18B20_GetDevices(&Devices, DS18B20_BUS_DEVICES, DS18B20_ROM) == ONEWIRE_NO_ERROR)
		{
			// Loop through each sensor
			for(uint8_t i = 0x00; i < Devices; i++)
			{
				// Set the alarm limit and start a new measurement
				if((DS18B20_SetAlarm(&DS18B20_ROM[i], DS18B20_ALARM_UPPER_LIMIT, DS18B20_ALARM_LOWER_LIMIT) | DS18B20_Measure(&DS18B20_ROM[i], DS18B20_RESOLUTION_11, &Temperature)) == ONEWIRE_NO_ERROR)
				{
				}
			}
		}
		else
		{
		}
	}
	else
	{
	}

    while(1) 
    {
    }
}