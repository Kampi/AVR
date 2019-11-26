/*
 * XMegaA3XPlained.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Hardware driver for the Atmel XPlainedA3BU Development board.

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

/** @file Common/Board/XMegaA3XPlained.h
 *  @brief Hardware driver for the Atmel XPlainedA3BU development board.
 *
 *  @author Daniel Kampert
 */

#ifndef XMEGAA3XPLAINED_H_
#define XMEGAA3XPLAINED_H_

 #define SPI_CLOCK									1000000
 #define SPI_DEFAULT_INTERFACE						&USARTD0

 #define I2C_CLOCK									100000
 #define I2C_DEFAULT_INTERFACE						&TWIC

 #include "Common/Board/MCU/XMega256A3BU.h"

 /*
	Hardware driver
 */
 #include "Arch/XMega/AC/AC.h"
 #include "Arch/XMega/ADC/ADC.h"
 #include "Arch/XMega/AES/AES.h"
 #include "Arch/XMega/CPU/CPU.h"
 #include "Arch/XMega/CRC/CRC.h"
 #include "Arch/XMega/DAC/DAC.h"
 #include "Arch/XMega/DMA/DMA.h"
 #include "Arch/XMega/I2C/I2C.h"
 #include "Arch/XMega/SPI/SPI.h"
 #include "Arch/XMega/GPIO/GPIO.h"
 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/USART/USART.h"
 #include "Arch/XMega/RTC32/RTC32.h"
 #include "Arch/XMega/IRCOM/IRCOM.h"
 #include "Arch/XMega/Timer/Timer.h"
 #include "Arch/XMega/Watchdog/Watchdog.h"
 #include "Arch/XMega/ClockManagement/PLL.h"
 #include "Arch/XMega/ClockManagement/DFLL.h"
 #include "Arch/XMega/EventSystem/EventSystem.h"
 #include "Arch/XMega/ClockManagement/SysClock.h"
 #include "Arch/XMega/ClockManagement/ExtClock.h"
 #include "Arch/XMega/BatteryBackup/BatteryBackup.h"
 #include "Arch/XMega/PowerManagement/PowerManagement.h"

 /*
	Peripheral includes
 */
 #include "Peripheral/AnalogSensors.h"
 #include "Peripheral/ST7565R/ST7565R.h"
 //#include "Peripheral/AT45DB642D/AT45DB642D.h"
 
 /*
	Services
 */
 //#include "Common/Services/USB/USB.h"
 #include "Common/Services/SleepManager/SleepManager.h"
 #include "Common/Services/DisplayManager/DisplayManager.h"

 /*
	Additional includes
 */
 #include <stdio.h>

#endif /* XMEGAA3XPLAINED_H_ */