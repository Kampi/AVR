/*
 * AES.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Daniel Kampert
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega AES encryption module.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file Arch/XMega/AES/AES.c
 *  @brief Driver for Atmel AVR XMega AES encryption module.
 *
 *  This file contains the implementation of the Atmel AVR XMega AES driver.
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/AES/AES.h"

#ifndef DOXYGEN
	static struct
	{
		AES_Callback_t ReadyCallback;
	} AES_Callbacks;
#endif

static uint8_t* _AES_DataPtr;

/** @brief	AES interrupt handler.
 */
static void _AES_InterruptHandler(void)
{
	// Clear the interrupt flag
	AES.STATUS |= AES_SRIF_bm;
	
	if(AES_Callbacks.ReadyCallback)
	{
		AES_Callbacks.ReadyCallback();
	}
}

void AES_ChangeInterruptLevel(const Interrupt_Level_t InterruptLevel)
{
	AES.INTCTRL = InterruptLevel;
}

void AES_InstallCallback(const AES_InterruptConfig_t* Config)
{
	AES.INTCTRL = Config->InterruptLevel;

	if(Config->Source == AES_READY_INTERRUPT)
	{
		AES_Callbacks.ReadyCallback = Config->Callback;
	}
}

void AES_RemoveCallback(AES_CallbackType_t Callback)
{
	if(Callback == AES_READY_INTERRUPT)
	{
		AES_Callbacks.ReadyCallback = NULL;
	}
}

const AES_Error_t AES_GenerateLastSubkey(const uint8_t* Key, uint8_t* SubKey)
{
	for(uint8_t i = 0x00; i < AES_KEYSIZE; i++)
	{
		AES.KEY = *(Key++);
	}

	for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
	{
		AES.STATE = 0x00;
	}

	// Set module in encryption mode and start it
	AES_SetEncryptMode();
	AES_Start();

	while(AES_IsBusy());

	// Check for error
	if(!(AES.STATUS & AES_ERROR_bm))
	{
		for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
		{
			*(SubKey++) = AES.KEY;
		}

		AES.STATUS = AES_SRIF_bm;
	}
	else
	{
		AES.STATUS = AES_ERROR_bm;

		return AES_ERROR;
	}

	return AES_NO_ERROR;
}

void AES_Run(uint8_t* Data, const uint8_t* Key, const AES_Direction_t Direction)
{
	_AES_DataPtr = Data;

	for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
	{
		AES.KEY = *(Key++);
	}

	for(uint8_t i = 0x00; i < AES_KEYSIZE; i++)
	{
		AES.STATE = *(_AES_DataPtr++);
	}

	if(Direction == AES_DECRYPT)
	{
		AES_SetDecryptMode();
	}
	else
	{
		AES_SetEncryptMode();
	}

	_AES_DataPtr = Data;

	AES_Start();
}

const AES_Error_t AES_Encrypt(const volatile uint8_t* DecryptedData, volatile uint8_t* EncryptedData, volatile const uint8_t* Key)
{	
	for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
	{
		AES.KEY = *(Key++);
	}

	for(uint8_t i = 0x00; i < AES_KEYSIZE; i++)
	{
		AES.STATE = *(DecryptedData++);
	}

	// Set the module in encryption mode and start it
	AES_SetEncryptMode();
	AES_Start();
	
	while(AES_IsBusy());

	// If no error, read data
	if(!(AES.STATUS & AES_ERROR_bm))
	{
		for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
		{
			*(EncryptedData++) = AES.STATE;
		}
		
		return AES_NO_ERROR;
	}

	return AES_ERROR;
}

const AES_Error_t AES_Decrypt(const volatile uint8_t* EncryptedData, volatile uint8_t* DecryptedData, const volatile uint8_t* Key)
{	
	for(uint8_t i = 0x00; i < AES_KEYSIZE; i++)
	{
		AES.KEY = *(Key++);
	}

	for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
	{
		AES.STATE = *(EncryptedData++);
	}

	// Set module in decryption mode and start it
	AES_SetDecryptMode();
	AES_Start();

	while(AES_IsBusy());

	// If no error, read data
	if(!(AES.STATUS & AES_ERROR_bm))
	{
		for(uint8_t i = 0; i < AES_DATASIZE; i++)
		{
			*(DecryptedData++) = AES.STATE;
		}

		return AES_NO_ERROR;
	}

	return AES_ERROR;
}

const AES_Error_t AES_CBC_Encrypt(const volatile uint8_t* DecryptedData, volatile uint8_t* EncryptedData, uint8_t* Key, const volatile uint8_t* InitVector, const uint16_t BlockCount)
{
	uint8_t* KeyTemp;

  	// Use the initialization vector for the first encryption
	for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
	{
		AES.STATE = *(InitVector++);
	}

	// Set module in encryption mode, enables the XOR feature and the AUTO start
	AES_SetEncryptMode();
	AES_EnableCBC();

	for(uint8_t Block = 0x00; Block < BlockCount; Block++)
	{
		KeyTemp = Key;
		for(uint8_t i = 0x00; i < AES_KEYSIZE; i++)
		{
			AES.KEY = *(KeyTemp++);
		}

		for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
		{
			AES.STATE = *(DecryptedData++);
		}

		while(AES_IsBusy());

		// If no error, read data
		if(!(AES.STATUS & AES_ERROR_bm))
		{
			for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
			{
				*(EncryptedData++) = AES.STATE;
			}
		}
		else
		{
			return AES_ERROR;
		}
	}

	// Disable auto mode and XOR feature
	AES_DisableCBC();

	return AES_NO_ERROR;
}

const AES_Error_t AES_CBC_Decrypt(uint8_t* EncryptedData, volatile uint8_t* DecryptedData, uint8_t* volatile Key, const uint8_t* InitVector, const uint16_t BlockCount)
{
	uint8_t* KeyTemp;

	for(uint8_t Block = BlockCount; Block > 0; Block--)
	{
		// Reset the pointer for the key each block
		KeyTemp = Key;
		for(uint8_t i = 0x00; i < AES_KEYSIZE; i++)
		{
			AES.KEY = *(KeyTemp++);
		}

		for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
		{
			AES.STATE = *(EncryptedData++);
		}

		// Set the module in decryption mode, enable XOR and start it
		AES_SetDecryptMode();
		AES_EnableCBC();

		while(AES_IsBusy());

		// Check for error
		if(!(AES.STATUS & AES_ERROR_bm))
		{
			// Use the initialization vector for the first block
			if(Block == BlockCount)
			{
				for(uint8_t i = 0; i < AES_DATASIZE; i++)
				{
					AES.STATE = *(InitVector++);
				}
			}
			else
			{
			  	uint8_t* LastBlock = EncryptedData - (AES_DATASIZE << 1);
				for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
				{
					AES.STATE = *(LastBlock++);
				}
			}

			for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
			{
				*(DecryptedData++) = AES.STATE;
			}

			AES.CTRL = AES.CTRL & (~AES_XOR_bm);
		}
		else
		{
			return AES_ERROR;
		}
	}

	// Disable auto mode and XOR feature
	AES_DisableCBC();

	return AES_NO_ERROR;
}

/*
    Interrupt vectors
*/
#ifndef DOXYGEN
	ISR(AES_INT_vect)
	{
		// If no error, read data
		if(!(AES.STATUS & AES_ERROR_bm))
		{
			for(uint8_t i = 0x00; i < AES_DATASIZE; i++)
			{
				*(_AES_DataPtr++) = AES.STATE;
			}
		}

		_AES_InterruptHandler();
	}
#endif