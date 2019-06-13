/*
 * AES.c
 *
 *  Copyright (C) Daniel Kampert, 2018
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

#include "AES.h"

const uint8_t AES_GenerateLastSubkey(const uint8_t* Key, uint8_t * Subkey)
{
	AES_Reset();

	for(uint8_t i = 0; i < AES_KEYSIZE; i++)
	{
		AES.KEY = *(Key++);
	}

	for(uint8_t i = 0; i < AES_DATASIZE; i++)
	{
		AES.STATE = 0x00;
	}

	/* 
		Set module in encryption mode and start it
	*/
	AES.CTRL = (AES.CTRL & (~AES_DECRYPT_bm)) | AES_START_bm;

	while(AES_IsBusy());

	/* 
		Check for error
	*/
	if(!(AES.STATUS & AES_ERROR_bm))
	{
		for(uint8_t i = 0; i < AES_DATASIZE; i++)
		{
			*(Subkey++) = AES.KEY;
		}

		AES.STATUS = AES_SRIF_bm;
	}
	else
	{
		AES.STATUS = AES_ERROR_bm;

		return -1;
	}
	
	return 0;
}

const uint8_t AES_Encrypt(const uint8_t* Data, uint8_t* EncryptedData, const uint8_t* Key)
{	
	for(uint8_t i = 0; i < AES_DATASIZE; i++)
	{
		AES.KEY = *(Key++);
	}

	for(uint8_t i = 0; i < AES_KEYSIZE; i++)
	{
		AES.STATE = *(Data++);
	}

	/* 
		Set the module in encryption mode and start it
	*/
	AES.CTRL = (AES.CTRL & (~AES_DECRYPT_bm)) | AES_START_bm;
	
	while(AES_IsBusy());

	/* 
		Check for error
	*/
	if(!(AES.STATUS & AES_ERROR_bm))
	{
		for(uint8_t i = 0; i < AES_DATASIZE; i++)
		{
			*(EncryptedData++) = AES.STATE;
		}
		
		return 0;
	}
	
	return -1;
}

const uint8_t AES_Decrypt(const uint8_t* EncryptedData, uint8_t* Data, const uint8_t* Key)
{	
	for(uint8_t i = 0; i < AES_KEYSIZE; i++)
	{
		AES.KEY = *(Key++);
	}

	for(uint8_t i = 0; i < AES_DATASIZE; i++)
	{
		AES.STATE = *(EncryptedData++);
	}

	/* 
		Set module in decryption mode and start it
	*/
	AES.CTRL |= (AES_START_bm | AES_DECRYPT_bm);

	while(AES_IsBusy());

	/* 
		Check for error
	*/
	if(!(AES.STATUS & AES_ERROR_bm))
	{
		for(uint8_t i = 0; i < AES_DATASIZE; i++)
		{
			*(Data++) = AES.STATE;
		}
		
		return 0;
	}

	return -1;
}

const uint8_t AES_CBC_Encrypt(const uint8_t* Data, uint8_t* EncryptedData, uint8_t* Key, const uint8_t* InitVector, const uint16_t BlockCount)
{
	uint8_t* KeyTemp;

  	/* 
	  Use the initialization vector for the first encryption
	*/
	for(uint8_t i = 0; i < AES_DATASIZE; i++)
	{
		AES.STATE = *(InitVector++);
	}

	/* 
		Set module in encryption mode, enables the XOR feature and the AUTO start
	*/
	AES.CTRL = (AES.CTRL & (~AES_DECRYPT_bm)) | AES_XOR_bm | AES_AUTO_bm;

	for(uint8_t Block = 0; Block < BlockCount; Block++)
	{
		// Reset the pointer for the key each block
		KeyTemp = Key;
		for(uint8_t i = 0; i < AES_KEYSIZE; i++)
		{
			AES.KEY = *(KeyTemp++);
		}

		for(uint8_t i = 0; i < AES_DATASIZE; i++)
		{
			AES.STATE = *(Data++);
		}

		while(AES_IsBusy());

		/* 
			Check for error
		*/
		if(!(AES.STATUS & AES_ERROR_bm))
		{
			for(uint8_t i = 0; i < AES_DATASIZE; i++)
			{
				*(EncryptedData++) = AES.STATE;
			}
		}
		else
		{
			/* 
				Disable auto mode and the XOR function
			*/
			AES.CTRL = (AES.CTRL & ~(AES_XOR_bm | AES_AUTO_bm));

			return -1;
		}
	}

	AES.CTRL = (AES.CTRL & ~(AES_XOR_bm | AES_AUTO_bm));

	return 0;
}

const uint8_t AES_CBC_Decrypt(uint8_t* EncryptedData, uint8_t* Data, uint8_t* Key, const uint8_t* InitVector, const uint16_t BlockCount)
{
	uint8_t* KeyTemp;

	for(uint8_t Block = BlockCount; Block > 0; Block--)
	{
		// Reset the pointer for the key each block
		KeyTemp = Key;
		for(uint8_t i = 0; i < AES_KEYSIZE; i++)
		{
			AES.KEY = *(KeyTemp++);
		}

		for(uint8_t i = 0; i < AES_DATASIZE; i++)
		{
			AES.STATE = *(EncryptedData++);
		}

		/* 
			Set the module in decryption mode, enable XOR and start it
		*/
		AES.CTRL |= (AES_DECRYPT_bm | AES_XOR_bm | AES_START_bm);
		while(AES_IsBusy());

		/* 
			Check for error
		*/
		if(!(AES.STATUS & AES_ERROR_bm))
		{
			/* 
				Use the initialization vector for the first block
			*/
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
				for(uint8_t i = 0; i < AES_DATASIZE; i++)
				{
					AES.STATE = *(LastBlock++);
				}
			}

			for(uint8_t i = 0; i < AES_DATASIZE; i++)
			{
				*(Data++) = AES.STATE;
			}
			
			AES.CTRL = AES.CTRL & (~AES_XOR_bm);
		}
		else
		{
			return -1;
		}
	}

	return 0;
}