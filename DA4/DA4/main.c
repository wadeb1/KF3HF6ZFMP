/*
 * DA4.c
 *
 * Created: 4/9/2016 8:31:48 PM
 * Author : Brandon
 */ 

#define F_CPU 8000000UL //8 MHz
#include <avr/io.h>
#include <util/delay.h>

int main ()
{
	unsigned int i, j, k;	// used to update duty cycles
	
	DDRD = 0xFF; 
	DDRB = 0xFF;			// output mode
	
	OCR0A = 25; // 10% duty start
	TCCR0A = 1<<COM0A1|1<<COM0A0|1<<WGM00; // PWM mode, set/reset on OCRxA
	TCCR0B = 0<<WGM02 |0<<CS02|0<<CS01|1<<CS00;   // PWM mode with 1 pre scalar
	
	OCR1A = 25; // 10% duty start
	TCCR1A = 1<<COM1A1|1<<COM1A0|1<<WGM10|0<<WGM11; // PWM mode, set/reset on OCRxA
	TCCR1B = 0<<WGM13 |0<<CS02|0<<CS01|1<<CS00;   // PWM mode with 1 pre scalar
		
	OCR2A = 25; // 10% duty start
	TCCR2A = 1<<COM2A1|1<<COM2A0|1<<WGM20; // PWM mode, set/reset on OCRxA
	TCCR2B = 0<<WGM22 |0<<CS02|0<<CS01|1<<CS00;   // PWM mode with 1 pre scalar
	
	while (1)
	{
		for(i = 25; i < 230; i+= 50)
		{
			OCR0A = i; // update OCRxA to approach 100% duty cycle
			
			for(j = 25; j < 230; j+= 50)
			{
				OCR1A = j; // update OCRxA to approach 100% duty cycle
				
				for(k = 25; k < 230; k+= 50)
				{
					OCR2A = k; // update OCRxA to approach 100% duty cycle
					_delay_ms(500); // hold color
				}
			}
		}
	}
	
	return 0;
}

