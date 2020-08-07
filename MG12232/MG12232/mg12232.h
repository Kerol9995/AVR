#ifndef MG12232_H_
#define MG12232_H_

#include "main.h"

#define DATA_DDR DDRD
#define DATA_PIN PIND
#define DATA_PORT PORTD
#define A0_PORT PORTB
#define RW_PORT PORTB
#define CS1_PORT PORTB
#define CS2_PORT PORTB
#define A0_PIN_NUM 2
#define RW_PIN_NUM 3
#define CS1_PIN_NUM 0
#define CS2_PIN_NUM 1

#define CMD_DISPLAY_ON 0xAF
#define CMD_DISPLAY_OFF 0xAE
#define CMD_START_LINE 0xDF
#define CMD_PAGE 0xB8
#define CMD_COLUMN 0x13
#define CMD_ADC_CW 0xA0
#define CMD_ADC_CCW 0xA1
#define CMD_DUTY_CYCLE_32 0xA9
#define CMD_STATIC_DRIVE_OFF 0xA4
#define CMD_STATIC_DRIVE_ON 0xA5
#define CMD_RMW_START 0xE0
#define CMD_RMW_END 0xEE
#define CMD_RESET 0xE2

void mg12232_init(void);
void data_write(uint8_t data, uint8_t chip);
void command_write(uint8_t data, uint8_t chip);
void clear_lcd(void);
void simbol_write(uint8_t simbol[], uint8_t chip);
void number_write(char t, uint8_t chip);
void mg12232_string_write(char str[], uint8_t page);

#endif /* MG12232_H_ */