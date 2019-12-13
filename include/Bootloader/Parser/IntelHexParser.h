/*
 * IntelHexParser.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Parser for the Intel-Hex file.

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

/** @file Common/Parser/IntelHexParser.h
 *  @brief Parser for the Intel-Hex file format.
 *
 *  This contains the prototypes and definitions for the Intel-Hex file format parser.
 *
 *  @author Daniel Kampert
 */

#ifndef INTELHEXPARSER_H_
#define INTELHEXPARSER_H_

 #include "Common/Common.h"

 #define PARSER_MAX_DATA_BYTES		16							/**< Maximum data bytes per record */

 #define PARSER_LENGTH_BYTES		0x02						/**< Length of the length field in bytes */
 #define PARSER_ADDRESS_BYTES		0x04						/**< Length of the address field in bytes */
 #define PARSER_TYPE_BYTES			0x02						/**< Length of the type field in bytes */
 #define PARSER_DATA_BYTES			0x02						/**< Length of the data field in bytes */
 #define PARSER_CHECK_BYTES			0x02						/**< Length of the checksum field in bytes */
 
 #define PARSER_LINE_END			0x0D						/**< Line ending for the record. Intel tools add a 0x0D, 0x0A to the end of each line */
 #define PARSER_START_BYTE			0x00						/**< Start byte for the current record */

 /** @brief State of the Intel hex file parser.
  */
 typedef enum
 {
	 PARSER_STATE_BUSY = 0x00,									/**< Busy */
	 PARSER_STATE_SUCCESSFULL = 0x01,							/**< Valid record line */
	 PARSER_STATE_ERROR = 0x02,									/**< Error while parsing the line */
	 PARSER_STATE_OVERFLOW = 0x03,								/**< Buffer overflow during line receive */
 } Parser_State_t;

 /** @brief Record types
  */
 typedef enum
 {
	 PARSER_TYPE_DATA = 0x00,									/**< Data record */
	 PARSER_TYPE_EOF = 0x01,									/**< End of file record */
	 PARSER_TYPE_ESA = 0x02,									/**< Extended segment address record */
	 PARSER_TYPE_SSA = 0x03,									/**< Start segment address record */
	 PARSER_TYPE_ELA = 0x04,									/**< Extended linear address record */
	 PARSER_TYPE_SLA = 0x05,									/**< Start linear address record */
 } Parser_Type_t;

 /** @brief Intel-Hex file line object
  */
 typedef struct
 {
	 uint8_t Bytes;												/**< Byte count */
	 uint16_t Address;											/**< Address */
	 Parser_Type_t Type;										/**< Record type */
	 unsigned char* pBuffer;									/**< Pointer to data buffer */
	 uint16_t Checksum;											/**< Checksum */
	 Bool_t Valid;												/**< Valid flag */
 } Parser_Line_t;

 /** @brief	Initialize the parser.
  */
 void Parser_Init(void);

 /** @brief				Receive a byte, store it into the line buffer and wait for a line.
  *  @param Received	Received byte
  *  @return			#PARSER_STATE_SUCCESSFULL when line end
  */
 Parser_State_t Parser_GetLine(const uint8_t Received);
 
 /** @brief			Parse the line buffer into a #IntelHexParser_Line_t object. Must be called after #IntelHexParser_GetLine.
  *  @param Line	Pointer to line object
  *  @return		Parser state. Must be polled until #PARSER_STATE_SUCCESSFULL or #PARSER_STATE_ERROR is returned
  */
 Parser_State_t IntelParser_ParseLine(Parser_Line_t* Line);

#endif /* INTELHEXPARSER_H_ */