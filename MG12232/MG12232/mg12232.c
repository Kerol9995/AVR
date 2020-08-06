#include "mg12232.h"


void mg12232_init(void){
	DATA_DDR= 0xff;
	DATA_PORT=0xff;
	DDRB=0x07;
	PORTB=0x07;
	PORTB=0x04;
	PORTB=0x07;
	PORTB=0x04;
	PORTB=0x07;
	PORTB=0x00;
	command_write(CMD_RESET,3);
	command_write(CMD_DUTY_CYCLE_32,3);
	command_write(CMD_STATIC_DRIVE_OFF,3);
	command_write(CMD_ADC_CCW,3);
	command_write(CMD_DISPLAY_ON,3);
	clear_lcd();
	command_write(CMD_PAGE+0,3);
	command_write(CMD_COLUMN+0,3);
	command_write(CMD_START_LINE+0,3);
}

void command_write(uint8_t data, uint8_t chip){
	DATA_DDR=0xff;
	DATA_PORT=data;
	A0_PORT &=~(1<<A0_PIN_NUM);
	if(chip&1) CS1_PORT |=(1<<CS1_PIN_NUM);
	if(chip&2) CS2_PORT |=(1<<CS2_PIN_NUM);
	CS1_PORT &=~((1<<CS1_PIN_NUM)|(1<<CS2_PIN_NUM));
	DATA_PORT=0;
	DATA_DDR=0;
}

void data_write(uint8_t data, uint8_t chip){
	DATA_DDR=0xff;
	DATA_PORT=data;
	A0_PORT |=(1<<A0_PIN_NUM);
	if(chip&1) CS1_PORT |=(1<<CS1_PIN_NUM);
	if(chip&2) CS2_PORT |=(1<<CS2_PIN_NUM);
	CS1_PORT &= ~((1 << CS1_PIN_NUM)|(1 << CS2_PIN_NUM));
	DATA_PORT=0;
	DATA_DDR=0;
}
void clear_lcd(void){
	for (uint8_t page = 0; page < 4; page++) {
		command_write(CMD_PAGE + page, 3);
		command_write(CMD_COLUMN + 0, 3);
		for (uint8_t x = 0; x < 61; x++) {
			data_write(0, 3);
		}
	}
}
void simbol_write(unsigned char simbol[], uint8_t chip){
	for (unsigned char a=0; a<6;a++)
	{
		data_write(simbol[a],chip);
	}
}
void mg12232_string_write(char str[]){
	uint8_t a;
	uint8_t chip;
	for (a=0;a<strlen(str);a++){		
		if (a==0){
			command_write(CMD_PAGE+0,3);
			command_write(CMD_COLUMN+1,3);
		}
		if (a==20){
			command_write(CMD_PAGE+1,3);
			command_write(CMD_COLUMN+1,3);
		}
		if (a==40){
			command_write(CMD_PAGE+2,3);
			command_write(CMD_COLUMN+1,3);
		}
		if (a==60){
			command_write(CMD_PAGE+3,3);
			command_write(CMD_COLUMN+1,3);
		}
		if ((a>=10 && a<=19)||(a>=30 && a<=39)||(a>=50 && a<=59)||(a>=70 && a<=79))chip=2;
		else chip=1;
		number_write(str[a],chip);
	}
}

unsigned char numbers [][6]={
	{0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110, 0x0},	//0
	{0b00000000, 0b01000010, 0b01111111, 0b01000000, 0b00000000, 0x0},	//1
	{0b01000010, 0b01100001, 0b01010001, 0b01001001, 0b01000110, 0x0},	//2
	{0b00100001, 0b01000001, 0b01000101, 0b01001011, 0b00110001, 0x0},	//3
	{0b00011000, 0b00010100, 0b00010010, 0b01111111, 0b00010000, 0x0},	//4
	{0b00100111, 0b01000101, 0b01000101, 0b01000101, 0b00111001, 0x0},	//5
	{0b00111100, 0b01001010, 0b01001001, 0b01001001, 0b00110000, 0x0},	//6
	{0b00000001, 0b00000001, 0b01111001, 0b00000101, 0b00000011, 0x0},	//7
	{0b00110110, 0b01001001, 0b01001001, 0b01001001, 0b00110110, 0x0},	//8
	{0b00000110, 0b01001001, 0b01001001, 0b00101001, 0b00011110, 0x0},	//9
	{0b00000000, 0b00010100, 0b00010100, 0b00010100, 0b00000000, 0x0},	//=
	{0b01111100, 0b00010010, 0b00010001, 0b00010010, 0b01111100, 0x0},	//A
	{0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110110, 0x0},	//B
	{0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00100010, 0x0},	//C
	{0b01111111, 0b01000001, 0b01000001, 0b00100010, 0b00011100, 0x0},	//D
	{0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b01001001, 0x0},	//E
	{0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00001001, 0x0},	//F
	{0b00111110, 0b01000001, 0b01000001, 0b01010001, 0b00110010, 0x0},	//G
	{0b01111111, 0b00001000, 0b00001000, 0b00001000, 0b01111111, 0x0},	//H
	{0b00000000, 0b01000001, 0b01111111, 0b01000001, 0b00000000, 0x0},	//I
	{0b00100000, 0b01000000, 0b01000000, 0b01000000, 0b00111111, 0x0},	//J
	{0b01111111, 0b00001000, 0b00010100, 0b00100010, 0b01000001, 0x0},	//K
	{0b01111111, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0x0},	//L
	{0b01111111, 0b00000010, 0b00000100, 0b00000010, 0b01111111, 0x0},	//M
	{0b01111111, 0b00000100, 0b00001000, 0b00010000, 0b01111111, 0x0},	//N
	{0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110, 0x0},	//O
	{0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00000110, 0x0},	//P
	{0b00111110, 0b01000001, 0b01010001, 0b01100001, 0b00111110, 0x0},	//Q
	{0b01111111, 0b00001001, 0b00011001, 0b00101001, 0b01000110, 0x0},	//R
	{0b00100110, 0b01001001, 0b01001001, 0b01001001, 0b00110010, 0x0},	//S
	{0b00000001, 0b00000001, 0b01111111, 0b00000001, 0b00000001, 0x0},	//T
	{0b00111111, 0b01000000, 0b01000000, 0b01000000, 0b00111111, 0x0},	//U
	{0b00011111, 0b00100000, 0b01000000, 0b00100000, 0b00011111, 0x0},	//V
	{0b01111111, 0b00100000, 0b00010000, 0b00100000, 0b01111111, 0x0},	//W
	{0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011, 0x0},	//X
	{0b00000011, 0b00000100, 0b01111000, 0b00000100, 0b00000011, 0x0},	//Y
	{0b01100001, 0b01010001, 0b01001001, 0b01000101, 0b01000011, 0x0},	//Z
	{0b00100000, 0b01010100, 0b01010100, 0b01010100, 0b01111000, 0x0},	//a
	{0b01111111, 0b01001000, 0b01000100, 0b01000100, 0b00111000, 0x0},	//b
	{0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0x0},	//c
	{0b00111000, 0b01000100, 0b01000100, 0b01001000, 0b01111111, 0x0},	//d
	{0b00111000, 0b01010100, 0b01010100, 0b01010100, 0b00011000, 0x0},	//e
	{0b00001000, 0b01111110, 0b00001001, 0b00000010, 0b00000000, 0x0},	//f
	{0b00001000, 0b01010100, 0b01010100, 0b01010100, 0b00111100, 0x0},	//g
	{0b01111111, 0b00010000, 0b00001000, 0b00001000, 0b01110000, 0x0},	//h
	{0b00000000, 0b00000000, 0b01111010, 0b00000000, 0b00000000, 0x0},	//i
	{0b00000000, 0b01000000, 0b01000000, 0b00111101, 0b00000000, 0x0},	//j
	{0b01111111, 0b00010000, 0b00101000, 0b01000100, 0b00000000, 0x0},	//k
	{0b00000000, 0b00000000, 0b01111111, 0b00000000, 0b00000000, 0x0},	//l
	{0b01111000, 0b00000100, 0b01111000, 0b00000100, 0b01111000, 0x0},	//m
	{0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b01111000, 0x0},	//n
	{0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00111000, 0x0},	//o
	{0b01111100, 0b00010100, 0b00010100, 0b00010100, 0b00001000, 0x0},	//p
	{0b00001000, 0b00010100, 0b00010100, 0b00010100, 0b01111100, 0x0},	//q
	{0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b00001000, 0x0},	//r
	{0b01001000, 0b01010100, 0b01010100, 0b01010100, 0b00100100, 0x0},	//s
	{0b00000100, 0b00111111, 0b01000100, 0b01000000, 0b00100000, 0x0},	//t
	{0b00111100, 0b01000000, 0b01000000, 0b00100000, 0b01111100, 0x0},	//u
	{0b00011100, 0b00100000, 0b01000000, 0b00100000, 0b00011100, 0x0},	//v
	{0b00111100, 0b01000000, 0b00110000, 0b01000000, 0b00111100, 0x0},	//w
	{0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100, 0x0},	//x
	{0b00001100, 0b01010000, 0b01010000, 0b01010000, 0b00111100, 0x0},	//y
	{0b01000100, 0b01100100, 0b01010100, 0b01001100, 0b01000100, 0x0},	//z
	{0b00000000, 0b00000000, 0b01000000, 0b00000000, 0b00000000, 0x0},	//.
	{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0x0},	//space
	{0b00000000, 0b00001000, 0b00001000, 0b00001000, 0b00000000, 0x0},	//-
	{0b00000000, 0b00001000, 0b00011100, 0b00001000, 0b00000000, 0x0}		//+
};

void number_write(char t, uint8_t chip){
	switch(t){
		case '0' : simbol_write(numbers[0],chip);break;
		case '1' : simbol_write(numbers[1],chip);break;
		case '2' : simbol_write(numbers[2],chip);break;
		case '3' : simbol_write(numbers[3],chip);break;
		case '4' : simbol_write(numbers[4],chip);break;
		case '5' : simbol_write(numbers[5],chip);break;
		case '6' : simbol_write(numbers[6],chip);break;
		case '7' : simbol_write(numbers[7],chip);break;
		case '8' : simbol_write(numbers[8],chip);break;
		case '9' : simbol_write(numbers[9],chip);break;
		case '=' : simbol_write(numbers[10],chip);break;
		case 'A' : simbol_write(numbers[11],chip);break;
		case 'B' : simbol_write(numbers[12],chip);break;
		case 'C' : simbol_write(numbers[13],chip);break;
		case 'D' : simbol_write(numbers[14],chip);break;
		case 'E' : simbol_write(numbers[15],chip);break;
		case 'F' : simbol_write(numbers[16],chip);break;
		case 'G' : simbol_write(numbers[17],chip);break;
		case 'H' : simbol_write(numbers[18],chip);break;
		case 'I' : simbol_write(numbers[19],chip);break;
		case 'J' : simbol_write(numbers[20],chip);break;
		case 'K' : simbol_write(numbers[21],chip);break;
		case 'L' : simbol_write(numbers[22],chip);break;
		case 'M' : simbol_write(numbers[23],chip);break;
		case 'N' : simbol_write(numbers[24],chip);break;
		case 'O' : simbol_write(numbers[25],chip);break;
		case 'P' : simbol_write(numbers[26],chip);break;
		case 'Q' : simbol_write(numbers[27],chip);break;
		case 'R' : simbol_write(numbers[28],chip);break;
		case 'S' : simbol_write(numbers[29],chip);break;
		case 'T' : simbol_write(numbers[30],chip);break;
		case 'U' : simbol_write(numbers[31],chip);break;
		case 'V' : simbol_write(numbers[32],chip);break;
		case 'W' : simbol_write(numbers[33],chip);break;
		case 'X' : simbol_write(numbers[34],chip);break;
		case 'Y' : simbol_write(numbers[35],chip);break;
		case 'Z' : simbol_write(numbers[36],chip);break;
		case 'a' : simbol_write(numbers[37],chip);break;
		case 'b' : simbol_write(numbers[38],chip);break;
		case 'c' : simbol_write(numbers[39],chip);break;
		case 'd' : simbol_write(numbers[40],chip);break;
		case 'e' : simbol_write(numbers[41],chip);break;
		case 'f' : simbol_write(numbers[42],chip);break;
		case 'g' : simbol_write(numbers[43],chip);break;
		case 'h' : simbol_write(numbers[44],chip);break;
		case 'i' : simbol_write(numbers[45],chip);break;
		case 'j' : simbol_write(numbers[46],chip);break;
		case 'k' : simbol_write(numbers[47],chip);break;
		case 'l' : simbol_write(numbers[48],chip);break;
		case 'm' : simbol_write(numbers[49],chip);break;
		case 'n' : simbol_write(numbers[50],chip);break;
		case 'o' : simbol_write(numbers[51],chip);break;
		case 'p' : simbol_write(numbers[52],chip);break;
		case 'q' : simbol_write(numbers[53],chip);break;
		case 'r' : simbol_write(numbers[54],chip);break;
		case 's' : simbol_write(numbers[55],chip);break;
		case 't' : simbol_write(numbers[56],chip);break;
		case 'u' : simbol_write(numbers[57],chip);break;
		case 'v' : simbol_write(numbers[58],chip);break;
		case 'w' : simbol_write(numbers[59],chip);break;
		case 'x' : simbol_write(numbers[60],chip);break;
		case 'y' : simbol_write(numbers[61],chip);break;
		case 'z' : simbol_write(numbers[62],chip);break;
		case '.' : simbol_write(numbers[63],chip);break;
		case ' ' : simbol_write(numbers[64],chip);break;
		case '-' : simbol_write(numbers[65],chip);break;
		case '+' : simbol_write(numbers[66],chip);break;
	}
}
