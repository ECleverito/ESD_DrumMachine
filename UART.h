#pragma once

#include <stdint.h>

//Initializes UART to B.R. 9600, 1 start bit, 1 stop bit, 
//for device Tx on PA2 and Rx on PA3.
void UART_init(void);

int putchar(int c);

void putstr(uint8_t *str, uint16_t len);

int getchar(void);
