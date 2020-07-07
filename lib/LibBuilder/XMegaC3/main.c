#include <stdio.h>
#include "Board.h"

#define	SAMPLES											8

void DMA_TransactionComplete(uint8_t Channel);
void RTC_Callback(void);
void SD_Ready(const SD_State_t State);
void USART_Rx_Callback(void);
void AES_Callback(void);

Graph_t ADC_Graph;
RingBuffer_t RxRing;
uint8_t RxData[32];

uint8_t AES_Data[16] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t AES_EncryptedData[16];

uint8_t AES_Key[] = {
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

extern const uint8_t PictureAtmel[];
Bitmap_t BitmapAtmel = {
	.Width = 100,
	.Height = 25,
	.Type = MEMORY_PROGMEM,
	.Data.Flash = PictureAtmel,
};

extern const uint8_t PictureSD[];
Bitmap_t BitmapSD = {
	.Width = 32,
	.Height = 32,
	.Type = MEMORY_PROGMEM,
	.Data.Flash = PictureAtmel,
};

/*
	AES configuration
*/
AES_InterruptConfig_t AES_InterruptConfig = {
	.Callback = AES_Callback,
	.InterruptLevel = INT_LVL_LO,
	.Source = AES_READY_INTERRUPT,
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
	.EnableSingleShot = true,
	.EnableRepeatMode = false,
	.BurstLength = DMA_BURSTLENGTH_2,
	.SrcReload = DMA_ADDRESS_RELOAD_BURST,
	.DstReload = DMA_ADDRESS_RELOAD_TRANSACTION,
	.SrcAddrMode = DMA_ADDRESS_MODE_INC,
	.DstAddrMode = DMA_ADDRESS_MODE_INC,
	.TriggerSource = DMA_TRIGGER_ADCA_CH0,
	.TransferCount = SAMPLES * 2,
	.RepeatCount = 0,
	.SrcAddress = (uintptr_t)&ADCA.CH0.RES,
	.DstAddress = (uintptr_t)Destination,
};

DMA_InterruptConfig_t Interrupt_DMA_ADC = {
	.Channel = &DMA.CH0, 
	.Source = DMA_TRANSACTION_INTERRUPT, 
	.InterruptLevel = INT_LVL_LO, 
	.Callback = DMA_TransactionComplete
};

/*
	USART configuration
 */
USART_Config_t Config_USART = {
	.Device = &USARTC0,
	.DeviceMode = USART_MODE_ASYNCH,
	.Direction = USART_DIRECTION_BOTH,
	.Baudrate = 19200,
	.Parity = USART_PARITY_NONE,
	.Stop = USART_STOP_1,
	.Size = USART_SIZE_8, 
	.BSCALE = 3, 
	.EnableDoubleSpeed = false,
	.EnableInterruptSupport = true,
	.InterruptLevel = INT_LVL_HI
};

USART_InterruptConfig_t USART_Callback = {
	.Device = &USARTC0,
	.InterruptLevel = INT_LVL_HI,
	.Source = USART_RXC_INTERRUPT,
	.Callback = USART_Rx_Callback,
};

/*
	ADC configuration
 */
ADC_Config_t Config_ADC = {
	.Device = &ADCA,
	.ConversionMode = ADC_CONVMODE_SIGNED, 
	.Format = ADC_CONV_FORM_12,
	.Reference =  ADC_REFERENCE_INTVCC, 
	.Prescaler = ADC_PRESCALER_4,
	.SamplingTime = 63
};

ADC_ChannelConfig_t LightPinConfig = {
	.Device = &ADCA, 
	.Channel = &ADCA.CH0, 
	.Gain = ADC_CHAN_GAIN_1, 
	.Mode = ADC_CHAN_SINGLE, 
	.Input = LIGHT_SENSOR
};

/*
	RTC configuration
*/
RTC_Config_t Config_RTC = {
	.ClockSource = RTC_CLOCK_TOSC32,
	.Prescaler = RTC_PRESCALER_1,
	.Period = 1023,
	.Count = 0,
	.Compare = 0,
};

RTC_InterruptConfig_t Interrupt_RTC = {
	.CallbackSource = RTC_COMP_INTERRUPT,
	.InterruptLevel = INT_LVL_LO,
	.Callback = RTC_Callback,
};

FATFS MicroSD_FS;
FRESULT FileStatus;
FIL LogFile;
char FilePath[] = "0:Log.txt";

int main(void)
{	
	/*
		Initialize main clock
	*/
	SysClock_Init();

	/*
		Disable all peripherals to reduce power consumption
	*/
	//PowerManagment_FreezeAll();

	/*
		Enable the LED outputs
	 */
	GPIO_SetDirection(GET_PERIPHERAL(LED0), GET_INDEX(LED0), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(LED0), GET_INDEX(LED0));
	GPIO_SetDirection(GET_PERIPHERAL(LED1), GET_INDEX(LED1), GPIO_DIRECTION_OUT);
	GPIO_Set(GET_PERIPHERAL(LED1), GET_INDEX(LED1));

	/*
		Initialize the USART
	*/
	USART_Init(&Config_USART);
	USART_InstallCallback(&USART_Callback);
	RingBuffer_Init(&RxRing, RxData, 32);

	/*
		Initialize the RTC
	*/
	RTC_Init(&Config_RTC);
	RTC_InstallCallback(&Interrupt_RTC);

	/*
		Initialize the DMA
	*/
	DMA_Init(&Config_DMA);
	DMA_Channel_InstallCallback(&Interrupt_DMA_ADC);
	DMA_Channel_Config(&ADC_TransmitConfig);

	/* 
		Initialize the ADC
	*/
	ADC_Init(&Config_ADC);
	ADC_SetupInput(&LightPinConfig);

	/*
		Enable global interrupts
	*/
	EnableGlobalInterrupts();
	PMIC_EnableAllInterruptLevel();

	/*
		Initialize the sleep manager
	*/
	//SleepManager_Init();
	//SleepManager_Lock(SLEEPMGR_ESTDBY);

	/*
		Initialize the display manager
	*/
	DisplayManager_Init();
	DisplayManager_DrawRect(12, 2, 102, 28, FILL_NO, 1, PIXELMASK_SET);
	DisplayManager_DrawBitmap(13, 3, &BitmapAtmel);
	for(uint32_t i = 0x00; i < 0xFFFFF; i++);
	DisplayManager_Clear();

	SD_InstallCallback(SD_Ready);

		//FileStatus = f_open(&LogFile, FilePath, FA_WRITE | FA_CREATE_ALWAYS);
		//FileStatus = f_puts("AAAAA", &LogFile);
		//FileStatus = f_close(&LogFile);
		
		//FileStatus = f_open(&LogFile, FilePath, FA_READ | FA_OPEN_EXISTING);
		//FileStatus = f_read(&LogFile, Bla, 5, &AA);
		//FileStatus = f_close(&LogFile);
	//DisplayManager_DrawString(0,10,Bla);
	
	USART_Write(Config_USART.Device, "Start...\n\r");
	
	DisplayManager_InitGraph(0, 0, "Light", 2048, &ADC_Graph);

	DMA_Channel_Enable(ADC_TransmitConfig.Channel);

	AES_InstallCallback(&AES_InterruptConfig);
	AES_Run(AES_Data, AES_Key, AES_ENCRYPT);

    while(1) 
    {
		//SleepManager_EnterSleep();
		for(uint8_t i = 0x00; i < 0xFF; i++);
    }
}

void DMA_TransactionComplete(uint8_t Channel)
{	
	DMA_Channel_Enable(ADC_TransmitConfig.Channel);
	
	// Calculate the average
	uint32_t Average = 0x00;
	for(uint8_t i = 0x00; i < (SAMPLES * 2); i += 0x02)
	{
		Average += (Destination[i + 1] << 0x08) + Destination[i];
	}
	Average = Average >> 0x03;

	DisplayManager_UpdateGraph(&ADC_Graph, Average);
}

void RTC_Callback(void)
{
	ADC_Channel_StartConversion(LightPinConfig.Channel);
}

void SD_Ready(const SD_State_t State)
{
	DisplayManager_Clear();

	if(State == SD_STATE_INSERT)
	{
		DisplayManager_DrawString(0, 0, "SD card inserted...");

		/*
			Initialize the SD card
		*/
		if(f_mount(&MicroSD_FS, "", 1) != FR_OK)
		{
			DisplayManager_DrawString(0, 8, "SD card error!");
		}
		else
		{
			DisplayManager_Clear();

			SD_CID_t SD_CID;
			SD_CSD_t SD_CSD;
			char Buffer[20];

			DisplayManager_DrawString(0, 0, "SD card initialized...");

			// Read SD card informations
			if((SD_GetCID(&SD_CID) != SD_SUCCESSFULL) || (SD_GetCSD(&SD_CSD) != SD_SUCCESSFULL))
			{
				DisplayManager_DrawString(0, 8, "Can not read card information!");
			}
			else
			{
				sprintf(Buffer, "ID: %s", SD_ID2Manufacturer(SD_CID.MID));
				DisplayManager_DrawString(0, 8, Buffer);
				sprintf(Buffer, "Serial: %lu", SD_CID.PSN);
				DisplayManager_DrawString(0, 16, Buffer);
			}
		}
	}
	else
	{
		DisplayManager_DrawString(0, 0, "SD card removed...");
	}
}

void USART_Rx_Callback(void)
{
	RingBuffer_Save(&RxRing, USART_GetChar(&USARTE0));
	
	if(RingBuffer_IsFull(&RxRing))
	{
		USART_SendChar(&USARTE0, XOFF);
		
		while(!RingBuffer_IsEmpty(&RxRing))
		{
			USART_SendChar(&USARTE0, RingBuffer_Load(&RxRing));
		}
		
		USART_SendChar(&USARTE0, '\n');
		USART_SendChar(&USARTE0, '\r');
		
		USART_SendChar(&USARTE0, XON);
	}
}

void AES_Callback(void)
{
}