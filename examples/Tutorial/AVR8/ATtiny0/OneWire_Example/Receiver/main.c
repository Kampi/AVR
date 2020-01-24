#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t Data = 0x00;

int main(void)
{
	PORTA.DIR = 0xF0;

	/*
		Initialize the USART in 1-Wire mode
			-> Pull Up for the TxD pin
			-> Loopback mode to connect Rx and Tx
			-> 8 data bits, 1 stop bit
			-> 115200 baud
	*/
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	USART0.CTRLA = USART_RXCIE_bm | USART_LBME_bm;
	USART0.CTRLB = USART_ODME_bm | USART_RXEN_bm | USART_TXEN_bm;
	USART0.CTRLC = USART_CHSIZE_8BIT_gc | USART_SBMODE_1BIT_gc;
	USART0.BAUD = 69;

	sei();

    while(1)
    {
	    asm volatile("nop");
    }

	return 0x00;
}

ISR(USART0_RXC_vect)
{
	uint8_t Data = 0x00;

	// Receive complete. Disable receive interrupt and get the data
	USART0.CTRLA &= ~USART_RXCIE_bm;
	Data = USART0.RXDATAL;

	// Output data and generate new data
	PORTA.OUT = Data << 0x04;

	if(Data < 15)
	{
		Data++;
	}
	else
	{
		Data = 0x00;
	}

	// Fill the Tx buffer, disable the receiver and enable data empty interrupt
	USART0.TXDATAL = Data;
	USART0.CTRLB &= ~USART_RXEN_bm;
	USART0.CTRLA |= USART_DREIE_bm;
}

ISR(USART0_DRE_vect)
{
	// Transmission done. Clear data empty interrupt, enable receiver and receiver interrupt
	USART0.CTRLA &= ~USART_DREIE_bm;
	USART0.CTRLA |= USART_RXCIE_bm;
	USART0.CTRLB |= USART_RXEN_bm;
}