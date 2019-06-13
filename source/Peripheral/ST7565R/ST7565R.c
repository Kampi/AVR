/*
 * ST7565R.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for ST7565R Graphic LCD.

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

/** @file Peripheral/ST7565R/ST7565R.c
 *  @brief Driver for ST7565R Graphic LCD.
 *
 *  This contains the implementation of the display driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/ST7565R/ST7565R.h"

/** @defgroup ST7565R
 *  @{
 */
 	/** @defgroup ST7565R-Commands
	 *  SD card commands.
	 *  @{
	 */
		#define ST7565R_CMD_DISPLAY_ON						0xAF
		#define ST7565R_CMD_DISPLAY_OFF						0xAE
		#define ST7565R_CMD_START_LINE(Line)				(0x40 | (Line))
		#define ST7565R_CMD_PAGE_ADDRESS(Page)				(0xB0 | (Page))
		#define ST7565R_CMD_COLUMN_ADDRESS_MSB(Column)		(0x10 | (Column))
		#define ST7565R_CMD_COLUMN_ADDRESS_LSB(Column)		(0x00 | (Column))
		#define ST7565R_CMD_ADC_NORMAL						0xA0
		#define ST7565R_CMD_ADC_REVERSE						0xA1
		#define ST7565R_CMD_DISPLAY_NORMAL					0xA6
		#define ST7565R_CMD_DISPLAY_REVERSE					0xA7
		#define ST7565R_CMD_DISPLAY_ALL_POINTS_OFF			0xA4
		#define ST7565R_CMD_DISPLAY_ALL_POINTS_ON			0xA5
		#define ST7565R_CMD_LCD_BIAS_1_DIV_5_DUTY33			0xA1
		#define ST7565R_CMD_LCD_BIAS_1_DIV_6_DUTY33			0xA2
		#define ST7565R_CMD_NORMAL_SCAN_DIRECTION			0xC0
		#define ST7565R_CMD_REVERSE_SCAN_DIRECTION			0xC8
		#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_0		0x20
		#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_1		0x21
		#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_2		0x22
		#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_3		0x23
		#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_4		0x24
		#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_5		0x25
		#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_6		0x26
		#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_7		0x27
		#define ST7565R_CMD_POWER_CTRL_ALL_ON				0x2F
		#define ST7565R_CMD_SLEEP_MODE						0xAC
		#define ST7565R_CMD_NORMAL_MODE						0xAD
		#define ST7565R_CMD_RESET							0xE2
		#define ST7565R_CMD_NOP								0xE3
		#define ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET		0x81
		#define ST7565R_CMD_ELECTRONIC_VOLUME(Volume)		(0x3F & (~Volume))
		#define ST7565R_CMD_BOOSTER_RATIO					0xF8
		#define ST7565R_CMD_BOOSTER_RATIO_2X_3X_4X			0x00
		#define ST7565R_CMD_BOOSTER_RATIO_5X				0x01
		#define ST7565R_CMD_BOOSTER_RATIO_6X				0x03
		#define ST7565R_CMD_STATUS_READ						0x00
		#define ST7565R_CMD_END								0xEE
		#define ST7565R_CMD_READ_MODIFY_WRITE				0xE0
	/** @} */ // end of ST7565R-Commands
/** @} */ // end of ST7565R

/** @brief			Write a command to the display.
 *  @param Command	Display command
 */
static void ST7565R_WriteCommand(const uint8_t Command)
{
	SPIM_CHIP_SELECT(ST7565R_SS_PORT, ST7565R_SS_PIN);
	GPIO_Clear(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN);
	SPIM_TRANSMIT(DISPLAY_INTERFACE, Command);
	GPIO_Set(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN);
	SPIM_CHIP_DESELECT(ST7565R_SS_PORT, ST7565R_SS_PIN);
}

/** @brief			Write multiple commands to the display.
 *  @param Data		Display commands
 *  @param Length	Number of command bytes (including parameter)
 */
static void ST7565R_WriteCommandBytes(const uint8_t* Data, const uint8_t Length)
{
	SPIM_CHIP_SELECT(ST7565R_SS_PORT, ST7565R_SS_PIN);
	GPIO_Clear(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN);
	
	for(uint8_t i = 0x00; i < Length; i++)
	{
		SPIM_TRANSMIT(DISPLAY_INTERFACE, *Data++);
	}
	
	GPIO_Set(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN);
	SPIM_CHIP_DESELECT(ST7565R_SS_PORT, ST7565R_SS_PIN);
}

void Display_Init(SPIM_Config_t* Config)
{
	const uint8_t Init[] = {
		ST7565R_CMD_ADC_NORMAL,
		ST7565R_CMD_DISPLAY_NORMAL,
		ST7565R_CMD_REVERSE_SCAN_DIRECTION,
		ST7565R_CMD_LCD_BIAS_1_DIV_6_DUTY33,
		ST7565R_CMD_POWER_CTRL_ALL_ON,
		ST7565R_CMD_BOOSTER_RATIO,
		ST7565R_CMD_BOOSTER_RATIO_2X_3X_4X,
		ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_1,
		ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET,
		ST7565R_CMD_ELECTRONIC_VOLUME(DISPLAY_CONTRAST_MIN),
		ST7565R_CMD_DISPLAY_ON,
	};
	
	// Set display back light as output in low state
	GPIO_SetDirection(ST7565R_BACKLIGHT_PORT, ST7565R_BACKLIGHT_PIN, GPIO_DIRECTION_OUT);
	GPIO_Clear(ST7565R_BACKLIGHT_PORT, ST7565R_BACKLIGHT_PIN);

	// Set display reset as output in high state
	GPIO_SetDirection(ST7565R_RESET_PORT, ST7565R_RESET_PIN, GPIO_DIRECTION_OUT);
	GPIO_Set(ST7565R_RESET_PORT, ST7565R_RESET_PIN);

	// Set register select as output in high state
	GPIO_SetDirection(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN, GPIO_DIRECTION_OUT);
	GPIO_Set(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN);

	// Set display SS as output in high state
	GPIO_SetDirection(ST7565R_SS_PORT, ST7565R_SS_PIN, GPIO_DIRECTION_OUT);
	GPIO_Set(ST7565R_SS_PORT, ST7565R_SS_PIN);

	// Reset the display
	ST7565R_HardwareReset();

	// Initialize SPI-Interface
	if(Config != NULL)
	{
		SPIM_INIT(Config);
	}

	// Set A0 to low state for "Command"
	GPIO_Clear(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN);

	ST7565R_WriteCommandBytes(Init, sizeof(Init));

	// Clear Display
	ST7565R_Clear();
}

void Display_Reset(void)
{
	ST7565R_HardwareReset();
	ST7565R_SoftwareReset();
}

void Display_Switch(const Bool_t State)
{
	if(State == TRUE)
	{
		ST7565R_WriteCommand(ST7565R_CMD_DISPLAY_ON);
	}
	else
	{
		ST7565R_WriteCommand(ST7565R_CMD_DISPLAY_OFF);
	}
}

void Display_WriteData(const uint8_t Data)
{
	SPIM_CHIP_SELECT(ST7565R_SS_PORT, ST7565R_SS_PIN);
	GPIO_Set(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN);
	SPIM_TRANSMIT(DISPLAY_INTERFACE, Data);
	GPIO_Clear(ST7565R_REGISTER_SELECT_PORT, ST7565R_REGISTER_SELECT_PIN);
	SPIM_CHIP_DESELECT(ST7565R_SS_PORT, ST7565R_SS_PIN);
}

void Display_SetPage(const uint8_t Page)
{
	ST7565R_WriteCommand(ST7565R_CMD_PAGE_ADDRESS(Page & 0x0F));
}

void Display_SetColumn(const uint8_t Column)
{
	uint8_t AddressTemp = Column;
	AddressTemp &= 0x7F;
	ST7565R_WriteCommand(ST7565R_CMD_COLUMN_ADDRESS_MSB(AddressTemp >> 4));
	ST7565R_WriteCommand(ST7565R_CMD_COLUMN_ADDRESS_LSB(AddressTemp & 0x0F));
}

void Display_SetStartLine(const uint8_t Line)
{
	ST7565R_WriteCommand(ST7565R_CMD_START_LINE(Line & 0x3F));
}

/*
	Display driver functions
*/
void ST7565R_HardwareReset(void)
{
	GPIO_Clear(ST7565R_RESET_PORT, ST7565R_RESET_PIN);

	// Wait a bit
 	for(uint8_t Counter = 0; Counter < 0xFF; Counter++);

 	GPIO_Set(ST7565R_RESET_PORT, ST7565R_RESET_PIN);

	// Wait a bit
 	for(uint8_t Counter = 0; Counter < 0x0FF; Counter++);
}

void ST7565R_SoftwareReset(void)
{
	ST7565R_WriteCommand(ST7565R_CMD_RESET);
}

void ST7565R_SleepEnable(const Bool_t State)
{
	if(State == TRUE)
	{
		ST7565R_WriteCommand(ST7565R_CMD_SLEEP_MODE);
	}
	else
	{
		ST7565R_WriteCommand(ST7565R_CMD_NORMAL_MODE);
	}
}

void ST7565R_SwitchInvert(const Bool_t Invert)
{
	if(Invert == TRUE)
	{
		ST7565R_WriteCommand(ST7565R_CMD_DISPLAY_REVERSE);
	}
	else
	{
		ST7565R_WriteCommand(ST7565R_CMD_DISPLAY_NORMAL);
	}
}

void ST7565R_SetContrast(const uint8_t Contrast)
{
	uint8_t ContrastTemp = Contrast;
	if (Contrast < DISPLAY_CONTRAST_MIN) 
	{
		ContrastTemp = DISPLAY_CONTRAST_MIN;
	}
	if (Contrast > DISPLAY_CONTRAST_MAX) 
	{
		ContrastTemp = DISPLAY_CONTRAST_MAX;
	}

	ST7565R_WriteCommand(ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET);
	ST7565R_WriteCommand(ST7565R_CMD_ELECTRONIC_VOLUME(ContrastTemp));
}

void ST7565R_SwitchBacklight(const Bool_t State)
{
	if(State == FALSE)
	{
		GPIO_Clear(ST7565R_BACKLIGHT_PORT, ST7565R_BACKLIGHT_PIN);
	}
	else if(State == TRUE)
	{
		GPIO_Set(ST7565R_BACKLIGHT_PORT, ST7565R_BACKLIGHT_PIN);
	}
}

void ST7565R_ClearLine(const uint8_t Line)
{
	uint8_t Column;

	Display_SetPage(Line);

	for(Column = 0; Column < DISPLAY_WIDTH; Column++)
	{
		Display_SetColumn(Column);
		Display_WriteData(0x00);
	}
}

void ST7565R_Clear(void)
{
	uint8_t Page, Column;
	for(Page = 0; Page < (DISPLAY_HEIGHT / DISPLAY_BYTES_PER_PIXEL); Page++)
	{
		for(Column = 0; Column < DISPLAY_WIDTH; Column++)
		{
			Display_SetPage(Page);
			Display_SetColumn(Column);
			Display_WriteData(0x00);
		}
	}
}