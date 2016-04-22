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
		_delay_us(1);
	}
}

int main()
{
	adc_init(); // initialize adc
	sei(); // enable global interrupts

	//PORTD pins as input
	DDRD = 0x00;
	//Enable internal pull ups
	PORTD = 0xFF;
	//Set PORTB1 pin as output
	DDRB = 0xFF;
	//TOP = ICR1;
	//output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM
	//ICR1 = 20000 defines 50Hz pwm
	ICR1 = 20000;
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10); //TCCR1A = 0x82
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
	while(1)
	{
		OCR1A=ADCvalue * 21 + 100; // empirically derived values to change duty cycle
	}

}
