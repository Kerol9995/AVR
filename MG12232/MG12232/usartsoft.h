#ifndef USARTSOFT_H_
#define USARTSOFT_H_

#include "main.h"
/*
*	���� ������������� ����� � ���� ������ ������� ����� ��������������
*	��� ���������� � �������� UART.
*/

#define TXPORT PORTB	// ��� ����� ��� ��������
#define RXPORT PINB		// ��� ����� �� �����
#define TXDDR DDRB		// ������� ����������� ����� �� ��������
#define RXDDR DDRB		// ������� ����������� ����� �� �����
#define TXD 3			// ����� ���� ����� ��� ������������� �� ��������
#define RXD 4			// ����� ���� ����� ��� ������������� �� �����

/*
*	���� �������� ���������, ������������ �������� �������� ������ (�������)
*	������ BAUD_DIV �������������� ��������� �������:
*	BAUD_DIV = (CPU_CLOCK / DIV) / BAUD_RATE
*	��� CPU_CLOCK - �������� ������� �����������, BAUD_RATE - �������� �������� UART,
*	� DIV - �������� �������� ������� �������, ���������� � �������� TCCR0B.
*	��������, �������� �� 8, �������� ����� 9600 ���:
*	BAUD_DIV = (9 600 000 / 8) / 9600 = 125 (0x7D).
*/

//#define T_DIV		0x01	// DIV = 1
//#define T_DIV		0x02	// DIV = 8
#define T_DIV		0x03	// DIV = 64
//#define T_DIV		0x04	// DIV = 256
//#define T_DIV		0x05	// DIV = 1024
#define BAUD_DIV	0xD0	// �������� = 9600 ���.

/*
*	���� ���� ���������� ���������� ���������� � ������� ��� ������ UART
*/

volatile uint16_t txbyte;
volatile uint8_t rxbyte;
volatile uint8_t txbitcount;
volatile uint8_t rxbitcount;

void usartsoft_init();
void usartsoft_transmit(uint8_t tb);
int16_t usartsoft_recieve(uint8_t* rb);



#endif /* USARTSOFT_H_ */