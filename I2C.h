#pragma once

#include <stdint.h>

void I2C1_init();

void I2C1_start();

void I2C1_stop();

void I2C1_sendAddr(uint8_t addr);

void I2C1_sendByte(uint8_t data);

uint8_t I2C1_readByte(void);

void I2C1_write(uint8_t addr, uint8_t data);