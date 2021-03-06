#include "Peripheral/DS1307/DS1307.h"

void Callback(Time_t Time);

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Bitrate = DS1307_CLOCK,
	.EnableInterruptSupport = false,
};

/*
	RTC time object
*/
Time_t CurrentTime = {
	.Second = 0,
	.Minute = 18,
	.Hour = 13,
	.DayOfWeek = MONDAY,
	.Day = 16,
	.Month = 9,
	.Year = 19,
	.HourMode = MODE_12_HOUR,
};

/*
	RTC configuration
*/
DS1307_InterruptConfig_t Config_RTC_Interrupts = {
	.Freq = DS1307_SQW_1HZ,
	.Port = GET_PERIPHERAL(DS1307_INT),
	.Pin = GET_INDEX(DS1307_INT),
	.Channel = DS1307_INT_CHANNEL,
	.Sense = GPIO_SENSE_FALLING,
	.Callback = Callback,
};

int main(void)
{
	// Initialize the DS1307 RTC
	DS1307_Init(&Config_I2CM, &CurrentTime, &Config_RTC_Interrupts);

	// Enable global interrupts
	EnableGlobalInterrupts();
	
    while(1) 
    {
    }
}

void Callback(Time_t Time)
{
}