#include "ds18b20.h"

uint8_t LastDeviceFlag;
uint8_t LastDiscrepancy;
uint8_t LastFamilyDiscrepancy;
uint8_t ROM_NO[8];
extern uint8_t Dev_ID[8][8];
extern uint8_t Dev_Cnt;

//функция определения датчика на шине
char ds18b20_Reset(void)				//dt - digital termomether | определим, есть ли устройство на шине
{	
	char status;
	cli();								//запрещаем прерывание
	DDRTEMP |= 1<<BITTEMP;				//притягиваем шину
	_delay_us(485);						//задержка как минимум на 480 микросекунд
	DDRTEMP &= ~(1<<BITTEMP);			//отпускаем шину
	_delay_us(100);						//задержка как максимум на 60 микросекунд
	if ((PINTEMP & (1<<BITTEMP))==0)	//проверяем, ответит ли устройство
	{
		status=1;						//устройство есть
	}
	else status=0;						//проверяем, ответит ли устройство
	_delay_us(385);						//задержка как минимум на 480 микросекунд, но хватит и 420, тк это с учетом времени прошедших команд
	return status;						//вернем результат
}

//функция записи бита на устройство
void ds18b20_WriteBit(char bit)
{
	cli();								//запрещаем прерывание
	DDRTEMP |= 1<<BITTEMP;				//притягиваем шину
	_delay_us(2);						//задержка как минимум на 2 микросекунды
	if(bit)
		DDRTEMP &= ~(1<<BITTEMP);		//отпускаем шину
	_delay_us(62);						//задержка как минимум на 60 микросекунд
	DDRTEMP &= ~(1<<BITTEMP);			//отпускаем шину
}

//функция записи байта на устройство
void ds18b20_WriteByte(char bit)
{
	char i;		
	for(i=0;i<8;i++)					//посылаем отдельно каждый бит на устройство
	{
		if((bit & (1<<i)) == 1<<i)		//посылаем 1
			ds18b20_WriteBit(1);
		else							//посылаем 0
			ds18b20_WriteBit(0);
	}	
}

//функция чтения бита с устройства
char ds18b20_ReadBit(void)
{
	cli();								//запрещаем прерывание
	char bit;							//переменная хранения бита
	DDRTEMP |= 1<<BITTEMP;				//притягиваем шину
	_delay_us(2);						//задержка как минимум на 2 микросекунды
	DDRTEMP &= ~(1<<BITTEMP);			//отпускаем шину
	_delay_us(10);
	bit = (PINTEMP & (1<<BITTEMP))>>BITTEMP; //читаем бит
	_delay_us(52);
	return bit;							//вернем результат
}

//функция чтения байта с устройства
char ds18b20_ReadByte(void)
{
	char data=0;
	for(char i=0;i<8;i++)
		data|=ds18b20_ReadBit()<<i;		//читаем бит
	return data;
}

//функция преобразования показаний датчика в температуру
int dt_check(void)
{
	unsigned char bt;//переменная для считывания байта
	unsigned int tt=0;
	if(ds18b20_Reset()==1) //если устройство нашлось
	{
		ds18b20_WriteByte(NOID); //пропустить идентификацию, тк у нас только одно устройство на шине
		ds18b20_WriteByte(T_CONVERT); //измеряем температуру
		_delay_ms(750); //в 12битном режиме преобразования - 750 милисекунд
		ds18b20_Reset(); //снова используем  те же манипуляции с шиной что и при проверке ее присутствия
		ds18b20_WriteByte(NOID); //пропустить идентификацию, тк у нас только одно устройство на шине
		ds18b20_WriteByte(READ_DATA); //даем команду на чтение данных с устройства
		bt = ds18b20_ReadByte(); //читаем младший бит
		tt = ds18b20_ReadByte(); //читаем старший бит MS
		tt = (tt<<8)|bt;//сдвигаем старший влево, младший пишем на его место, тем самым получаем общий результат
	}
	return tt;
}

//преобразование температуры в единицы
char converttemp (unsigned int tt)
{
	char t = tt>>4;//сдвиг и отсечение части старшего байта
	return t;
}
uint8_t ds18b20_SearhRom(uint8_t *Addr)
{
	uint8_t id_bit_number;
	uint8_t last_zero, rom_byte_number, search_result;
	uint8_t id_bit, cmp_id_bit;
	uint8_t rom_byte_mask, search_direction;
	//проинициализируем переменные
	id_bit_number = 1;
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;
	if (!LastDeviceFlag)
	{
		ds18b20_Reset();
		ds18b20_WriteByte(0xF0);
	}
	do
	{
		id_bit = ds18b20_ReadBit();
		cmp_id_bit = ds18b20_ReadBit();
		if ((id_bit == 1) && (cmp_id_bit == 1))
		break;
		else
		{
			if (id_bit != cmp_id_bit)
			search_direction = id_bit; // bit write value for search
			else
			{
				if (id_bit_number < LastDiscrepancy)
				search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
				else
				search_direction = (id_bit_number == LastDiscrepancy);
				if (search_direction == 0)
				{
					last_zero = id_bit_number;
					if (last_zero < 9)
					LastFamilyDiscrepancy = last_zero;
				}
			}
			if (search_direction == 1)
			ROM_NO[rom_byte_number] |= rom_byte_mask;
			else
			ROM_NO[rom_byte_number] &= ~rom_byte_mask;
			ds18b20_WriteBit(search_direction);
			id_bit_number++;
			rom_byte_mask <<= 1;
			if (rom_byte_mask == 0)
			{
				rom_byte_number++;
				rom_byte_mask = 1;
			}
		}
	} while(rom_byte_number < 8); // считываем байты с 0 до 7 в цикле
	if (!(id_bit_number < 65))
	{
		// search successful so set LastDiscrepancy,LastDeviceFlag,search_result
		LastDiscrepancy = last_zero;
		// check for last device
		if (LastDiscrepancy == 0)
		LastDeviceFlag = 1;
		search_result = 1;
	}
	if (!search_result || !ROM_NO[0])
	{
		LastDiscrepancy = 0;
		LastDeviceFlag = 0;
		LastFamilyDiscrepancy = 0;
		search_result = 0;
	}
	else
	{
		for (int i = 0; i < 8; i++) Addr[i] = ROM_NO[i];
	}
	return search_result;
}
//-----------------------------------------------
uint8_t ds18b20_init(uint8_t mode)
{
	int i = 0, j=0;
	uint8_t dt[8];
	
	if(mode==SKIP_ROM)
	{		
		if(!ds18b20_Reset()) return 1;
		//SKIP ROM
		ds18b20_WriteByte(0xCC);
		//WRITE SCRATCHPAD
		ds18b20_WriteByte(0x4E);
		//TH REGISTER 100 градусов
		ds18b20_WriteByte(0x64);
		//TL REGISTER - 30 градусов
		ds18b20_WriteByte(0x1E);
		//Resolution 12 bit
		ds18b20_WriteByte(RESOLUTION_12BIT);
	}
	else
	{
		for(i=1;i<=8;i++)
		{
			if(ds18b20_SearhRom(dt))
			{
				Dev_Cnt++;
				memcpy(Dev_ID[Dev_Cnt-1],dt,sizeof(dt));
			}
			else break;
		}
		for(i=1;i<=Dev_Cnt;i++)
		{
			if(!ds18b20_Reset()) return 1;
			//Match Rom
			ds18b20_WriteByte(0x55);
			for(j=0;j<=7;j++)
			{
				ds18b20_WriteByte(Dev_ID[i-1][j]);
			}
			//WRITE SCRATCHPAD
			ds18b20_WriteByte(0x4E);
			//TH REGISTER - 30 градусов
			ds18b20_WriteByte(0x1E);
			//TL REGISTER - 20 градусов
			ds18b20_WriteByte(0x14);
			//Resolution 12 bit
			ds18b20_WriteByte(RESOLUTION_12BIT);
		}
	}
	return 0;
}
//----------------------------------------------------------
void ds18b20_MeasureTemperCmd(uint8_t mode, uint8_t DevNum)
{
	int i = 0;
	ds18b20_Reset();
	if(mode==SKIP_ROM)
	{
		//SKIP ROM
		ds18b20_WriteByte(0xCC);
	}
	else
	{
		//Match Rom
		ds18b20_WriteByte(0x55);
		for(i=0;i<=7;i++)
		{
			ds18b20_WriteByte(Dev_ID[DevNum-1][i]);
		}
	}
	//CONVERT T
	ds18b20_WriteByte(0x44);
}
//----------------------------------------------------------
void ds18b20_ReadStratcpad(uint8_t mode, uint8_t *Data, uint8_t DevNum)
{
	uint8_t i;
	ds18b20_Reset();
	if(mode==SKIP_ROM)
	{
		//SKIP ROM
		ds18b20_WriteByte(0xCC);
	}
	else
	{
		//Match Rom
		ds18b20_WriteByte(0x55);
		for(i=0;i<=7;i++)
		{
			ds18b20_WriteByte(Dev_ID[DevNum-1][i]);
		}
	}
	//READ SCRATCHPAD
	ds18b20_WriteByte(0xBE);
	for(i=0;i<=8;i++)
	{
		Data[i] = ds18b20_ReadByte();
	}
}
//----------------------------------------------------------
uint8_t ds18b20_GetSign(uint16_t dt)
{
	//ѕроверим 11-й бит
	if (dt&(1<<11)) return 1;
	else return 0;
}
//----------------------------------------------------------
float ds18b20_Convert(uint16_t dt)
{
	float t;
	t = (float) ((dt&0x07FF)>>4); //отборосим знаковые и дробные биты
// 	//ѕрибавим дробную часть
	t += (float)(dt&0x000F) / 16.0f;
	return t;
}
//----------------------------------------------------------
