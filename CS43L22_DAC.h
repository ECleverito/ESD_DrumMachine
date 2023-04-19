#pragma once

#define DAC_ADDR 0x94

//Register map
#define	CHIP_ID_AND_REV_REG	0x01

#define INIT_SEQ_REG_1	0x99
#define INIT_SEQ_VAL_1	0x00
#define INIT_SEQ_REG_2	0x80
#define INIT_SEQ_VAL_2	0x47
#define INIT_SEQ_REG_3	0x32
#define INIT_SEQ_VAL_3	0x80
#define INIT_SEQ_REG_4	0x32
#define INIT_SEQ_VAL_4	0x00
#define INIT_SEQ_REG_5	0x00
#define INIT_SEQ_VAL_5	0x00

#include <stdint.h>

void dac_powerup_seq(void);

void writeDACreg(uint8_t regAddr, uint8_t val);

uint8_t readDACreg(uint8_t regAddr);
