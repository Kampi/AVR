/*
 * AT45DB642D.c
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the AT45DB642 SPI flash memory.

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

/** @file Peripheral/AT45DB642D/AT45DB642D.c
 *  @brief Driver for the AT45DB642 SPI flash memory.
 *
 *  This file contains the implementation of the flash memory driver.
 *
 *  @author Daniel Kampert
 */

#include "Peripheral/AT45DB642D/AT45DB642D.h"

/** @defgroup AT45DB642D
 *  @{
 */
	/** @defgroup AT45DB642D-Commands
	 *  AT45DB642D commands.
	 *  @{
	 */
		/** @defgroup AT45DB642D-Commands-Read
		 *  AT45DB642D read commands.
		 *  @{
		 */
			#define AT45DB642D_CMD_MAINPAGE_READ				0xD2
			#define AT45DB642D_CMD_READ_BUFFER1					0xD4
			#define AT45DB642D_CMD_READ_BUFFER2					0xD6
		/** @} */ // end of AT45DB642D-Commands-Read

		/** @defgroup AT45DB642D-Commands-Programm_Erase
		 *  AT45DB642D Programm/Erase commands.
		 *  @{
		 */
			#define AT45DB642D_CMD_WRITE_BUFFER1				0x84
			#define AT45DB642D_CMD_WRITE_BUFFER2				0x87
			#define AT45DB642D_CMD_BUFFER1_TO_MEMORY_ERASE		0x83
			#define AT45DB642D_CMD_BUFFER2_TO_MEMORY_ERASE		0x86
			#define AT45DB642D_CMD_BUFFER1_TO_MEMORY			0x88
			#define AT45DB642D_CMD_BUFFER2_TO_MEMORY			0x89
			#define AT45DB642D_CMD_CHIP_ERASE_BYTE1				0xC7
			#define AT45DB642D_CMD_CHIP_ERASE_BYTE2				0x94
			#define AT45DB642D_CMD_CHIP_ERASE_BYTE3				0x80
			#define AT45DB642D_CMD_CHIP_ERASE_BYTE4				0x9A
		/** @} */ // end of AT45DB642D-Commands-Programm_Erase

		/** @defgroup AT45DB642D-Commands-Protection-Security
		 *  AT45DB642D Protection/Security commands.
		 *  @{
		 */
			#define AT45DB642D_CMD_ENABLE_SECTOR_PROT_BYTE1		0x3D
			#define AT45DB642D_CMD_ENABLE_SECTOR_PROT_BYTE2		0x2A
			#define AT45DB642D_CMD_ENABLE_SECTOR_PROT_BYTE3		0x7F
			#define AT45DB642D_CMD_ENABLE_SECTOR_PROT_BYTE4		0xA9
			#define AT45DB642D_CMD_DISABLE_SECTOR_PROT_BYTE1	0x3D
			#define AT45DB642D_CMD_DISABLE_SECTOR_PROT_BYTE2	0x2A
			#define AT45DB642D_CMD_DISABLE_SECTOR_PROT_BYTE3	0x7F
			#define AT45DB642D_CMD_DISABLE_SECTOR_PROT_BYTE4	0x9A
		/** @} */ // end of AT45DB642D-Commands-Protection-Security

		/** @defgroup AT45DB642D-Commands-Misc
		 *  AT45DB642D Miscellaneous commands.
		 *  @{
		 */
			#define AT45DB642D_CMD_CHANGE_PAGE_SIZE_BYTE1		0x3D
			#define AT45DB642D_CMD_CHANGE_PAGE_SIZE_BYTE2		0x2A
			#define AT45DB642D_CMD_CHANGE_PAGE_SIZE_BYTE3		0x80
			#define AT45DB642D_CMD_CHANGE_PAGE_SIZE_BYTE4		0xA6
			#define AT45DB642D_CMD_MODE_POWER_DOWN				0xB9
			#define AT45DB642D_CMD_MODE_STANDBY					0xAB
			#define AT45DB642D_CMD_READ_STATUS					0xD7
			#define AT45DB642D_CMD_READ_ID						0x9F
		/** @} */ // end of AT45DB642D-Commands-Misc
	/** @} */ // end of AT45DB642D-Commands

	/** @defgroup AT45DB642D-Status
	 *  AT45DB642D Status register
	 *  @{
	 */
		#define AT45DB642D_BUSY									0x07
	/** @} */ // end of AT45DB642D-Status
/** @} */ // end of AT45DB642D

#if(MCU_ARCH == MCU_ARCH_XMEGA)
	#if(AT45DB642D_INTERFACE_TYPE == INTERFACE_USART_SPI)
		#define AT45DB642D_SPIM_INIT(Config)											USART_SPI_Init(Config)
		#define AT45DB642D_SPIM_TRANSMIT(Data)											USART_SPI_SendData(&CONCAT(AT45DB642D_INTERFACE), Data)
		#define AT45DB642D_SPIM_SET_CLOCK(SPIClock, Clock)								USART_SPI_SetClockRate(&CONCAT(AT45DB642D_INTERFACE), SPIClock, Clock, false)
		#define AT45DB642D_SPIM_GET_CLOCK(Clock)										USART_SPI_GetClockRate(&CONCAT(AT45DB642D_INTERFACE), Clock)
		#define AT45DB642D_SPIM_CHIP_SELECT(Port, Pin)									USART_SPI_SelectDevice(Port, Pin)
		#define AT45DB642D_SPIM_CHIP_DESELECT(Port, Pin)								USART_SPI_DeselectDevice(Port, Pin)
	#elif(AT45DB642D_INTERFACE_TYPE == INTERFACE_SPI)
		#define AT45DB642D_SPIM_INIT(Config)											SPIM_Init(Config)
		#define AT45DB642D_SPIM_TRANSMIT(Data)											SPIM_SendData(&CONCAT(AT45DB642D_INTERFACE), Data)
		#define AT45DB642D_SPIM_SET_CLOCK(SPIClock, Clock)								SPIM_SetClock(&CONCAT(AT45DB642D_INTERFACE), SPIClock, Clock)
		#define AT45DB642D_SPIM_GET_CLOCK(Clock)										SPIM_GetClock(&CONCAT(AT45DB642D_INTERFACE), Clock)
		#define AT45DB642D_SPIM_CHIP_SELECT(Port, Pin)									SPIM_SelectDevice(Port, Pin)
		#define AT45DB642D_SPIM_CHIP_DESELECT(Port, Pin)								SPIM_DeselectDevice(Port, Pin)
	#else
		#error "Interface not supported for AT45DB642D!"
	#endif
#elif(MCU_ARCH == MCU_ARCH_AVR8)
	#define AT45DB642D_SPIM_INIT(Config)												SPIM_Init(Config)
	#define AT45DB642D_SPIM_TRANSMIT(Data)												SPIM_SendData(Data)
	#define AT45DB642D_SPIM_SET_CLOCK(SPIClock, Clock)									SPIM_SetClock(SPIClock, Clock)
	#define AT45DB642D_SPIM_GET_CLOCK(Clock)											SPIM_GetClock(Clock)
	#define AT45DB642D_SPIM_CHIP_SELECT(Port, Pin)										SPIM_SelectDevice(Port, Pin)
	#define AT45DB642D_SPIM_CHIP_DESELECT(Port, Pin)									SPIM_DeselectDevice(Port, Pin)
#else
	#error "Architecture not supported for AT45DB642D!"
#endif

/*
static uint8_t AT45DB642D_ReadStatus(void)
{
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_READ_STATUS);
	return AT45DB642D_SPI_TRANSMIT(0x00);;
}*/

/*
static bool AT45DB642D_IsBusy(void)
{
	return (!((AT45DB642D_ReadStatus() & (0x01 << AT45DB642D_BUSY)) >> AT45DB642D_BUSY));
}*/

void AT45DB642D_Init(SPIM_Config_t* Config)
{
	if(Config != NULL)
	{
		AT45DB642D_SPIM_INIT(Config);
	}
}
/*
uint16_t Flash_ReadID(void)
{
	uint8_t DeviceID[4];

	uint8_t A = AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_READ_ID);
	DeviceID[0] = AT45DB642D_SPI_TRANSMIT(0x00);
	DeviceID[1] = AT45DB642D_SPI_TRANSMIT(0x00);
	DeviceID[2] = AT45DB642D_SPI_TRANSMIT(0x00);
	DeviceID[3] = AT45DB642D_SPI_TRANSMIT(0x00);

	return ((DeviceID[1] << 0x08) | DeviceID[0]);
}



uint8_t Flash_ReadByte(void)
{
	return AT45DB642D_SPI_TRANSMIT(0x00);
}

void Flash_ReadPage(const uint16_t Page, const uint16_t Offset, uint8_t* Data)
{
	uint32_t Address_Temp = (((uint32_t)(Page & 0x1FFF)) << 0x0A) | (Offset & 0x3FF);

	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_MAINPAGE_READ);
	AT45DB642D_SPI_TRANSMIT((Address_Temp & 0xFF0000) >> 0x10);
	AT45DB642D_SPI_TRANSMIT((Address_Temp & 0x00FF00) >> 0x08);
	AT45DB642D_SPI_TRANSMIT((Address_Temp & 0x0000FF));
	AT45DB642D_SPI_TRANSMIT(0x00);
	AT45DB642D_SPI_TRANSMIT(0x00);
	AT45DB642D_SPI_TRANSMIT(0x00);
	AT45DB642D_SPI_TRANSMIT(0x00);

	for(uint16_t i = 0x00; i < DATAFLASH_PAGE_SIZE; i++)
	{
		*Data++ = AT45DB642D_SPI_TRANSMIT(0x00);
	}
}
















void AT45DB642D_WriteData(AT45DB642D_Buffer_t Buffer, uint16_t Address, uint8_t Count, uint8_t* Data)
{
	uint8_t BufferCommand = AT45DB642D_CMD_WRITE_BUFFER1;
	uint16_t Bytes = 0x00;
	
	// Mask 10 address bits
	uint16_t Address_Temp = (Address & 0x3FF);
	
	if(Buffer == AT45DB642D_BUFFER_2)
	{
		BufferCommand = AT45DB642D_CMD_WRITE_BUFFER2;
	}
	
	//AT45DB642D_SPI_CHIP_SELECT(FLASH_SS_PORT, FLASH_SS_PIN);
	AT45DB642D_SPI_TRANSMIT(BufferCommand);
	AT45DB642D_SPI_TRANSMIT(0x00);
	AT45DB642D_SPI_TRANSMIT((Address_Temp & 0xFF00) >> 0x08);
	AT45DB642D_SPI_TRANSMIT((Address_Temp & 0xFF));
	
	for(Bytes = 0x00; Bytes < Count; Bytes++)
	{
		AT45DB642D_SPI_TRANSMIT(*Data++);
	}
	
	//AT45DB642D_SPI_CHIP_DESELECT(FLASH_SS_PORT, FLASH_SS_PIN);
}












void AT45DB642D_PowerDown(void)
{
	//AT45DB642D_SPI_CHIP_SELECT(FLASH_SS_PORT, FLASH_SS_PIN);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_MODE_POWER_DOWN);
	//AT45DB642D_SPI_CHIP_DESELECT(FLASH_SS_PORT, FLASH_SS_PIN);
}

void AT45DB642D_Standby(void)
{
	//AT45DB642D_SPI_CHIP_SELECT(FLASH_SS_PORT, FLASH_SS_PIN);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_MODE_STANDBY);
	//AT45DB642D_SPI_CHIP_DESELECT(FLASH_SS_PORT, FLASH_SS_PIN);
}

void AT45DB642D_EnableSectorProtection(void)
{
	//AT45DB642D_SPI_CHIP_SELECT(FLASH_SS_PORT, FLASH_SS_PIN);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_ENABLE_SECTOR_PROT_BYTE1);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_ENABLE_SECTOR_PROT_BYTE2);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_ENABLE_SECTOR_PROT_BYTE3);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_ENABLE_SECTOR_PROT_BYTE4);
	//AT45DB642D_SPI_CHIP_DESELECT(FLASH_SS_PORT, FLASH_SS_PIN);
}

void AT45DB642D_DisableSectorProtection(void)
{
	//AT45DB642D_SPI_CHIP_SELECT(FLASH_SS_PORT, FLASH_SS_PIN);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_DISABLE_SECTOR_PROT_BYTE1);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_DISABLE_SECTOR_PROT_BYTE2);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_DISABLE_SECTOR_PROT_BYTE3);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_DISABLE_SECTOR_PROT_BYTE4);
	//AT45DB642D_SPI_CHIP_DESELECT(FLASH_SS_PORT, FLASH_SS_PIN);
}

void AT45DB642D_EraseMemory(void)
{
	//AT45DB642D_SPI_CHIP_SELECT(FLASH_SS_PORT, FLASH_SS_PIN);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_CHIP_ERASE_BYTE1);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_CHIP_ERASE_BYTE2);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_CHIP_ERASE_BYTE3);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_CHIP_ERASE_BYTE4);
	//AT45DB642D_SPI_CHIP_DESELECT(FLASH_SS_PORT, FLASH_SS_PIN);
}

void AT45DB642D_ChangePageSize(void)
{
	//AT45DB642D_SPI_CHIP_SELECT(FLASH_SS_PORT, FLASH_SS_PIN);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_CHANGE_PAGE_SIZE_BYTE1);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_CHANGE_PAGE_SIZE_BYTE2);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_CHANGE_PAGE_SIZE_BYTE3);
	AT45DB642D_SPI_TRANSMIT(AT45DB642D_CMD_CHANGE_PAGE_SIZE_BYTE4);
	//AT45DB642D_SPI_CHIP_DESELECT(FLASH_SS_PORT, FLASH_SS_PIN);
}

void AT45DB642D_Flush(AT45DB642D_Buffer_t Buffer, uint16_t PageAddress)
{
	uint8_t BufferCommand = AT45DB642D_CMD_BUFFER1_TO_MEMORY_ERASE;

	// Wait until device is ready
	while(!(AT45DB642D_ReadStatus() & 0x80));

	// Mask 10 address bits
	uint16_t PageAddress_Temp = (PageAddress & 0x3FF);

	if(Buffer == AT45DB642D_BUFFER_2)
	{
		BufferCommand = AT45DB642D_CMD_BUFFER2_TO_MEMORY_ERASE;
	}

	//AT45DB642D_SPI_CHIP_SELECT(FLASH_SS_PORT, FLASH_SS_PIN);
	AT45DB642D_SPI_TRANSMIT(BufferCommand);
	AT45DB642D_SPI_TRANSMIT(0x00);
	AT45DB642D_SPI_TRANSMIT((PageAddress_Temp & 0xFF00) >> 0x08);
	AT45DB642D_SPI_TRANSMIT((PageAddress_Temp & 0x00FF));	
	//AT45DB642D_SPI_CHIP_DESELECT(FLASH_SS_PORT, FLASH_SS_PIN);
}*/