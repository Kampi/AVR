/*
 * BatteryBackup.h
 * 
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega battery backup.

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

/** @file BatteryBackup/BatteryBackup.h
 *  @brief Driver for XMega battery backup.
 *
 *  @author Daniel Kampert
 */

#ifndef BATTERYBACKUP_H_
#define BATTERYBACKUP_H_

 #include <avr/io.h>
 #include <util/atomic.h>

 typedef enum
 {
	 BATTERY_STATUS_NO_POWER = 0x00,
	 BATTERY_STATUS_BBPOR = 0x01,
	 BATTERY_STATUS_BBBOD = 0x02,
	 BATTERY_STATUS_XOSCFAIL = 0x03,
	 BATTERY_STATUS_OK = 0x04,
	 BATTERY_STATUS_INIT = 0x05
 } BatteryStatus_t;

 BatteryStatus_t Battery_CheckStatus(void);
 void Battery_Init(void);
 void Battery_WriteRegister(uint8_t Register, uint8_t Data);
 uint8_t Battery_ReadRegister(uint8_t Register);

#endif /* BATTERYBACKUP_H_ */