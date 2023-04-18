#include "CS43L22_DAC.h"

#include "I2C.h"

void dac_powerup_seq()
{
	//Bring RESET line high
	GPIOD->ODR |= GPIO_ODR_OD4;
	writeDACreg(INIT_SEQ_REG_1, INIT_SEQ_VAL_1);
	writeDACreg(INIT_SEQ_REG_2, INIT_SEQ_VAL_2);
	writeDACreg(INIT_SEQ_REG_3, INIT_SEQ_VAL_3);
	writeDACreg(INIT_SEQ_REG_4, INIT_SEQ_VAL_4);
	writeDACreg(INIT_SEQ_REG_5, INIT_SEQ_VAL_5);
	
}

void writeDACreg(uint8_t regAddr, uint8_t val)
{
	I2C_start();
	I2C_sendAddr(DAC_ADDR);
	I2C_sendByte(regAddr);
	I2C_sendByte(val);
	I2C_stop();
}

uint8_t readDACreg(uint8_t regAddr)
{
	I2C_start();
	I2C_sendAddr(DAC_ADDR+1);
	I2C_sendByte(regAddr);
	uint8_t returnVal = I2C_readByte();
	I2C_stop();
	return returnVal;
}