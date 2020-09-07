/*
 * NEC_IR.c
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

/** @file Interfaces/IR-Remote/NEC_IR.c
 *  @brief NEC IR interface driver for AVR.
 *
 *  This contains the implementation of the IR interface.
 *
 *  @author Daniel Kampert
 */

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#include "Arch/XMega/Timer/Timer.h"
#else
	#error "Architecture not supported for IR remote service!"
#endif

#include "Interfaces/IR-Remote/NEC_IR.h"

/** @brief Timing tolerance in timer ticks.
 */
#define IR_PROTOCOL_TOLERANCE		5

/** @brief Length of the initial burst (9 ms) in timer ticks.
 */
#define IR_PROTOCOL_BURST			180

/** @brief Length of the space (4.5 ms) in timer ticks.
 */
#define IR_PROTOCOL_SPACE			90

/** @brief Length of the bit burst (562.5 us) in timer ticks.
 */
#define IR_PROTOCOL_BIT_BURST		13

/** @brief Length of a logical zero bit space (562.5 us) in timer ticks.
 */
#define IR_PROTOCOL_BIT_ZERO		13

/** @brief Length of a logical one space (1.6875 ms) in timer ticks.
 */
#define IR_PROTOCOL_BIT_ONE			34

/** @brief Interval of the timer ticks in us.
 */
#define IR_TICK_INTERVAL			50

/** @brief Maximum message length of an NEC IR message.
 */
#define IR_MESSAGE_LENGTH			66

/** @brief Timer callback for the IR receiver state machine.
 */
static void _IR_Timer0OverflowCallback(void);

/** @brief Timer configuration object to create an interrupt every 50 us.
 */
extern Timer0_Config_t _IR_TimerConfig;

/** @brief Timer interrupt configuration object.
 */
extern Timer0_InterruptConfig_t _IR_TimerInterrupt;

/** @brief Timer ticks to store the bit length information.
 */
static uint8_t _IR_TimerTicks;

/** @brief Timer ticks for the timeout.
 */
static uint8_t _IR_TimeOutTicks;

/** @brief Temporary pointer to message object during the message receive process.
 */
static IR_Message_t* _IR_MessageBuffer;

/** @brief Message buffer for the bit timings. The buffer contains two bytes for
 *		   the pulse and space time of each bit, the timing of the first space 
 *		   after the leading burst and the final pulse.
 */
static uint8_t _IR_Data[IR_MESSAGE_LENGTH];

/** @brief Current state of the receiver state machine.
 */
static IR_RecState_t _IR_RecState;

/** @brief Overflow callback for the timer device. This callback handles 
 *		   the timing measurement for the IR protocol and the IR state machine. 
 */
static void _IR_Timer0OverflowCallback(void)
{
	uint8_t InputState = GPIO_Read(GET_PERIPHERAL(IR_REC_INPUT), GET_INDEX(IR_REC_INPUT));

	_IR_TimerTicks++;

	switch(_IR_RecState)
	{
		// Wait for a falling edge of the leading burst to indicate a new transmission
		case IR_STATE_IDLE:
		{
			if(InputState)
			{
				// Not enough ticks for a valid leading burst
				if(_IR_TimerTicks < IR_PROTOCOL_BURST)
				{
					// No leading burst detected. Increase the timeout.
					_IR_TimeOutTicks++;					
				}
				// Receive a valid burst. Prepare to receive the following space.
				else
				{
					_IR_MessageBuffer->Length = 0x00;

					_IR_RecState = IR_STATE_REC_SPACE;
				}

				_IR_TimerTicks = 0x00;
			}

			break;
		}
		// Wait for a falling edge and store the ticks or wait for a transmission end
		// when no falling edge occurs
		case IR_STATE_REC_SPACE:
		{
			if(!InputState)
			{
				_IR_Data[_IR_MessageBuffer->Length++] = _IR_TimerTicks;
				_IR_TimerTicks = 0x00;

				#if(defined(IR_USE_LED))
					GPIO_Set(GET_PERIPHERAL(IR_ACTIVE_LED), GET_INDEX(IR_ACTIVE_LED));
				#endif

				_IR_RecState = IR_STATE_REC_BURST;
			}
			else if(_IR_TimerTicks > (IR_TIMEOUT / IR_TICK_INTERVAL))
			{
				_IR_RecState = IR_STATE_STOP;
			}

			break;
		}
		// Wait for a rising edge and store the ticks
		case IR_STATE_REC_BURST:
		{
			if(InputState)
			{
				_IR_Data[_IR_MessageBuffer->Length++] = _IR_TimerTicks;
				_IR_TimerTicks = 0x00;

				#if(defined(IR_USE_LED))
					GPIO_Clear(GET_PERIPHERAL(IR_ACTIVE_LED), GET_INDEX(IR_ACTIVE_LED));
				#endif

				_IR_RecState = IR_STATE_REC_SPACE;
			}

			break;
		}
		// Hold the timer when the state machine is stopped
		case IR_STATE_STOP:
		{
			_IR_MessageBuffer->Valid = true;

			break;
		}
	}
}

/** @brief Process the received message and create the final output message.
 */
static bool _IR_ProcessData(void)
{
	if(_IR_MessageBuffer->Length == 0x02)
	{
		_IR_MessageBuffer->IsRepeat = true;
		_IR_MessageBuffer->Data.Value = 0x00;

		// Check the timing for a valid repeat code. Skip the final pulse
		if((_IR_Data[0x00] < ((IR_PROTOCOL_SPACE / 2) + IR_PROTOCOL_TOLERANCE)) && (_IR_Data[0x00] > ((IR_PROTOCOL_SPACE / 2) - IR_PROTOCOL_TOLERANCE)))
		{
			_IR_MessageBuffer->Valid = true;
		}
		else
		{
			_IR_MessageBuffer->Valid = false;
		}
	}
	else
	{
		_IR_MessageBuffer->IsRepeat = false;

		// Check for a valid space after the initial burst
		if((_IR_Data[0] > (IR_PROTOCOL_SPACE + IR_PROTOCOL_TOLERANCE)) || (_IR_Data[0] < (IR_PROTOCOL_SPACE - IR_PROTOCOL_TOLERANCE)))
		{
			_IR_MessageBuffer->Valid = false;
		}
		else
		{
			// Process the remaining message. Skip the final pulse
			for(uint8_t i = 0x01; i < (_IR_MessageBuffer->Length - 0x01); i += 0x02)
			{
				_IR_MessageBuffer->Data.Value <<= 0x01;

				// Compare the pulse and space timings to make a decision about the received bit
				if((_IR_Data[i] < (IR_PROTOCOL_BIT_BURST + IR_PROTOCOL_TOLERANCE)) && (_IR_Data[i] > (IR_PROTOCOL_BIT_BURST - IR_PROTOCOL_TOLERANCE)) &&
				   (_IR_Data[i + 0x01] < (IR_PROTOCOL_BIT_ZERO + IR_PROTOCOL_TOLERANCE)) && (_IR_Data[i + 0x01] > (IR_PROTOCOL_BIT_ZERO - IR_PROTOCOL_TOLERANCE))
				)
				{
					_IR_MessageBuffer->Data.Value |= 0x00;
				}
				else if((_IR_Data[i] < (IR_PROTOCOL_BIT_BURST + IR_PROTOCOL_TOLERANCE)) && (_IR_Data[i] > (IR_PROTOCOL_BIT_BURST - IR_PROTOCOL_TOLERANCE)) &&
						(_IR_Data[i + 0x01] < (IR_PROTOCOL_BIT_ONE + IR_PROTOCOL_TOLERANCE)) && (_IR_Data[i + 0x01] > (IR_PROTOCOL_BIT_ONE - IR_PROTOCOL_TOLERANCE))
				)
				{
					_IR_MessageBuffer->Data.Value |= 0x01;
				}
			}

			// Check the data for errors by comparing the inverse fields with the non-inverse fields
			if((_IR_MessageBuffer->Data.Field[0x00] == (~_IR_MessageBuffer->Data.Field[0x01])) &&
			   (_IR_MessageBuffer->Data.Field[0x02] == (~_IR_MessageBuffer->Data.Field[0x03])))
			{
				_IR_MessageBuffer->Valid = false;
			}
			else
			{
				_IR_MessageBuffer->Valid = true;
			}
		}
	}

	return _IR_MessageBuffer->Valid;
}

void IR_Init(void)
{
	// Configure the receiver pin as input
	GPIO_SetDirection(GET_PERIPHERAL(IR_REC_INPUT), GET_INDEX(IR_REC_INPUT), GPIO_DIRECTION_IN);

	#if(defined(IR_USE_LED))
		GPIO_SetDirection(GET_PERIPHERAL(IR_ACTIVE_LED), GET_INDEX(IR_ACTIVE_LED), GPIO_DIRECTION_OUT);
	#endif

	// Calculate the period value for the timer
	_IR_TimerConfig.Period = F_CPU / ((((uint32_t)0x01) << (_IR_TimerConfig.Prescaler - 0x01)) * (1000000 / IR_TICK_INTERVAL));
	_IR_TimerInterrupt.Callback = _IR_Timer0OverflowCallback;

	Timer0_Init(&_IR_TimerConfig);
	Timer0_InstallCallback(&_IR_TimerInterrupt);

	#if(MCU_ARCH == MCU_ARCH_XMEGA)
		PMIC_EnableInterruptLevel(_IR_TimerInterrupt.InterruptLevel);
	#endif

	EnableGlobalInterrupts();
}

bool IR_GetMessage(IR_Message_t* Message)
{
	_IR_MessageBuffer = Message;
	_IR_MessageBuffer->Valid = false;

	_IR_TimerTicks = 0x00;
	_IR_TimeOutTicks = 0x00;
	_IR_RecState = IR_STATE_IDLE;

	while(!_IR_MessageBuffer->Valid)
	{
		if(_IR_TimeOutTicks > (IR_TIMEOUT / IR_TICK_INTERVAL))
		{
			_IR_RecState = IR_STATE_STOP;

			// Clear the valid flag
			_IR_MessageBuffer->Valid = false;

			return _IR_MessageBuffer->Valid;
		}
	}

	return _IR_ProcessData();
}