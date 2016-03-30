// Brandon Wade CPE 301 DA 2

#include <avr/io.h>
#include <stdint.h> // needed for uint8_t
#include <avr/interrupt.h>

#define FOSC 8000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD -1

volatile char ReceivedChar;
volatile uint8_t ADCvalue; // Global variable, set to volatile if used withISR

void usart_init (void)
{
	UCSR0B = (1<<TXEN0)|(1<<RXEN0); // enable USART transmitter
	UCSR0C = ((1<<UCSZ01)|(1<<UCSZ00)|(0<<UMSEL00)); // character size is 8 bits, asynchronous usart
	UBRR0H = 0x0;
	UBRR0L = 51; // for baud of 9600
}

void adc_init (void)
{
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // 64 prescale for 8Mhz
	ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB = 0; // 0 for free running mode
	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADIE); // Enable Interrupts
	ADCSRA |= (1 << ADSC); // Start the ADC conversion
}

void timer_init (void)
{
	TCNT1H = 0xC2;
	TCNT1L = 0xF6; // set count
	TCCR1A = 0x00; // normal mode
	TCCR1B = 0x04; // set pre scalar
	TIMSK1 = 0x01; // enable timer interrupt
}

void usart_send (unsigned char ch)
{
	while(! (UCSR0A & (1<<UDRE0))); // wait till the transmit buffer is ready to receive data
	UDR0 = ch; // send character to output buffer
}

ISR(ADC_vect)
{
	ADCvalue = ADCH; // only need to read the high value for 8 bit
}

ISR(TIMER1_OVF_vect)
{
	//while(!(UCSR0A & (1<<RXC0)));
	char c;
	c = ADCvalue; // get temp
	usart_send(c / 10 + '0'); // print 10s place
	usart_send(c % 10 + '0'); // print 1s place
	usart_send('F'); // print unit 
	usart_send('\n'); // end line
}

int main(void)
{
	timer_init(); // initialize timer
	usart_init(); // initialize usart
	adc_init(); // initialize adc
	sei(); // enable global interrupts
	while(1) // loop forever
	{
	}
	return 0;
}