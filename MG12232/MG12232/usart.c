#include "usart.h"



void USART_ini(unsigned int	ubrr)
{
	DDRD |=(1<<0)|(1<<1);
	PORTD &= ~((1<<0)|(1<<1));
	UBRR0H=(unsigned char) (ubrr>>8);
	UBRR0L=(unsigned char) ubrr;
	UCSR0A|=(1<<U2X0);//удвоение частоты
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);//вкл прием и пердачу по USART
	UCSR0B|=(1<<RXCIE0);//разрешает перрывание при приеме
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);// 2 стопбита(USBSn=1), посылка 8 бит(UCSZn1:0=3), асинхронный режим(UMSELn1:0=0), без контроля четности(UPMn1:0=0)
}

void USART_Transmit(unsigned char data)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;//передает данные, только если буфер пуст
}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

#define BAUD 9600
#define STOP_BITS 1

#define SOFT_TX_BIT 0
#define SOFT_TX_DDR DDRB
#define SOFT_TX_PORT PORTB

#define MICROSECONDS_OVERHEAD_ADJUST 0

#define MICROSECONDS_PER_BIT ((1000000ul / BAUD) - MICROSECONDS_OVERHEAD_ADJUST)

FILE static usartout = {0};

void serial_configure();
int usart_putchar (char c, FILE *stream);

void serial_configure() {

	SOFT_TX_PORT |= (1<<SOFT_TX_BIT);
	SOFT_TX_DDR |= (1<<SOFT_TX_BIT);
	fdev_setup_stream (&usartout, usart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &usartout;
	
}

int usart_putchar (char c, FILE *stream) {

	uint8_t  bit_mask;

	// start bit
	SOFT_TX_PORT &= ~(1<<SOFT_TX_BIT);
	_delay_us(MICROSECONDS_PER_BIT);

	// data bits
	for (bit_mask=0x01; bit_mask; bit_mask<<=1) {
		if (c & bit_mask) {
			SOFT_TX_PORT |= (1<<SOFT_TX_BIT);
		}
		else {
			SOFT_TX_PORT &= ~(1<<SOFT_TX_BIT);
		}
		_delay_us(MICROSECONDS_PER_BIT);
	}

	// stop bit(s)
	SOFT_TX_PORT |= (1<<SOFT_TX_BIT);
	_delay_us(MICROSECONDS_PER_BIT * STOP_BITS);

	return c;

}