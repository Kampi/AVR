/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: ADXL345 example for XMega.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file main.c
 *  @brief ADXL345 example for XMega.
 *
 *  Software for the XMega ADXL345 example from
 *  <>
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/USART/USART.h"
#include "Peripheral/ADXL345/ADXL345.h"

void ADXL345_DataReady(void);
void ADXL345_Tap(void);

/*
	USART configuration
 */
USART_Config_t Config_USART = {
	.Device = &USARTE0,
	.DeviceMode = USART_MODE_ASYNCH,
	.Direction = USART_DIRECTION_BOTH,
	.Baudrate = 19200,
	.Parity = USART_PARITY_NONE,
	.Stop = USART_STOP_1,
	.Size = USART_SIZE_8,
	.BSCALE = -5,
	.EnableDoubleSpeed = FALSE,
	.EnableInterruptSupport = TRUE,
	.InterruptLevel = INT_LVL_HI
};

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = ADXL345_INTERFACE,
	.BitRate = ADXL345_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

/*
	ADXL345 configuration
*/
ADXL345_FIFOConfig_t ConfigFifo = {
	.Mode = ADXL345_FIFO_STREAM,
	.Samples = 30
};

ADXL345_Callbacks_t ADXL345_Callbacks = {
	.WatermarkCallback = ADXL345_DataReady,
	.SingleTapCallback = ADXL345_Tap
};
	
ADXL345_Configuration_t Config_ADXL345 = {
	.Range = ADXL345_RANGE_16G,
	.FullResolution = TRUE,
	.SupplyVoltage = ADXL345_SUPPLY_3V3,
	.DataRate = ADXL345_RATE_100HZ,
	.FIFO = &ConfigFifo
};

ADXL345_InterruptConfig_t ADXL345_Interrupts = {
	.Port = &PORTB,
	.Pin = 0,
	.IOChannel = GPIO_INTERRUPT_0,
	.DeviceChannel = ADXL345_INTERRUPT_1,
	.InterruptLevel = INT_LVL_HI,
	.Sense = ADXL345_POL_HIGH,
	.Type = ADXL345_WATERMARK_INTERRUPT | ADXL345_SINGLE_TAP_INTERRUPT,
	.Callbacks = &ADXL345_Callbacks
};

ADXL345_DataPoint_t Data[ADXL345_FIFO_SIZE];

int main(void)
{
	/*
		Initialize the USART
	*/
	USART_Init(&Config_USART);
	
	/*
		Initialize the ADXL345
	*/
	ADXL345_Init(&Config_I2CM, &Config_ADXL345);
	
	/*
		Enable global interrupts
	*/
	EnableGlobalInterrupts();
	PMIC_EnableAllInterruptLevel();
	
	// Start message
	USART_Print(Config_USART.Device, "--- ADXL345 example ---");
	
	ADXL345_SingleTapConfig(250, 1, ADXL345_AXIS_Z);
	ADXL345_InstallCallback(&ADXL345_Interrupts);
	ADXL345_SetDeviceMode(ADXL345_MODE_MEASURE);

    while(1)
    {
		ADXL345_WaitForData(&Data[0]);
    }
}

void ADXL345_DataReady(void)
{
 	ADXL345_FIFOClear(Data);
}

void ADXL345_Tap(void)
{
}