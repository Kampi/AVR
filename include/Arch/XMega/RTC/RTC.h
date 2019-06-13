/*
 * RTC.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega RTC module.

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

/** @file Arch/XMega/RTC/RTC.h
 *  @brief Driver for XMega RTC module.
 *
 *  This file contains the prototypes and definitions for the XMega RTC driver.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef RTC_H_
#define RTC_H_

 #include "Common/Common.h"

 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/ClockManagement/SysClock.h"
 #include "Arch/XMega/ClockManagement/ExtClock.h"
 #include "Arch/XMega/PowerManagement/PowerManagement.h"

 /** 
  * RTC callback definition
  */
 typedef void (*RTC_Callback_t)(void);

 /** 
  * RTC clock sources
  */
 typedef enum
 {
	 RTC_CLOCK_ULP = 0x00,					/**< 1 kHz from internal 32 kHz ULP oscillator */
	 RTC_CLOCK_TOSC = 0x01,					/**< 1 kHz from external 32 kHz oscillator on TOSC */
	 RTC_CLOCK_RCOSC = 0x02,				/**< 1 kHz from internal 32 kHz oscillator */
	 RTC_CLOCK_TOSC32 = 0x05,				/**< 32 kHz from external 32 kHz oscillator on TOSC */
	 RTC_CLOCK_RCOSC32 = 0x06,				/**< 32 kHz from internal 32 kHz oscillator */
	 RTC_CLOCK_EXT = 0x07,					/**< External clock on TOSC1 */
 } RTC_ClockSource_t;

 /** 
  * RTC prescaler
  */
 typedef enum
 {
	 RTC_PRESCALER_1 = 0x01,				/**< Prescaler 1 */
	 RTC_PRESCALER_2 = 0x02,				/**< Prescaler 2 */
	 RTC_PRESCALER_8 = 0x03,				/**< Prescaler 8 */
	 RTC_PRESCALER_16 = 0x04,				/**< Prescaler 16 */
	 RTC_PRESCALER_64 = 0x05,				/**< Prescaler 64 */
	 RTC_PRESCALER_256 = 0x06,				/**< Prescaler 256 */
	 RTC_PRESCALER_1024 = 0x07,				/**< Prescaler 1024 */
 } RTC_Prescaler_t;

 /** 
  * RTC callback types
  */
 typedef enum
 {
	 RTC_OVFL_INTERRUPT = 0x01,				/**< RTC overflow interrupt */
	 RTC_COMP_INTERRUPT = 0x02,				/**< RTC compare interrupt */
 } RTC_CallbackType_t;

 /** 
  * RTC configuration object
  */
 typedef struct
 {
	 RTC_ClockSource_t ClockSource;			/**< RTC clock source */
	 RTC_Prescaler_t Prescaler;				/**< RTC prescaler */
	 uint32_t Period;						/**< RTC period value */ 
	 uint32_t Count;						/**< RTC count value */ 
	 uint32_t Compare;						/**< RTC compare value */ 
 } RTC_Config_t;

 /** 
  * RTC interrupt configuration object
  */
 typedef struct
 {
	 RTC_CallbackType_t CallbackSource;		/**< RTC interrupt type */ 
	 Interrupt_Level_t InterruptLevel;		/**< Interrupt level */ 
	 RTC_Callback_t Callback;				/**< Function pointer to RTC callback */ 
 } RTC_InterruptConfig_t;

 /** @brief			Enable the clock source for the RTC module.
  *  @param Source	Clock source for the RTC module
  */
 static inline void RTC_WaitSync(void) __attribute__ ((always_inline));
 static inline void RTC_WaitSync(void)
 {
	 while(RTC.STATUS & RTC_SYNCBUSY_bm);
 }

 /** @brief			Enable the clock source for the RTC module.
  *  @param Source	Clock source for the RTC module
  */
 static inline void RTC_EnableClockSource(const RTC_ClockSource_t Source) __attribute__ ((always_inline));
 static inline void RTC_EnableClockSource(const RTC_ClockSource_t Source)
 {
	 switch(Source)
	 {
		 // Internal 32 kHz RC oscillator
		 case(RTC_CLOCK_ULP):
		 {
			 break;
		 }
		 case(RTC_CLOCK_RCOSC):
		 case(RTC_CLOCK_RCOSC32):
		 {
			 OSC.CTRL |= OSC_RC32KEN_bm;
			 while(!(OSC.STATUS & OSC_RC32KRDY_bm));
			 break;
		 }
		 // External 32 kHz oscillator
		 case(RTC_CLOCK_TOSC):
		 case(RTC_CLOCK_TOSC32):
		 {
			 OSC.XOSCCTRL = OSC_X32KLPM_bm | OSC_XOSCSEL_32KHz_gc;
			 OSC.CTRL = OSC_XOSCEN_bm;
			 while(!(OSC.STATUS & OSC_XOSCRDY_bm));
			 break;
		 }
		 // External clock
		 case(RTC_CLOCK_EXT):
		 {
			 break;
		 }
		 default:
		 {
			 break;
		 }
	 }
	 
	 CLK.RTCCTRL = ((Source & 0x07) << 0x01) | CLK_RTCEN_bm;
 }

 /** @brief	Disable the clock source for the RTC module.
  */
 static inline void RTC_DisableClockSource(void) __attribute__ ((always_inline));
 static inline void RTC_DisableClockSource(void)
 {
	 CLK.RTCCTRL &= ~CLK_RTCEN_bm;
	 
	 // Disable internal 32 kHz oscillator
	 OSC.CTRL |= OSC_RC32KEN_bm;
 }

 /** @brief				Enable the RTC module.
  *  @param Clock		Clock source for the RTC
  *  @param Prescaler	RTC clock prescaler
  */
 static inline void RTC_Enable(const RTC_ClockSource_t Clock, const RTC_Prescaler_t Prescaler) __attribute__ ((always_inline));
 static inline void RTC_Enable(const RTC_ClockSource_t Clock, const RTC_Prescaler_t Prescaler)
 {
	 RTC_EnableClockSource(Clock);

	 RTC.CTRL = Prescaler;
 }

 /** @brief	Disable the RTC module.
  */
 static inline void RTC_Disable(void) __attribute__ ((always_inline));
 static inline void RTC_Disable(void)
 {
	 RTC_DisableClockSource();

	 RTC.CTRL = 0x00;
 }

 /** @brief			Set the count register of the RTC module.
  *  @param Count	Count value
  */
 static inline void RTC_SetCount(const uint16_t Count) __attribute__ ((always_inline));
 static inline void RTC_SetCount(const uint16_t Count)
 {
	 RTC_WaitSync();
	 RTC.CNT = Count;
 }

 /** @brief		Get the count register of the RTC module.
  *  @return	Count value
  */
 static inline uint16_t RTC_GetCount(void) __attribute__ ((always_inline));
 static inline uint16_t RTC_GetCount(void)
 {
	 return RTC.CNT;
 }

 /** @brief			Set the period register of the RTC module.
  *  @param Period	Period value
  */
 static inline void RTC_SetPeriod(const uint16_t Period) __attribute__ ((always_inline));
 static inline void RTC_SetPeriod(const uint16_t Period)
 {
	 RTC_WaitSync();
	 RTC.PER = Period;
 }

 /** @brief		Get the period register of the RTC module.
  *  @return	Period value
  */
 static inline const uint16_t RTC_GetPeriod(void) __attribute__ ((always_inline));
 static inline const uint16_t RTC_GetPeriod(void)
 {
	 return RTC.PER;
 }

 /** @brief			Set the compare register of the RTC module.
  *  @param Compare	Compare value
  */
 static inline void RTC_SetCompare(const uint16_t Compare) __attribute__ ((always_inline));
 static inline void RTC_SetCompare(const uint16_t Compare)
 {
	 RTC_WaitSync();
	 RTC.COMP = Compare;
 } 

 /** @brief		Get the compare register of the RTC module.
  *  @return	Compare value
  */
 static inline const uint16_t RTC_GetCompare(void) __attribute__ ((always_inline));
 static inline const uint16_t RTC_GetCompare(void)
 {
	 return RTC.COMP;
 }

 /** @brief		Get the prescaler of the RTC module.
  *  @return	Compare value
  */
 static inline const RTC_Prescaler_t RTC_GetPrescaler(void) __attribute__ ((always_inline));
 static inline const RTC_Prescaler_t RTC_GetPrescaler(void)
 {
	 return (RTC_Prescaler_t)(RTC.CTRL & 0x07);
 }

 /** @brief			Initialize the RTC module.
  *  @param Config	Pointer to RTC configuration struct
  */
 void RTC_Init(RTC_Config_t* Config);
 
 /** @brief			Get the configuration of the RTC module.
  *  @param Config	Pointer to RTC device configuration struct
  */
 void RTC_GetConfig(RTC_Config_t* Config);
 
 /** @brief					Change the interrupt level of the RTC module interrupts.
  *  @param Callback		Callback which should be changed
  *  @param InterruptLevel	New interrupt level
  */
 void RTC_ChangeInterruptLevel(const RTC_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel);
 
 /** @brief			Install a new callback for the RTC module.
  *  @param Config	Pointer to configuration structure
  */
 void RTC_InstallCallback(const RTC_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Callback	Callback which should be removed
  */
 void RTC_RemoveCallback(const RTC_CallbackType_t Callback);

#endif /* RTC_H_ */