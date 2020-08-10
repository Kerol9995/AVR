#ifndef USARTSOFT_H_
#define USARTSOFT_H_

#include "main.h"
/*
*	Ќиже настраиваютс€ порты и пины портов которые будут использоватьс€
*	как передатчик и приемник UART.
*/

#define TXPORT PORTB	// »м€ порта дл€ передачи
#define RXPORT PINB		// »м€ порта на прием
#define TXDDR DDRB		// –егистр направлени€ порта на передачу
#define RXDDR DDRB		// –егистр направлени€ порта на прием
#define TXD 3			// Ќомер бита порта дл€ использовани€ на передачу
#define RXD 4			// Ќомер бита порта дл€ использовани€ на прием

/*
*	Ќиже задаютс€ константы, определ€ющие скорость передачи данных (бодрейт)
*	расчет BAUD_DIV осуществл€етс€ следующим образом:
*	BAUD_DIV = (CPU_CLOCK / DIV) / BAUD_RATE
*	где CPU_CLOCK - тактова€ частота контроллера, BAUD_RATE - желаема€ скорость UART,
*	а DIV - значение делител€ частоты таймера, задающеес€ в регистре TCCR0B.
*	Ќапример, делитель на 8, скорость порта 9600 бод:
*	BAUD_DIV = (9 600 000 / 8) / 9600 = 125 (0x7D).
*/

//#define T_DIV		0x01	// DIV = 1
//#define T_DIV		0x02	// DIV = 8
#define T_DIV		0x03	// DIV = 64
//#define T_DIV		0x04	// DIV = 256
//#define T_DIV		0x05	// DIV = 1024
#define BAUD_DIV	0xD0	// —корость = 9600 бод.

/*
*	Ќиже идут объ€влени€ глобальных переменных и функций дл€ работы UART
*/

volatile uint16_t txbyte;
volatile uint8_t rxbyte;
volatile uint8_t txbitcount;
volatile uint8_t rxbitcount;

void usartsoft_init();
void usartsoft_transmit(uint8_t tb);
int16_t usartsoft_recieve(uint8_t* rb);



#endif /* USARTSOFT_H_ */