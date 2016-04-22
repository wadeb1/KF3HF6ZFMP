// Brandon Wade
#define F_CPU 8000000UL //XTAL = 8MHZ
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define ENABLE 0

volatile uint8_t ADCvalue; // Global variable, set to volatile if used withIS

void adc_init (void)
{
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0) | (1 <<REFS1); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // 64 prescale for 8Mhz
	ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB = 0; // 0 for free running mode
	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADIE); // Enable Interrupts
	ADCSRA |= (1 << ADSC); // Start the ADC conversion
}

ISR(ADC_vect)
{
	ADCvalue = ADCH; // only need to -read the high value for 8 bit
}

void timer_init (void)
{
	TCCR0A = 1<<COM0A1|1<<COM0A0|1<<WGM00; // PWM mode, set/reset on OCRxA
	TCCR0B = 0<<WGM02 |0<<CS02|0<<CS01|1<<CS00;   // PWM mode with 1 pre scalar
}

int main()
{
	float f;
	float i;
	
	timer_init(); // initialize timer
	adc_init(); // initialize adc
	sei(); // enable global interrupts
	
	DDRD = 0xFF; //make PORTB output pin
	DDRC = 0x00;
	while(1)
	{
		
		f = (float)ADCvalue * (1.1 / 1024) * .01;
		i = f * 1000; // conversion to change duty cycle
		OCR0A = i * 210; // update duty cycle over time
	}
	return 0 ;
}
