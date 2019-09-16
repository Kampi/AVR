#include "Peripheral/__Rework__/DS1307/DS1307.h"

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = &DS1307_INTERFACE,
	.BitRate = DS1307_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

int main(void)
{
	DS1307_Init(&Config_I2CM, NULL);
	
    while(1) 
    {
    }
}