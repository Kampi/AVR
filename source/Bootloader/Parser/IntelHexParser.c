/*
 * IntelHexParser.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Bootloader parser for the Intel-Hex file.

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

/** @file Common/Parser/IntelHexParser.c
 *  @brief Bootloader parser for the Intel-Hex file format.
 *
 *  This file contains the implementation for the Intel-Hex file format parser.
 *
 *  @author Daniel Kampert
 */

#include "Bootloader/Parser/IntelHexParser.h"

/** @brief States for the parsing state machine.
 */
typedef enum
{
	PARSER_INIT = 0x00,										/**< Initial state */
	PARSER_GET_SIZE = 0x01,									/**< Get the size from the line */
	PARSER_GET_ADDRESS = 0x02,								/**< Get the address from the line */
	PARSER_GET_TYPE = 0x03,									/**< Get the type and the data from the line */
	PARSER_GET_CHECK = 0x05,								/**< Get the checksum from the line */
	PARSER_HANDLE_ERROR = 0x06,								/**< Error handler */
} StateMachine_t;

/** @brief	Line buffer for the parsing state machine.
 */
static uint8_t _ParserBuffer[PARSER_LENGTH_BYTES + PARSER_ADDRESS_BYTES + PARSER_TYPE_BYTES + (PARSER_MAX_DATA_BYTES * PARSER_DATA_BYTES) + PARSER_CHECK_BYTES];

/** @brief	Boolean value to flag the parser as active.
 */
static bool _IsActive;

/** @brief	Current line buffer index.
 */
static uint8_t _Index;

/** @brief	Current state for the parser state machine.
 */
static StateMachine_t _ParserState;

/** @brief	Current state of the parsing engine.
 */
static Parser_State_t _ParserEngineState;

/** @brief			Convert a hexadecimal string number into an integer value.
 *  @param ASCII	Number as a string
 *  @param Length	String length
 *  @return			Integer value of the string
 */
static uint16_t Hex2Num(const uint8_t Length)
{
	uint16_t Temp = 0x00;

	for(uint8_t i = 0x00; i < Length; i++)
	{
		uint8_t c = _ParserBuffer[_Index++];

		if(c >= '0' && c <= '9')
		{
			c -= '0';
		}
		else if(c >= 'A' && c <= 'F')
		{
			c -= 'A' - 10;
		}
		else if(c >= 'a' && c <= 'f')
		{
			c -= 'a' - 10;
		}

		Temp = (Temp << 0x04) + c;
	}

	return Temp;
}

void Parser_Init(void)
{
	_ParserState = PARSER_INIT;
	_IsActive = false;
}

Parser_State_t Parser_GetByte(const uint8_t Received)
{
	if(_IsActive)
	{
		if(Received == PARSER_LINE_END)
		{
			_Index = 0x00;
			_IsActive = false;
			_ParserState = PARSER_INIT;

			return PARSER_STATE_SUCCESSFUL;
		}

		if(_Index < sizeof(_ParserBuffer))
		{
			_ParserBuffer[_Index++] = Received;
		}
		else
		{
			_Index = 0x00;

			return PARSER_STATE_OVERFLOW;
		}
	}

	// Wait for the beginning of a new line
	if(Received == ':')
	{
		_Index = 0x00;
		_IsActive = true;
	}

	return PARSER_STATE_BUSY;
}

Parser_State_t Parser_Parse(Parser_Block_t* Line)
{
	_ParserEngineState = PARSER_STATE_BUSY;

	do
	{
		switch(_ParserState)
		{
			case PARSER_INIT:
			{
				Line->Checksum = 0x00;
				Line->Length = 0x00;

				_ParserState = PARSER_GET_SIZE;

				break;
			}
			case PARSER_GET_SIZE:
			{
				uint8_t Length = Hex2Num(PARSER_LENGTH_BYTES);
				Line->Length = Length;
				Line->Checksum += Length;

				_ParserState = PARSER_GET_ADDRESS;

				break;
			}
			case PARSER_GET_ADDRESS:
			{
				uint16_t Address = Hex2Num(PARSER_ADDRESS_BYTES);
				Line->Address = Address;
				Line->Checksum += (Address >> 0x08) + (Address & 0xFF);

				_ParserState = PARSER_GET_TYPE;

				break;
			}
			case PARSER_GET_TYPE:
			{
				uint8_t Type = Hex2Num(PARSER_TYPE_BYTES);
				Line->Type = Type;
				Line->Checksum += Type;

				switch(Type)
				{
					case PARSER_TYPE_DATA:
					{
						for(uint8_t i = 0x00; i < Line->Length; i++)
						{
							uint8_t Data = Hex2Num(PARSER_DATA_BYTES);
							*(Line->pBuffer + i) = Data;
							Line->Checksum += Data;
						}

						_ParserState = PARSER_GET_CHECK;

						break;
					}
					case PARSER_TYPE_EOF:
					{
						_ParserState = PARSER_GET_CHECK;

						break;
					}
					case PARSER_TYPE_ESA:
					{
						Line->Offset = 0x00;

						for(uint8_t i = 0x00; i < 0x02; i++)
						{
							uint8_t Data = Hex2Num(PARSER_DATA_BYTES);
							Line->Offset |= Data;
							Line->Offset <<= 0x04;
							Line->Checksum += Data;
						}

						// Multiply the address with 4 (according to the specification)
						Line->Offset <<= 0x04;

						_ParserState = PARSER_GET_CHECK;

						break;
					}
					case PARSER_TYPE_SSA:
					{
						Line->StartAddress = 0x00;

						for(uint8_t i = 0x00; i < 0x04; i++)
						{
							uint8_t Data = Hex2Num(PARSER_DATA_BYTES);
							Line->StartAddress |= Data;
							Line->StartAddress <<= 0x04;
							Line->Checksum += Data;
						}

						_ParserState = PARSER_GET_CHECK;

						break;
					}
					case PARSER_TYPE_ELA:
					{
						break;
					}
					case PARSER_TYPE_SLA:
					{
						break;
					}
					default:
					{
						_ParserState = PARSER_HANDLE_ERROR;

						break;
					}
				}

				break;
			}
			case PARSER_GET_CHECK:
			{
				uint8_t Checksum_Temp = ~(Line->Checksum & 0xFF) + 0x01;
				Line->Checksum = Hex2Num(PARSER_CHECK_BYTES);

				if(Line->Checksum == Checksum_Temp)
				{
					Line->Valid = true;
					_ParserEngineState = PARSER_STATE_SUCCESSFUL;
				}
				else
				{
					Line->Valid = false;
					_ParserEngineState = PARSER_STATE_ERROR;
				}

				break;
			}
			case PARSER_HANDLE_ERROR:
			{
				_ParserEngineState = PARSER_STATE_ERROR;

				break;
			}
		}
	}
	while(_ParserEngineState == PARSER_STATE_BUSY);

	return _ParserEngineState;
}