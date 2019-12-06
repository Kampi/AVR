/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: MLX90399 magnetic sensor example for XMega.

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
 *  @brief MLX90399 i2c magnetic sensor example for XMega.
 *
 *  @author Daniel Kampert
 */

// ToDo: Chip im Timer über Trigger Pin triggern

#include <stdio.h> 
#include "Arch/XMega/Timer0/Timer0.h"
#include "Peripheral/MLX90393/MLX90393.h"
#include "Common/Services/DisplayManager/DisplayManager.h"

#define LED0_PORT									&PORTR
#define LED0_PIN									0
#define LED1_PORT									&PORTR
#define LED1_PIN									1
#define BUTTON0_PORT								&PORTF
#define BUTTON0_PIN									1
#define BUTTON1_PORT								&PORTF
#define BUTTON1_PIN									2

void MLX90393_Callback(void);

typedef enum
{
	STATE_START = 0x00,
	STATE_WAIT_FIRST_CALIBRATION,
	STATE_FIRST_CALIBRATION,
	STATE_WAIT_SECOND_CALIBRATION,
	STATE_SECOND_CALIBRATION,
	STATE_CALIBRATION_END,
	STATE_WAIT,
} State_t;

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = MLX90393_INTERFACE,
	.BitRate = MLX90393_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

/*
	MLX90393 configuration
*/
MLX90393_Config_t Config_MLX90393 = {
	.Gain = 0x07,
	.HallConfig = 
	{
		.Duration = 0x00,
		.Periods = 0x00,
	},
	.Resolution = 0x02,
	.DigFilt = 0x00,
	.Oversampling = 0x00,
};

Timer0_Config_t Config_Timer0 = {
	.Device = &TCC0,
	.Period = 64,
	.Prescaler = TIMER_PRESCALER_1024,
};

Timer0_InterruptConfig_t Callback_Timer0 = 
{
	.Callback = MLX90393_Callback,
	.Device = &TCC0,
	.InterruptLevel = INT_LVL_LO,
	.Source = TIMER_OVERFLOW_INTERRUPT,
};

uint16_t Distance;
Bool_t Active = FALSE;

MLX90393_DataPoint_t Calibration_Without;
MLX90393_DataPoint_t Calibration_With;
MLX90393_DataPoint_t Difference = {0, 0x7F0C, 0};

State_t State = STATE_WAIT;

int main(void)
{
	for(uint16_t i = 0x00; i< 0xFFFF; i++);
	
	/*
		Initialize the system clock
	*/
	SysClock_Init();

	/*
		Initialize the LED
			-> Output
			-> PR0 (LED0 active low
			-> PR1 (LED0 active low)
	*/
	GPIO_SetDirection(LED0_PORT, LED0_PIN, GPIO_DIRECTION_OUT);
	GPIO_SetDirection(LED1_PORT, LED1_PIN, GPIO_DIRECTION_OUT);
	GPIO_SwitchInvert(LED0_PORT, (0x01 << LED0_PIN), TRUE);
	GPIO_SwitchInvert(LED1_PORT, (0x01 << LED1_PIN), TRUE);
	GPIO_Clear(LED0_PORT, LED0_PIN);
	GPIO_Clear(LED1_PORT, LED1_PIN);

	/*
		Initialize the buttons
			-> Input
			-> PF1 (BUTTON0 active low
			-> PF2 (BUTTON1 active low)
	*/
	GPIO_SetDirection(BUTTON0_PORT, BUTTON0_PIN, GPIO_DIRECTION_IN);
	GPIO_SetDirection(BUTTON1_PORT, BUTTON1_PIN, GPIO_DIRECTION_IN);
	GPIO_SwitchInvert(BUTTON0_PORT, (0x01 << BUTTON0_PIN), TRUE);
	GPIO_SwitchInvert(BUTTON1_PORT, (0x01 << BUTTON1_PIN), TRUE);
	
	GPIO_SetDirection(&PORTB, 0, GPIO_DIRECTION_OUT);
	GPIO_SetDirection(&PORTB, 2, GPIO_DIRECTION_OUT);

	/*
		Initialize Timer 0
			-> Normal mode
			-> t = 1024 / 32 MHz * 64 = 4 ms
			-> Total sensor convertion time for single measurement mode can be calculated with:
				t = t_stby + t_active + m * t_conv + t_convt
				t_stby		= 600 us (max)
				t_active	= 8 us
				m			= 1 axis
				t_conv		= (2 + 2^DigFilt) * 2^Oversampling * 0.064 ms
				t_convt		= 0 (no temperature measurement)
	*/
	Timer0_Init(&Config_Timer0);

	/*
		Initialize the display
	*/
	DisplayManager_Init();

	/*
		Initialize MLX90393 I2C magnetic sensor
	*/
	if(MLX90393_Init(&Config_I2CM, &Config_MLX90393) == I2C_NO_ERROR)
	{
		GPIO_Clear(LED0_PORT, LED0_PIN);
		GPIO_Clear(LED1_PORT, LED1_PIN);
	}
	else
	{
		// Set the status LED
		GPIO_Set(LED0_PORT, LED0_PIN);
		GPIO_Set(LED1_PORT, LED1_PIN);
		
		return -1;
	}

	/*
		Enable interrupts
	*/
	PMIC_EnableAllInterruptLevel();
	EnableGlobalInterrupts();

	// Clear the display
	DisplayManager_Clear();

	while(1)
	{
		switch(State)
		{
			case STATE_START:
			{
				DisplayManager_DrawString(0, 0, "SW0: Start calibration"); 
				DisplayManager_DrawString(0, 8, "SW1: Start measurement");
	
				if(GPIO_Read(BUTTON0_PORT, BUTTON0_PIN) == TRUE)
				{
					DisplayManager_Clear();
					State = STATE_WAIT_FIRST_CALIBRATION;
				}
				else if(GPIO_Read(BUTTON1_PORT, BUTTON1_PIN) == TRUE)
				{
					DisplayManager_Clear();
					DisplayManager_DrawString(0, 0, "Start measurement...");

					State = STATE_WAIT;
				}

				break;
			}
			case STATE_WAIT_FIRST_CALIBRATION:
			{
				DisplayManager_DrawString(0, 0, "Remove magnets and place"); 
				DisplayManager_DrawString(0, 8, "the device inside of the pipe.");
				DisplayManager_DrawString(0, 16, "Press SW0 to start calibration...");
				
				if(GPIO_Read(BUTTON0_PORT, BUTTON0_PIN) == TRUE)
				{
					DisplayManager_Clear();
					DisplayManager_DrawString(0, 0, "Start calibration...");

					State = STATE_FIRST_CALIBRATION;
				}
				
				break;
			}
			case STATE_FIRST_CALIBRATION:
			{
				uint32_t x = 0x00;
				uint32_t y = 0x00;
				uint32_t z = 0x00;

				// Read data points
				for(uint8_t i = 0x00; i < MLX90393_KALIBRATION_LENGTH; i++)
				{
					MLX90393_DataPoint_t Data;
					if(MLX90393_Single(MLX90393_AXIS_Z, &Data) == I2C_NO_ERROR)
					{
						x += Data.x;
						y += Data.y;
						z += Data.z;
					}
				}

				// Get the average
				Calibration_Without.x = x / MLX90393_KALIBRATION_LENGTH;
				Calibration_Without.y = y / MLX90393_KALIBRATION_LENGTH;
				Calibration_Without.z = z / MLX90393_KALIBRATION_LENGTH;
				
				DisplayManager_ClearLine(0);
				State = STATE_WAIT_SECOND_CALIBRATION;
				
				break;
			}
			case STATE_WAIT_SECOND_CALIBRATION:
			{
				DisplayManager_DrawString(0, 0, "Calibration successful!");
				DisplayManager_DrawString(0, 8, "Add the magnets and press SW0...");

				if(GPIO_Read(BUTTON0_PORT, BUTTON0_PIN) == TRUE)
				{
					DisplayManager_Clear();
					DisplayManager_DrawString(0, 0, "Start calibration...");
	
					State = STATE_SECOND_CALIBRATION;
				}

				break;				
			}
			case STATE_SECOND_CALIBRATION:
			{
				uint32_t x = 0x00;
				uint32_t y = 0x00;
				uint32_t z = 0x00;

				// Read data points
				for(uint8_t i = 0x00; i < MLX90393_KALIBRATION_LENGTH; i++)
				{
					MLX90393_DataPoint_t Data;
					if(MLX90393_Single(MLX90393_AXIS_Y, &Data) == I2C_NO_ERROR)
					{
						x += Data.x;
						y += Data.y;
						z += Data.z;
					}
				}

				// Get the average
				Calibration_With.x = x / MLX90393_KALIBRATION_LENGTH;
				Calibration_With.y = y / MLX90393_KALIBRATION_LENGTH;
				Calibration_With.z = z / MLX90393_KALIBRATION_LENGTH;

				DisplayManager_Clear();

				State = STATE_CALIBRATION_END;
				
				break;
			}
			case STATE_CALIBRATION_END:
			{
				char Buffer[20];
				
				Difference.x =  Calibration_With.x - Calibration_Without.x;
				Difference.y =  Calibration_With.y - Calibration_Without.y;
				Difference.z =  Calibration_With.z - Calibration_Without.z;
				
				DisplayManager_DrawString(0, 0, "Difference:");
				sprintf(Buffer, "x: %d", Difference.x);
				DisplayManager_DrawString(0, 8, Buffer);
				sprintf(Buffer, "y: %d", Difference.y);
				DisplayManager_DrawString(0, 16, Buffer);
				sprintf(Buffer, "z: %d", Difference.z);
				DisplayManager_DrawString(0, 24, Buffer);
				
				State = STATE_WAIT;
				
				break;
			}
			case STATE_WAIT:
			{
				Timer0_InstallCallback(&Callback_Timer0);
		
				while(1)
				{

				}
			}
		}
	}
}

void MLX90393_Callback(void)
{
	MLX90393_DataPoint_t Data;

	GPIO_Toggle(&PORTB, 0);
	
	I2C_Error_t Error = MLX90393_Single(MLX90393_AXIS_Y, &Data);
	if(Error == I2C_NO_ERROR)
	{
		uint16_t Delta = 0x00;

		GPIO_Clear(LED1_PORT, LED1_PIN);
		
		if(Data.y > Difference.y)
		{
			Delta = Data.y - Difference.y;
		}
		else
		{
			 Delta = Difference.y - Data.y;
		}
		
		if((Delta > 0x170) & (Active == FALSE))
		{
			Active = TRUE;
			Distance++;
			GPIO_Set(LED0_PORT, LED0_PIN);
			GPIO_Set(&PORTB, 2);
			
			char Buffer[20];

			DisplayManager_Clear();
			sprintf(Buffer, "Distance: %d", Distance);
			DisplayManager_DrawString(0, 0, Buffer);
		}

		if(Delta < 0xFF)
		{
			Active = FALSE;
			GPIO_Clear(LED0_PORT, LED0_PIN);
			GPIO_Clear(&PORTB, 2);
		}
	}
	else
	{
		GPIO_Set(LED1_PORT, LED1_PIN);
	}
}