/*
 * IR.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR NEC IR interface implementation.

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

/** @file Interfaces/IR/IR.c
 *  @brief AVR NEC IR interface implementation.
 *
 *  This contains the implementation of the IR interface.
 *
 *  @author Daniel Kampert
 */

#include "Interfaces/IR/IR.h"

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#include "Arch/XMega/Timer/Timer.h"
#else
	#error "Architecture not supported for IR remote service!"
#endif

/** @brief Length of the initial burst (9 ms) in timer ticks.
 */
#define IR_PROTOCOL_BURST			180

/** @brief Length of the space (4.5 ms) in timer ticks.
 */
#define IR_PROTOCOL_SPACE			180

/** @brief Length of the bit burst (562.5 us) in timer ticks
 */
#define IR_PROTOCOL_BIT_BURST		180

/** @brief Length of a logical zero bit space (562.5 us) in timer ticks
 */
#define IR_PROTOCOL_BIT_ZERO		180

/** @brief Length of a logical one space (1.6875 ms) in timer ticks
 */
#define IR_PROTOCOL_BIT_ONE			180

static void _Timer0Callback(void);

static IR_RecState_t _IRRecState;

// Configure the timer for a interrupt every 50 us
static Timer0_Config_t _TimerConfig = 
{
	.Device = &TCD0,
	.Prescaler = TIMER_PRESCALER_8,
	.Period = 207,
};

static Timer0_InterruptConfig_t _TimerInterrupt =
{
	.Device = &TCD0,
	.Source = TIMER_OVERFLOW_INTERRUPT,
	.InterruptLevel = INT_LVL_HI,
	.Callback = _Timer0Callback
};

static uint8_t Ticks;
static uint8_t Buffer[70];
static uint8_t BufferIndex;

static void _Timer0Callback(void)
{
	// Read the receiver pin
	uint8_t InputState = GPIO_Read(GET_PERIPHERAL(IR_INPUT), GET_INDEX(IR_INPUT));

	Ticks++;

	switch(_IRRecState)
	{
		// Wait for the leading burst to indicate a new transmission
		case IR_STATE_IDLE:
		{
			if(InputState)
			{
				// Not enough ticks for a valid leading burst
				if(Ticks < IR_PROTOCOL_BURST)
				{
					Ticks = 0x00;
				}
				// Receive a valid burst. Prepare to receive the following space.
				else
				{
					Ticks = 0x00;
					BufferIndex = 0x00;

					_IRRecState = IR_STATE_REC_ONE;
				}
			}

			break;
		}
		// Wait for a falling edge and store the ticks or wait for a transmission end
		// when no falling edge occurs
		case IR_STATE_REC_ONE:
		{
			if(!InputState)
			{
				Buffer[BufferIndex++] = Ticks;
				Ticks = 0x00;
				_IRRecState = IR_STATE_REC_ZERO;
			}
			else if(Ticks > 100)
			{
				_IRRecState = IR_STATE_STOP;
			}

			break;
		}
		// Wait for a rising edge and store the ticks
		case IR_STATE_REC_ZERO:
		{
			if(InputState)
			{
				Buffer[BufferIndex++] = Ticks;
				Ticks = 0x00;
				_IRRecState = IR_STATE_REC_ONE;
			}

			break;
		}
		// Hold the timer when the state machine is stopped
		case IR_STATE_STOP:
		{
			Ticks = 0x00;

			break;
		}
	}
}

void IR_Init(void)
{
	Timer0_Init(&_TimerConfig);
	Timer0_InstallCallback(&_TimerInterrupt);

	_IRRecState = IR_STATE_IDLE;
	
	PMIC_EnableAllInterruptLevel();
	EnableGlobalInterrupts();
}

void IR_Read(void)
{

}