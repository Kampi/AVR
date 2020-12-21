/*
 * IR_Codes.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: IR remote key code mappings.

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

/** @file Interfaces/IR-Remote/IR_Codes.h
 *  @brief IR remote key code mappings.
 *		   NOTE: The mappings can be differ between the different remote controls!
 *
 *  @author Daniel Kampert
 */

#ifndef IR_CODES_H_
#define IR_CODES_H_

 /** @brief Remote control button 1.
  */
 #define IR_REMOTE_KEY_1							0x00FFA25D

 /** @brief Remote control button 2.
  */
 #define IR_REMOTE_KEY_2							0x00FF629D

 /** @brief Remote control button 3.
  */
 #define IR_REMOTE_KEY_3							0x00FFE21D

 /** @brief Remote control button 4.
  */
 #define IR_REMOTE_KEY_4							0x00FF22DD

 /** @brief Remote control button 5.
  */
 #define IR_REMOTE_KEY_5							0x00FF02FD

 /** @brief Remote control button 6.
  */
 #define IR_REMOTE_KEY_6							0x00FFC23D

 /** @brief Remote control button 7.
  */
 #define IR_REMOTE_KEY_7							0x00FFE01F

 /** @brief Remote control button 8.
  */
 #define IR_REMOTE_KEY_8							0x00FFA857

 /** @brief Remote control button 9.
  */
 #define IR_REMOTE_KEY_9							0x00FF906F

 /** @brief Remote control button 0.
  */
 #define IR_REMOTE_KEY_0							0x00FF9867

 /** @brief Remote control button hash.
  */
 #define IR_REMOTE_KEY_HASH							0x01FF604C

 /** @brief Remote control button star.
  */
 #define IR_REMOTE_KEY_STAR							0x00FF6897

 /** @brief Remote control button arrow up.
  */
 #define IR_REMOTE_KEY_ARROW_UP						0x00FF18E7

 /** @brief Remote control button arrow down.
  */
 #define IR_REMOTE_KEY_ARROW_DOWN					0x00FF4AB5

 /** @brief Remote control button arrow left.
  */
 #define IR_REMOTE_KEY_ARROW_LEFT					0x00FF10EF

 /** @brief Remote control button arrow right.
  */
 #define IR_REMOTE_KEY_ARROW_RIGHT					0x00FF5AA5

 /** @brief Remote control button OK.
  */
 #define IR_REMOTE_KEY_OK							0x00FF38C7

#endif /* IR_CODES_H_ */
