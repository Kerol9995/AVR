#include "dht11.h"

	char dht11_dat[5];
	char dht11_in;

char read_dht11_dat()
{
	char i = 0;
	char result=0;
	for(i=0; i< 8; i++)
	{
		while(!(PINC & (1<<(DHT11_PIN))))
		{}; // wait forever until analog input port 0 is '1' (NOTICE: PINC reads all the analog input ports
		//and _BV(X) is the macro operation which pull up position 'X' to '1' and the rest positions to '0'. it is equivalent to 1<
		_delay_us(30);
		if(PINC & (1<<(DHT11_PIN))) //if analog input port 0 is still '1' after 30 us
		result |=(1<<(7-i)); //this position is 1
		while((PINC & (1<<(DHT11_PIN)))); // wait '1' finish
	}
	return result;
}

void dht11_start()
{
	dht11_dat[0]=0;
	dht11_dat[2]=0;
	DDRC |= (1<<(DHT11_PIN));// start condition
	PORTC &= ~(1<<(DHT11_PIN)); // 1. pull-down i/o pin for 18ms
	_delay_ms(20);
	PORTC |= (1<<(DHT11_PIN)); // 2. pull-up i/o pin for 40ms
	_delay_us(30);
	DDRC &= ~(1<<(DHT11_PIN)); //let analog port 0 be input port
	_delay_us(1);

	dht11_in = PINC & (1<<(DHT11_PIN)); // read only the input port 0
	if (dht11_in)
	{
// 		page_addr(2);
// 		column_addr(1);
// 		number_write(1);
		/*		Serial.println("dht11 start condition 1 not met"); //wait for DHT11 response signal:LOW*/
	}
	else
	{
		_delay_us(80);
		dht11_in = PINC & (1<<(DHT11_PIN)); //
		if(!dht11_in)
		{
// 			page_addr(2);
// 			column_addr(1);
// 			number_write(2);
			/*		Serial.println("dht11 start condition 2 not met"); //wait for second response signal:HIGH*/
		}
		else
		{
			_delay_us(80);// now ready for data reception
			for (int i=0; i<5; i++)
			{ dht11_dat[i] = read_dht11_dat();} //received 40 bits data. Details are described in datasheet

			DDRC |= (1<<(DHT11_PIN)); //let analog port 0 be output port after all the data have been received
			PORTC |= (1<<(DHT11_PIN)); //let the value of this port be '1' after all the data have been received
			char dht11_check_sum = dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3];// check check_sum
			if(dht11_dat[4]!= dht11_check_sum)
			{
// 				page_addr(2);
// 				column_addr(1);
// 				number_write(3);
				/*		Serial.println("DHT11 checksum error");*/
			}
// 			else
// 			{
// 				clear_lcd();
// 				page_addr(0);
// 				column_addr(1);
// 				dht11_dat[2]+=3;
// 				char T=dht11_dat[2]/10;
// 				char t=dht11_dat[2]%10;
// 				number_write('T');
// 				number_write('=');
// 				number_write(T);
// 				number_write(t);
// 				page_addr(1);
// 				column_addr(1);
// 				char H=dht11_dat[0]/10;
// 				char h=dht11_dat[0]%10;
// 				number_write('H');
// 				number_write('=');
// 				number_write(H);
// 				number_write(h);
// 			}
		}
	}
}

char dht11_temp(){
// 	dht11_start();
	return dht11_dat[2];
}

char dht11_humid(){
// 	dht11_start();
	return dht11_dat[0];
}