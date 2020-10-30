/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: SD card example for XMega.

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

/** @file main.c
 *  @brief SD card example for XMega.
 *
 *  Software for the XMega SD card example from
 *		https://www.kampis-elektroecke.de/2020/11/avr-mit-einer-sd-karte-erweitern-teil-1/
 *		https://www.kampis-elektroecke.de/2019/04/avr-mit-einer-sd-karte-erweitern-teil-2/
 * 		https://www.kampis-elektroecke.de/2019/05/avr-mit-einer-sd-karte-erweitern-teil-3/
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/SD/SD.h"
#include "../source/Services/FatFs/fatfs-r0.13c/source/ff.h"

SPIM_Config_t SDConfig = {
	.SPIClock = 1000000,
	.DataOrder = SPI_DATAORDER_MSB_FIRST,
	.Mode = SPI_MODE_0,
	.Device = &USARTD0
};

SD_Error_t Error;

FATFS MicroSD_FS;
FRESULT FileStatus;
FIL LogFile;
UINT Return;
char FilePath[] = "0:File.txt";
char ReadData[13];

int main(void)
{
	SysClock_Init();
	
	Error = SD_Init(&SDConfig);
	if(Error != SD_SUCCESSFULL)
	{
		return -1;
	}
	
	// Read the STATUS register
	SD_Status_t Status;
	Error = SD_GetStatus(&Status);
	
	// Read the CID register
	SD_CID_t CID;
	Error = SD_GetCID(&CID);
	
	// Read the CSD register
	SD_CSD_t CSD;
	Error = SD_GetCSD(&CSD);
	
	// Read the boot sector of the SD card
	uint8_t FirstSector[512];
	Error = SD_ReadDataBlock(0x00, FirstSector);

	// Read two sectors of the SD card
	uint8_t Sectors[1024];
	Error = SD_ReadDataBlocks(0x00, 2, Sectors);

	FileStatus = f_mount(&MicroSD_FS, "0:", 1);
	if(FileStatus == FR_OK)
	{
		FileStatus = f_open(&LogFile, FilePath, FA_WRITE | FA_CREATE_ALWAYS);
		FileStatus = f_puts("Hello, World!", &LogFile);
		FileStatus = f_close(&LogFile);
			
		FileStatus = f_open(&LogFile, FilePath, FA_READ | FA_OPEN_EXISTING);
		FileStatus = f_read(&LogFile, ReadData, 13, &Return);
		FileStatus = f_close(&LogFile);
	}

    while(1) 
    {
    }
}

