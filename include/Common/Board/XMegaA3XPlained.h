/*
 * XMegaA3XPlained.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Hardware definitions for the Atmel XPlainedA3BU Development board.

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
 *  @brief Hardware definitions for the Atmel XPlainedA3BU development board.
 *
 *  @author Daniel Kampert
 */

#ifndef XMEGAA3XPLAINED_H_
#define XMEGAA3XPLAINED_H_

 /*
	Define the MCU for the used board
 */
 #include "Common/Board/MCU/XMega256A3BU.h"
 
 /*
	Peripheral definitions
 */

 /* 
	LEDs
 */
 // Yellow LED0
 #define LED0										PORTR, 0

 // Yellow LED1
 #define LED1										PORTR, 1

 // Red/Green LED
 #define LED2										PORTD, 4

 // Green LED
 #define LED3										PORTD, 5
 
 /* 
	Push buttons
 */
 #define SW0										PORTE, 5
 #define SW1										PORTF, 1
 #define SW2										PORTF, 2
 
 /*
	Analog sensors
 */
 // Light sensor
 #define LIGHT_SENSOR								0

 // Temperature sensor
 #define TEMPERATURE_SENSOR							1

 // SPI interface for SPI peripherals
 #define SPI_CLOCK									1000000
 #define SPI_DEFAULT_INTERFACE						&USARTD0							

 // I2C interface for SPI peripherals
 #define I2C_CLOCK									100000
 #define I2C_DEFAULT_INTERFACE						&TWIC
 
 /*
	Reference voltage [mV]
 */
 #define REFERENCE_VOLTAGE							3300

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
 #include "Arch/XMega/Timer0/Timer0.h"
 #include "Arch/XMega/Timer1/Timer1.h"
 #include "Arch/XMega/Timer2/Timer2.h"
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