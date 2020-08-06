#ifndef DHT11_H_
#define DHT11_H_

#include "main.h"

#define DHT11_PIN 0 // define analog port 0

char read_dht11_dat();
void dht11_start();
char dht11_temp();
char dht11_humid();


#endif /* DHT11_H_ */