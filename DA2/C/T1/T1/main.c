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
	PORTC = 0x00; // PORTC initialized to 0
	
    while (1) 
    {
		TCNT1H = 0x85; 
		TCNT1L = 0xEE; // Set Count
		TCCR1A = 0x00; // Normal Mode
		TCCR1B = 0x03; // Set Pre Scalar
		
		while((TIFR1 & 0x01) == 0); // Wait for flag
		
		TCCR1A = 0x00;
		TIFR1 |= 1<<TOV1; // Clear flag
		
		PORTC ^= 0x01; // toggle PC0
    }
}