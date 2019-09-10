/*
 * PCA9685.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for PCA9685 PWM controller.

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file PCA9685.c
 *  @brief Driver for NXP PCA9685 PWM controller.
 *
 *  This file contains the implementation of the PWM controller driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/PCA9685/PCA9685.h"

/** @defgroup PCA9685
 *  @{
 */
	/** @defgroup PCA9685-Register
	 *  PCA9685 register addresses.
	 *  @{
	 */
		#define PCA9685_REGISTER_MODE1				0x00
		#define PCA9685_REGISTER_MODE2				0x01
		#define PCA9685_REGISTER_SUBADR1			0x02
		#define PCA9685_REGISTER_SUBADR2			0x03
		#define PCA9685_REGISTER_SUBADR3			0x04
		#define PCA9685_REGISTER_ALLCALLADR			0x05
		#define PCA9685_REGISTER_LED0_ON_L			0x06
		#define PCA9685_REGISTER_LED0_ON_H			0x07
		#define PCA9685_REGISTER_LED0_OFF_L			0x08
		#define PCA9685_REGISTER_LED0_OFF_H			0x09
		#define PCA9685_REGISTER_LED1_ON_L			0x0A
		#define PCA9685_REGISTER_LED1_ON_H			0x0B
		#define PCA9685_REGISTER_LED1_OFF_L			0x0C
		#define PCA9685_REGISTER_LED1_OFF_H			0x0D
		#define PCA9685_REGISTER_LED2_ON_L			0x0E
		#define PCA9685_REGISTER_LED2_ON_H			0x0F
		#define PCA9685_REGISTER_LED2_OFF_L			0x10
		#define PCA9685_REGISTER_LED2_OFF_H			0x11
		#define PCA9685_REGISTER_LED3_ON_L			0x12
		#define PCA9685_REGISTER_LED3_ON_H			0x13
		#define PCA9685_REGISTER_LED3_OFF_L			0x14
		#define PCA9685_REGISTER_LED3_OFF_H			0x15
		#define PCA9685_REGISTER_LED4_ON_L			0x16
		#define PCA9685_REGISTER_LED4_ON_H			0x17
		#define PCA9685_REGISTER_LED4_OFF_L			0x18
		#define PCA9685_REGISTER_LED4_OFF_H			0x19
		#define PCA9685_REGISTER_LED5_ON_L			0x1A
		#define PCA9685_REGISTER_LED5_ON_H			0x1B
		#define PCA9685_REGISTER_LED5_OFF_L			0x1C
		#define PCA9685_REGISTER_LED5_OFF_H			0x1D
		#define PCA9685_REGISTER_LED6_ON_L			0x1E
		#define PCA9685_REGISTER_LED6_ON_H			0x1F
		#define PCA9685_REGISTER_LED6_OFF_L			0x20
		#define PCA9685_REGISTER_LED6_OFF_H			0x21
		#define PCA9685_REGISTER_LED7_ON_L			0x22
		#define PCA9685_REGISTER_LED7_ON_H			0x23
		#define PCA9685_REGISTER_LED7_OFF_L			0x24
		#define PCA9685_REGISTER_LED7_OFF_H			0x25
		#define PCA9685_REGISTER_LED8_ON_L			0x26
		#define PCA9685_REGISTER_LED8_ON_H			0x27
		#define PCA9685_REGISTER_LED8_OFF_L			0x28
		#define PCA9685_REGISTER_LED8_OFF_H			0x29
		#define PCA9685_REGISTER_LED9_ON_L			0x2A
		#define PCA9685_REGISTER_LED9_ON_H			0x2B
		#define PCA9685_REGISTER_LED9_OFF_L			0x2C
		#define PCA9685_REGISTER_LED9_OFF_H			0x2D
		#define PCA9685_REGISTER_LED10_ON_L			0x2E
		#define PCA9685_REGISTER_LED10_ON_H			0x2F
		#define PCA9685_REGISTER_LED10_OFF_L		0x30
		#define PCA9685_REGISTER_LED10_OFF_H		0x31
		#define PCA9685_REGISTER_LED11_ON_L			0x32
		#define PCA9685_REGISTER_LED11_ON_H			0x33
		#define PCA9685_REGISTER_LED11_OFF_L		0x34
		#define PCA9685_REGISTER_LED11_OFF_H		0x35
		#define PCA9685_REGISTER_LED12_ON_L			0x36
		#define PCA9685_REGISTER_LED12_ON_H			0x37
		#define PCA9685_REGISTER_LED12_OFF_L		0x38
		#define PCA9685_REGISTER_LED12_OFF_H		0x39
		#define PCA9685_REGISTER_LED13_ON_L			0x3A
		#define PCA9685_REGISTER_LED13_ON_H			0x3B
		#define PCA9685_REGISTER_LED13_OFF_L		0x3C
		#define PCA9685_REGISTER_LED13_OFF_H		0x3D
		#define PCA9685_REGISTER_LED14_ON_L			0x3E
		#define PCA9685_REGISTER_LED14_ON_H			0x3F
		#define PCA9685_REGISTER_LED14_OFF_L		0x40
		#define PCA9685_REGISTER_LED14_OFF_H		0x41
		#define PCA9685_REGISTER_LED15_ON_L			0x42
		#define PCA9685_REGISTER_LED15_ON_H			0x43
		#define PCA9685_REGISTER_LED15_OFF_L		0x44
		#define PCA9685_REGISTER_LED15_OFF_H		0x45
		#define PCA9685_REGISTER_ALL_LED_ON_L		0xFA
		#define PCA9685_REGISTER_ALL_LED_ON_H		0xFB
		#define PCA9685_REGISTER_ALL_LED_OFF_L		0xFC
		#define PCA9685_REGISTER_ALL_LED_OFF_H		0xFD
		#define PCA9685_REGISTER_PRESCALER			0xFE
		#define PCA9685_REGISTER_TESTMODE			0xFF
	/** @} */ // end of PCA9685-Register
	 
 	/** @defgroup PCA9685-Control
	 *  PCA9685 control bits.
	 *  @{
	 */ 
		#define PCA9685_RESTART						0x07
		#define PCA9685_EXTCLOCK					0x06
		#define PCA9685_AI							0x05
		#define PCA9685_SLEEP						0x04
		#define PCA9685_SUB1						0x03
		#define PCA9685_SUB2						0x02
		#define PCA9685_SUB3						0x01
		#define PCA9685_ALLCALL						0x00
		#define PCA9685_INVRT						0x04
		#define PCA9685_OCH							0x03
		#define PCA9685_OUTDRV						0x02
		#define PCA9685_OUTNE1						0x01
		#define PCA9685_OUTNE0						0x00
	/** @} */ // end of PCA9685-Control
/** @} */ // end of PCA9685

const I2C_Error_t PCA9685_Init(I2CM_Config_t* Config, const PCA9685_ClockSource_t Source)
{
	I2C_Error_t ErrorCode = I2C_NO_ERROR;
	
	if(Config != NULL)
	{
		PCA9685_I2C_INIT(Config);
	}
	
	ErrorCode = PCA9685_SwitchSleep(FALSE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = PCA9685_SetClockSource(Source);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	ErrorCode = PCA9685_SwitchAutoIncrement(TRUE);
	
	PCA9685_SetChannel(PCA9685_CHANNEL_ALL, 409, 1228);

	return ErrorCode;
}

const I2C_Error_t PCA9685_SetOutputChange(const PCA9685_OutputChange_t Mode)
{
	if(Mode == PCA9685_OCH_ACK)
	{
		return PCA9685_SwitchBit(PCA9685_REGISTER_MODE2, PCA9685_OCH, TRUE);
	}
	
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE2, PCA9685_OCH, FALSE); 
}

const I2C_Error_t PCA9685_SetOutputDrive(const PCA9685_OutputDrive_t Drive)
{
	if(Drive == PCA9685_OUTDRV_OD)
	{
		return PCA9685_SwitchBit(PCA9685_REGISTER_MODE2, PCA9685_OUTDRV, FALSE); 
	}

	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE2, PCA9685_OUTDRV, TRUE); 
}

const I2C_Error_t PCA9685_SetOutputEnable(const PCA9685_OutputEnable_t Mode)
{
	uint8_t Data = 0x00;
	I2C_Error_t ErrorCode = I2C_NO_ERROR;

	ErrorCode = PCA9685_I2C_READBYTE(PCA9685_REGISTER_MODE2, &Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	if(Mode == PCA9685_OUTNE_MODE_0)
	{
		Data &= ~((0x01 << PCA9685_OUTNE1) | (0x01 << PCA9685_OUTNE0));
	}
	else if(Mode == PCA9685_OUTNE_MODE_1)
	{
		Data &= ~((0x01 << PCA9685_OUTNE1) | (0x01 << PCA9685_OUTNE0));
		Data |= (0x01 << PCA9685_OUTNE0);
	}
	else
	{
		Data &= ~((0x01 << PCA9685_OUTNE1) | (0x01 << PCA9685_OUTNE0));
		Data |= (0x01 << PCA9685_OUTNE1) | (0x01 << PCA9685_OUTNE0);
	}
	
	return PCA9685_I2C_WRITEBYTE(PCA9685_REGISTER_MODE2, &Data);
}

const I2C_Error_t PCA9685_Reset(void)
{
	return PCA9685_RESET();
}

const I2C_Error_t PCA9685_SetClockSource(const PCA9685_ClockSource_t Source)
{
	if(Source == PCA9685_CLOCK_INT)
	{
		return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_EXTCLOCK, FALSE);
	}
	
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_EXTCLOCK, TRUE);
}

const I2C_Error_t PCA9685_SwitchBit(const uint8_t Register, const uint8_t Bit, const Bool_t Enable)
{
	uint8_t Data = 0x00;
	I2C_Error_t ErrorCode = PCA9685_I2C_READBYTE(Register, &Data);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}

	if(Enable == TRUE)
	{
		Data |= (0x01 << Bit);
	}
	else
	{
		Data &= ~(0x01 << Bit);
	}
	
	return PCA9685_I2C_WRITEBYTE(Register, &Data);
}

const I2C_Error_t PCA9685_Restart(void)
{
	I2C_Error_t ErrorCode = PCA9685_SwitchSleep(FALSE);
	if(ErrorCode != I2C_NO_ERROR)
	{
		return ErrorCode;
	}
	
	// Wait a bit
	for(uint16_t i; i < 0xFFFF; i++);
	
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_RESTART, TRUE);
}

const I2C_Error_t PCA9685_SwitchAutoIncrement(const Bool_t Enable)
{
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_AI, Enable);
}

const I2C_Error_t PCA9685_SwitchSleep(const Bool_t Enable)
{
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_SLEEP, Enable);
}

const I2C_Error_t PCA9685_SwitchSub1(const Bool_t Enable)
{
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_SUB1, Enable);
}

const I2C_Error_t PCA9685_SwitchSub2(const Bool_t Enable)
{
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_SUB2, Enable);
}

const I2C_Error_t PCA9685_SwitchSub3(const Bool_t Enable)
{
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_SUB3, Enable);
}

const I2C_Error_t PCA9685_SwitchAllCall(const Bool_t Enable)
{
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE1, PCA9685_ALLCALL, Enable);
}

const I2C_Error_t PCA9685_SwitchInvert(const Bool_t Enable)
{
	return PCA9685_SwitchBit(PCA9685_REGISTER_MODE2, PCA9685_INVRT, Enable);
}

const I2C_Error_t PCA9685_SetPrescaler(const uint8_t Prescaler)
{
	return PCA9685_I2C_WRITEBYTE(PCA9685_REGISTER_PRESCALER, &Prescaler);
}

const I2C_Error_t PCA9685_GetPrescaler(uint8_t* Prescaler)
{
	return PCA9685_I2C_READBYTE(PCA9685_REGISTER_PRESCALER, Prescaler);
}

const I2C_Error_t PCA9685_SetChannel(const PCA9685_Channel_t Channel, const uint16_t On, const uint16_t Off)
{
	uint32_t Data = (((uint32_t)Off) << 0x10) | On;
	uint8_t PWMChannel = 0x00;
	
	if(Channel == PCA9685_CHANNEL_ALL)
	{
		PWMChannel = PCA9685_REGISTER_ALL_LED_ON_L;
	}
	else
	{
		PWMChannel = PCA9685_REGISTER_LED0_ON_L + (Channel << 0x02);
	}
	
	return PCA9685_I2C_WRITE(PWMChannel, (uint8_t*)&Data);
}

#if(defined PCA9685_OE_PORT & defined PCA9685_OE_PIN)
	void PCA9685_SwitchOutputEnable(const Bool_t Enable)
	{
		if(Enable == TRUE)
		{
			GPIO_Clear(PCA9685_OE_PORT, PCA9685_OE_PIN);
		}
		else
		{
			GPIO_Set(PCA9685_OE_PORT, PCA9685_OE_PIN);
		}
	}
#endif