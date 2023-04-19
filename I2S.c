#include "I2S.h"

#include "stm32f411xe.h"

void I2S3_init()
{
	//Send clock to SPI3(I2S3)
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	
	//Send clock to GPIOA & GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	//Configure following pins accordingly:
	//		PC7  - MCK
	//		PC10 - SCK
	//		PC12 - SD  
	//		PA4  - WS  
	
	//Set all pins used as outputs to open drains
	GPIOC->OTYPER |= GPIO_OTYPER_OT7;
	GPIOC->OTYPER |= GPIO_OTYPER_OT10;
	GPIOC->OTYPER |= GPIO_OTYPER_OT12;
	GPIOA->OTYPER |= GPIO_OTYPER_OT4;
	
	//Set all pins to fast speed (0b10)
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_1;
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10_1;
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_1;
	
	//Set all pins to "alternate function mode" 6(I2S3)
	GPIOC->AFR[0] |= GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_1;
	GPIOC->AFR[1] |= GPIO_AFRH_AFRH2_2 | GPIO_AFRH_AFRH2_1;
	GPIOC->AFR[1] |= GPIO_AFRH_AFRH4_2 | GPIO_AFRH_AFRH4_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL4_2 | GPIO_AFRL_AFRL4_1;
	
	//I2S configuration
	//Disable I2S initially
	I2S3ext->I2SCFGR &= ~SPI_I2SCFGR_I2SE;
	
	//Select I2S mode, master transmitter, right-justified,
	//low-level steady-state, 16-bit
	I2S3ext->I2SCFGR |= SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG_1 | \
									 SPI_I2SCFGR_I2SSTD_1;
									 
	//Enable Master clock output
	I2S3ext->I2SPR |= SPI_I2SPR_MCKOE;
	
	//Set I2S linear prescaler
	//SPI3->I2SPR |= (SPI_I2SPR_
	
	//Enable I2S3
	I2S3ext->I2SCFGR |= SPI_I2SCFGR_I2SE;
	
}

void I2S3_send(uint16_t data)
{
	I2S3ext->DR = data;
	
	while(!(I2S3ext->SR & SPI_SR_TXE))
		;
}