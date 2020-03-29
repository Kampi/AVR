/*
 * AES.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega AES encryption module.

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

#ifndef AES_H_
#define AES_H_

 #include <avr/io.h>
 #include <avr/interrupt.h>
 
 #define AES_DATASIZE							16
 #define AES_KEYSIZE							16

 static inline uint8_t AES_IsBusy(void) __attribute__ ((always_inline));
 static inline uint8_t AES_IsBusy(void)
 {
	 return (!(AES.STATUS & (AES_SRIF_bm | AES_ERROR_bm)));
 }
 
 static inline void AES_Reset(void) __attribute__ ((always_inline));
 static inline void AES_Reset(void)
 {
	 AES.CTRL = AES_RESET_bm;
 }

 const uint8_t AES_GenerateLastSubkey(const uint8_t* Key, uint8_t* Subkey);
 const uint8_t AES_Encrypt(const uint8_t* Data, uint8_t* EncryptedData, const uint8_t* Key);
 const uint8_t AES_Decrypt(const uint8_t* EncryptedData, uint8_t* Data, const uint8_t* Key);
 const uint8_t AES_CBC_Encrypt(const uint8_t* Data, uint8_t* EncryptedData, uint8_t* Key, const uint8_t* InitVector, const uint16_t BlockCount);
 const uint8_t AES_CBC_Decrypt(uint8_t* EncryptedData, uint8_t* Data, uint8_t* Key, const uint8_t* InitVector, const uint16_t BlockCount);
 
#endif /* AES_H_ */