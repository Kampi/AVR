/*
 * SD.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Low level SD card interface for AVR MCUs.

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

/** @file Peripheral/SD/SD.c
 *  @brief Low level SD card interface for AVR MCUs.
 *
 *  This file contains the implementation of the low level functions for the SD card driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#include "Peripheral/SD/SD.h"

/** @defgroup SD
 *  @{
 */
	/** @defgroup SD-Commands
	 *  SD card commands.
	 *  @{
	 */
		#define SD_CMD_GO_IDLE								0
		#define SD_CMD_SEND_OP_COND							1
		#define SD_CMD_ALL_SEND_CID							2
		#define SD_CMD_SEND_RELATIVE_ADDR					3
		#define SD_CMD_SET_DSR								4
		#define SD_CMD_SWITCH_FUNC							6
		#define SD_CMD_SELECT_DESELECT_CARD					7
		#define SD_CMD_IF_COND								8
		#define SD_CMD_SEND_CSD								9
		#define SD_CMD_SEND_CID								10
		#define SD_CMD_STOP_TRANSMISSION					12
		#define SD_CMD_SEND_STATUS							13
		#define SD_CMD_GO_INACTIVE							15
		#define SD_CMD_SET_BLOCKLEN							16
		#define SD_CMD_READ_SINGLE_BLOCK					17
		#define SD_CMD_READ_MULTIPLE_BLOCK					18
		#define SD_CMD_WRITE_SINGLE_BLOCK					24
		#define SD_CMD_WRITE_MULTIPLE_BLOCK					25
		#define SD_CMD_PROGRAMM_CSD							27
		#define SD_CMD_SET_WRITE_PROT						28
		#define SD_CMD_SET_CLR_PROT							29
		#define SD_CMD_SET_SEND_PROT						30
		#define SD_CMD_ERASE_WR_BLK_START					32
		#define SD_CMD_ERASE_WR_BLK_END						33
		#define SD_CMD_ERASE								38
		#define SD_CMD_APP_SEND_OP_COND						41
		#define SD_CMD_LOCK_UNLOCK							42
		#define SD_CMD_APP_CMD								55
		#define SD_CMD_READ_OCR								58
		 
		#define SD_CMD_ACMD13								(0x80 | SD_CMD_SEND_STATUS)
		#define SD_CMD_ACMD41								(0x80 | SD_CMD_APP_SEND_OP_COND)
	/** @} */ // end of Commands

	/** @defgroup SD-Token
	 *  SD card command tokens.
	 *  @{
	 */
		#define SD_TOKEN_DATA_CMD25							0xFC								/**< Data token for CMD25 */
		#define SD_TOKEN_STOP								0xFD								/**< Stop token for CMD25 */
		#define SD_TOKEN_DATA								0xFE								/**< Data token for CMD17/28/24 */
	/** @} */ // end of Token

	/** @defgroup SD-Errors
	 *  SD card error codes.
	 *  @{
	 */
		#define SD_STATE_SUCCESSFULL						0x00								/**< No error */
		#define SD_STATE_IDLE								0x01								/**< SD card enters idle state */
		#define SD_STATE_RESET								0x02								/**< Reset state */
		#define SD_STATE_ILLEGAL_COMMAND					0x04								/**< Illegal command error */
		#define SD_STATE_CRC_ERROR							0x08								/**< CRC error */
		#define SD_STATE_ERASE_ERROR						0x10								/**< Error during erase operation */
		#define SD_STATE_ADDRESS_ERROR						0x20								/**< Address error */
		#define SD_STATE_PARAMETER_ERROR					0x40								/**< Command parameter error */
	/** @} */ // end of Errors
/** @} */ // end of SD

#ifndef DOXYGEN
	static uint32_t Wait = 0x00;
	
	SD_Callback_t __SD_Callback__;
#endif

/*
 * Current card type
*/	
static SD_CardType_t __CardType = SD_VER_UNKNOWN;

/*
 *	Current SD card status
*/
static SD_Status_t __CardStatus;

#ifdef SD_WITH_SWA
	/** @brief	SD card insert callback.
	 */
	static void SD_CardCallback(void)
	{
		if(__SD_Callback__ != NULL)
		{
			if(SD_CheckForCard())
			{
				__SD_Callback__(SD_STATE_REMOVED);
			}
			else
			{
				__SD_Callback__(SD_STATE_INSERT);
			}
		}
	}
#endif

/** @brief	Select the SD card
 */
static void SD_Select(void)
{
	// Create 8 clock pulse before activating the card
	SPIM_TRANSMIT(SD_INTERFACE, 0xFF);

	SPIM_CHIP_SELECT(SD_SS_PORT, SD_SS_PIN);
}

/** @brief	Deselect the SD card
 */
static void SD_Deselect(void)
{
	SPIM_CHIP_DESELECT(SD_SS_PORT, SD_SS_PIN);

	// Create 80 clock pulse after releasing the card
	for(uint8_t i = 0x00; i < 0x0A; i++)
	{
		SPIM_TRANSMIT(SD_INTERFACE,0xFF);
	}
}

/** @brief			Send a command to the SD card.
 *  @param Command	Command code
 *  @param Arg		Command argument
 *  @return			R1 response
 */
static const uint8_t SD_SendCommand(const uint8_t Command, const uint32_t Arg)
{
	uint8_t Response = 0x00;
	uint8_t CommandTemp = Command;

	// Dummy CRC + Stop
	uint8_t Checksum = 0x01;

	SD_Select();

	// Send ACMD<n> command
	if(CommandTemp & 0x80)
	{
		// Clear ACMD-Flag
		CommandTemp &= 0x7F;
		
		Response = SD_SendCommand(SD_ID_TO_CMD(SD_CMD_APP_CMD), 0x00);
		if(Response > 0x01)
		{
			return Response;
		}
	}

	SPIM_TRANSMIT(SD_INTERFACE, CommandTemp);
	SPIM_TRANSMIT(SD_INTERFACE, (Arg >> 0x18) & 0xFF);
	SPIM_TRANSMIT(SD_INTERFACE, (Arg >> 0x10) & 0xFF);
	SPIM_TRANSMIT(SD_INTERFACE, (Arg >> 0x08) & 0xFF);
	SPIM_TRANSMIT(SD_INTERFACE, Arg);

	if(CommandTemp == SD_ID_TO_CMD(SD_CMD_GO_IDLE))
	{
		// Valid CRC for CMD0(0)
		Checksum = 0x95;
	}
	else if(CommandTemp == SD_ID_TO_CMD(SD_CMD_IF_COND))
	{
		// Valid CRC for CMD8(0x1AA)
		Checksum = 0x87;
	}

	SPIM_TRANSMIT(SD_INTERFACE, Checksum);

	if(CommandTemp == SD_ID_TO_CMD(SD_CMD_STOP_TRANSMISSION))
	{
		// Skip stuff byte when transmission stop
		SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
	}

	// Wait for the response (0 - 8 bytes for SD cards and 1 - 8 bytes for MMC)
	for(uint8_t i = 0x00; i < 0x08; i++)
	{
		uint8_t DataIn = SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
		if(DataIn != 0xFF)
		{
			// 8 dummy cycles if the command is a write command
			if(SD_ID_TO_CMD(SD_CMD_WRITE_SINGLE_BLOCK) || SD_ID_TO_CMD(SD_CMD_WRITE_MULTIPLE_BLOCK))
			{
				SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
			}

			return DataIn;
		}
	}

	return SD_NO_RESPONSE;
}

/** @brief			Read a single block of data from the SD card.
 *  @param Length	Block length
 *  @param Buffer	Pointer to data buffer
 *  @return			Error code
 */
static const SD_Error_t SD_ReadBlock(const uint32_t Length, uint8_t* Buffer)
{
	uint8_t Response = 0xFF;

	// Wait for the data token
	Wait = 0x00;
	while((++Wait < 0x2710) && (Response == 0xFF))
	{
		Response = SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
		if(Wait >= 0x2710)
		{
			SD_Deselect();

			return SD_NO_RESPONSE;
		}
	}

	// Get the data
	for(uint32_t i = 0x00; i < Length; i++)
	{
		*Buffer++ = SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
	}

	// Skip checksum
	SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
	SPIM_TRANSMIT(SD_INTERFACE, 0xFF);

	return SD_SUCCESSFULL;
}

/** @brief			Read a single block of data from the SD card.
 *  @param Buffer	Pointer to data buffer
 *  @param Length	Block length
 *  @param Token	Stop token
 *  @return			Error code
 */
static const SD_Error_t SD_WriteBlock(const uint8_t* Buffer, const uint32_t Length, const uint8_t Token)
{
	uint8_t* Buffer_Temp = (uint8_t*)Buffer;

	// Send the token
	SPIM_TRANSMIT(SD_INTERFACE, Token);
	if(Token != SD_TOKEN_STOP)
	{
		// Send the data
		for(uint32_t i = 0x00; i < Length; i++)
		{
			SPIM_TRANSMIT(SD_INTERFACE, *Buffer_Temp++);
		}

		// Skip checksum
		SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
		SPIM_TRANSMIT(SD_INTERFACE, 0xFF);

		// Get the response
		if((SPIM_TRANSMIT(SD_INTERFACE, 0xFF) & 0x1F) != 0x05)
		{
			return SD_NO_RESPONSE;
		}
	}

	// Wait until the card clears busy state
	while(SPIM_TRANSMIT(SD_INTERFACE, 0xFF) != 0xFF);

	return SD_SUCCESSFULL;
}

/** @brief		Perform a software reset with the SD card.
 *  @return		Error code
 */
static const SD_Error_t SD_SoftwareReset(void)
{
	uint8_t Repeat = 0x00;

	for(uint8_t i = 0x00; i < 0x0A; i++)
	{
		SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
	}

	while(SD_SendCommand(SD_ID_TO_CMD(SD_CMD_GO_IDLE), 0x00) != SD_STATE_IDLE)
	{
		// Initialization failed
		if(Repeat++ == 0x0A)
		{
			SD_Deselect();

			return SD_NO_RESPONSE;
		}
	}

	SD_Deselect();

	return SD_SUCCESSFULL;
}

/** @brief		Initialize the SD card.
 *  @return		Error code
 */
static const SD_Error_t SD_InitializeCard(void)
{
	uint8_t Response = 0x00;
	uint8_t Buffer[4];

	Response = SD_SendCommand(SD_ID_TO_CMD(SD_CMD_IF_COND), 0x1AA);
	for(uint8_t i = 0x00; i < 0x04; i++)
	{
		Buffer[i] = SPIM_TRANSMIT(SD_INTERFACE, 0xFF);	
	}

	if(Response == SD_STATE_IDLE)
	{
		/*
			Version 2 or later SD card
		*/

		uint32_t R3 = ((uint32_t)Buffer[3]) << 0x18;
		R3 |= ((uint32_t)Buffer[2]) << 0x10;
		R3 |= ((uint32_t)Buffer[1]) << 0x08;
		R3 |= ((uint32_t)Buffer[0]);

		// Send ACMD41 and check for ready
		Wait = 0x00;
		while((++Wait < 0x2710) && (SD_SendCommand(SD_ID_TO_CMD(SD_CMD_ACMD41), ((uint32_t)0x01 << 0x1E)) != 0x00))
		{
			if(Wait >= 0x2710)
			{
				SD_Deselect();

				return SD_NO_RESPONSE;
			}
		}

		// Send CMD58 to read OCR
		Response = SD_SendCommand(SD_ID_TO_CMD(SD_CMD_READ_OCR), 0x00);
		for(uint8_t i = 0x00; i < 0x04; i++)
		{
			Buffer[i] = SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
		}

		if(Response == SD_STATE_SUCCESSFULL)
		{
			R3 = ((uint32_t)Buffer[3]) << 0x18;
			R3 |= ((uint32_t)Buffer[2]) << 0x10;
			R3 |= ((uint32_t)Buffer[1]) << 0x08;
			R3 |= ((uint32_t)Buffer[0]);
		}
		else
		{
			__CardType = SD_VER_UNKNOWN;
		}

		// Check if the CCS bit is set
		if(R3 & ((uint32_t)0x01 << 0x1E))
		{
			__CardType = SD_VER_2_HI;
		}
		else
		{
			__CardType = SD_VER_2_STD;
		}
	}
	else if(Response & SD_STATE_ILLEGAL_COMMAND)
	{
		/*
			Version 1 SD card or MMC
		*/

		// Check for version 1 SD card
		Wait = 0x00;
		while(++Wait < 0xFF) 
		{
			if(SD_SendCommand(SD_ID_TO_CMD(SD_CMD_ACMD41), 0x00) == SD_STATE_SUCCESSFULL)
			{
				__CardType = SD_VER_1_STD;

				break;
			}
		}

		// Check for multimedia card
		Wait = 0x00;
		if(Response & SD_STATE_ILLEGAL_COMMAND)
		{
			while(++Wait < 0xFF)
			{
				if(SD_SendCommand(SD_ID_TO_CMD(SD_CMD_SEND_OP_COND), 0x00) == SD_STATE_SUCCESSFULL) 
				{
					__CardType = SD_MMC;

					break;
				}
			}
		}
	}
	else
	{
		SD_Deselect();

		return Response;
	}

	SD_Deselect();

	return SD_SUCCESSFULL;
}

/** @brief				Set the block length of the SD card.
 *						NOTE: Only available for version 1 cards or MMC. 
 *  @param BlockLength	Block length
 *  @return				Error code
 */
static SD_Error_t SD_SetBlockLength(const uint16_t BlockLength)
{
	SD_Error_t ErrorCode = SD_SendCommand(SD_ID_TO_CMD(SD_CMD_SET_BLOCKLEN), BlockLength);
	SD_Deselect();

	return ErrorCode;
}

const SD_Error_t SD_Init(SPIM_Config_t* Config)
{
	uint32_t OldFreq = 0x00;
	SD_Error_t ErrorCode = SD_SUCCESSFULL;

	// Set SD card SS pin as output in high state
	GPIO_SetDirection(SD_SS_PORT, SD_SS_PIN, GPIO_DIRECTION_OUT);
	GPIO_Set(SD_SS_PORT, SD_SS_PIN);

	if(Config != NULL)
	{
		SPIM_INIT(Config);
	}

	// Use 100 kHz to initialize the card
	OldFreq = SPIM_GET_CLOCK(SD_INTERFACE, SysClock_GetClock());
	SPIM_SET_CLOCK(SD_INTERFACE, 100000, SysClock_GetClock());

	// Reset the SD card
	ErrorCode = SD_SoftwareReset();
	if(ErrorCode != SD_SUCCESSFULL)
	{
		return ErrorCode;
	}

	// Initialize the SD card
	ErrorCode = SD_InitializeCard();
	if(ErrorCode != SD_SUCCESSFULL)
	{
		return ErrorCode;
	}

	// Change the frequency back to the old value
	SPIM_SET_CLOCK(SD_INTERFACE, OldFreq, SysClock_GetClock());

	// Set the block length to 512 (only necessary if the card is not a SDXC or SDHX card)
	if((__CardType != SD_VER_2_HI) && (__CardType != SD_VER_2_STD))
	{
		ErrorCode = SD_SetBlockLength(SD_BLOCK_SIZE);
		if(ErrorCode != SD_SUCCESSFULL)
		{
			return ErrorCode;
		}
	}

	return ErrorCode;
}

#ifdef SD_WITH_SWA
	void SD_InstallCallback(SD_Callback_t Callback)
	{
		GPIO_InterruptConfig_t Interrupt_SWA = {
			.Port = SD_SWA_PORT,
			.Pin = SD_SWA_PIN,
			.Channel = GPIO_INTERRUPT_0,
			.InterruptLevel = INT_LVL_LO,
			.Sense = GPIO_SENSE_BOTH,
			.PullConfig = GPIO_OUTPUTCONFIG_PULLUP,
			.Callback = SD_CardCallback
		};
		
		// Set the card SWA pin as input
		GPIO_SetDirection(SD_SWA_PORT, SD_SWA_PIN, GPIO_DIRECTION_IN);
		GPIO_InstallCallback(&Interrupt_SWA);
		
		__SD_Callback__ = Callback;
	}
	
	void SD_RemoveCallback(void)
	{
		__SD_Callback__ = NULL;
	}
#endif

void SD_Sync(void)
{
	SD_Deselect();
}

const SD_Error_t SD_GetCSD(SD_CSD_t* CSD)
{
	uint8_t* Ptr = (uint8_t*)CSD;

	// Get the data
	if((SD_SendCommand(SD_ID_TO_CMD(SD_CMD_SEND_CSD), 0x00) == SD_SUCCESSFULL) && (SD_ReadBlock(sizeof(SD_CSD_t), Ptr) == SD_SUCCESSFULL))
	{
		SD_Deselect();

		return SD_SUCCESSFULL;
	}

	SD_Deselect();

	return SD_NO_RESPONSE;
}

const SD_Error_t SD_GetCID(SD_CID_t* CID)
{
	uint8_t* Ptr = (uint8_t*)CID;

	// Get the data
	if((SD_SendCommand(SD_ID_TO_CMD(SD_CMD_SEND_CID), 0x00) == SD_SUCCESSFULL) && (SD_ReadBlock(sizeof(SD_CID_t), Ptr) == SD_SUCCESSFULL))
	{
		SD_Deselect();

		return SD_SUCCESSFULL;
	}

	SD_Deselect();

	return SD_NO_RESPONSE;
}

const SD_Error_t SD_GetOCR(SD_OCR_t* OCR)
{
	uint8_t* Ptr = (uint8_t*)OCR;

	if(SD_SendCommand(SD_ID_TO_CMD(SD_CMD_READ_OCR), 0x00) != SD_SUCCESSFULL)
	{
		SD_Deselect();

		return SD_NO_RESPONSE;
	}

	// Get the data
	for(uint8_t i = 0x00; i < sizeof(SD_OCR_t); i++)
	{
		*Ptr++ = SPIM_TRANSMIT(SD_INTERFACE, 0xFF);
	}

	SD_Deselect();

	return SD_SUCCESSFULL;
}

const SD_Error_t SD_GetStatus(SD_Status_t* Status)
{
	uint8_t* Ptr = (uint8_t*)Status;

	if((SD_SendCommand(SD_ID_TO_CMD(SD_CMD_ACMD13), 0x00) == SD_SUCCESSFULL) && (SD_ReadBlock(sizeof(SD_Status_t), Ptr) == SD_SUCCESSFULL))
	{
		SD_Deselect();

		return SD_SUCCESSFULL;
	}

	SD_Deselect();

	return SD_NO_RESPONSE;
}

const SD_Error_t SD_GetSectors(uint32_t* Sectors)
{
	uint32_t Size;
	uint8_t CSD[16];

	SD_Error_t ErrorCode = SD_GetCSD((SD_CSD_t*)CSD);
	if(ErrorCode != SD_SUCCESSFULL)
	{
		return ErrorCode;
	}

	/*
		Version 2 or later SD card
	*/
	if((__CardType == SD_VER_2_STD) || (__CardType == SD_VER_2_HI))
	{
		Size = CSD[9] + (CSD[8] << 8) + 0x01;
		*Sectors = Size << 0x0A;
	}
	/*
		Version 1 or MMC
	*/
	else
	{
		uint8_t Shift = (CSD[5] & 0x0E) + ((CSD[10] & 0x80) >> 0x07) + ((CSD[9] & 0x03) << 0x01) + 0x02;
		Size = (CSD[8] >> 0x06) + (CSD[7] << 0x02) + ((CSD[6] & 0x03) << 0x0A) + 0x01;
		*Sectors = Size << (Shift - 0x09);
	}

	return SD_SUCCESSFULL;
}

const SD_Error_t SD_GetEraseBlockSize(uint16_t* EraseSize)
{
	/*
		Version 2 or later SD card
	*/
	if((__CardType == SD_VER_2_STD) || (__CardType == SD_VER_2_HI))
	{
		SD_Error_t Error = SD_GetStatus(&__CardStatus);

		*EraseSize = __CardStatus.ERASE_SIZE;

		return Error;
	}
	/*
		Version 1 or MMC
	*/
	else
	{
		uint8_t Buffer[16];

		if((SD_SendCommand(SD_ID_TO_CMD(SD_CMD_SEND_CSD), 0x00) != 0x00) && (SD_ReadBlock(16, Buffer) != SD_SUCCESSFULL))
		{
			SD_Deselect();

			return SD_NO_RESPONSE;
		}

		if(__CardType == SD_VER_1_STD)
		{
			// ToDo
		}
		else
		{
			// ToDo
		}
	}

	SD_Deselect();

	return SD_SUCCESSFULL;
}

const SD_CardType_t SD_GetCardType(void)
{
	return __CardType;
}

const SD_Error_t SD_ReadDataBlock(const uint32_t Address, uint8_t* Buffer)
{
	if((SD_SendCommand(SD_ID_TO_CMD(SD_CMD_READ_SINGLE_BLOCK), Address) == SD_SUCCESSFULL) && (SD_ReadBlock(SD_BLOCK_SIZE, Buffer) == SD_SUCCESSFULL))
	{
		SD_Deselect();

		return SD_SUCCESSFULL;
	}

	SD_Deselect();

	return SD_NO_RESPONSE;
}

const SD_Error_t SD_ReadDataBlocks(const uint32_t Address, const uint32_t Blocks, uint8_t* Buffer)
{
	SD_Error_t ErrorCode;

	// Send the command
	if(SD_SendCommand(SD_ID_TO_CMD(SD_CMD_READ_MULTIPLE_BLOCK), Address) != SD_SUCCESSFULL)
	{
		SD_Deselect();

		return SD_NO_RESPONSE;
	}

	// Read all data
	for(uint32_t i = 0x00; i < Blocks; i++)
	{
		ErrorCode = SD_ReadBlock(SD_BLOCK_SIZE, Buffer);
		if(ErrorCode != SD_SUCCESSFULL)
		{
			SD_Deselect();

			return ErrorCode;
		}

		Buffer += SD_BLOCK_SIZE;
	}

	// Send stop token
	SD_SendCommand(SD_ID_TO_CMD(SD_CMD_STOP_TRANSMISSION), 0x00);
		
	SD_Deselect();

	return SD_SUCCESSFULL;
}

const SD_Error_t SD_WriteDataBlock(const uint32_t Address, const uint8_t* Buffer)
{
	if((SD_SendCommand(SD_ID_TO_CMD(SD_CMD_WRITE_SINGLE_BLOCK), Address) == SD_SUCCESSFULL) && (SD_WriteBlock(Buffer, SD_BLOCK_SIZE, SD_TOKEN_DATA) == SD_SUCCESSFULL))
	{
		SD_Deselect();

		return SD_SUCCESSFULL;
	}

	SD_Deselect();

	return SD_NO_RESPONSE;
}

const SD_Error_t SD_WriteDataBlocks(const uint32_t Address, const uint32_t Blocks, const uint8_t* Buffer)
{
	SD_Error_t ErrorCode;

	// Send the command
	if(SD_SendCommand(SD_ID_TO_CMD(SD_CMD_WRITE_MULTIPLE_BLOCK), Address) != SD_SUCCESSFULL)
	{
		SD_Deselect();

		return SD_NO_RESPONSE;
	}

	// Write all data
	for(uint32_t i = 0x00; i < Blocks; i++)
	{
		ErrorCode = SD_WriteBlock(Buffer, SD_BLOCK_SIZE, SD_TOKEN_DATA_CMD25);
		if(ErrorCode != SD_SUCCESSFULL)
		{
			SD_Deselect();

			return ErrorCode;
		}

		Buffer += SD_BLOCK_SIZE;
	}

	// Send stop token
	SD_WriteBlock(NULL, 0, SD_TOKEN_STOP);

	SD_Deselect();

	return SD_SUCCESSFULL;
}