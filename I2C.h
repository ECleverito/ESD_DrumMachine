#pragma once

#include "stm32f411xe.h"

#include <stdint.h>

#define AUDIO_SDA_PORT	Port

void I2C_init();

void I2C_start();

void I2C_stop();

void I2C_sendAddr(uint8_t addr);

void I2C_sendByte(uint8_t data);

uint8_t I2C_readByte(void);

void I2C_write(uint8_t addr, uint8_t data);