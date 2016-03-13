/*
 * DA2T1C.c
 *
 * Created: 3/7/2016 10:47:19 AM
 * Author : Brandon
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int i = 0;
int j = 0; // helpers to keep track of 5th and 10th pulses

ISR(TIMER1_OVF_vect)
{	
	TIFR1 |= 1<<TOV1; // reset flag
	
	PORTC ^= 0x01; // toggle PC0
		
	if(PORTC & 0x01)
	{
		PORTB++; // inc count
		if(++i == 5)
		{
			PORTC ^= 0x20; // toggle on 5th
			i = 0;
				
			if(++j == 2)
			{
				PORTC ^= 0x40; // toggle on 10th
				j = 0;
			}
		}
	}	
		
	TCNT1H = 0x85;
	TCNT1L = 0xEE; // set count
}

int main(void)
{
    DDRC |= 0x61; // PORTC configured to output mode
	DDRB |= 0xFF;
	PORTC = 0x00; // PORTC initialized to 0
	PORTB = 0x00; // PORTB initialized to 0
	
	TCNT1H = 0x85;
	TCNT1L = 0xEE; // set count
	TCCR1A = 0x00; // normal mode
	TCCR1B = 0x03; // set pre scalar
	
	TIMSK1 = 0x01; // enable timer interrupt
	sei(); // enable global interrupts
	
    while (1)  // wait for interrupts, loop forever
    {	
    }
}