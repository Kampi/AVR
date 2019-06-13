/*
 * AES.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega AES encryption module

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

/** @file Arch/XMega/AES/AES.h
 *  @brief Driver for XMega AES encryption module. The driver supports
 *		   ECB, and CBC encryption mode.
 *
 *  This file contains the prototypes and definitions for the XMega AES driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef AES_H_
#define AES_H_

 #include "Common/Common.h"

 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/PowerManagement/PowerManagement.h"

 /** @brief	AES callback definition
 */
 typedef void (*AES_Callback_t)(void);
 
 /** @brief	AES callback types.
  */
 typedef enum
 {
	 AES_READY_INTERRUPT = 0x00,		/**< AES ready interrupt */
 } AES_CallbackType_t;

 /** @brief	AES interrupt configuration object.
  */
 typedef struct
 {
	 AES_CallbackType_t Source;			/**< AES interrupt type */ 
	 Interrupt_Level_t InterruptLevel;	/**< Interrupt level */ 
	 AES_Callback_t Callback;			/**< Function pointer to AES callback */ 
 } AES_InterruptConfig_t;

 /** @brief Enable the AES module.
  */
 static inline void AES_Enable(void) __attribute__ ((always_inline));
 static inline void AES_Enable(void)
 {
	 AES_PowerEnable();
 }

 /** @brief Start the AES module.
  */
 static inline void AES_Start(void) __attribute__ ((always_inline));
 static inline void AES_Start(void)
 {
	 AES.CTRL |= AES_START_bm;
 }

 /** @brief Stops the AES module.
  */
 static inline void AES_Stop(void) __attribute__ ((always_inline));
 static inline void AES_Stop(void)
 {
	 AES.CTRL &= AES_START_bm;
 }

 /** @brief Perform a reset of the AES module.
  */
 static inline void AES_Reset(void) __attribute__ ((always_inline));
 static inline void AES_Reset(void)
 {
	 AES.CTRL |= (0x01 << AES_RESET_bp);
 }

 /** @brief		Check if the AES module is busy.
  *  @return	#TRUE if the module is busy
  */
 static inline Bool_t AES_IsBusy(void) __attribute__ ((always_inline));
 static inline Bool_t AES_IsBusy(void)
 {
	 return (!(AES.STATUS & (AES_SRIF_bm | AES_ERROR_bm)));
 }

 /** @brief	Set the module in encrypt mode.
  */
 static inline void AES_SetEncryptMode(void) __attribute__ ((always_inline));
 static inline void AES_SetEncryptMode(void)
 {
	 AES.CTRL &= ~AES_DECRYPT_bm;
 }
 
 /** @brief	Set the module in decrypt mode.
  */
 static inline void AES_SetDecryptMode(void) __attribute__ ((always_inline));
 static inline void AES_SetDecryptMode(void)
 {
	 AES.CTRL |= AES_DECRYPT_bm;
 }

 /** @brief	Enable CBC settings for the AES module.
  */
 static inline void AES_EnableCBC(void) __attribute__ ((always_inline));
 static inline void AES_EnableCBC(void)
 {
	 AES.CTRL |= AES_XOR_bm | AES_AUTO_bm;
 }
 
 /** @brief	Disable CBC settings for the AES module.
  */
 static inline void AES_DisableCBC(void) __attribute__ ((always_inline));
 static inline void AES_DisableCBC(void)
 {
	 AES.CTRL &= ~(AES_XOR_bm | AES_AUTO_bm);
 }

 /** @brief					Change the interrupt level of all AES module interrupts.
  *  @param InterruptLevel	New interrupt level
  */
 void AES_ChangeInterruptLevel(const Interrupt_Level_t InterruptLevel);
 
 /** @brief			Install a new AES interrupt callback.
  *  @param Config	Pointer to configuration structure
  */
 void AES_InstallCallback(const AES_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Callback	Callback which should be removed
  */
 void AES_RemoveCallback(AES_CallbackType_t Callback);
 
 /** @brief				Generate a new sub key from a given key.
  *  @param Callback	Pointer to key memory
  *  @param SubKey		Pointer to sub key memory
  *  @return			#TRUE if successfully
  */
 const Bool_t AES_GenerateLastSubkey(const uint8_t* Key, uint8_t* SubKey);
 
 /** @brief					Encrypt a single data block (16 bytes) by using ECB mode.
  *  @param Data			Pointer to data memory
  *  @param EncryptedData	Pointer to encrypted data memory
  *  @param Key				Pointer to key memory
  *  @return				#TRUE if successfully. Indexed by Bool_t
  */
 const Bool_t AES_Encrypt(const volatile uint8_t* Data, volatile uint8_t* EncryptedData, const volatile uint8_t* Key);
 
 /** @brief					Encrypt a single data block (16 bytes) by using ECB mode.
  *  @param EncryptedData	Pointer to encrypted data memory
  *  @param Data			Pointer to data memory
  *  @param Key				Pointer to key memory
  *  @return				#TRUE if successfully
  */
 const Bool_t AES_Decrypt(const volatile uint8_t* EncryptedData, volatile uint8_t* Data, const volatile uint8_t* Key);
 
 /** @brief					Encrypt a multiple data blocks (16 bytes) by using CBC mode.
  *  @param Data			Pointer to data memory
  *  @param EncryptedData	Pointer to encrypted data memory
  *  @param Key				Pointer to key memory
  *  @param InitVector		Pointer to initialization vector
  *  @param BlockCount		Number of blocks
  *  @return				#TRUE if successfully
  */
 const Bool_t AES_CBC_Encrypt(const volatile uint8_t* Data, volatile uint8_t* EncryptedData, uint8_t* Key, const volatile uint8_t* InitVector, const uint16_t BlockCount);
 
 /** @brief					Encrypt a multiple data blocks (16 bytes) by using CBC mode.
  *  @param EncryptedData	Pointer to encrypted data memory
  *  @param Data			Pointer to data memory
  *  @param Key				Pointer to key memory
  *  @param InitVector		Pointer to initialization vector
  *  @param BlockCount		Number of blocks
  *  @return				#TRUE if successfully
  */
 const Bool_t AES_CBC_Decrypt(uint8_t* EncryptedData, volatile uint8_t* Data, uint8_t* volatile Key, const uint8_t* InitVector, const uint16_t BlockCount);

#endif /* AES_H_ */