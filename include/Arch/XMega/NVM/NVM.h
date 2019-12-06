/*
 * NVM.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega NVM

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

/** @file Arch/XMega/NVM/NVM.h
 *  @brief Driver for XMega non volatile memory controller. 
 *
 *  This file contains the prototypes and definitions for the XMega NVM driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef NVM_H_
#define NVM_H_

 #include "Common/Common.h" 
 #include "Arch/XMega/CPU/CPU.h"

 /** @brief NVM production signatures.
  */
 typedef enum
 {
	 RCOSC2M = 0x00,
	 RCOSC2MA = 0x01,
	 RCOSC32K = 0x02,
	 RCOSC32M = 0x03,
	 RCOSC32MA = 0x04,
	 LOTNUM0 = 0x08,
	 LOTNUM1 = 0x09,
	 LOTNUM2 = 0x0A,
	 LOTNUM3 = 0x0B,
	 LOTNUM4 = 0x0C,
	 LOTNUM5 = 0x0D,
	 WAFNUM = 0x10,
	 COORDX0 = 0x12,
	 COORDX1 = 0x13,
	 COORDY0 = 0x14,
	 COORDY1 = 0x15,
	 USBCAL0 = 0x1A,
	 USBCAL1 = 0x1B,
	 USBRCOSC = 0x1C,
	 USBRCOSCA = 0x1D,
	 ADCACAL0 = 0x20,
	 ADCACAL1 = 0x21,
	 ADCBCAL0 = 0x24,
	 ADCBCAL1 = 0x25,
	 TEMPSENSE0 = 0x2E,
	 TEMPSENSE1 = 0x2F,
	 DACA0OFFCAL = 0x30,
	 DACA0GAINCAL = 0x31,
	 DACB0OFFCAL = 0x32,
	 DACB0GAINCAL = 0x33,
	 DACA1OFFCAL = 0x34,
	 DACA1GAINCAL = 0x35,
	 DACB1OFFCAL = 0x36,
	 DACB1GAINCAL = 0x37,
 } NVM_Signature_t;

 /** @brief	Enable the self programming lock.
  *			NOTE: This bit can only cleared with a reset!
  */
 static inline void NVM_EnableSPMLock(void) __attribute__((always_inline));
 static inline void NVM_EnableSPMLock(void)
 {
	 uint8_t Flags = CPU_IRQSave();
	 
	 asm volatile(	"movw r30,  %0"	"\n\t"
					"ldi  r16,  %2"	"\n\t"
					"out   %3, r16"	"\n\t"
					"st     Z,  %1"	"\n\t"
					::
					"r" (&NVM.CTRLA),
					"r" (NVM_SPMLOCK_bm),
					"M" (CCP_IOREG_gc),
					"i" (&CCP)
					: "r16", "r30", "r31"
				);
	 
	 CPU_IRQRestore(Flags);
 }

 /** @brief			Enable/Disable the power reduction mode for the flash memory.
  *  @param Enable	Enable/Disable
  */
 static inline void NVM_SwitchFlashPowerReduction(const Bool_t Enable) __attribute__((always_inline));
 static inline void NVM_SwitchFlashPowerReduction(const Bool_t Enable)
 {
	 if(Enable == TRUE)
	 {
		 NVM.CTRLB |= NVM_FPRM_bm;
	 }
	 else
	 {
		 NVM.CTRLB &= ~NVM_FPRM_bm;
	 }
 }

 /** @brief			Enable/Disable the power reduction mode for the EEPROM.
  *  @param Enable	Enable/Disable
  */
 static inline void NVM_SwitchEEPROMPowerReduction(const Bool_t Enable) __attribute__((always_inline));
 static inline void NVM_SwitchEEPROMPowerReduction(const Bool_t Enable)
 {
	 if(Enable == TRUE)
	 {
		 NVM.CTRLB |= NVM_EPRM_bm;
	 }
	 else
	 {
		 NVM.CTRLB &= ~NVM_EPRM_bm;
	 }
 }

 /** @brief			Enable/Disable the EEPROM data mapping.
  *  @param Enable	Enable/Disable
  */
 static inline void NVM_SwitchEEPROMMapping(const Bool_t Enable) __attribute__((always_inline));
 static inline void NVM_SwitchEEPROMMapping(const Bool_t Enable)
 {
	 if(Enable == TRUE)
	 {
		 NVM.CTRLB |= NVM_EEMAPEN_bm;
	 }
	 else
	 {
		 NVM.CTRLB &= ~NVM_EEMAPEN_bm;
	 }
 }
 
 /** @brief		Read the lock bit register.
  *  @return	Lock bits
  */
 static inline const uint8_t NVM_GetLockbits(void) __attribute__((always_inline));
 static inline const uint8_t NVM_GetLockbits(void)
 {
	 return NVM.LOCK_BITS;
 }

 /** @brief			Execute a NVM command.
  *  @param Command	NVM command
  */
 static inline void NVM_ExecuteCommand(const uint8_t Command) __attribute__((always_inline));
 static inline void NVM_ExecuteCommand(const uint8_t Command)
 {
	 uint8_t OldCmd = NVM.CMD;
 
	 NVM.CMD = Command;

	 uint8_t Flags = CPU_IRQSave();

	 asm volatile(	"movw r30,  %0"	    "\n\t"
					"ldi  r16,  %2"	    "\n\t"
					"out   %3, r16"     "\n\t"
					"st     Z,  %1"     "\n\t"
					::
					"r" (&NVM.CTRLA),
					"r" (NVM_CMDEX_bm),
					"M" (CCP_IOREG_gc),
					"i" (&CCP)
					: "r16", "r30", "r31"
	 );
	 
	 CPU_IRQRestore(Flags);
 
	 NVM.CMD = OldCmd;
 }

 /*
	 EEPROM functions 
 */

 /** @brief			Write one byte into the EEPROM.
  *  @param Page	Page address
  *  @param Offset	Page offset
  *  @param Data	Data byte
  */
 void NVM_EEPROMWriteByte(const uint8_t Page, const uint8_t Offset, const uint8_t Data);

 /** @brief			Write one page into the EEPROM.
  *					NOTE: This function fill the remaining bytes with 0!
  *  @param Page	Page address
  *  @param	Length	Length of data
  *  @param Data	Pointer to data
  */
 void NVM_EEPROMWritePage(const uint8_t Page, const uint8_t Length, const uint8_t* Data);

 /** @brief			Read one byte from the EEPROM.
  *  @param Page	Page address
  *  @param Offset	Page offset
  *  @return		Data byte
  */
 uint8_t NVM_EEPROMReadByte(const uint8_t Page, const uint8_t Offset);

 /** @brief			Erase a single EEPROM page.
  *  @param Page	Page address
  */
 void NVM_EEPROMErasePage(const uint8_t Page);
 
 /** @brief	Erase the content of the complete EEPROM.
  */
 void NVM_EEPROMErase(void);

 /*
	 Flash functions
 */

 /** @brief	Erase the application section.
  */
 void NVM_EraseApplication(void);

 /** @brief	Clear the flash buffer of the NVM controller.
  */
 void NVM_ClearFlashBuffer(void);

 /** @brief			Write one word to the flash buffer.
  *  @param Offset	page offset
  *  @param Data	Data word
  */
 void NVM_FlashWriteWord(const uint16_t Offset, const uint16_t Data);

 /** @brief			Copy a complete page into the flash buffer.
  *  @param Data	Pointer to data buffer
  */
 void NVM_FlashWritePage(const uint16_t* Data);

 /** @brief			Flush the flash buffer and write the content to the given page in flash memory.
  *  @param Page	Page address
  */
 void NVM_FlushFlash(const uint16_t Page);

 /*
	 User Signature Row functions
 */

 /** @brief	Clear the user signature page.
  */
 void NVM_EraseUserSignature(void);
 
 /** @brief	Flush the flash buffer and write the data to the user signature flash page.
  */
 void NVM_FlushUserSignature(void);

 /** @brief			Read a one word from the user signature row.
  *  @param Address	Address
  *  @return		Signature byte
  */
 uint16_t NVM_UserSignatureReadWord(const uint16_t Address);
 
 /** @brief			Read the complete user signature page.
  *  @param Data	Pointer to data buffer
  */
 void NVM_UserSignatureReadPage(const uint16_t* Data);
 
 /*
	 Miscellaneous functions
 */

 /** @brief			Configure the NVM-Controller for a CRC over a flash memory segment.
  *  @param Start	Start address
  *  @param Length	Segment length
  */
 void NVM_FlashRangeCRC(const uint32_t Start, const uint32_t Length);
 
 /** @brief			Read a calibration byte from the signature row.
  *  @param Index	Byte index
  *  @return		Calibration byte
  */
 const uint8_t NVM_ReadCalibrationByte(const uint8_t Index);

 /** @brief	Disable the SPM command.
  */
 void NVM_LockSPM(void);

#endif /* NVM_H_ */