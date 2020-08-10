#include "adc.h"
//----------------------------------------
extern uint16_t adc_value;
//----------------------------------------
char high_adc=0,low_adc=0;
//----------------------------------------
ISR(ADC_vect)
{
	low_adc = ADCL;
	high_adc = ADCH;//������� ����� �������� ADC ������ ���� ������� ���������, ����� �� ����������� ��������������
	adc_value = ((uint16_t)high_adc<<8)|low_adc;
}

void adc_init(void)
{	
	ADCSRA |= (1<<ADEN) // ���������� ������������� ���
// 	|(1<<ADSC)//������ ��������������
// 	|(1<<ADATE)//����������� ����� ������ ���
// 	|(1<<ADIE)//���������� ���������� �� ���
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//�������� 128 = 125 ���
	ADMUX |= (1<<REFS0)|(1<<MUX1)|(1<<MUX0); // ���=AVcc, ���� ADC3 |(1<<MUX1)|(1<<MUX0)
}

unsigned int adc_convert (void)

{
	ADCSRA |= (1<<ADSC); //�������� ��������������
	while((ADCSRA & (1<<ADSC))); //�������� ����������� �� �������-�������� ��������������
	return (unsigned int) ADC;
}