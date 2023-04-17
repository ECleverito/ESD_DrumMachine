
#include "IRQ.h"

#include "UART.h"

#include "cbfifo.h"

#include <stm32f411xe.h>

#include <stdbool.h>

void USART2_IRQHandler(void)
{	
	__disable_irq();
	
	uint32_t USART_Int_flags = USART2->SR;
	
	//Rx
	if(USART_Int_flags & USART_SR_RXNE)
	{
		uint8_t inChar = (USART2->DR & 0x000000FF);
		cbfifo_enqueue(RX_BUFF,&inChar,1);
		USART2->SR &= ~USART_SR_RXNE;
	}
	
	//Tx
	if(USART_Int_flags & USART_CR1_TCIE)
	{
		if(cbfifo_length(TX_BUFF)>0)
		{
			uint8_t outChar;
			cbfifo_dequeue(TX_BUFF, &outChar, 1);
			USART2->DR = (outChar & 0x000000FF);
		}

		USART2->SR &= ~USART_CR1_TCIE;
	}
	
	__enable_irq();
}
