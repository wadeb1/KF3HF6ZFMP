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
	ADCvalue = ADCH; // only need to read the high value for 8 bit
}

void timer_init (void)
{
	OCR0A = 125; // 10% duty start
	TCCR0A = 1<<COM0A1|1<<COM0A0|1<<WGM00; // PWM mode, set/reset on OCRxA
	TCCR0B = 0<<WGM02 |0<<CS02|0<<CS01|1<<CS00;   // PWM mode with 1 pre scalar
}

void delay(int j)
{
	for (int i = 0; i <j; i++)
	{
		_delay_us(1); // generates variable timer based off j passed
	}
}

int main()
{
	adc_init(); // initialize adc
	sei(); // enable global interrupts
	
	DDRC = 0x00;
	DDRB = 0xFF; // output ports
	while(1)
	{
		PORTB = 0X06;
		delay(ADCvalue * 169 + 20000); // empirically derived delay
		PORTB = 0x0C;
		delay(ADCvalue * 169 + 20000); // empirically derived delay
		PORTB = 0x09;
		delay(ADCvalue * 169 + 20000); // empirically derived delay
		PORTB = 0x03;
		delay(ADCvalue * 169 + 20000); // empirically derived delay
	}

	return 0 ;
}
