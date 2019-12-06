/*
 * AC.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for AVR8 AC

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */ 

#ifndef AC_H_
#define AC_H_

 #if(MCU_NAME == MCU_NAME_ATMEGA32)
	 #include "Board/MCU/Mega32.h"
 #else
	  #error "No valid mcu"
 #endif
 
 #include "Arch/AVR8/ATmega/CPU/CPU.h"
 
 #include "Common/AC_Base.h"

#endif /* AC_H_ */