#include "adc.h"
//----------------------------------------
extern uint16_t adc_value;
//----------------------------------------
char high_adc=0,low_adc=0;
//----------------------------------------
ISR(ADC_vect)
{
	low_adc = ADCL;
	high_adc = ADCH;//Верхняя часть регистра ADC должна быть считана последней, иначе не продолжится преобразование
	adc_value = ((uint16_t)high_adc<<8)|low_adc;
}

void adc_init(void)
{	
	ADCSRA |= (1<<ADEN) // Разрешение использования АЦП
// 	|(1<<ADSC)//Запуск преобразования
// 	|(1<<ADATE)//Непрерывный режим работы АЦП
// 	|(1<<ADIE)//Разрешение прерывания от АЦП
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//Делитель 128 = 125 кГц
	ADMUX |= (1<<REFS0)|(1<<MUX1)|(1<<MUX0); // ИОН=AVcc, вход ADC3 |(1<<MUX1)|(1<<MUX0)
}

unsigned int adc_convert (void)

{
	ADCSRA |= (1<<ADSC); //Начинаем преобразование
	while((ADCSRA & (1<<ADSC))); //проверим закончилось ли аналого-цифровое преобразование
	return (unsigned int) ADC;
}