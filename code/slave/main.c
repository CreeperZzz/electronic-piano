#include "avr.h"
#include "uart.h"
#include <avr/interrupt.h>

unsigned int note_wait[] = {0, 382, 361, 341, 321, 303, 286, 270, 255, 241, 227, 215, 202,
	191, 180, 170, 161, 152, 143, 135, 128, 120, 114, 107, 101,
96, 90, 85, 80, 76, 72, 68, 64, 60, 57, 54, 51};

unsigned int note_hz[] = {0, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
	262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988};

void wait10us(unsigned short m)
{
	TCCR0 = 1;
	while (m--) {
		TCNT0 = (unsigned char)(256 - XTAL_FRQ * 0.00001);
		SET_BIT(TIFR, TOV0);
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

void blink(unsigned char count)
{
	int i;
	for(i=0;i<count;i++)
	{
		SET_BIT(PORTB, 4);
		avr_wait(200);
		CLR_BIT(PORTB, 4);
		avr_wait(200);
	}
	
}

unsigned char playingnote = 0;

ISR(USART_RXC_vect)
{
	playingnote = UDR;
}

int main(void)
{
	SET_BIT(DDRB, 3);
	SET_BIT(DDRB, 4);
	uart_init();
	sei();
	//char c = 0;
    while(1)
    {
		if(playingnote == 0)
			CLR_BIT(PORTB, 3);
		else
		{
			SET_BIT(PORTB, 3);
			wait10us(note_wait[playingnote]);
			CLR_BIT(PORTB, 3);
			wait10us(note_wait[playingnote]);
		}
    }
}