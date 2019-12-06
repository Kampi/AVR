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

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#if(ST7565R_INTERFACE_TYPE == INTERFACE_USART_SPI)
		#define ST7565R_SPIM_INIT(Config)											USART_SPI_Init(Config)
		#define ST7565R_SPIM_TRANSMIT(Data)											USART_SPI_SendData(&CONCAT(DISPLAY_INTERFACE), Data)
		#define ST7565R_SPIM_SET_CLOCK(SPIClock, Clock)								USART_SPI_SetClockRate(&CONCAT(DISPLAY_INTERFACE), SPIClock, Clock, FALSE)
		#define ST7565R_SPIM_GET_CLOCK(Clock)										USART_SPI_GetClockRate(&CONCAT(DISPLAY_INTERFACE), Clock)
		#define ST7565R_SPIM_CHIP_SELECT(Port, Pin)									USART_SPI_SelectDevice(Port, Pin)
		#define ST7565R_SPIM_CHIP_DESELECT(Port, Pin)								USART_SPI_DeselectDevice(Port, Pin)
	#elif(ST7565R_INTERFACE_TYPE == INTERFACE_SPI)
		#define ST7565R_SPIM_INIT(Config)											SPIM_Init(Config)
		#define ST7565R_SPIM_TRANSMIT(Data)											SPIM_SendData(&CONCAT(DISPLAY_INTERFACE), Data)
		#define ST7565R_SPIM_SET_CLOCK(SPIClock, Clock)								SPIM_SetClock(&CONCAT(DISPLAY_INTERFACE), SPIClock, Clock)
		#define ST7565R_SPIM_GET_CLOCK(Clock)										SPIM_GetClock(&CONCAT(DISPLAY_INTERFACE), Clock)
		#define ST7565R_SPIM_CHIP_SELECT(Port, Pin)									SPIM_SelectDevice(Port, Pin)
		#define ST7565R_SPIM_CHIP_DESELECT(Port, Pin)								SPIM_DeselectDevice(Port, Pin)
	#else
		#error "Interface not supported for ST7565R!"
	#endif
#elif(MCU_ARCH == MCU_ARCH_AVR8)
	#define ST7565R_SPIM_INIT(Config)												SPIM_Init(Config)
    #define ST7565R_SPIM_TRANSMIT(Data)												SPIM_SendData(Data)
    #define ST7565R_SPIM_SET_CLOCK(SPIClock, Clock)									SPIM_SetClock(SPIClock, Clock)
    #define ST7565R_SPIM_GET_CLOCK(Clock)											SPIM_GetClock(Clock)
    #define ST7565R_SPIM_CHIP_SELECT(Port, Pin)										SPIM_SelectDevice(Port, Pin)
    #define ST7565R_SPIM_CHIP_DESELECT(Port, Pin)									SPIM_DeselectDevice(Port, Pin)
#else
	 #error "Architecture not supported for ST7565R!"
#endif

/** @brief			Write a command to the display.
 *  @param Command	Display command
 */
static void ST7565R_WriteCommand(const uint8_t Command)
{
	ST7565R_SPIM_CHIP_SELECT(GET_PERIPHERAL(ST7565R_SS), GET_INDEX(ST7565R_SS));
	GPIO_Clear(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT));
	ST7565R_SPIM_TRANSMIT(Command);
	GPIO_Set(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT));
	ST7565R_SPIM_CHIP_DESELECT(GET_PERIPHERAL(ST7565R_SS), GET_INDEX(ST7565R_SS));
}

/** @brief			Write multiple commands to the display.
 *  @param Data		Display commands
 *  @param Length	Number of command bytes (including parameter)
 */
static void ST7565R_WriteCommandBytes(const uint8_t* Data, const uint8_t Length)
{
	ST7565R_SPIM_CHIP_SELECT(GET_PERIPHERAL(ST7565R_SS), GET_INDEX(ST7565R_SS));
	GPIO_Clear(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT));
	
	for(uint8_t i = 0x00; i < Length; i++)
	{
		ST7565R_SPIM_TRANSMIT(*Data++);
	}
	
	GPIO_Set(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT));
	ST7565R_SPIM_CHIP_DESELECT(GET_PERIPHERAL(ST7565R_SS), GET_INDEX(ST7565R_SS));
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
	GPIO_SetDirection(GET_PERIPHERAL(ST7565R_BACKLIGHT), GET_INDEX(ST7565R_BACKLIGHT), GPIO_DIRECTION_OUT);
	GPIO_Clear(GET_PERIPHERAL(ST7565R_BACKLIGHT), GET_INDEX(ST7565R_BACKLIGHT));

	// Set display reset as output in high state
	GPIO_SetDirection(GET_PERIPHERAL(ST7565R_RESET), GET_INDEX(ST7565R_RESET), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(ST7565R_RESET), GET_INDEX(ST7565R_RESET));

	// Set register select as output in high state
	GPIO_SetDirection(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT));

	// Set display SS as output in high state
	GPIO_SetDirection(GET_PERIPHERAL(ST7565R_SS), GET_INDEX(ST7565R_SS), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(ST7565R_SS), GET_INDEX(ST7565R_SS));

	// Reset the display
	ST7565R_HardwareReset();

	// Initialize SPI-Interface
	if(Config != NULL)
	{
		ST7565R_SPIM_INIT(Config);
	}

	// Set A0 to low state for "Command"
	GPIO_Clear(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT));

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
	ST7565R_SPIM_CHIP_SELECT(GET_PERIPHERAL(ST7565R_SS), GET_INDEX(ST7565R_SS));
	GPIO_Set(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT));
	ST7565R_SPIM_TRANSMIT(Data);
	GPIO_Clear(GET_PERIPHERAL(ST7565R_REGISTER_SELECT), GET_INDEX(ST7565R_REGISTER_SELECT));
	ST7565R_SPIM_CHIP_DESELECT(GET_PERIPHERAL(ST7565R_SS), GET_INDEX(ST7565R_SS));
}

void Display_SetPage(const uint8_t Page)
{
	ST7565R_WriteCommand(ST7565R_CMD_PAGE_ADDRESS(Page & 0x0F));
}

void Display_SetColumn(const uint8_t Column)
{
	uint8_t AddressTemp = Column;
	AddressTemp &= 0x7F;
	ST7565R_WriteCommand(ST7565R_CMD_COLUMN_ADDRESS_MSB(AddressTemp >> 0x04));
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
	GPIO_Clear(GET_PERIPHERAL(ST7565R_RESET), GET_INDEX(ST7565R_RESET));

	// Wait a bit
 	for(uint8_t Counter = 0; Counter < 0xFF; Counter++);

 	GPIO_Set(GET_PERIPHERAL(ST7565R_RESET), GET_INDEX(ST7565R_RESET));

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
		GPIO_Clear(GET_PERIPHERAL(ST7565R_BACKLIGHT), GET_INDEX(ST7565R_BACKLIGHT));
	}
	else if(State == TRUE)
	{
		GPIO_Set(GET_PERIPHERAL(ST7565R_BACKLIGHT), GET_INDEX(ST7565R_BACKLIGHT));
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
	for(Page = 0; Page < (DISPLAY_HEIGHT / DISPLAY_PIXEL_PER_BYTE); Page++)
	{
		for(Column = 0; Column < DISPLAY_WIDTH; Column++)
		{
			Display_SetPage(Page);
			Display_SetColumn(Column);
			Display_WriteData(0x00);
		}
	}
}