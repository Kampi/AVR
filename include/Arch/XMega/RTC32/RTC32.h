/*
 * RTC32.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega RTC32 module.

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

/** @file Arch/XMega/RTC32/RTC32.h
 *  @brief Driver for XMega RTC32 module.
 *
 *  This file contains the prototypes and definitions for the XMega RTC32. driver.
 *
 *  NOTE: You have to enable the brown out detection for the battery backup system.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs.
 */

#ifndef RTC32_H_
#define RTC32_H_

 #include "Common/Common.h"

 #include "Arch/XMega/PMIC/PMIC.h"
 #include "Arch/XMega/ClockManagement/SysClock.h"

 /** 
  * RTC32 callback definition
  */
 typedef void (*RTC32_Callback_t)(void);

 /** 
  * RTC32 callback types
  */
 typedef enum
 {
	 RTC32_OVFL_INTERRUPT = 0x01,			/**< RTC32 overflow interrupt */ 
	 RTC32_COMP_INTERRUPT = 0x02,			/**< RTC32 compare interrupt */ 
 } RTC32_CallbackType_t;

 /** 
  * RTC32 configuration object
  */
 typedef struct
 {
	 uint32_t Period;						/**< RTC period value */ 
	 uint32_t Count;						/**< RTC count value */ 
	 uint32_t Compare;						/**< RTC compare value */ 
 } RTC32_Config_t;

 /** 
  * RTC32 interrupt configuration object
  */
 typedef struct
 {
	 RTC32_CallbackType_t CallbackSource;	/**< RTC32 interrupt type */ 
	 Interrupt_Level_t InterruptLevel;		/**< Interrupt level */ 
	 RTC32_Callback_t Callback;				/**< Function pointer to RTC32 callback */ 
 } RTC32_InterruptConfig_t;

 /** @brief Request a sync and wait for finish.
  */
 static inline void RTC32_Sync(void) __attribute__ ((always_inline));
 static inline void RTC32_Sync(void)
 {
	 RTC32.SYNCCTRL |= RTC32_SYNCCNT_bm;
	 while(RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm);
 }

 /** @brief			Set the count register of the RTC32 module.
  *  @param Count	Count value
  */
 static inline void RTC32_SetCount(const uint32_t Count) __attribute__ ((always_inline));
 static inline void RTC32_SetCount(const uint32_t Count)
 {
	 RTC32.CNT = Count;
 }

 /** @brief		Get the count register of the RTC32 module.
  *  @return	Count value
  */
 static inline uint32_t RTC32_GetCount(void) __attribute__ ((always_inline));
 static inline uint32_t RTC32_GetCount(void)
 {
	 RTC32_Sync();
	 return RTC32.CNT;
 }

 /** @brief			Set the period register of the RTC32 module.
  *  @param Period	Period value
  */
 static inline void RTC32_SetPeriod(const uint32_t Period) __attribute__ ((always_inline));
 static inline void RTC32_SetPeriod(const uint32_t Period)
 {
	 RTC32.PER = Period;
 }

 /** @brief		Get the period register of the RTC32 module.
  *  @return	Period value
  */
 static inline const uint32_t RTC32_GetPeriod(void) __attribute__ ((always_inline));
 static inline const uint32_t RTC32_GetPeriod(void)
 {
	 RTC32_Sync();
	 return RTC32.PER;
 }

 /** @brief			Set the compare register of the RTC32 module.
  *  @param Compare	Compare value
  */
 static inline void RTC32_SetCompare(const uint32_t Compare) __attribute__ ((always_inline));
 static inline void RTC32_SetCompare(const uint32_t Compare)
 {
	 RTC32.COMP = Compare;
 } 

 /** @brief		Get the compare register of the RTC32 module.
  *  @return	Compare value
  */
 static inline const uint32_t RTC32_GetCompare(void) __attribute__ ((always_inline));
 static inline const uint32_t RTC32_GetCompare(void)
 {
	 RTC32_Sync();
	 return RTC32.COMP;
 }

 /** @brief	Enable the RTC32 module.
  */
 static inline void RTC32_Enable(void) __attribute__ ((always_inline));
 static inline void RTC32_Enable(void)
 {
	 RTC32.CTRL |= RTC32_ENABLE_bm;
	 while(RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm);
 }

 /** @brief	Disable the RTC32 module.
  */
 static inline void RTC32_Disable(void) __attribute__ ((always_inline));
 static inline void RTC32_Disable(void)
 {
	 RTC32.CTRL &= ~RTC32_ENABLE_bm;
	 while(RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm);
 }

 /** @brief			Initialize the RTC32 module.
  *  @param Config	Pointer to RTC32 configuration struct
  */
 void RTC32_Init(RTC32_Config_t* Config);
 
 /** @brief			Get the configuration of the RTC32 module.
  *  @param Config	Pointer to RTC32 device configuration struct
  */
 void RTC32_GetConfig(RTC32_Config_t* Config);
 
 /** @brief					Change the interrupt level of the RTC32 module interrupts.
  *  @param Callback		Callback which should be changed
  *  @param InterruptLevel	New interrupt level
  */
 void RTC32_ChangeInterruptLevel(const RTC32_CallbackType_t Callback, const Interrupt_Level_t InterruptLevel);
 
 /** @brief			Install a new callback for the RTC32 module.
  *  @param Config	Pointer to configuration structure
  */
 void RTC32_InstallCallback(const RTC32_InterruptConfig_t* Config);
 
 /** @brief				Remove an installed callback.
  *  @param Callback	Callback which should be removed
  */
 void RTC32_RemoveCallback(const RTC32_CallbackType_t Callback);

#endif /* RTC32_H_ */