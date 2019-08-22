/*
 * NVM_Bootloader_XMega.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR XMega NVM controller for XMega bootloader.

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

/** @file Common/Services/Bootloader/Arch/XMega/NVM_Bootloader_XMega.c
 *  @brief AVR XMega NVM controller for XMega bootloader.
 *
 *  This file contains the implementation for the AVR NVM controller for the XMega bootloader.
 *
 *  @author Daniel Kampert
 */

#include <avr/io.h>

#define NVM_CMD_ERASE_APP_gc					0x20
#define NVM_CMD_LOAD_FLASH_BUFFER_gc			0x23
#define NVM_CMD_ERASE_FLASH_BUFFER_gc			0x26
#define NVM_CMD_ERASE_WRITE_FLASH_PAGE_gc		0x2F
#define CCP_SPM_gc								0x9D
#define CCP_IOREG_gc							0xD8

;--
;	Wait until the NVM controller becomes ready.
;
;	Input:
;		r24					Word address
;		r23:r22				Page address
;		r21:r20				Data byte
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
;		r24					Word address
;		r23:r22				Data byte
;
;	Return:
;		-
;--
.section .text
.global NVM_LoadFlashBuffer
NVM_LoadFlashBuffer:

	; Clear the Z pointer
	clr		ZH
	clr		ZL

	; Save the word address
	mov		ZL, r24

	; Perform the address calculation
	lsl		ZL
	rol		ZH

	; Copy data word to r1:r0 (used by SPM)
	movw	r0, r22

	; Load NVM command
	ldi		r26, NVM_CMD_LOAD_FLASH_BUFFER_gc

	; Execute SPM command
	call	NVM_ExecuteSPM

	ret

;--
;	Input:
;		r25:r24				Page address
;
;	Return:
;		-
;--
.section .text
.global NVM_FlushFlashBuffer
NVM_FlushFlashBuffer:

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