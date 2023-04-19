#include "app.h"

//Define frequency of oscillator on disco to override
//default value defined in device header file
#define HSE_VALUE	8000000
#include "stm32f411xe.h"

#include "I2C.h"
#include "I2S.h"
#include "UART.h"
#include "cbfifo.h"
#include "CS43L22_DAC.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

extern	bool PB_flag;

void init(void)
{	
	setSysClk_HSE_8MHz();
	
	//Select HSE as PLLI2S source
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	
	//Limit VCO input freq. to 2 MHz based on HSE 8 MHz clock
	//Set to 4 (8 MHz / 4 = 2 MHz)
	RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SM_2;
	
	//Set VCO output freq. to > 100 MHz and < 432 MHz
	RCC->PLLI2SCFGR |= RCC_PLLI2SCFGR_PLLI2SN_2 | RCC_PLLI2SCFGR_PLLI2SN_5 \
										 | RCC_PLLI2SCFGR_PLLI2SN_6;
	RCC->PLLI2SCFGR &= ~RCC_PLLCFGR_PLLN_7;
	
	//By default PLLI2SR already set to 2, dividing VCO clock by 2,
	//which sets resulting I2S clock at 100 MHz
	
	//From online example
	RCC->PLLI2SCFGR |= (258 << 6); // N value = 258
  	RCC->PLLI2SCFGR |= (3 << 28); // R value = 3
	//Enable PLLI2S
	RCC->CR |= RCC_CR_PLLI2SON;
		
	while(!(RCC->CR & RCC_CR_PLLI2SRDY))
		;
	
	//DAC Reset Pin
	//Enable GPIOD
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	//Set as GP output
	GPIOD->MODER |= GPIO_MODER_MODE4_0;	
	
	I2C1_init();
	I2S3_init();
	
	//UART_init();
	
}

void setSysClk_HSE_8MHz(void)
{
	SystemInit();

	//Enable HSE & wait for it to stabilize
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY))
		;

	//Configure Flash
	//Enable prefetch, instruction cache, data cache
	FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
	//Zero wait-state latency
	FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk;
	
	//Set HSE as system clock source & wait for switch to complete
	RCC->CFGR |= RCC_CFGR_SW_HSE;	
	while(!(RCC->CFGR & RCC_CFGR_SWS_HSE))
		;
	
	SystemCoreClock = HSE_VALUE;

}

void setSysClk_84MHz(void)
{
	SystemInit();
	
	//Enable HSE & wait for it to stabilize
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY))
		;
	
	
}

void mainLoop(void)
{
	dac_powerup_seq();
	
	//Read Chip ID & Rev.
	readDACreg(CHIP_ID_AND_REV_REG);
	
	I2S3_send(0xDEAD);
	I2S3_send(0xBEEF);
}

void commandProcessor(void)
{
    uint8_t currChar;
	
    while(1<2)
    {
			while(cbfifo_length(RX_BUFF)==0)
				;
			
			if(cbfifo_length(RX_BUFF)!=0)
			{
				cbfifo_dequeue(RX_BUFF,&currChar,1);

				//Echo all characters
				putchar(currChar);
				
				switch(currChar)
				{
					default:
						printInstructions();
				}			
			}			

    }
}

void printInstructions(void)
{
	char instructionsStr[] = "\r\n\nThe following commands are available:\r\n"
														"\t\'+\' - Increase duty cycle by 5%\r\n"
														"\t\'-\' - Decrease duty cycle by 5%\r\n"
														"\t\'P\' - Print the duty cycle\r\n"
														"\nAdditionally, the blue pushbutton on the"
														" board may be pushed to move the duty cycle by"
														" 10% between the bounds of 0-100%\r\n\n";
	putstr(instructionsStr,sizeof(instructionsStr)/sizeof(instructionsStr[0]));
}

