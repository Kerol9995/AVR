#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "mg12232.h"
#include "dht11.h"
#include "ds18b20.h"
#include "usart.h"
#include "adc.h"
#include "timer0.h"

unsigned int adc_value;

#endif