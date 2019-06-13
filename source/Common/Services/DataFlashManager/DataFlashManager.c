/*
 * DataFlashManager.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Data flash memory management service.

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

/** @file Common/Services/DataFlashManager/DataFlashManager.c
 *  @brief Data flash memory management service.
 *
 *  This contains the implementation of the flash memory service.
 *
 *  @author Daniel Kampert
 */

#include "Common/Services/DataFlashManager/DataFlashManager.h"

static SPIM_Config_t Config_SPI = {
	.EnableDoubleSpeed = FALSE,
	.DataOrder = SPI_DATAORDER_MSB_FIRST,
	.Prescaler = SPI_PRESCALER_16,
	.Mode = SPI_MODE_0
};

static void DataFlashManager_WaitBusy(const uint8_t Index)
{
	DataFlash_Select(Index);
	while(Flash_IsBusy());
	DataFlash_Deselect(Index);
}

Flash_State_t DataFlashManager_Init(void)
{
	uint8_t Buffer[512];

	Flash_Init(&Config_SPI);

	DataFlash_InitCS();

	for(uint8_t i = 0x00; i < DATAFLASH_COUNT; i++)
	{
		DataFlash_Select(i);

		if(Flash_ReadID() != DATAFLASH_ID)
		{
			return FLASH_ERROR_INVALID_FLASH;
		}

		DataFlash_Deselect(i);
	}

	DataFlashManager_WaitBusy(0);

	DataFlash_Select(0);
	Flash_ReadPage(0, 0, Buffer);
	DataFlash_Deselect(0);

	return FLASH_STATE_NO_ERROR;
}