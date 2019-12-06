/*
 * GPIO.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR8 XMega GPIO interface.

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

/** @file Arch/XMega/GPIO/GPIO.h
 *  @brief Driver for Atmel AVR8 GPIO XMega interface.
 *
 *  This file contains the prototypes and definitions for the AVR8 XMega GPIO driver.
 *
 *  @author Daniel Kampert
 *  @bug - Event mode
 */

#ifndef GPIO_H_
#define GPIO_H_

 #include "Common/Common.h"

 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/EventSystem/EventSystem.h"

 /** @brief	GPIO callback definition.
 */
 typedef void (*GPIO_Callback_t)(void);

 /** @brief GPIO output directions
  */
 typedef enum
 {
	 GPIO_DIRECTION_IN = 0x00,							/**< Direction input */ 
	 GPIO_DIRECTION_OUT = 0x01,							/**< Direction output */ 
 } GPIO_Direction_t;

 /** @brief GPIO clock period outputs
  */
 typedef enum
 {
	 GPIO_CLKOUT_1X = 0x00,								/**< Output CLK_PER */ 
	 GPIO_CLKOUT_2X = 0x01,								/**< Output CLK_PER2 */ 
	 GPIO_CLKOUT_3X = 0x02,								/**< Output CLK_PER3 */ 
 } GPIO_ClkOut_t;

 /** @brief GPIO pull configuration
  */
 typedef enum
 {
	 GPIO_OUTPUTCONFIG_TOTEM = (0x00 << 0x03),			/**< Totem pull configuration */ 
	 GPIO_OUTPUTCONFIG_BUSKEEPER = (0x01 << 0x03),		/**< Bus keeper configuration */ 
	 GPIO_OUTPUTCONFIG_PULLDOWN = (0x02 << 0x03),		/**< Pull down configuration */ 
	 GPIO_OUTPUTCONFIG_PULLUP = (0x03 << 0x03),			/**< Pull up configuration */ 
	 GPIO_OUTPUTCONFIG_WIREDOR = (0x04 << 0x03),		/**< Wired or configuration */ 
	 GPIO_OUTPUTCONFIG_WIREDAND = (0x05 << 0x03),		/**< Wired and configuration */ 
	 GPIO_OUTPUTCONFIG_WIREDORDOWN = (0x06 << 0x03),	/**< Wired or pull down configuration */ 
	 GPIO_OUTPUTCONFIG_WIREDANDUP = (0x07 << 0x03),		/**< Wired and pull up configuration */ 
 } GPIO_PullConfig_t;

 /** @brief GPIO interrupt channels
  */
 typedef enum
 {
	 GPIO_INTERRUPT_0 = 0x01,							/**< GPIO interrupt channel 0 */ 
	 GPIO_INTERRUPT_1 = 0x02,							/**< GPIO interrupt channel 1 */ 
 } GPIO_InterruptChannel_t;

 /** @brief GPIO input sense configurations
  */
 typedef enum 
 {
	GPIO_SENSE_BOTH = 0x00,								/**< Sense rising and falling edge */ 
	GPIO_SENSE_RISING = 0x01,							/**< Sense rising edge only */ 
	GPIO_SENSE_FALLING = 0x02,							/**< Sense falling edge only */ 
	GPIO_SENSE_LOWLEVEL = 0x03,							/**< Sense low level */
 } GPIO_InputSense_t;

 /** @brief GPIO interrupt configuration object
  */
 typedef struct
 {
	 PORT_t* Port;										/**< Pointer to port object */
	 uint8_t Pin;										/**< Pin number */
	 GPIO_InterruptChannel_t Channel;					/**< Interrupt channel */
	 GPIO_InputSense_t Sense;							/**< GPIO interrupt type */
	 Interrupt_Level_t InterruptLevel;					/**< Interrupt level */
	 GPIO_PullConfig_t PullConfig;						/**< Pull configuration */
	 GPIO_Callback_t Callback;							/**< Function pointer to GPIO callback */
 } GPIO_InterruptConfig_t;

 /** @brief				Set the direction of an I/O port.
  *  @param Port		Pointer to port object
  *  @param Mask		Port mask
  *  @param Direction	I/O Direction
  */
 static inline void GPIO_SetPortDirection(PORT_t* Port, const uint8_t Mask, const GPIO_Direction_t Direction) __attribute__((always_inline));
 static inline void GPIO_SetPortDirection(PORT_t* Port, const uint8_t Mask, const GPIO_Direction_t Direction)
 {
	if(Direction == GPIO_DIRECTION_IN)
	{
		Port->DIRCLR = Mask;
	}
	else if(Direction == GPIO_DIRECTION_OUT)
	{
		Port->DIRSET = Mask;
	}
 }

 /** @brief				Set the direction of an I/O pin.
  *  @param Port		Pointer to port object
  *  @param Pin			Pin number
  *  @param Direction	I/O Direction
  */
 static inline void GPIO_SetDirection(PORT_t* Port, const uint8_t Pin, const GPIO_Direction_t Direction) __attribute__((always_inline));
 static inline void GPIO_SetDirection(PORT_t* Port, const uint8_t Pin, const GPIO_Direction_t Direction)
 {
	if(Direction == GPIO_DIRECTION_IN)
	{
		Port->DIRCLR = (0x01 << Pin);
	}
	else if(Direction == GPIO_DIRECTION_OUT)
	{
		Port->DIRSET = (0x01 << Pin);
	}
 }

 /** @brief			Set an I/O as output.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_SetDir(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void GPIO_SetDir(PORT_t* Port, const uint8_t Pin)
 {	
	Port->DIRSET = 0x01 << Pin;
 }

 /** @brief			Set an I/O as input.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_ClearDir(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void GPIO_ClearDir(PORT_t* Port, const uint8_t Pin)
 {
	Port->DIRCLR = 0x01 << Pin;
 }

 /** @brief			Toggle the direction of an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_ToggleDirection(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void GPIO_ToggleDirection(PORT_t* Port, const uint8_t Pin)
 {
	Port->DIRTGL = 0x01 << Pin;
 }

 /** @brief			Read the value of an I/O port.
  *  @param Port	Pointer to port object
  *  @return		I/O state
  */
 static inline const Bool_t GPIO_ReadPort(PORT_t* Port) __attribute__((always_inline));
 static inline const Bool_t GPIO_ReadPort(PORT_t* Port)
 {
	 return Port->IN;
 }

 /** @brief			Read the value of an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  *  @return		I/O state
  */
 static inline const Bool_t GPIO_Read(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline const Bool_t GPIO_Read(PORT_t* Port, const uint8_t Pin)
 {
	 return (Port->IN & (0x01 << Pin)) >> Pin;
 }

 /** @brief			Apply a data mask to set the complete port.
  *  @param Port	Pointer to port object
  *  @param Mask	Data mask
  */
 static inline void GPIO_SetPort(PORT_t* Port, const uint8_t Mask) __attribute__((always_inline));
 static inline void GPIO_SetPort(PORT_t* Port, const uint8_t Mask)
 {	
	Port->OUT = Mask;
 }

 /** @brief			Set an I/O to high state.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Set(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void GPIO_Set(PORT_t* Port, const uint8_t Pin)
 {	
	Port->OUTSET = 0x01 << Pin;
 }

 /** @brief			Apply a data mask to clear the complete port.
  *  @param Port	Pointer to port object
  *  @param Mask	Data mask
  */
 static inline void GPIO_ClearPort(PORT_t* Port, const uint8_t Mask) __attribute__((always_inline));
 static inline void GPIO_ClearPort(PORT_t* Port, const uint8_t Mask)
 {
	Port->OUTCLR = Mask;
 }

 /** @brief			Clear an I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Clear(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void GPIO_Clear(PORT_t* Port, const uint8_t Pin)
 {
	Port->OUTCLR = 0x01 << Pin;
 }

 /** @brief			Toggle the output of an I/O pin.
  *  @param Port	Pointer to port object
  *  @param Pin		Pin number
  */
 static inline void GPIO_Toggle(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void GPIO_Toggle(PORT_t* Port, const uint8_t Pin)
 {
	Port->OUTTGL = 0x01 << Pin;
 }

 /** @brief			Enable/Disable the invert mode of the masked I/O.
  *  @param Port	Pointer to port object
  *  @param Mask	I/O mask
  *  @param Enable	Enable/Disable
  */
 static inline void GPIO_SwitchInvert(PORT_t* Port, const uint8_t Mask, const Bool_t Enable) __attribute__((always_inline));
 static inline void GPIO_SwitchInvert(PORT_t* Port, const uint8_t Mask, const Bool_t Enable)
 {
	 PORTCFG.MPCMASK = Mask;

	 if(Enable == TRUE)
	 {
		 Port->PIN0CTRL |= PORT_INVEN_bm;
	 }
	 else
	 {
		 Port->PIN0CTRL &= ~PORT_INVEN_bm;
	 }
 }

 /** @brief			Enable/Disable the slew rate limit of the masked I/O.
  *  @param Port	Pointer to port object
  *  @param Mask	I/O mask
  *  @param Enable	Enable/Disable
  */
 static inline void GPIO_SwitchSlewRate(PORT_t* Port, const uint8_t Mask, const Bool_t Enable) __attribute__((always_inline));
 static inline void GPIO_SwitchSlewRate(PORT_t* Port, const uint8_t Mask, const Bool_t Enable)
 {
	 PORTCFG.MPCMASK = Mask;
	 
	 if(Enable == TRUE)
	 {
		 Port->PIN0CTRL |= PORT_SRLEN_bm;
	 }
	 else
	 {
		 Port->PIN0CTRL &= ~PORT_SRLEN_bm;
	 }
 }

 /** @brief			Switch the position for the clock and the event output from pin 7 to pin 4 or vice versa.
  *  @param Enable	Enable/Disable
  */
 static inline void GPIO_SwitchOutputLoc(const Bool_t Enable) __attribute__((always_inline));
 static inline void GPIO_SwitchOutputLoc(const Bool_t Enable)
 {
	 if(Enable == TRUE)
	 {
		 PORTCFG.CLKEVOUT |= PORTCFG_CLKEVPIN_bm;
	 }
	 else
	 {
		 PORTCFG.CLKEVOUT &= ~PORTCFG_CLKEVPIN_bm;
	 }	
 }

 /** @brief			Configure the output clock for the CLKOUT pin.
  *  @param Clock	Output clock
  */
 static inline void GPIO_ConfigClockOutput(const GPIO_ClkOut_t Clock) __attribute__((always_inline));
 static inline void GPIO_ConfigClockOutput(const GPIO_ClkOut_t Clock)
 {
	 PORTCFG.CLKEVOUT = (PORTCFG.CLKEVOUT & (~(0x03 << 0x02))) | (Clock << 0x02);
 }

 /** @brief			Enable the output of the peripheral clock on Pin 7.
  *  @param Port	Pointer to clock output port
  */
 static inline void GPIO_EnableClockOutput(PORT_t* Port) __attribute__((always_inline));
 static inline void GPIO_EnableClockOutput(PORT_t* Port)
 {
	 // Configure the IO as an output
	 GPIO_SetDirection(Port, 7, GPIO_DIRECTION_OUT);
	 
	 if(Port == &PORTC)
	 {
		 PORTCFG.CLKEVOUT |= 0x01;
	 }
	 else if(Port == &PORTD)
	 {
		 PORTCFG.CLKEVOUT |= 0x02;
	 }
	 else if(Port == &PORTE)
	 {
		 PORTCFG.CLKEVOUT |= 0x03;
	 }
 }

 /** @brief			Disable the output of the peripheral clock on Pin 7.
  *  @param Port	Pointer to clock output port
  */
 static inline void GPIO_DisableClockOutput(PORT_t* Port) __attribute__((always_inline));
 static inline void GPIO_DisableClockOutput(PORT_t* Port)
 {
	 if(Port == &PORTC)
	 {
		 PORTCFG.CLKEVOUT &= ~0x01;
	 }
	 else if(Port == &PORTD)
	 {
		 PORTCFG.CLKEVOUT &= ~0x02;
	 }
	 else if(Port == &PORTE)
	 {
		 PORTCFG.CLKEVOUT &= ~0x03;
	 }
 } 

 /** @brief	Disable the output of all peripheral clocks on pin 7.
  *			NOTE: Can not used together on the same port with the clock output.
  */
 static inline void GPIO_DisableAllClockOutput(void) __attribute__((always_inline));
 static inline void GPIO_DisableAllClockOutput(void)
 {
	 PORTCFG.CLKEVOUT &= ~0x03;
 }

 /** @brief	Enable the output of the RTC clock on port C pin 6.
  *			NOTE: Can not used together on the same port with the clock output.
  */
 static inline void GPIO_EnableRTCClock(void) __attribute__((always_inline));
 static inline void GPIO_EnableRTCClock(void)
 {
	 PORTCFG.CLKEVOUT |= PORTCFG_RTCOUT_bm;
 }
 
 /** @brief	Disable the output of the RTC clock on port C pin 6.
  *			NOTE: Can not used together on the same port with the clock output.
  */
 static inline void GPIO_DisableRTCClock(void) __attribute__((always_inline));
 static inline void GPIO_DisableRTCClock(void)
 {
	 PORTCFG.CLKEVOUT &= ~PORTCFG_RTCOUT_bm;
 }
 
 /** @brief			Output event channel 0 on pin 7 of a given port.
  *					NOTE: Can not used together on the same port with the clock output.
  *  @param Port	Pointer to port object
  *  @param Channel	Event channel
  */
 static inline void GPIO_EnableEventOutput(PORT_t* Port, const Event_Channel_t Channel) __attribute__((always_inline));
 static inline void GPIO_EnableEventOutput(PORT_t* Port, const Event_Channel_t Channel)
 {
	 GPIO_SetDirection(Port, 0x07, GPIO_DIRECTION_OUT);
	 
	 if(Port == &PORTC)
	 {
		 PORTCFG.CLKEVOUT |= (0x01 << 0x04);
	 }
	 else if(Port == &PORTD)
	 {
		 PORTCFG.CLKEVOUT |= (0x02 << 0x04);
	 }
	 else if(Port == &PORTE)
	 {
		 PORTCFG.CLKEVOUT |= (0x03 << 0x04);
	 }

	 PORTCFG.EVOUTSEL |= (Channel & 0x07);
 }

 /** @brief			Disable the output of an event channel at a given port.
  *  @param Port	Pointer to port object
  *  @param Channel	Event channel
  */
 static inline void GPIO_DisableEventOutput(PORT_t* Port, const Event_Channel_t Channel) __attribute__((always_inline));
 static inline void GPIO_DisableEventOutput(PORT_t* Port, const Event_Channel_t Channel)
 {
	 if(Port == &PORTC)
	 {
		 PORTCFG.CLKEVOUT &= ~(0x01 << 0x04);
	 }
	 else if(Port == &PORTD)
	 {
		 PORTCFG.CLKEVOUT &= ~(0x02 << 0x04);
	 }
	 else if(Port == &PORTE)
	 {
		 PORTCFG.CLKEVOUT &= ~(0x03 << 0x04);
	 }
	 
	 PORTCFG.EVOUTSEL &= ~(Channel & 0x07);
 }

 /** @brief			Disable all event outputs.
  *  @param Port	Pointer to port object
  */
 static inline void GPIO_DisableAllEventOutput(PORT_t* Port) __attribute__((always_inline));
 static inline void GPIO_DisableAllEventOutput(PORT_t* Port)
 {
	 PORTCFG.CLKEVOUT &= ~(0x03 << 0x04);
	 PORTCFG.EVOUTSEL = 0x00;
 }

 /** @brief			Set the configuration of the masked I/O.
  *					NOTE: Use this to set the input and output configuration at the same time.
  *  @param Port	Pointer to port object
  *  @param Pin		I/O pin
  *  @param Config	Configuration mask
  */
 static inline void GPIO_SetConfig(PORT_t* Port, const uint8_t Pin, const uint8_t Config) __attribute__((always_inline));
 static inline void GPIO_SetConfig(PORT_t* Port, const uint8_t Pin, const uint8_t Config)
 {
	 asm volatile("break");
	 PORTCFG.MPCMASK = (0x01 << Pin);
	 
	 Port->PIN0CTRL = (Port->PIN0CTRL & (~0x3F)) | Config;
 }

 /** @brief			Set the pull configuration of the I/O.
  *  @param Port	Pointer to port object
  *  @param Pin		I/O pin
  *  @param Config	Pull configuration
  */
 static inline void GPIO_SetPullConfig(PORT_t* Port, const uint8_t Pin, const GPIO_PullConfig_t Config) __attribute__((always_inline));
 static inline void GPIO_SetPullConfig(PORT_t* Port, const uint8_t Pin, const GPIO_PullConfig_t Config)
 {
	 PORTCFG.MPCMASK = (0x01 << Pin);
	 
	 Port->PIN0CTRL = (Port->PIN0CTRL & (~0x38)) | Config;
 }
 
 /** @brief			Set the interrupt sense of the I/O.
					NOTE: Only PORTA - PORTF support this option.
  *  @param Port	Pointer to port object
  *  @param Pin		I/O pin
  *  @param Sense	Interrupt sense
  */
 static inline void GPIO_SetInputSense(PORT_t* Port, const uint8_t Pin, const GPIO_InputSense_t Sense) __attribute__((always_inline));
 static inline void GPIO_SetInputSense(PORT_t* Port, const uint8_t Pin, const GPIO_InputSense_t Sense)
 {
	 PORTCFG.MPCMASK = (0x01 << Pin);
	 
	 Port->PIN0CTRL = (Port->PIN0CTRL & (~0x07)) | Sense;
 }

 /** @brief			Enable the digital buffer of the I/O.
					NOTE: Only PORTA - PORTF support this option.
  *  @param Port	Pointer to port object
  *  @param Pin		I/O pin
  */
 static inline void GPIO_EnableBuffer(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void GPIO_EnableBuffer(PORT_t* Port, const uint8_t Pin)
 {
	 PORTCFG.MPCMASK = (0x01 << Pin);
	 
	 Port->PIN0CTRL &= ~0x07;
 }

 /** @brief			Disable the digital buffer of the I/O.
					NOTE: Only PORTA - PORTF support this option.
  *  @param Port	Pointer to port object
  *  @param Pin		I/O pin
  */
 static inline void GPIO_DisableBuffer(PORT_t* Port, const uint8_t Pin) __attribute__((always_inline));
 static inline void GPIO_DisableBuffer(PORT_t* Port, const uint8_t Pin)
 {
	 PORTCFG.MPCMASK = (0x01 << Pin);
	 
	 Port->PIN0CTRL |= 0x07;
 }

 /** @brief			Install a new GPIO interrupt callback.
  *  @param Config	Pointer to configuration structure.
  */
 void GPIO_InstallCallback(GPIO_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Port		Pointer to GPIO port object
  *  @param Channel		Interrupt channel
  */
 void GPIO_RemoveCallback(PORT_t* Port, const GPIO_InterruptChannel_t Channel);

 /** @brief					Change the interrupt level of a GPIO port interrupt.
  *  @param Port			Pointer to GPIO port object
  *  @param Pin				Pin number
  *  @param Channel			Interrupt channel
  *  @param InterruptLevel	New interrupt level.
  */
 void GPIO_ChangeInterruptLevel(PORT_t* Port, const uint8_t Pin, const GPIO_InterruptChannel_t Channel, const Interrupt_Level_t InterruptLevel);

#endif /* GPIO_H_ */