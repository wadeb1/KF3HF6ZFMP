/*
 * DA2T1C.c
 *
 * Created: 3/7/2016 10:47:19 AM
 * Author : Brandon
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRC |= 0x01; // PORTC configured to output mode
	DDRB |= 0xFF;
	PORTC = 0x00; // PORTC initialized to 0
	PORTB = 0x00; // PORTB initialized to 0
	
    while (1) 
    {	
		TCNT1H = 0x85;
		TCNT1L = 0xEE; // set count
		TCCR1A = 0x00; // normal mode
		TCCR1B = 0x03; // set pre scalar
		
		while((TIFR1 & 0x01) == 0); // wait for flag
		
		TCCR1A = 0x00;
		TIFR1 |= 1<<TOV1; // reset flag
		
		PORTC ^= 0x01; // toggle PC0
		
		if(PORTC & 0x01)
			PORTB++; // increment count
    }
}