#include "I2C.h"

void I2C_init()
{	
	//Enable AHB1 I2C1 Clock Path
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	
	//Enable GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	GPIOB -> OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT9;
	GPIOB -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR9;
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD9_0;
	
	//Disable I2C1
	I2C1->CR1 &= ~I2C_CR1_PE;
	
	//Reset I2C1
	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;
	
	//Setting "alternate function mode" for pins 6 and 9 on Port B
	GPIOB->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE9_1;
	GPIOB->MODER &= ((~GPIO_MODER_MODE6_0) & (~GPIO_MODER_MODE9_0));
	
	//Set Alternate Function 4 for pins 6 & 9 on Port B
	//PB6 => I2C1.SCL
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2;
	GPIOB->AFR[0] &= ((~GPIO_AFRL_AFSEL6_0) & (~GPIO_AFRL_AFSEL6_1));
	//PB9 => I2C1.SDA
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_2;
	GPIOB->AFR[1] &= ((~GPIO_AFRH_AFSEL9_0) & (~GPIO_AFRH_AFSEL9_1));
	
	GPIOB->IDR = 0;
	
	//Program peripheral input clock for I2C1 (8 MHz)
	I2C1->CR2 |= I2C_CR2_FREQ_5;
	
	//Program Clock Control Reg. for Standard-mode (100 kHz)
	I2C1->CCR |= 160;
	
	//Configure Rise Time
	I2C1->TRISE = 33;
	
	//Enable I2C1
	I2C1->CR1 |= I2C_CR1_PE;
	
}

void I2C_start()
{
	I2C1->CR1 &= ~I2C_CR1_POS;
	//I2C1->CR1 |= I2C_CR1_ACK;
	
	//Set start condition bit
	I2C1->CR1 |= I2C_CR1_START;
	
	//Wait for start condition to be generated
	while(!(I2C1->SR1 & I2C_SR1_SB))
		;
}

void I2C_stop()
{
	uint8_t reg;
	
	I2C1->CR1 |= I2C_CR1_STOP;
	
	reg = I2C1->SR1;
	reg = I2C1->SR2;
	
}

void I2C_sendAddr(uint8_t addr)
{	
	uint8_t reg;
	
	//Send address
	I2C1->DR = addr;
	
	reg = I2C1->SR1;
	reg = I2C1->SR2;
	
	//Wait for transmission to complete
	while(!(I2C1->SR1 & I2C_SR1_ADDR))
		;
	
	reg = I2C1->SR1;
	reg = I2C1->SR2;
}

void I2C_sendByte(uint8_t data)
{	
	//Send data
	I2C1->DR = data;
	
	//Wait for transmission to complete
	while(!(I2C1->SR1 & I2C_SR1_TXE))
		;	
}

uint8_t I2C_readByte(void)
{
	uint8_t returnVal=0;
	
	returnVal=I2C1->DR;
	//Wait for transmission to complete
	while(!(I2C1->SR1 & I2C_SR1_RXNE))
		;
	
	return returnVal;
}

	