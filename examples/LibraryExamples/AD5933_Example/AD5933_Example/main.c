/*
 * main.c
 * 
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: AD5933 example for XMega.

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

/** @file main.c
 *  @brief AD5933 example for XMega.
 *
 *  Software for the XMega AD5933 example from
 *  https://www.kampis-elektroecke.de/2019/02/impedanzmessung-mit-einem-ad5933-impedance-converter/
 *
 *  @author Daniel Kampert
 */

#include "Arch/XMega/PMIC/PMIC.h"
#include "Arch/XMega/USART/USART.h"
#include "Peripheral/AD5933/AD5933.h"

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = &AD5933_INTERFACE,
	.BitRate = AD5933_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

/*
	USART configuration
 */
USART_Config_t Config_USART = {
	.Device = &USARTE0,
	.DeviceMode = USART_MODE_ASYNCH,
	.Direction = USART_DIRECTION_BOTH,
	.Baudrate = 19200,
	.Parity = USART_PARITY_NONE,
	.Stop = USART_STOP_1,
	.Size = USART_SIZE_8, 
	.BSCALE = -5, 
	.EnableDoubleSpeed = FALSE,
	.EnableInterruptSupport = TRUE,
	.InterruptLevel = INT_LVL_HI
};

/*
	AD5933 configuration
*/
AD5933_Configuration_t Config_AD5933 = {
	.ClockSource = AD5933_CLOCK_INT,
	.Gain = AD5933_GAIN_1,
	.OutputVoltage = AD5933_OUTPUT_2V,
};

AD5933_SweepSetup_t Config_Sweep = {
	.StartFrequency = 30000,
	.FrequencyIncrement = 100,
	.Cycles = AD5933_SWEEP_LENGTH,
	.SettlingCycles = 1,
	.Mult = AD5933_SETTLING_X1,
};

AD5933_SingleFrequency_t Config_Single = {
	.Frequency = 30000,
	.SettlingCycles = 1,
	.Mult = AD5933_SETTLING_X1,
};

int16_t Temperature;
AD5933_DataPoint_t AD5933_SingleImpedance;
AD5933_CalPoint_t AD5933_SingleCalibrationData;

AD5933_CalPoint_t AD5933_SweepCalibrationData[AD5933_SWEEP_LENGTH];
AD5933_DataPoint_t AD5933_SweepImpedance[AD5933_SWEEP_LENGTH];

int main(void)
{

	// Initialize the USART interface
	USART_Init(&Config_USART);
	
	// Initialize the AD5933 and the I2C interface
	AD5933_Init(&Config_I2CM, &Config_AD5933);

	// Enable global interrupts
	EnableGlobalInterrupts();
	PMIC_EnableAllInterruptLevel();

	// Start message
	USART_Print(Config_USART.Device, "--- AD5933 example ---");

	// Read the temperature
	AD5933_ReadTemperature(&Temperature);

	// Set the output frequency
	AD5933_ConfigSingleFrequency(&Config_Single);

	// Run a calibration and a measurement at a single frequency with an 100 ohms resistor
	AD5933_SingleCalibration(&AD5933_SingleCalibrationData, AD5933_CALIBRATIONIMPEDANCE);
	AD5933_SingleMeasurement(&AD5933_SingleCalibrationData, &AD5933_SingleImpedance);

	// Configure a new frequency sweep
	AD5933_ConfigSweep(&Config_Sweep);
	
	/*
		//Two point calibration example

		AD5933_CalPoint_t CalA;
		AD5933_CalPoint_t CalB;
		
		uint32_t FreqA = 30000;
		uint32_t FreqB = 60000;

		AD5933_SetStartFrequency(FreqA);
		AD5933_SingleCalibration(&CalA, CalibrationImpedance);

		AD5933_SetStartFrequency(FreqB);
		AD5933_SingleCalibration(&CalB, CalibrationImpedance);

		double dF = FreqB - FreqA;
		double dG = CalB.GainFactor - CalA.GainFactor;
		double dP = CalB.Phase - CalA.Phase;

		AD5933_SweepMeasurment2PC(dG, dF, dP, Config_Sweep.FrequencyIncrement, CalA.GainFactor, CalA.Phase, AD5933_SweepImpedance);
	*/
	
	// Run a calibration and a measurement over the programmed frequency sweep with an 100 ohms resistor
	AD5933_SweepCalibration(AD5933_SweepCalibrationData, AD5933_CALIBRATIONIMPEDANCE);
	AD5933_SweepMeasurment(AD5933_SweepCalibrationData, AD5933_SweepImpedance);

    while(1) 
    {
    }
	
	return 0;
}

