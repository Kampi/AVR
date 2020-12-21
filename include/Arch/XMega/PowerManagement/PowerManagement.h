/*
 * PowerManagement.h
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for Atmel AVR XMega power management.

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

/** @file Arch/XMega/PowerManagement/PowerManagement.h
 *  @brief Driver for Atmel AVR XMega power management.
 *
 * This file contains the prototypes and definitions for the Atmel AVR XMega power management driver.
 *
 *  @author Daniel Kampert
 */

#ifndef POWERMANAGEMENT_H_
#define POWERMANAGEMENT_H_

 #include "Common/Common.h"

 #include "Arch/XMega/PMIC/PMIC.h"

 /** @brief	UCPU sleep modes.
 */
 typedef enum
 {
	 SLEEPMODE_IDLE = 0x00,			/**< Idle */ 	
	 SLEEPMODE_PDOWN = 0x02,		/**< Power down */ 
	 SLEEPMODE_PSAVE = 0x03,		/**< Power save */ 
	 SLEEPMODE_STDBY = 0x06,		/**< Standby */ 
	 SLEEPMODE_ESTDBY = 0x07,		/**< Extended standby */ 
 } SleepMode_t;

 /** @brief			Set the device into sleep mode.
  *  @param Mode	Sleep mode
  */
 static inline void EnableSleep(const SleepMode_t Mode) __attribute__((always_inline));
 static inline void EnableSleep(const SleepMode_t Mode)
 {
	 SLEEP.CTRL |= SLEEP_SEN_bm;
	 SLEEP.CTRL = Mode | (SLEEP.CTRL & ~SLEEP_SMODE_gm);
	 
	 // Enable interrupts to wake up
	 sei();
	 
	 // Enter sleep mode
	 asm volatile ("sleep" "\n\t" ::); 
	 
	 // Wake up here
	 SLEEP.CTRL &= ~SLEEP_SEN_bm;
 }
 
 /** @brief	Disable all peripherals.
  */
 static inline void PowerManagment_FreezeAll(void) __attribute__((always_inline));
 static inline void PowerManagment_FreezeAll(void)
 {
	 PR.PRGEN = 0x5F;
	 PR.PRPA = 0x07;
	  
	 #if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
		 PR.PRPB = 0x07;
	 #endif
	
	 PR.PRPC = 0x7F;
	 PR.PRPD = 0x7F;
	 PR.PRPE = 0x7F;
	 PR.PRPF = 0x7F;
 }
 
 /** @brief	Enable all peripherals.
  */
 static inline void PowerManagment_UnfreezeAll(void) __attribute__((always_inline));
 static inline void PowerManagment_UnfreezeAll(void)
 {
	 PR.PRGEN = 0x00;
	 PR.PRPA = 0x00;
	  
	 #if(MCU_NAME == MCU_NAME_ATXMEGA256A3BU)
		  PR.PRPB = 0x00;
	 #endif
	 
	 PR.PRPC = 0x00;
	 PR.PRPD = 0x00;
	 PR.PRPE = 0x00;
	 PR.PRPF = 0x00;
 }

 /** @brief			Enable TC0 module.
  *  @param Device	Pointer to TC0 object
  */
 static inline void Timer0_PowerEnable(TC0_t* Device) __attribute__((always_inline));
 static inline void Timer0_PowerEnable(TC0_t* Device)
 {
	 if(Device == &TCC0)
	 {
		 PR.PRPC &= ~PR_TC0_bm;
	 }
	 else if(Device == &TCD0)
	 {
		 PR.PRPD &= ~PR_TC0_bm;
	 }
	 else if(Device == &TCE0)
	 {
		 PR.PRPE &= ~PR_TC0_bm;
	 }
	 else if(Device == &TCF0)
	 {
		 PR.PRPF &= ~PR_TC0_bm;
	 }
 }

 /** @brief			Disable TC0 module.
  *  @param Device	Pointer to TC0 object
  */
 static inline void Timer0_PowerDisable(TC0_t* Device) __attribute__((always_inline));
 static inline void Timer0_PowerDisable(TC0_t* Device)
 {
	 if(Device == &TCC0)
	 {
		 PR.PRPC |= PR_TC0_bm;
	 }
	 else if(Device == &TCD0)
	 {
		 PR.PRPD |= PR_TC0_bm;
	 }
	 else if(Device == &TCE0)
	 {
		 PR.PRPE |= PR_TC0_bm;
	 }
	 else if(Device == &TCF0)
	 {
		 PR.PRPF |= PR_TC0_bm;
	 }
 }

 /** @brief			Enable TC1 module.
  *  @param Device	Pointer to TC1 object
  */
 static inline void Timer1_PowerEnable(TC1_t* Device) __attribute__((always_inline));
 static inline void Timer1_PowerEnable(TC1_t* Device)
 {
	 if(Device == &TCC1)
	 {
		 PR.PRPC &= ~PR_TC0_bm;
	 }
	 
	 #if(defined TCD1)
		 else if(Device == &TCD1)
		 {
			 PR.PRPD &= ~PR_TC0_bm;
		 }
	 #endif
	 
	 #if(defined TCE1)
		 else if(Device == &TCE1)
		 {
			 PR.PRPE &= ~PR_TC0_bm;
		 }
	 #endif
 }

 /** @brief			Disable TC1 module
  *  @param Device	Pointer to TC1 object
  */
 static inline void Timer1_PowerDisable(TC1_t* Device) __attribute__((always_inline));
 static inline void Timer1_PowerDisable(TC1_t* Device)
 {
	 if(Device == &TCC1)
	 {
		 PR.PRPC |= PR_TC1_bm;
	 }

	 #if(defined TCD1)
		 else if(Device == &TCD1)
		 {
			 PR.PRPD |= PR_TC1_bm;
		 }
	 #endif

	 #if(defined TCE1)
		 else if(Device == &TCE1)
		 {
			 PR.PRPE |= PR_TC1_bm;
		 }
	 #endif
 }

 /** @brief			Enable TC2 module
  *  @param Device	Pointer to TC2 object
  */
 static inline void Timer2_PowerEnable(TC2_t* Device) __attribute__((always_inline));
 static inline void Timer2_PowerEnable(TC2_t* Device)
 {
	 // ToDo
 }

 /** @brief			Disable TC2 module.
  *  @param Device	Pointer to TC2 object
  */
 static inline void Timer2_PowerDisable(TC2_t* Device) __attribute__((always_inline));
 static inline void Timer2_PowerDisable(TC2_t* Device)
 {
	 // ToDo
 }

 /** @brief			Enable AC module.
  *  @param Device	Pointer to AC object
  */
 static inline void AC_PowerEnable(AC_t* Device) __attribute__((always_inline));
 static inline void AC_PowerEnable(AC_t* Device)
 {
	 if(Device == &ACA)
	 {
	 	 PR.PRPA &= ~PR_AC_bm;
	 }
	 
	 #if(defined ACB)
		 else if(Device == &ACB)
		 {
			 PR.PRPB &= ~PR_AC_bm;
		 }
	 #endif
 }

 /** @brief			Disable AC module.
  *  @param Device	Pointer to AC object
  */
 static inline void AC_PowerDisable(AC_t* Device) __attribute__((always_inline));
 static inline void AC_PowerDisable(AC_t* Device)
 {
	 Device->CTRLA &= ~AC_ENABLE_bm;

	 if(Device == &ACA)
	 {
		 PR.PRPA |= PR_AC_bm;
	 }

	 #if(defined ACB)
		 else if(Device == &ACB)
		 {
			 PR.PRPB |= PR_AC_bm;
		 }
	 #endif
 }

 /** @brief			Enable ADC module.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_PowerEnable(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_PowerEnable(ADC_t* Device)
 {
	 if(Device == &ADCA)
	 {
		 PR.PRPA &= ~PR_ADC_bm;
	 }

	 #if(defined ADCB)
		 else if(Device == &ADCB)
		 {
			 PR.PRPB &= ~PR_ADC_bm;
		 }
	 #endif
 }

 /** @brief			Disable ADC module.
  *  @param Device	Pointer to ADC object
  */
 static inline void ADC_PowerDisable(ADC_t* Device) __attribute__((always_inline));
 static inline void ADC_PowerDisable(ADC_t* Device)
 {
	 Device->CTRLA &= ~ADC_ENABLE_bm;

	 if(Device == &ADCA)
	 {
		 PR.PRPA |= PR_ADC_bm;
	 }

	 #if(defined ADCB)
		 else if(Device == &ADCB)
		 {
			 PR.PRPB |= PR_ADC_bm;
		 }
	 #endif
 }

 #if((defined DACA) || (defined DACB))
	 /** @brief			Enable DAC module.
	  *  @param Device	Pointer to DAC object
	  */
	 static inline void DAC_PowerEnable(DAC_t* Device) __attribute__((always_inline));
	 static inline void DAC_PowerEnable(DAC_t* Device)
	 {
		 #if(defined DACB)
			 if(Device == &DACB)
			 {
				 PR.PRPB &= ~PR_DAC_bm;
			 }
		 #endif
		 
		 #if(defined DACA)
			 if(Device == &DACA)
			 {
				 PR.PRPA &= ~PR_DAC_bm;
			 }
		 #endif
	 }

	 /** @brief			Disable DAC module.
	  *  @param Device	Pointer to DAC object
	  */
	 static inline void DAC_PowerDisable(DAC_t* Device) __attribute__((always_inline));
	 static inline void DAC_PowerDisable(DAC_t* Device)
	 {
		 Device->CTRLA &= ~DAC_ENABLE_bm;

		 #if(defined DACB)
			 if(Device == &DACB)
			 {
				 PR.PRPB |= PR_DAC_bm;
			 }
		 #endif
		 
		 #if(defined DACA)
			 if(Device == &DACA)
			 {
				 PR.PRPA |= PR_DAC_bm;
			 }
		 #endif
	 }
 #endif

 /** @brief			Enable USART module.
  *  @param Device	Pointer to USART object
  */
 static inline void USART_PowerEnable(USART_t* Device) __attribute__((always_inline));
 static inline void USART_PowerEnable(USART_t* Device)
 {
	 if(Device == &USARTC0)
	 {
		 PR.PRPC &= ~PR_USART0_bm;
	 }
	 
	 #if(defined USARTC1)
		 if(Device == &USARTC1)
		 {
			 PR.PRPC &= ~PR_USART1_bm;
		 }
	 #endif

	 if(Device == &USARTD0)
	 {
		 PR.PRPD &= ~PR_USART0_bm;
	 }
	 
	 #if(defined USARTD1)
		 if(Device == &USARTD1)
		 {
	 		 PR.PRPD &= ~PR_USART1_bm;
		 }
	 #endif
	 
	 if(Device == &USARTE0)
	 {
		 PR.PRPE &= ~PR_USART0_bm;
	 }

	 #if(defined USARTE1)
		 if(Device == &USARTE1)
		 {
			 PR.PRPE &= ~PR_USART1_bm;
		 }
	 #endif

	 #if(defined USARTF0)
		 if(Device == &USARTF0)
		 {
			 PR.PRPF &= ~PR_USART0_bm;
		 }
	 #endif
 
	 #if(defined USARTF1)
		 if(Device == &USARTF1)
		 {
			 PR.PRPF &= ~PR_USART1_bm;
		 }
	 #endif
 }

 /** @brief			Disable USART module.
  *  @param Device	Pointer to USART object
  */
 static inline void USART_PowerDisable(USART_t* Device) __attribute__((always_inline));
 static inline void USART_PowerDisable(USART_t* Device)
 {
	 if(Device == &USARTC0)
	 {
		 PR.PRPC |= PR_USART0_bm;
	 }

	 #if(defined USARTC1)
		 if(Device == &USARTC1)
		 {
			 PR.PRPC |= PR_USART1_bm;
		 }
	 #endif

	 if(Device == &USARTD0)
	 {
		 PR.PRPD |= PR_USART0_bm;
	 }

	 #if(defined USARTD1)
		 if(Device == &USARTD1)
		 {
			 PR.PRPD |= PR_USART1_bm;
		 }
	 #endif

	 if(Device == &USARTE0)
	 {
		  PR.PRPE |= PR_USART0_bm;
	 }
		 
	 #if(defined USARTE1)
		 if(Device == &USARTE1)
		 {
			 PR.PRPE |= PR_USART1_bm;
		 }
	 #endif
	 
	 #if(defined USARTF0)
		 if(Device == &USARTF0)
		 {
			 PR.PRPF |= PR_USART0_bm;
		 }
	 #endif
	 

	 #if(defined USARTF1)
		 if(Device == &USARTF1)
		 {
			 PR.PRPF |= PR_USART1_bm;
		 }
	 #endif
 }

 /** @brief			Enable TWI module.
  *  @param Device	Pointer to TWI object
  */
 static inline void I2C_PowerEnable(TWI_t* Device) __attribute__((always_inline));
 static inline void I2C_PowerEnable(TWI_t* Device)
 {
	 if(Device == &TWIC)
	 {
		 PR.PRPC &= ~PR_TWI_bm;
	 }
		 
	 if(Device == &TWIE)
	 {
		 PR.PRPE &= ~PR_TWI_bm;
	 }
	  
	 #if(defined TWID)
		 if(Device == &TWID)
		 {
			 PR.PRPD &= ~PR_TWI_bm;
		 }
	 #endif
	 
	 #if(defined TWIF)
		 if(Device == &TWIF)
		 {
			 PR.PRPF &= ~PR_TWI_bm;
		 }
	 #endif
 }

 /** @brief			Disable TWI module.
  *  @param Device	Pointer to TWI object
  */
 static inline void I2C_PowerDisable(TWI_t* Device) __attribute__((always_inline));
 static inline void I2C_PowerDisable(TWI_t* Device)
 {
	 if(Device == &TWIC)
	 {
		 PR.PRPC |= PR_TWI_bm;
	 }
		 
	 if(Device == &TWIE)
	 {
		 PR.PRPE |= PR_TWI_bm;
	 }
	 
	 #if(defined TWID)
		 if(Device == &TWID)
		 {
			 PR.PRPD |= PR_TWI_bm;
		 }
	 #endif
	 
	 #if(defined TWIF)
		 if(Device == &TWIF)
		 {
			 PR.PRPF |= PR_TWI_bm;
		 }
	 #endif
 }

 /** @brief			Enable SPI module.
  *  @param Device	Pointer to SPI object.
  */
 static inline void SPI_PowerEnable(SPI_t* Device) __attribute__((always_inline));
 static inline void SPI_PowerEnable(SPI_t* Device)
 {
	 if(Device == &SPIC)
	 {
		 PR.PRPC &= ~PR_SPI_bm;
	 }

	 if(Device == &SPID)
	 {
		 PR.PRPD &= ~PR_SPI_bm;
	 }
	 
	 #if(defined SPIE)
		 if(Device == &SPIE)
		 {
			 PR.PRPE &= ~PR_SPI_bm;
		 }
	 #endif
	 
	 #if(defined SPIF)
		 if(Device == &SPIF)
		 {
			 PR.PRPF &= ~PR_SPI_bm;
		 }
	 #endif
 }

 /** @brief			Disable SPI module.
  *  @param Device	Pointer to SPI object
  */
 static inline void SPI_PowerDisable(SPI_t* Device) __attribute__((always_inline));
 static inline void SPI_PowerDisable(SPI_t* Device)
 {
	 if(Device == &SPIC)
	 {
		 PR.PRPC |= PR_SPI_bm;
	 }

	 if(Device == &SPID)
	 {
		 PR.PRPD |= PR_SPI_bm;
	 }
	 
	 #if(defined SPIE)
		 if(Device == &SPIE)
		 {
			 PR.PRPE |= PR_SPI_bm;
		 }
	 #endif
	 
	 #if(defined SPIF)
		 if(Device == &SPIF)
		 {
			 PR.PRPF |= PR_SPI_bm;
		 }
	 #endif
 }

 /** @brief	Enable the RTC module.	
  */
 #if(defined RTC)
	 static inline void RTC_PowerEnable(void) __attribute__((always_inline));
	 static inline void RTC_PowerEnable(void)
	 {
		 PR.PRGEN &= ~PR_RTC_bm;
	 }

	/** @brief	Enable the RTC module.	
	 */
	 static inline void RTC_PowerDisable(void) __attribute__((always_inline));
	 static inline void RTC_PowerDisable(void)
	 {
		 PR.PRGEN |= PR_RTC_bm;
	 }
 #endif

 /** @brief	Enable the DMA module.	
  */
 static inline void DMA_PowerEnable(void) __attribute__((always_inline));
 static inline void DMA_PowerEnable(void)
 {
	 PR.PRGEN &= ~PR_DMA_bm;
 }

 /** @brief	Disable the DMA module.
  */
 static inline void DMA_PowerDisable(void) __attribute__((always_inline));
 static inline void DMA_PowerDisable(void)
 {
	 PR.PRGEN |= PR_DMA_bm;
 }

 /** @brief	Enable the AES module.
  */
 static inline void AES_PowerEnable(void) __attribute__((always_inline));
 static inline void AES_PowerEnable(void)
 {
	 PR.PRGEN &= ~PR_AES_bm;
 }

 /** @brief	Disable the AES module.
  */
 static inline void AES_PowerDisable(void) __attribute__((always_inline));
 static inline void AES_PowerDisable(void)
 {
	 PR.PRGEN |= PR_AES_bm;
 }

 /** @brief	Enable the USB module.
  */
 static inline void USB_PowerEnable(void) __attribute__((always_inline));
 static inline void USB_PowerEnable(void)
 {
	 PR.PRGEN &= ~PR_USB_bm;
 }

 /** @brief	Disable the USB module.
  */
 static inline void USB_DisableEnable(void) __attribute__((always_inline));
 static inline void USB_DisableEnable(void)
 {
	 PR.PRGEN |= PR_USB_bm;
 }

#endif /* POWERMANAGEMENT_H_ */