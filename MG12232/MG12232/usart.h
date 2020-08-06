#ifndef USART_H_
#define USART_H_

#include "main.h"


void USART_ini(unsigned int MYUBRR);
void USART_Transmit(unsigned char data);
unsigned char USART_Receive( void );


#endif