
//Circular buffer code used in PES Spring 2022 written by myself, Erich Clever
//Modified slightly for this lab to provide two buffer for UART Tx and Rx

#include "cbfifo.h"

#include <stddef.h>
#include <stdint.h>

#define	CB_SIZE	128

// Struct:  cbfifo_t - Circular FIFO buffer of fixed size in which size 
//						refers to number of bytes held
//
// Members: list[] (uint8_t) - Array of bytes
//			  read (uint32_t) - Index of next byte to be output
//			  write (uint32_t) - Index of destination for next byte input
//			  isFull (uint8_t) - Flag to indicate if CB is full
//
typedef struct{
	
	uint8_t list[CB_SIZE];
	uint32_t read,write;
	uint8_t isFull;
	
}cbfifo_t;


//Initialization of circular buffer
static cbfifo_t Rx_cbfifo = { .read = 0, .write = 0, .isFull = 0 };
static cbfifo_t Tx_cbfifo = { .read = 0, .write = 0, .isFull = 0 };

size_t cbfifo_enqueue(UARTbuff_t UARTbuff, void *buf, size_t nbyte){
	
	cbfifo_t *targetBuff;
	
	switch(UARTbuff)
	{
		case RX_BUFF:
			targetBuff = &Rx_cbfifo;
			break;
		case TX_BUFF:
			targetBuff = &Tx_cbfifo;
			break;
	}
	
	uint32_t size = sizeof(targetBuff->list)/sizeof(targetBuff->list[0]);
	
	//If input buffer is a NULL pointer,
	//an error is returned
	if(buf == NULL)
		return (size_t)-1;
	
	//User-specified number of bytes (nbytes)
	//are written to the CB or until it is full
	for(int i=0;i<nbyte;i++){
		
		//If CB is full, the number of write
		//operations that have occurred 
		//will indicate how many bytes were
		//written successfully to the CB
		if(targetBuff->isFull)
			return i;
		
		targetBuff->list[targetBuff->write] = *(uint8_t*)(buf+i);
		//White, Chp.6
		targetBuff->write = (targetBuff->write + 1) & (size - 1);
		
		//If the wrapped subtraction from cbfifo_length()
		//returns 0 after an enqueue, this indicates that
		//the CB is full.
		if(!cbfifo_length(UARTbuff))
			targetBuff->isFull = 1;

	}
	
	return nbyte;
}

size_t cbfifo_dequeue(UARTbuff_t UARTbuff, void *buf, size_t nbyte){
	
	cbfifo_t *targetBuff;
	
	switch(UARTbuff)
	{
		case RX_BUFF:
			targetBuff = &Rx_cbfifo;
			break;
		case TX_BUFF:
			targetBuff = &Tx_cbfifo;
			break;
	}
	
	uint32_t size = sizeof(targetBuff->list)/sizeof(targetBuff->list[0]);
	
	//If input buffer is a NULL pointer,
	//an error is returned
	if( buf == NULL)
		return (size_t)-1;
	
	//Check if CB is empty before attempting
	//to dequeue
	if(cbfifo_length(UARTbuff)==0 && !targetBuff->isFull)
		return 0;
	
	//User-specified number of bytes (nbytes)
	//are read from the CB or until it is empty
	for(int i=0;i<nbyte;i++){
		
		//Check isFull flag before
		//deciding if CB is empty
		if(targetBuff->isFull){
			targetBuff->isFull=0;
		}
		else{
			if(!cbfifo_length(UARTbuff))
				return i;
		}
		
		*(uint8_t*)(buf+i) = targetBuff->list[targetBuff->read];
		//White, Chp.6
		targetBuff->read = (targetBuff->read + 1) & (size - 1);
			
	}
	
	return nbyte;

}

size_t cbfifo_length(UARTbuff_t UARTbuff){
	
	cbfifo_t *targetBuff;
	
	switch(UARTbuff)
	{
		case RX_BUFF:
			targetBuff = &Rx_cbfifo;
			break;
		case TX_BUFF:
			targetBuff = &Tx_cbfifo;
			break;
	}
	
	uint32_t size = sizeof(targetBuff->list)/sizeof(targetBuff->list[0]);
	
	//If CB has been filled by enqueueing,
	//this flag will be set and will return
	//an accurate length
	if(targetBuff->isFull)
		return CB_SIZE;

	//White, Chp.6
	return ((targetBuff->write - targetBuff->read) 
					& (size-1));
	
}

size_t cbfifo_capacity(UARTbuff_t UARTbuff){

	cbfifo_t *targetBuff;
	
	switch(UARTbuff)
	{
		case RX_BUFF:
			targetBuff = &Rx_cbfifo;
			break;
		case TX_BUFF:
			targetBuff = &Tx_cbfifo;
			break;
	}
	
	return sizeof(targetBuff->list)/sizeof(targetBuff->list[0]);
	
}
