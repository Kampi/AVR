/*
 * SleepManager.h
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AVR sleep manager service.

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

/** @file Services/SleepManager/SleepManager.h
 *  @brief AVR sleep manager service.
 *
 *  This contains the prototypes and definitions for the sleep service.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef SLEEPMANAGER_H_
#define SLEEPMANAGER_H_
 
 #if(MCU_ARCH == MCU_ARCH_XMEGA)
	 #include "Arch/XMega/CPU/CPU.h"
	 #include "Arch/XMega/PMIC/PMIC.h"
	 #include "Arch/XMega/PowerManagement/PowerManagement.h"
 #endif

 /** @brief	Sleep modes for sleep manager
  * 		NOTE: You can only sleep modes which are supported by the target device
  */
 typedef enum
 {
	 SLEEPMGR_ACTIVE = 0x00,						/**< No sleep mode */
	 SLEEPMGR_IDLE = 0x01,							/**< Idle mode */
	 SLEEPMGR_PDOWN = 0x02,							/**< Power down mode */
	 SLEEPMGR_PSAVE = 0x03,							/**< Power save mode */
	 SLEEPMGR_STDBY = 0x04,							/**< Standby mode */
	 SLEEPMGR_ESTDBY = 0x05,						/**< Extended standby */
 } SleepMgr_Modes_t;

 /** @brief	Initialize the sleep manager.
  */
 void SleepManager_Init(void);

 /** @brief		Get the deepest allowed sleep mode.
  *  @return	Deepest sleep mode
  */
 const SleepMgr_Modes_t SleepManager_GetSleepMode(void);

 /** @brief			Increase the lock count for a specific sleep manager mode.
  *  @param Mode	Sleep manager mode
  */
 void SleepManager_Lock(const SleepMgr_Modes_t Mode);
 
 /** @brief			Decrease the lock count for a specific sleep manager mode.
  *  @param Mode	Sleep manager mode
  */
 void SleepManager_Unlock(const SleepMgr_Modes_t Mode);

 /** @brief	Set the device in the deepest allowed sleep mode.
  */
 void SleepManager_EnterSleep(void);

#endif /* SLEEPMANAGER_H_ */