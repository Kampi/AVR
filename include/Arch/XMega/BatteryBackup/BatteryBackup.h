/*
 * BatteryBackup.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega battery backup system.

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

/** @file Arch/XMega/BatteryBackup/BatteryBackup.h
 *  @brief Driver for Atmel AVR XMega battery backup system.
 *
 *  This file contains the prototypes and definitions for the Atmel AVR XMega battery backup driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef BATTERYBACKUP_H_
#define BATTERYBACKUP_H_

 #include "Common/Common.h"
 
 #include "Arch/XMega/CPU/CPU.h"
 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/ClockManagement/ExtClock.h"
 
 /** @brief	RTC clock frequency.
  */
 typedef enum
 {
	BATTERYBACKUP_CLOCK_1HZ = 0x00,			/**< Clock frequency 1 Hz */ 
	BATTERYBACKUP_CLOCK_1KHZ = 0x01,		/**< Clock frequency 1 kHz */ 
 } BatteryBackup_Clock_t;

 /** @brief	Battery backup data register.
  */
 typedef enum
 {
	 BATTERYBACKUP_REGISTER0 = 0x00,		/**< Register 0 */ 
	 BATTERYBACKUP_REGISTER1 = 0x01,		/**< Register 1 */ 
 } BatteryBackupBackupRegister_t;

 /** @brief	Battery backup status.
  */
 typedef enum
 {
	 BATTERYBACKUP_STATUS_OK = 0x00,		/**< Status "OK" */ 
	 BATTERYBACKUP_STATUS_INIT = 0x01,		/**< Status "Initialization" */ 
	 BATTERYBACKUP_STATUS_BBPOR = 0x02,		/**< "Status "Battery backup power-on detection" */ 
	 BATTERYBACKUP_STATUS_BBBOD = 0x03,		/**< Status "Battery backup brown-out detection" */ 
	 BATTERYBACKUP_STATUS_XOSCFAIL = 0x04,	/**< Status "Crystal Oscillator Failure" */ 
	 BATTERYBACKUP_STATUS_NO_POWER = 0x05,	/**< Status "No power" */ 
 } BatteryBackupStatus_t;

 /** @brief Enable the external oscillator.
  */
 static inline void BatteryBackup_EnableExtOsc(void) __attribute__((always_inline));
 static inline void BatteryBackup_EnableExtOsc(void)
 {
	 VBAT.CTRL |= VBAT_XOSCEN_bm;
	 while(!(VBAT.STATUS & VBAT_XOSCRDY_bm));
 }

 /** @brief Disable the external oscillator.
  */
 static inline void BatteryBackup_DisableExtOsc(void) __attribute__((always_inline));
 static inline void BatteryBackup_DisableExtOsc(void)
 {
	 VBAT.CTRL &= ~VBAT_XOSCEN_bm;
 }

 /** @brief Reset the battery backup system.
  */
 void BatteryBackup_Reset(void);

 /** @brief			Configure the external oscillator.
  *  @param Clock	RTC clock frequency
  *  @param HighESR	Enable/Disable high ESR mode
  */
 void BatteryBackup_ConfigXOSC(const BatteryBackup_Clock_t Clock, const bool HighESR);
 
 /** @brief				Write data to a backup register
  *  @param Register	Backup register
  *	 @param Data		Data byte
  */
 void BatteryBackup_WriteBackup(const BatteryBackupBackupRegister_t Register, const uint8_t Data);

 /** @brief				Read data from a backup register
  *  @param Register	Backup register
  *	 @return			Data byte
  */
 const uint8_t BatteryBackup_ReadBackup(const BatteryBackupBackupRegister_t Register);

 /** @brief				Check the battery backup module
  *  @param FirstStart	#true if the microcontroller has his first start
  *  @return			Status of the battery backup
  */
 const BatteryBackupStatus_t BatteryBackup_Check(const bool FirstStart);

#endif /* BATTERYBACKUP_H_ */