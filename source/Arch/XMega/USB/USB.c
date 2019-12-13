/*
 * USB.c
 *
 * Created: 11.05.2017 21:28:03
 *  Author: Kampi
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega USB

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

#include "XMega/USB/UDD.h"
#include "XMega/USB/USB.h"
#include "XMega/ClockManagment/SysClock.h"

struct USB_Data_t
{
	uint32_t fifo[USB_ENDPOINTS + 1];
	USB_EP_t EP[2 * (USB_ENDPOINTS + 1)];
	uint16_t frame_number;
};
static volatile struct USB_Data_t USB_Data;

void USB_Init(USB_ClockSource_t USBClock)
{
	USB_PowerEnable();
	
	if(USBClock == USB_SOURCE_PLL)
	{
		//PLL_Config_t USBPLL_Config;
	}
	else if(USBClock == USB_SOURCE_RC32)
	{
		DFLL_Disable(&DFLLRC32M);
		
		// Set the calibrated 32 MHz oscillator as clock source for the USB
		SysClock_ConfigUSBClock(USB_SOURCE_RC32, SYSCLOCK_USB_PRESCALER_1);
		
		DFLL_Enable(&DFLLRC32M);
	}
	
	USB_SetSpeed(USB_SPEED_HIGH);
	
	// Calibrate the USB interface
	USB_Calibrate();
	
	// Configure USB interface
	USB_SetMaxEndpoints(USB_ENDPOINTS);
	USB_Enable();
	USB_StoreFrameNumber(TRUE);
}

void USB_Calibrate(void)
{
	NVM.CMD  = NVM_CMD_READ_CALIB_ROW_gc;
	USB.CAL0 = pgm_read_byte(offsetof(NVM_PROD_SIGNATURES_t, USBCAL0));
	NVM.CMD  = NVM_CMD_READ_CALIB_ROW_gc;
	USB.CAL1 = pgm_read_byte(offsetof(NVM_PROD_SIGNATURES_t, USBCAL1));
}

void USB_Reset(void)
{
	USB_EnableFIFO();
	USB_EnableInterrupts(INT_LVL_LO);

	USB_Attach();
}

void UDD_Init(void)
{
	USB_Init(USB_SOURCE_RC32);

	USB.EPPTR = (uint16_t)USB_Data.EP;
	USB_Reset();
}

ISR(USB_BUSEVENT_vect)
{
	if(USB.INTFLAGSACLR & USB_SOFIF_bm)
	{
		USB.INTFLAGSACLR = USB_SOFIF_bm;
	}
	
	if(USB_INTFLAGSASET & USB_RSTIF_bm)
	{
	}
	
	if (USB_INTFLAGSASET & USB_SUSPENDIF_bm)
	{
		USB_INTFLAGSACLR = USB_SUSPENDIF_bm;
	}

	if (USB_INTFLAGSASET & USB_RESUMEIF_bm)
	{
		USB_INTFLAGSACLR = USB_RESUMEIF_bm;
	}
}

ISR(USB_TRNCOMPL_vect)
{
	static uint8_t TT = 0;
}