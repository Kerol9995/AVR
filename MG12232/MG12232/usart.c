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