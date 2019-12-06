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
 *  @brief Hardware driver driver and on-board peripheral the Atmel AVR XPlainedC3 development board.
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
	 Board specific configurations
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
 #define SW0										PORTF, 1
 #define SW1										PORTF, 2
 
 /*
	Analog inputs
 */
 #define LIGHT_SENSOR								0
 #define TEMPERATURE_SENSOR							1

 /*
	 Default interfaces 
 */
 #define SPI_CLOCK									1000000
 #define SPI_DEFAULT_INTERFACE						&USARTD0							

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
 #include "Services/FatFs/FatFs.h"
 #include "Services/SleepManager/SleepManager.h"
 #include "Services/DisplayManager/DisplayManager.h"

#endif /* XMEGAC3XPLAINED_H_ */