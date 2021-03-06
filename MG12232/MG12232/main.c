#include "main.h"

#define BAUD 115200
#define MYUBRR F_CPU/8/BAUD-1

uint8_t Dev_ID[8][8]={0};
uint8_t Dev_Cnt;
uint8_t dt[8];
char str1[64];
// float n=0;
float Cppm=0;
unsigned int b;
uint8_t CO2_read[9]= {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
uint8_t CO2_data[9]={0};
	

// ISR(TIMER0_OVF_vect){
// 	b++;
// }


int main(void)
{
	portbhistory=0xff;
	adc_value=0;
// 	USART_ini(MYUBRR);
// 	timer0_init();
	mg12232_init();
	adc_init();
	
// 	SW_UART_Enable();

	uint8_t co2_d = 0;
	usartsoft_init();
	
// 	uint16_t raw_temper;
// 	uint16_t temper_H;
// 	uint16_t temper_L;
// 	char c;
// 	uint8_t i;	
	uint8_t a;
// 	char status = ds18b20_init(NO_SKIP_ROM);
// 	sprintf (str1,"Init Status: %d\r\n",status);
// 	for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 	sprintf(str1,"Dev count: %d\r\n", Dev_Cnt);
// 	for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 	for(i=1;i<=Dev_Cnt;i++)
// 	{
// 		sprintf(str1,"Device %d\r\n", i);
// 		for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 		sprintf(str1,"ROM RAW: %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
// 		Dev_ID[i-1][0], Dev_ID[i-1][1], Dev_ID[i-1][2], Dev_ID[i-1][3],
// 		Dev_ID[i-1][4], Dev_ID[i-1][5], Dev_ID[i-1][6], Dev_ID[i-1][7]);
// 		for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 		sprintf(str1,"Family CODE: 0x%02X\r\n", Dev_ID[i-1][0]);
// 		for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 		sprintf(str1,"ROM CODE: 0x%02X%02X%02X%02X%02X%02X\r\n", Dev_ID[i-1][6], Dev_ID[i-1][5],
// 		Dev_ID[i-1][4], Dev_ID[i-1][3], Dev_ID[i-1][2], Dev_ID[i-1][1]);
// 		for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 		sprintf(str1,"CRC: 0x%02X\r\n", Dev_ID[i-1][7]);
// 		for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 	}
    while (1) 
    {
		sei();
		clear_lcd();
// 		
// // 		for(i=1;i<=Dev_Cnt;i++)
// // 		{
// // 		}
// 		if (b)
// 		{
// 			ds18b20_MeasureTemperCmd(SKIP_ROM, 0);
// 		} 
// 		else
// 		{
// 			for(i=1;i<=Dev_Cnt;i++)
// 			{
// 				ds18b20_ReadStratcpad(NO_SKIP_ROM, dt, i);
// // 				sprintf(str1,"STRATHPAD %d: %02X %02X %02X %02X %02X %02X %02X %02X %02X; ",
// // 				i, dt[0], dt[1], dt[2], dt[3], dt[4], dt[5], dt[6], dt[7], dt[8]);
// // 				for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 				raw_temper = ((uint16_t)dt[1]<<8)|dt[0];
// 				if(ds18b20_GetSign(raw_temper)) c='-';
// 				else c='+';
// // 				float t=ds18b20_Convert(raw_temper);
// 				temper_H = (raw_temper&0x0fff)>>4;
// // 				temper_L = (raw_temper&0x000f)*625;
// 				if (i==1){
// 					command_write(CMD_PAGE+1,3);
// 					command_write(CMD_COLUMN+1,3);
// 				}
// 				if (i==3){
// 					command_write(CMD_PAGE+2,3);
// 					command_write(CMD_COLUMN+1,3);
// 					}if (i==5){
// 					command_write(CMD_PAGE+3,3);
// 					command_write(CMD_COLUMN+1,3);
// 				}
// 				uint8_t chip;
// 				if(i==2||i==4||i==6){chip=2;command_write(CMD_COLUMN+18,2);}
// 				else chip=1;
// 				sprintf(str1,"%d=%c%d", i, c, temper_H);
// 				for (a=0;a<strlen(str1);a++)number_write(str1[a],chip);
// // 				sprintf(str1,"Raw t: 0x%04X; t: %c%.3f\r\n", raw_temper, c, t);
// // 				for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 			}			
// 		}
// 		
		adc_value=adc_convert();
		Cppm=(adc_value-82)*15.167236;			//4.854*3.125
		dht11_start();
		sprintf(str1,"T=%d  H=%d ADC=%.f \r\n", dht11_temp()+3, dht11_humid(), Cppm);
		mg12232_string_write(str1, 0);
// // 		for (a=0;a<strlen(str1);a++)USART_Transmit(str1[a]);
// 		
// 		if (usartsoft_recieve(&co2_d)>=0)
// 		{
// 			usartsoft_transmit(co2_d);
// 		}
// 		uint8_t CO2_read[9]= {65, 66, 66, 66, 66, 66, 66, 66, 67};
		sei();
		for (a=0;a<9;a++)usartsoft_transmit(CO2_read[a]);
		for (a=0;a<9;a++)CO2_data[a]=usartsoft_recieve(&co2_d);
		
// 		usartsoft_transmit(co2_d);
		sprintf(str1," %x %x %x %x %x %x %x %x %x ", CO2_data[0], CO2_data[1], CO2_data[2], CO2_data[3], CO2_data[4], CO2_data[5], CO2_data[6], CO2_data[7], CO2_data[8]);
		mg12232_string_write(str1, 1);
// 						
		_delay_ms(3000);
    }
}
