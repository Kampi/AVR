#include "Peripheral/MCP2515/MCP2515.h"

#define MESSAGE_SIZE			4

void MCP2515_RxCallback(void);
void MCP2515_ErrorCallback();

/*
	SPI configuration
*/
SPIM_Config_t Config_SPI = {
	.Mode = SPI_MODE_0,
	.Prescaler = SPI_PRESCALER_64
};

/*
	MCP2515 configuration
*/
MCP2515_Config_t Config = {
	.EnableOneShot = FALSE,
	.EnableLoopBack = TRUE,
	.EnableRollover = FALSE,
	.Channel = MCP2515_INT_CHANNEL,
	.Port = GET_PERIPHERAL(MCP2515_INT),
	.Pin = GET_INDEX(MCP2515_INT),
	.RxCallback = MCP2515_RxCallback,
	.ErrorCallback = MCP2515_ErrorCallback,
};

// Data buffer for CAN message
uint8_t TxBuffer[MESSAGE_SIZE];
uint8_t RxBuffer[MCP2515_MAX_DATABYTES];

MCP2515_FilterConfig_t Filter0 = {
	.Filter = MCP2515_RXF0,
	.Mask = 0x7FF,
	.ID = 0x01
};

CAN_Message_t TxStandardRemote = {
	.Type = MCP2515_STANDARD_REMOTE,
	.ID = 0x03,
	.Length = 8
};

CAN_Message_t TxExtendedRemote = {
	.Type = MCP2515_EXTENDED_REMOTE,
	.ID = 0x30F0F,
	.Length = 8
};

CAN_Message_t TxStandardData = {
	.Type = MCP2515_STANDARD_DATA,
	.ID = 0x02,
	.Length = sizeof(TxBuffer),
	.pData = TxBuffer
};

CAN_Message_t TxExtendedData = {
	.Type = MCP2515_EXTENDED_DATA,
	.ID = 0x30F0E,
	.Length = sizeof(TxBuffer),
	.pData = TxBuffer
};

CAN_Message_t RxMessage = {
	.pData = RxBuffer,
};

int main(void)
{
	// Initialize the MCP2515 CAN controller
	MCP2515_Init(&Config_SPI, &Config);
	
	// Enable global interrupts
	EnableGlobalInterrupts();

	// Fill the buffer
	for(uint8_t i = 0x00; i < MESSAGE_SIZE; i++)
	{
		TxBuffer[i] = i;
	}

	// Set the filter rules for the receive buffer
	MCP2515_SwitchFilter(MCP2515_RX0, MCP2515_FILTER_RECEIVE_STD);
	MCP2515_SwitchFilter(MCP2515_RX1, MCP2515_FILTER_RECEIVE_STD);

	//MCP2515_ConfigFilter(&Filter0);

	MCP2515_PrepareMessage(&TxExtendedRemote, MCP2515_PRIO_HIGHEST, MCP2515_TX0);
	MCP2515_RequestTransmission(MCP2515_TX0);

	//MCP2515_PrepareMessage(&TxExtendedData, MCP2515_PRIO_HIGHEST, MCP2515_TX1);
	//MCP2515_PrepareMessage(&TxStandardData, MCP2515_PRIO_HIGHEST);
	//MCP2515_PrepareMessage(&TxStandardRemote, MCP2515_PRIO_HIGHEST);
	MCP2515_RequestTransmission(MCP2515_TX1);

    while(1) 
    {
    }
}

void MCP2515_RxCallback(void)
{
	MCP2515_ReadMessage(&RxMessage);
	
	volatile uint8_t Z;
	Z++;
}

void MCP2515_ErrorCallback()
{
	volatile uint8_t y;
	y++;
}