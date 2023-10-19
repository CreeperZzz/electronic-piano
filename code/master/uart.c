#include "uart.h"
#include "avr.h"
void uart_init()
{
	UBRRL=0x33;                 //Hex value for Baud rate 9600
	UCSRB=0x18;                //Initialization of UCSRB reg
	UCSRC=0x06;                //Initialization of UCSRC reg
}

void uart_sendbyte(char b)
{
	//lcd_put('s');
	while(!GET_BIT(UCSRA, 5)); //wait for transmission complete
	UDR = b;				//load byte into register
	CLR_BIT(UCSRA, 5);		//start transmission
}

char uart_recvbyte()
{
	char c;
	while(!GET_BIT(UCSRA, 7));	//wait for receiving complete
	c=UDR;						//save received byte into buffer
	//CLR_BIT(UCSRA, 7);				//clear receive flag
	return c;
}