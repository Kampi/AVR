 /*
 * NVM_Flash.s
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for the Atmel AVR XMega NVM controller.

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

 /** @file Arch/XMega/NVM/NVM_Flash.s
 *  @brief Driver for the Atmel AVR XMega NVM controller.
 *
 *  This file contains the implementation of the flash routines for the Atmel AVR XMega NVM driver.
 *
 *		Note on IAR calling convention:
 *			Scratch registers:		R18-R27, R30-R31
 *			Preserved registers:	R2-R17, R28-R29
 *			Parameter registers:	R8-R25 (2-,4-, or 8- byte alignment)
 *			Return registers:		R18-R25 (up to 64-bit)
 *
 *  @author Daniel Kampert
 */
 
#include <avr/io.h>

#define NVM_CMD_NO_OPERATION_gc					0x00
#define NVM_CMD_READ_USER_SIG_ROW_gc			0x01
#define NVM_CMD_READ_CALIB_ROW_gc				0x02
#define NVM_CMD_READ_EEPROM_gc					0x06
#define NVM_CMD_READ_FUSES_gc					0x07	
#define NVM_CMD_WRITE_LOCK_BITS_gc				0x08
#define NVM_CMD_ERASE_USER_SIG_ROW_gc			0x18
#define NVM_CMD_WRITE_USER_SIG_ROW_gc			0x1A
#define NVM_CMD_ERASE_APP_gc					0x20
#define NVM_CMD_ERASE_APP_PAGE_gc				0x22
#define NVM_CMD_LOAD_FLASH_BUFFER_gc			0x23
#define NVM_CMD_WRITE_APP_PAGE_gc				0x24
#define NVM_CMD_ERASE_WRITE_APP_PAGE_gc			0x25
#define NVM_CMD_ERASE_FLASH_BUFFER_gc			0x26
#define NVM_CMD_ERASE_BOOT_PAGE_gc				0x2A
#define NVM_CMD_WRITE_BOOT_PAGE_gc				0x2C
#define NVM_CMD_ERASE_WRITE_BOOT_PAGE_gc		0x2D
#define NVM_CMD_ERASE_EEPROM_gc					0x30
#define NVM_CMD_ERASE_EEPROM_PAGE_gc			0x32
#define NVM_CMD_LOAD_EEPROM_BUFFER_gc			0x33
#define NVM_CMD_WRITE_EEPROM_PAGE_gc			0x34
#define NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc		0x35
#define NVM_CMD_ERASE_EEPROM_BUFFER_gc			0x36
#define NVM_CMD_APP_CRC_gc						0x38
#define NVM_CMD_BOOT_CRC_gc						0x39
#define NVM_CMD_FLASH_RANGE_CRC_gc				0x3A
#define CCP_SPM_gc								0x9D
#define CCP_IOREG_gc							0xD8

;--
;	Wait until the NVM controller becomes ready.
;
;	Input:
;		-
;
;	Return:
;		-
;--
NVM_WaitBusy:

	push	r20

	NVM_Busy_Loop:
		lds		r20, NVM_STATUS
		sbrc	r20, NVM_NVMBUSY_bp
		rjmp	NVM_Busy_Loop

	pop		r20

	ret

;--
;	This function executes the SPM command. It must be placed in the bootloader
;	section of the microcontroller.
;
;	Input:
;		r1:r0				Data for SPM command
;		r31:r30				Lowest 2 bytes of the z pointer
;		r26					NVM command
;		RAMPZ				High byte of the z pointer
;
;	Return:
;		-
;--
.section .boot, "ax"
NVM_ExecuteSPM:

	; Load the NVM command
	sts		NVM_CMD, r26

	; Unlock SPM command
	ldi		r19, CCP_SPM_gc
	sts		CCP, r19

	spm

	; Clear __zero_reg__ (r1) for AVRGCC
	clr		r1

	; Clear the NVM command
	sts		NVM_CMD, r1

	ret

;--
;	Input:
;		r31:r30				Lowest 2 bytes of the z pointer
;		r26					NVM command
;
;	Return:
;		r24:r25				Result from LPM operation
;--
.section .text
NVM_ExecuteLPM:

	; Save SREG
	in		r18, SREG

	; Disable global interrupts
	cli

	; Load the NVM command
	sts		NVM_CMD, r26

	lpm		r24, Z+
	lpm		r25, Z

	; Restore SREG
	out		SREG, r18

	ret

;--
;	Input:
;		-
;
;	Return:
;		-
;--
.section .text
.global NVM_EraseUserSignature
NVM_EraseUserSignature:

	; Load NVM command
	ldi		r26, NVM_CMD_ERASE_USER_SIG_ROW_gc

	call	NVM_ExecuteSPM

	ret

;--
;	Input:
;		-
;
;	Return:
;		-
;--
.section .text
.global NVM_FlushUserSignature
NVM_FlushUserSignature:

	; Load NVM command
	ldi		r26, NVM_CMD_WRITE_USER_SIG_ROW_gc

	; Execute SPM command
	call	NVM_ExecuteSPM

	ret

;--
;	Input:
;		r25:r24				Read address
;
;	Return:
;		r25:r24				Data word
;--
.section .text
.global NVM_UserSignatureReadWord
NVM_UserSignatureReadWord:

	; Save the address
	movw	ZL, r24

	; Load NVM command
	ldi		r26, NVM_CMD_READ_USER_SIG_ROW_gc

	; Execute LPM command
	rcall	NVM_ExecuteLPM

	ret

;--
;	Input:
;		r25:r24				Pounter to read buffer
;
;	Return:
;		-
;--
.section .text
.global NVM_UserSignatureReadPage
NVM_UserSignatureReadPage:

	; Save RAMPZ
	in		r18, RAMPZ

	; Clear the address to set the start to the beginning of the page
	sts		RAMPZ, r1
	clr		ZL
	clr		ZH

	; Load NVM command
	ldi		r26, NVM_CMD_READ_USER_SIG_ROW_gc
	sts		NVM_CMD, r26

	; Load the data pointer
	movw	XL, r24

	; Save the size of one page (page size is given in bytes!)
	ldi		r21, ((APP_SECTION_PAGE_SIZE / 2) & 0xFF)

	; Save SREG
	in		r18, SREG

	; Disable global interrupts
	cli

	NVM_FlashUserSignatureReadPage_Loop:

		; Save the data bytes of the current word
		lpm		r24, Z+
		lpm		r25, Z+

		st		X+, r24
		st		X+, r25

		; Decrement and repeat until zero
		dec		r21
		brne	NVM_FlashUserSignatureReadPage_Loop

	; Restore SREG
	out		SREG, r18

	; Clear the NVM command
	sts		NVM_CMD, r1

	; Clear __zero_reg__ (r1) for AVRGCC
	clr		r1

	; Restore RAMPZ
	out		RAMPZ, r18

	ret

;--
;	Input:
;		-
;
;	Return:
;		-
;--
.section .text
.global NVM_EraseApplication
NVM_EraseApplication:

	; Save RAMPZ
	in		r18, RAMPZ

	; Clear the Z pointer so the Z pointer will point into the application section
	sts		RAMPZ, r1
	clr		ZH
	clr		ZL

	; Load NVM command
	ldi		r26, NVM_CMD_ERASE_APP_gc

	; Execute SPM command
	call	NVM_ExecuteSPM

	; Busy - wait
	call	NVM_WaitBusy

	; Restore RAMPZ
	out		RAMPZ, r18

	ret

;--
;	Input:
;		-
;
;	Return:
;		-
;--
.section .text
.global NVM_ClearFlashBuffer
NVM_ClearFlashBuffer:

	; Load NVM command
	ldi		r26, NVM_CMD_ERASE_FLASH_BUFFER_gc
	sts		NVM_CTRLA, r26

	; Execute command
	ldi		r26, NVM_CMDEX_bp
	sts		NVM_CTRLA, r26

	; Busy - wait
	call	NVM_WaitBusy

	ret

;--
;	Input:
;		r25:r24				Page offset
;		r23:r22				Data word
;
;	Return:
;		-
;--
.section .text
.global NVM_FlashWriteWord
NVM_FlashWriteWord:

	; Save RAMPZ
	in		r18, RAMPZ

	; Clear RAMPZ
	sts		RAMPZ, r1

	; Save the address
	movw	ZL, r24

	; Copy data word to r1:r0 (used by SPM)
	movw	r0, r22

	; Load NVM command
	ldi		r26, NVM_CMD_LOAD_FLASH_BUFFER_gc

	; Execute SPM command
	call	NVM_ExecuteSPM

	; Restore RAMPZ
	out		RAMPZ, r18

	ret

;--
;	Input:
;		r25:r24				Pointer to data buffer
;
;	Return:
;		-
;--
.section .boot, "ax"
.global NVM_FlashWritePage
NVM_FlashWritePage:

	; Save RAMPZ
	in		r18, RAMPZ

	; Clear the address to set the start to the beginning of the page
	sts		RAMPZ, r1
	clr		ZL
	clr		ZH

	; Load NVM command
	ldi		r26, NVM_CMD_LOAD_FLASH_BUFFER_gc
	sts		NVM_CMD, r26

	; Load the data pointer
	movw	XL, r24

	; Save the size of one page (page size is given in bytes!)
	ldi		r21, ((APP_SECTION_PAGE_SIZE / 2) & 0xFF)

	; Save the protection signature
	ldi		r19, CCP_SPM_gc

	NVM_FlashUserSignatureWritePage_Loop:

		; Load the data bytes of the current word
		ld		r0, X+
		ld		r1, X+ 

		; Unlock SPM command
		sts		CCP, r19

		spm

		; Increase the address pointer
		adiw	ZL, 2 

		; Decrement and repeat until zero
		dec		r21
		brne	NVM_FlashUserSignatureWritePage_Loop

	; Clear __zero_reg__ (r1) for AVRGCC
	clr		r1

	; Clear the NVM command
	sts		NVM_CMD, r1

	; Restore RAMPZ
	out		RAMPZ, r18

	ret

;--
;	Input:
;		r25:r24				Page address
;
;	Return:
;		-
;--
.section .text
.global NVM_FlushFlash
NVM_FlushFlash:

	; Save RAMPZ
	in		r18, RAMPZ

	; Clear the Z pointer
	sts		RAMPZ, r1
	clr		ZH

	; Save the page address
	mov		r19, r25
	mov		ZH, r24

	; Perform the address calculation
	lsl		ZH
	rol		r19

	; Save the high byte into the RAMPZ register
	sts		RAMPZ, r19

	; Load NVM command
	ldi		r26, NVM_CMD_ERASE_WRITE_FLASH_PAGE_gc
	call	NVM_ExecuteSPM

	; Busy - wait
	call	NVM_WaitBusy

	; Restore RAMPZ
	out		RAMPZ, r18

	ret

;--
;	Input:
;		-
;
;	Return:
;		-
;--
.section .text
.global NVM_LockSPM
NVM_LockSPM:

	; Load the signature register
	ldi	r18, CCP_IOREG_gc

	; Load the SPM lock bit
	ldi	r19, NVM_SPMLOCK_bm

	; Unlock SPM lock bit
	sts	CCP, r18

	; Set the bit
	sts	NVM_CTRLB, r19

	ret