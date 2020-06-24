/*
 * SD_ID.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *  Website: www.kampis-elektroecke.de
 *  File info: SD card IDs.
 
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

/** @file Peripheral/SD/SD_ID.h
 *  @brief SD card IDs.

 *  @author Daniel Kampert
 */

#ifndef SD_ID_H_
#define SD_ID_H_

/** @addtogroup SD
 *  @{
 */
	 /*\@}*/
	 /** @defgroup SD-Manufacturer
	  *  SD card manufacturer IDs.
	  *  @{
	  */
		 #define SD_MANUFACTURER_TOSHIBA					0x02				/**< Manufacturer ID for Toshiba */
		 #define SD_MANUFACTURER_SANDISK					0x03				/**< Manufacturer ID for SanDisk */
	 /** @} */ // end of Manufacturer
 /** @} */

 /** @brief		Convert a SD card manufacturer ID to a manufacturer string.
  *  @param ID	Manufacturer ID
  *  @return	Pointer to manufacturer string
  */
 static inline const char* SD_ID2Manufacturer(const uint8_t ID) __attribute__ ((always_inline));
 static inline const char* SD_ID2Manufacturer(const uint8_t ID)
 {
	 switch(ID)
	 {
		 case SD_MANUFACTURER_TOSHIBA:
		 {
			 return "Toshiba";

			 break;
		 }
		 case SD_MANUFACTURER_SANDISK:
		 {
			 return "SanDisk";

			 break;
		 }
		 default:
		 {
			 return "";
			 
			 break;
		 }
	 }
 }

#endif /* SD_ID_H_ */