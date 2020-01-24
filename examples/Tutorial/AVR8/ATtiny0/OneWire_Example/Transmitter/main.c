#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t Data = 0x01;

int main(void)
{
	/*
		Initialize the USART in 1-Wire mode
			-> Pull Up for the TxD pin
			-> Loopback mode to connect Rx and Tx
			-> 8 data bits, 1 stop bit
			-> 115200 baud
	*/
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	USART0.CTRLA = USART_LBME_bm;
	USART0.CTRLB = USART_ODME_bm | USART_RXEN_bm | USART_TXEN_bm;
	USART0.CTRLC = USART_CHSIZE_8BIT_gc | USART_SBMODE_1BIT_gc;
	USART0.BAUD = 69;

	sei();

    while(1)
    {
		// Small delay
		for(uint32_t i = 0x00; i < 100000; i++);

		// Start transmission. Fill Tx buffer, disable receiver and enable data empty interrupt
		USART0.TXDATAL = Data;
		USART0.CTRLB &= ~USART_RXEN_bm;
		USART0.CTRLA |= USART_DREIE_bm;
    }

	return 0x00;
}

ISR(USART0_RXC_vect)
{
	// Receive complete. Disable receive interrupt and get the data
	USART0.CTRLA &= ~USART_RXCIE_bm;
	Data = USART0.RXDATAL;
}

ISR(USART0_DRE_vect)
{
	// Transmission done. Clear data empty interrupt, enable receiver and receiver interrupt
	USART0.CTRLA &= ~USART_DREIE_bm;
	USART0.CTRLA |= USART_RXCIE_bm;
	USART0.CTRLB |= USART_RXEN_bm;
}