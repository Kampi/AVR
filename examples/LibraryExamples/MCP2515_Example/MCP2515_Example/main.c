#include "Peripheral/__Rework__/MCP2515/MCP2515.h"

/*
	SPI configuration
*/
SPIM_Config_t Config_SPI = {
	.Device = &MCP2515_INTERFACE,
	.Mode = SPI_MODE_0,
};

/*
	MCP2515 interrupt configuration
*/
MCP2515_InterruptConfig_t InterruptConfig = {
	.Port = GET_PERIPHERAL(MCP2515_INT),
	.Pin = GET_INDEX(MCP2515_INT),
};

int main(void)
{
	MCP2515_Init(&Config_SPI, &InterruptConfig);
	
    while(1) 
    {
    }
}