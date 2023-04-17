#include "app.h"

//Define frequency of oscillator on disco to override
//default value defined in device header file
#define HSE_VALUE	8000000
#include "stm32f411xe.h"

#include "UART.h"
#include "cbfifo.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

extern	bool PB_flag;

void init(void)
{
	//Enable HSE and select as system clock
	RCC->CR |= RCC_CR_HSEON;
	RCC->CFGR |= RCC_CFGR_SW_HSE;	
	RCC->CFGR |= RCC_CFGR_SWS_HSE;
	
	SystemInit();
	
	SystemCoreClockUpdate();
	
	UART_init();
	
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

