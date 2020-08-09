#include "usart.h"

void USART_ini(unsigned int	ubrr)
{
	DDRD |=(1<<0)|(1<<1);
	PORTD &= ~((1<<0)|(1<<1));
	UBRR0H=(unsigned char) (ubrr>>8);
	UBRR0L=(unsigned char) ubrr;
	UCSR0A|=(1<<U2X0);//�������� �������
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);//��� ����� � ������� �� USART
	UCSR0B|=(1<<RXCIE0);//��������� ���������� ��� ������
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);// 2 ��������(USBSn=1), ������� 8 ���(UCSZn1:0=3), ����������� �����(UMSELn1:0=0), ��� �������� ��������(UPMn1:0=0)
}

void USART_Transmit(unsigned char data)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;//�������� ������, ������ ���� ����� ����
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}