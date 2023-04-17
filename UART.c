#include "UART.h"

#include "stm32f411xe.h"
//#include "core_cm4.h"


//Collaborated on this with Kiran Jojare
void UART_init(void)
{
	//Enable AHB1 GPIOA clock path to route
	//USART1 to these pins
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	//Enable APB1 USART2 clock path
	RCC->APB1ENR = RCC_APB1ENR_USART2EN;
	
	//Setting "alternate function mode" for pins 2 and 3 on
	//port A
	GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
	
	//Select alternate function 7 (USART2 Tx) for pin A2
	//Select alternate function 7 (USART12 Rx) for pin A3
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 \
										| GPIO_AFRL_AFSEL2_2 \
										| GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 \
										| GPIO_AFRL_AFSEL3_2;
	
	//USARTDIV mantissa is 0d52, and fraction is 0d0.625 for a
	//dividor of 52.0625 that leads to a baud rate of 9600
	//based on a selected system clock of 8 MHz (See calculations on page
	//520 of the STM32F411xC/E Ref. Manual RM0383)
	USART2->BRR = 52 << USART_BRR_DIV_Mantissa_Pos;
	USART2->BRR |= 1 << USART_BRR_DIV_Fraction_Pos;
	
	//Enable USART2 in oversampling by 16 mode, 1-start bit, 8 data bits,
	//1 stop bit, no parity, with first clock transition as first data capture
	//edge1;
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	
	
	//Enable interrupts
	USART2->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE;
	//Clear all interrupt flags initially
	USART2->SR = 0;
	__NVIC_ClearPendingIRQ(USART2_IRQn);
	__NVIC_EnableIRQ(USART2_IRQn);
}


int putchar(int c)
{
	while(!(USART2->SR & USART_SR_TXE))
		;
	
	USART2->DR = c & 0x000000FF;
	return c;
	
}

void putstr(uint8_t *str, uint16_t len)
{
	for(int i=0;i<len;i++)
	{
		if(str[i]=='\0')
		{
			break;
		}
		putchar(str[i]);
	}
}

int getchar(void)
{
	while(!(USART2->SR & USART_SR_RXNE))
		;
	
	return (int)(USART2->DR & 0x000000FF);
	
}
