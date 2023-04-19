#include "CS43L22_DAC.h"

#include "I2C.h"

#include "stm32f411xe.h"

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
	I2C1_start();
	I2C1_sendAddr(DAC_ADDR);
	I2C1_sendByte(regAddr);
	I2C1_sendByte(val);
	I2C1_stop();
}

uint8_t readDACreg(uint8_t regAddr)
{
	//Aborted writed to set register to read
	I2C1_start();
	I2C1_sendAddr(DAC_ADDR);
	I2C1_sendByte(regAddr);
	I2C1_stop();
	
	//Read from DAC with desired address set
	I2C1_start();
	I2C1_sendAddr(DAC_ADDR+1);
	uint8_t returnVal = I2C1_readByte();
	I2C1_stop();
	return returnVal;
}