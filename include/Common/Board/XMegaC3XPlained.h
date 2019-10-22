/*
 * XMegaC3XPlained.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Hardware driver for the Atmel XPlainedC3 Development board.

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

/** @file Common/Board/XMegaC3XPlained.h
 *  @brief Hardware driver for the Atmel XPlainedC3 development board.
 *
 *  @author Daniel Kampert
 */

#ifndef XMEGAC3XPLAINED_H_
#define XMEGAC3XPLAINED_H_

 /*
	Define the MCU for the board
 */
 #include "Common/Board/MCU/XMega384C3.h"

 /*
	Hardware driver
 */
 #include "Arch/XMega/AC/AC.h"
 #include "Arch/XMega/ADC/ADC.h"
 #include "Arch/XMega/AES/AES.h"
 #include "Arch/XMega/CPU/CPU.h"
 #include "Arch/XMega/CRC/CRC.h"
 #include "Arch/XMega/DMA/DMA.h"
 #include "Arch/XMega/I2C/I2C.h"
 #include "Arch/XMega/SPI/SPI.h"
 #include "Arch/XMega/NVM/NVM.h"
 #include "Arch/XMega/RTC/RTC.h"
 #include "Arch/XMega/GPIO/GPIO.h"
 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/USART/USART.h"
 #include "Arch/XMega/Watchdog/Watchdog.h"
 #include "Arch/XMega/ClockManagement/PLL.h"
 #include "Arch/XMega/ClockManagement/DFLL.h"
 #include "Arch/XMega/EventSystem/EventSystem.h"
 #include "Arch/XMega/ClockManagement/SysClock.h"
 #include "Arch/XMega/ClockManagement/ExtClock.h"
 #include "Arch/XMega/PowerManagement/PowerManagement.h"
 
 /*
	Peripheral driver
 */
 #include "Peripheral/SD/SD.h"
 #include "Peripheral/AnalogSensors.h"
 
 /*
	Services
 */
 #include "Common/Services/FatFs/FatFs.h"
 #include "Common/Services/SleepManager/SleepManager.h"
 #include "Common/Services/DisplayManager/DisplayManager.h"

#endif /* XMEGAC3XPLAINED_H_ */