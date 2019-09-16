#include "Peripheral/__Rework__/DS1307/DS1307.h"

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = &DS1307_INTERFACE,
	.Bitrate = DS1307_CLOCK,
	.EnableInterruptSupport = FALSE,
};

/*
	RTC time object
*/
Time_t CurrentTime = {
	.Second = 0,
	.Minute = 18,
	.Hour = 12,
	.DayOfWeek = MONDAY,
	.Day = 16,
	.Month = 9,
	.Year = 19
};

/*
	RTC configuration
*/
DS1307_Config_t Config_RTC = {
	.Time = &CurrentTime,
	.Mode = MODE_12_HOUR
};

int main(void)
{
	DS1307_Init(&Config_I2CM, &Config_RTC);

	// Wait a bit
	for(uint16_t i = 0x00; i < 0xFFFF; i++);
	
	DS1307_GetTime(&CurrentTime);
	
    while(1) 
    {
    }
}