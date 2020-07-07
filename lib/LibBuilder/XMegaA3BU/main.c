#include "Board.h"

#include <stdio.h>
#include <stdlib.h>

#define	AES_BLOCKS								3

void DMA_TransactionComplete(uint8_t Channel);
void SW0_Callback(void);
void SW1_Callback(void);
void SW2_Callback(void);
void AC_Callback(void);
void Timer_Tick(void);
void RTC32_Tick(void);
void AES_Callback(void);

uint8_t I2C_WriteBuffer[TWI_BUFFER_SIZE];
uint8_t I2C_ReadBuffer[TWI_BUFFER_SIZE];
uint8_t I2C_SlaveBuffer[TWI_BUFFER_SIZE];

/*
 *	GPIO configuration
 */
GPIO_InterruptConfig_t Interrupt_SW0 = {
	.Port = GET_PERIPHERAL(SW0),
	.Pin = GET_INDEX(SW0),
	.Channel = GPIO_INTERRUPT_0,
	.InterruptLevel = INT_LVL_HI,
	.Sense = GPIO_SENSE_FALLING,
	.Callback = SW0_Callback
};

/*
 *	Note: Both push buttons use the same port and you can use only one
 *	callback per level for a complete port! So you have to use one callback
 *	for both buttons or use one channel per button.
 */
GPIO_InterruptConfig_t Interrupt_SW1 = {
	.Port = GET_PERIPHERAL(SW1),
	.Pin = GET_INDEX(SW1),
	.Channel = GPIO_INTERRUPT_0,
	.InterruptLevel = INT_LVL_HI,
	.Sense = GPIO_SENSE_FALLING,
	.Callback = SW1_Callback
};

GPIO_InterruptConfig_t Interrupt_SW2 = {
	.Port = GET_PERIPHERAL(SW2),
	.Pin = GET_INDEX(SW2),
	.Channel = GPIO_INTERRUPT_1,
	.InterruptLevel = INT_LVL_HI,
	.Sense = GPIO_SENSE_FALLING,
	.Callback = SW2_Callback
};

/*
	Timer configuration
 */
Timer0_Config_t Config_Timer0 = {
	.Device = &TCF0,
	.Prescaler = TIMER_PRESCALER_1024,
	.Period = 626
};

Timer0_InterruptConfig_t Interrupt_Timer0 = {
	.Device = &TCF0,
	.Source = TIMER_OVERFLOW_INTERRUPT,
	.InterruptLevel = INT_LVL_HI,
	.Callback = Timer_Tick
};

Timer0_WaveConfig_t WaveConfig_Timer0 = {
	.Device = &TCE0,
	.Channel = TIMER_CCB,
	.Prescaler = TIMER_PRESCALER_256,
	.Mode = TIMER_MODE_3,
	.Period = 1000,
	.DutyCycle = 100,
	.EnableInvert = false
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
	.EnableDoubleSpeed = false,
	.EnableInterruptSupport = true,
	.InterruptLevel = INT_LVL_HI
};

USART_InterruptConfig_t Interrupt_USART = {
	.Device = &USARTE0,
	.Source = USART_RXC_INTERRUPT,
	.InterruptLevel = INT_LVL_LO
};

/*
	SPI configuration
 */
SPIM_Config_t Config_SPI = {
	.SPIClock = SPI_CLOCK,
	.DataOrder = SPI_DATAORDER_MSB_FIRST,
	.Mode = SPI_MODE_0,
	.Prescaler = SPI_PRESCALER_16,
	.Device = SPI_DEFAULT_INTERFACE
};

/*
	I2C configuration
*/
I2CM_Config_t Config_I2CM = {
	.Device = I2C_DEFAULT_INTERFACE,
	.Bitrate = I2C_CLOCK,
	.Timeout = I2C_TIMEOUT_DISABLE,
};

I2CS_Config_t Config_I2CS = {
	.Device = &TWIE,
	.Address = 0x40,
	.InterruptLevel = INT_LVL_HI,
	.Buffer = I2C_SlaveBuffer
};

/*
	ADC configuration
 */
ADC_Config_t Config_ADC = {
	.Device = &ADCA, 
	.DMARequest = ADC_DMA_OFF, 
	.ConversionMode = ADC_CONVMODE_SIGNED, 
	.Format = ADC_CONV_FORM_12,
	.Reference =  ADC_REFERENCE_INTVCC, 
	.Prescaler = ADC_PRESCALER_512,
};

ADC_ChannelConfig_t AnalogPinConfig = {
	.Device = &ADCA, 
	.Channel = &ADCA.CH2, 
	.Gain = ADC_CHAN_GAIN_1, 
	.Mode = ADC_CHAN_SINGLE, 
	.Input = 4
};

/*
	Configure the pin with the temperature sensor for ADC channel 1
 */
ADC_ChannelConfig_t LightPinConfig = {
	.Device = &ADCA,
	.Channel = &ADCA.CH1,
	.Gain = ADC_CHAN_GAIN_1,
	.Mode = ADC_CHAN_SINGLE,
	.Input = LIGHT_SENSOR
};

/*
	Config the pin with the temperature sensor for ADC channel 0
 */
ADC_ChannelConfig_t TempPinConfig = {
	.Device = &ADCA,
	.Channel = &ADCA.CH0,
	.Gain = ADC_CHAN_GAIN_1,
	.Mode = ADC_CHAN_SINGLE,
	.Input = TEMPERATURE_SENSOR
};

/*
	DMA configuration
 */
const DMA_DestinationBuffer Destination;
DMA_Config_t Config_DMA = {
	.BufferMode = DMA_DOUBLEBUFFER_DISABLE, 
	.Priority = DMA_PRIORITY_RR
};

DMA_TransferConfig_t ADC_TransmitConfig = {
	.Channel = &DMA.CH0,
	.EnableSingleShot = false,
	.EnableRepeatMode = false,
	.BurstLength = DMA_BURSTLENGTH_2,
	.SrcReload = DMA_ADDRESS_RELOAD_TRANSACTION,
	.DstReload = DMA_ADDRESS_RELOAD_TRANSACTION,
	.SrcAddrMode = DMA_ADDRESS_MODE_INC,
	.DstAddrMode = DMA_ADDRESS_MODE_INC,
	.TriggerSource = DMA_TRIGGER_ADCA_CH0,
	.TransferCount = 2,
	.RepeatCount = 0,
	.SrcAddress = (uintptr_t)&ADCA.CH0.RES,
};

DMA_InterruptConfig_t Interrupt_DMA_ADC = {
	.Channel = &DMA.CH0, 
	.Source = DMA_TRANSACTION_INTERRUPT, 
	.InterruptLevel = INT_LVL_LO, 
	.Callback = DMA_TransactionComplete
};

/*
	DAC configuration
 */
DAC_Config_t Config_DAC = {
	.Device = &DACB,
	.Channel = DAC_CHANNEL_0 | DAC_CHANNEL_1, 
	.OutputConfig = DAC_SINGLE_CHANNEL_0, 
	.Reference = DAC_REFERENCE_INT1V, 
	.Adjustment = DAC_ADJUST_RIGHT
};

/*
	AC configuration
 */
AC_Config_t Config_AC = {
	.Device = &ACA,
	.Comparator = AC_COMPARATOR_0,
	.Hysteresis = AC_HYSTERESIS_SMALL,
	.Negative = AC_MUX_N_DAC,
	.Positive = AC_MUX_P_PIN4,
	.EnableHighSpeed = false,
};

AC_InterruptConfig_t Interrupt_AC = {
	.Device = &ACA,
	.Source = AC_COMP0_INTERRUPT,
	.Mode = AC_SENSE_BOTH,
	.InterruptLevel = INT_LVL_HI,
	.Callback = AC_Callback
};

/*
	RTC32 configuration
*/
RTC32_Config_t Config_RTC32 = 
{
	.Compare = 0x1FF,
	.Count = 0x00,
	.Period = 0xFFFFFFFF
};

RTC32_InterruptConfig_t Interrupt_RTC32 = 
{
	.Callback = RTC32_Tick,
	.InterruptLevel = INT_LVL_HI,
	.CallbackSource = RTC32_COMP_INTERRUPT,
};

/*
	AES configuration
*/
AES_InterruptConfig_t Interrupt_AES = {
	.InterruptLevel = INT_LVL_LO,
	.Source = AES_READY_INTERRUPT,
	.Callback = AES_Callback
};

/*
	AES key and data
 */
uint8_t InitVector[AES_DATASIZE] = {
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t EncryptionKey[AES_DATASIZE] = {
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t EncryptionData[AES_DATASIZE * AES_BLOCKS] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,	0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t EncryptedData[AES_DATASIZE * AES_BLOCKS];
uint8_t DecryptedData[AES_DATASIZE * AES_BLOCKS];

extern const uint8_t PictureAtmel[];
Bitmap_t BitmapAtmel = {
	.Width = 100,
	.Height = 25,
	.Type = MEMORY_PROGMEM,
	.Data.Flash = PictureAtmel,
};

int main(void)
{
	/*
		Initialize CPU clock
	*/
	SysClock_Init();
	
	/*
		USB
	*/
	//UDC_Start();

	/*
		Enable the LED outputs
	 */
	GPIO_SetDirection(GET_PERIPHERAL(LED0), GET_INDEX(LED0), GPIO_DIRECTION_OUT);
	GPIO_Clear(GET_PERIPHERAL(LED0), GET_INDEX(LED0));
	GPIO_SetDirection(GET_PERIPHERAL(LED1), GET_INDEX(LED1), GPIO_DIRECTION_OUT);
	GPIO_Clear(GET_PERIPHERAL(LED1), GET_INDEX(LED1));
	GPIO_SetDirection(GET_PERIPHERAL(LED2), GET_INDEX(LED2), GPIO_DIRECTION_OUT);
	
	/*
		Enable push buttons
	*/
	GPIO_SetDirection(GET_PERIPHERAL(SW0), GET_INDEX(SW0), GPIO_DIRECTION_IN);
	GPIO_SetDirection(GET_PERIPHERAL(SW1), GET_INDEX(SW1), GPIO_DIRECTION_IN);
	GPIO_SetDirection(GET_PERIPHERAL(SW2), GET_INDEX(SW2), GPIO_DIRECTION_IN);
	GPIO_InstallCallback(&Interrupt_SW0);
	GPIO_InstallCallback(&Interrupt_SW1);
	GPIO_InstallCallback(&Interrupt_SW2);
	
	/*
		Set output for clock measurement
	 */
	GPIO_EnableClockOutput(&PORTC);

	/*
		Initialize Timer 0
		NOTE: Timer 0 generate an interrupt every 20 ms @ 32 MHz
	*/
	//Timer0_Init(&Config_Timer0);
	//Timer0_InstallCallback(&Interrupt_Timer0);
	//Timer0_WaveInit(&WaveConfig_Timer0);
	
	/*
		CRC
		Checksum must be 0xE5CC for the given example
	*/
	//CRC_Data_t CRCData = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
	//uint32_t Checksum = CRC_Data(CRCData, CRC_LENGTH, CRC_LENGTH_16);
	//uint32_t CRCMemory = CRC_Memory(0, FLASH_SIZE);

	/*
		Initialize the DMA
	*/
	DMA_Init(&Config_DMA);
	DMA_Channel_InstallCallback(&Interrupt_DMA_ADC);
	DMA_Channel_Config(&ADC_TransmitConfig);
	DMA_Channel_Enable(ADC_TransmitConfig.Channel);
	
	/* 
		Initialize the ADC
	*/
	ADC_Init(&Config_ADC);
	ADC_SetupInput(&LightPinConfig);
	ADC_SetupInput(&TempPinConfig);
	ADC_SetupInput(&AnalogPinConfig);
	ADC_SetDMARequest(Config_ADC.Device, ADC_DMA_CH01);

	/*
		Initialize the DAC
	*/
	DAC_Init(&Config_DAC);
	DAC_WriteVoltage(Config_DAC.Device, Config_DAC.Channel, 1.00);

	/*
		Initialize the AC
	 */
	AC_Init(&Config_AC);
	AC_EnableOutput(Config_AC.Device, Config_AC.Comparator);
	//AC_InstallCallback(&Interrupt_AC);

	/*
		Initialize the USART
	*/
	USART_Init(&Config_USART);
	
	/*
		Initialize the I2C
	*/
	I2CM_Init(&Config_I2CM);
	I2CS_Init(&Config_I2CS);

	/*
		Configure event systems
			-> SW0 trigger the ADC for reading the temperature sensor over the event system
			-> ADC channel 0 will use DMA to transfer the data
	*/
	//GPIO_SwitchOutputLoc(TRUE);
	//Event_SetPinSource(EVENT_CHANNEL_7, SW0), SW0));
	//ADC_ConfigEvent(Config_ADC.Device, ADC_EVENT_7, ADC_EMODE_CH0);

	/*
		Configure AES module
	*/
	AES_Reset();
	AES_InstallCallback(&Interrupt_AES);

	/*
		Initialize display
	 */
	DisplayManager_Init();
	DisplayManager_SwitchBacklight(true);
	DisplayManager_DrawRect(12, 2, 102, 28, FILL_NO, 1, PIXELMASK_SET);
	DisplayManager_DrawBitmap(13, 3, &BitmapAtmel);
	for(uint32_t i = 0x00; i < 0xFFFFF; i++);
	DisplayManager_Clear();

	/*
		Initialize data flash
	*/
	//if(AT45DB642D_Init(NULL))
	{
		//ST7565R_WriteText(0, 0, "Flash: OK");
	}
	//else
	{
		//ST7565R_WriteText(0, 0, "Flash: ERROR");
	}
	
	/*
		Battery backup and RTC32
	*/
	/*
	BatteryBackupStatus_t Status = BatteryBackup_Check(TRUE);
	switch(Status)
	{
		case BATTERYBACKUP_STATUS_OK:
		{
			break;
		}
		case BATTERYBACKUP_STATUS_INIT:
		case BATTERYBACKUP_STATUS_BBPOR:
		case BATTERYBACKUP_STATUS_BBBOD:
		{
			//ST7565R_WriteText(1, 0, "ERROR");
		}
		case BATTERYBACKUP_STATUS_XOSCFAIL:
		{
			//ST7565R_WriteText(1, 0, "Ext. Osc. failure!");
		}
		case BATTERYBACKUP_STATUS_NO_POWER:
		{
			//ST7565R_WriteText(1, 0, "No battery power");
		}
		default:
		{
			BatteryBackup_ConfigXOSC(BATTERYBACKUP_CLOCK_1HZ, TRUE);
			RTC32_Init(&Config_RTC32);
			RTC32_InstallCallback(&Interrupt_RTC32);
		}
	}
	*/
	/*
		Initialize the watchdog
	*/
	//Watchdog_SetTimeout(WATCHDOG_TIMEOUT_8KCLK);
	//Watchdog_Enable();

	DisplayManager_DrawString(0, 0, "Clock:");
	//ST7565R_WriteUInt(3, 8 * FONT_WIDTH, SysClock_GetClock() / 1000000);
	//ST7565R_WriteText(3, 10 * FONT_WIDTH, "MHz");

	/*
		Enable interrupts
	*/
	EnableGlobalInterrupts();
	PMIC_EnableAllInterruptLevel();
	
	ADC_Channel_StartConversion(LightPinConfig.Channel);
	ADC_Channel_StartConversion(TempPinConfig.Channel);
	
	//AES_CBC_Encrypt(EncryptionData, EncryptedData, EncryptionKey, InitVector, AES_BLOCKS);

	while(1)
	{
		//Watchdog_Reset();
		//AC_WaitForComparator(Config_AC.Device, Config_AC.Comparator);
	}
	
	return 0;
}

void DMA_TransactionComplete(uint8_t Channel)
{
	uint16_t Result = (Destination[1] << 0x08) + Destination[0];

	DMA_Channel_Enable(ADC_TransmitConfig.Channel);
	ADC_Channel_StartConversion(TempPinConfig.Channel);
}

void SW0_Callback(void)
{
	DisplayManager_Clear();
	DisplayManager_DrawString(3, 0, "SW0 pressed...");
	
	if(!GPIO_Read(GET_PERIPHERAL(SW0), GET_INDEX(SW0)))
	{
		DisplayManager_SwitchBacklight(false);
		DisplayManager_DrawCircleSegment(60, 15, 5, CIRCLE_SEGMENT_QUADRANT1, FILL_SOLID, PIXELMASK_CLEAR);
	}
	else
	{
		DisplayManager_SwitchBacklight(true);
		DisplayManager_DrawCircle(60, 15, 5, FILL_SOLID, PIXELMASK_SET);
	}
}

void SW1_Callback(void)
{
	DisplayManager_Clear();
	DisplayManager_DrawString(3, 0, "SW1 pressed...");

	TIMER0_INCREASEDUTY(WaveConfig_Timer0.Device, WaveConfig_Timer0.Channel);
}

void SW2_Callback(void)
{
	DisplayManager_Clear();
	DisplayManager_DrawString(3, 0, "SW2 pressed...");

	TIMER0_DECREASEDUTY(WaveConfig_Timer0.Device, WaveConfig_Timer0.Channel);
}

void AC_Callback(void)
{
	//ST7565R_WriteText(3, 0, "AC callback...");
}

void Timer_Tick(void)
{
}

void RTC32_Tick(void)
{
	DisplayManager_Clear();
	DisplayManager_DrawString(3, 0, "Uptime: ");
}

void AES_Callback(void)
{
}