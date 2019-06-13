/*
 * diskio.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Low level disk operations for FatFs driver.

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

/** @file Common/Services/FatFs/diskio.h
 *  @brief Low level disk operations for FatFs driver.
 *
 *  @author Daniel Kampert
 */

#include "fatfs-r0.13c/source/ff.h"
#include "fatfs-r0.13c/source/diskio.h"

#include "Peripheral/SD/SD.h"

#define DEV_MMC					0							/**< Map MMC/SD card to physical drive 0 */
#define DEV_USB					1							/**< Map USB MSD to physical drive 1 */

SPIM_Config_t __InterfaceConfig = {
	.SPIClock = 1000000,
	.DataOrder = SPI_DATAORDER_MSB_FIRST,
	.Mode = SPI_MODE_0,
	.Device = SD_INTERFACE
};

// Disk status
static volatile DSTATUS __MMCStatus = STA_NOINIT;
static volatile DSTATUS __USBStatus = STA_NOINIT;

DSTATUS disk_initialize(
	BYTE pdrv				/* Physical drive number to identify the drive */
)
{
	switch(pdrv)
	{
		case DEV_MMC:
		{
			if(SD_Init(&__InterfaceConfig) == SD_SUCCESSFULL)
			{
				__MMCStatus &= ~STA_NOINIT;

				return __MMCStatus;
			}
		}
		case DEV_USB:
		{
			return __USBStatus;
		}
	}

	return STA_NOINIT;
}

DSTATUS disk_status(
	BYTE pdrv				/* Physical drive number to identify the drive */
)
{
	switch(pdrv)
	{
		case DEV_MMC:
		{
			return __MMCStatus;
		}
		case DEV_USB:
		{
			return __USBStatus;
		}
	}
	
	return RES_PARERR;
}

uint32_t get_fattime(void)
{
	// Dummy time
	return 1555997382;
}

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive number to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	switch(pdrv)
	{
		case DEV_MMC:
		{
			if(__MMCStatus & STA_NOINIT)
			{
				return RES_NOTRDY;
			}
			
			// Read a single block
			if(count == 1)
			{
				if(SD_ReadDataBlock(sector, buff) == SD_SUCCESSFULL)
				{
					return RES_OK;
				}
			}
			// Read multiple blocks
			else
			{
				if(SD_ReadDataBlocks(sector, count, buff) == SD_SUCCESSFULL)
				{
					return RES_OK;
				}
			}
			
			return RES_ERROR;
		}
		case DEV_USB:
		{
			return RES_NOTRDY;
		}
	}

	return RES_PARERR;
}

DRESULT disk_write(
	BYTE pdrv,			/* Physical drive number to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	switch(pdrv)
	{
		case DEV_MMC:
		{
			if(__MMCStatus & STA_NOINIT)
			{
				return RES_NOTRDY;
			}
			
			// Write a single block
			if(count == 1)
			{
				if(SD_WriteDataBlock(sector, buff) == SD_SUCCESSFULL)
				{
					return RES_OK;
				}
			}
			// Write multiple blocks
			else
			{
				if(SD_WriteDataBlocks(sector, count, buff) == SD_SUCCESSFULL)
				{
					return RES_OK;
				}
			}
			
			return RES_ERROR;
		}
		case DEV_USB:
		{
			return RES_NOTRDY;
		}
	}

	return RES_PARERR;
}

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive number (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	BYTE *ptr = (BYTE*)buff;

	switch(pdrv)
	{
		case DEV_MMC:
		{
			switch(cmd) 
			{
				case GET_BLOCK_SIZE:
				{
					if(SD_GetEraseBlockSize((uint16_t*)ptr) == SD_SUCCESSFULL)
					{
						return RES_OK;
					}
					
					return RES_OK;
				}
				case GET_SECTOR_COUNT:
				{
					if(SD_GetSectors((DWORD*)ptr) == SD_SUCCESSFULL)
					{
						return RES_OK;
					}
					
					return RES_ERROR;
				}
				case GET_SECTOR_SIZE:
				{
					*(WORD*)buff = SD_BLOCK_SIZE;
					
					// ToDo: Support for older SD cards with smaller sectors than 512 Byte

					return RES_OK;
				}
				case CTRL_SYNC:
				{
					SD_Sync();

					return RES_OK;
				}
				default:
				{
					return RES_PARERR;
				}
			}
		}
		default:
		{
			return RES_PARERR;
		}
	}
	
	return RES_PARERR;
}