// Brandon Wade
#define F_CPU 8000000UL
#include <avr/io.h> //standard AVR header
#include <util/delay.h> //delay header
#include <avr/interrupt.h>
#define LCD_DPRT PORTD //LCD DATA PORT
#define LCD_DDDR DDRD //LCD DATA DDR
#define LCD_DPIN PIND //LCD DATA PIN
#define LCD_CPRT PORTB //LCD COMMANDS PORT
#define LCD_CDDR DDRB //LCD COMMANDS DDR
#define LCD_CPIN PINB //LCD COMMANDS PIN
#define LCD_RS 0 //LCD RS
#define LCD_RW 1 //LCD RW
#define LCD_EN 2 //LCD EN

volatile uint8_t ADCvalue; // Global variable, set to volatile if used withIS

void lcdCommanda (unsigned char cmnd)
{
	LCD_DPRT = cmnd; //send cmnd to data port
	LCD_CPRT &= ~(1<<LCD_RS); //RS = 0 for command
	LCD_CPRT &= ~(1<<LCD_RW); //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN); //EN = 1 for H-to-L pulse
	_delay_us(1); //wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN); //EN = 0 for H-to_L pulse
	_delay_us(100); //wait to make enable wide
}

void lcdData(unsigned char data)
{
	LCD_DPRT = data; //send data to data port
	LCD_CPRT |= (1<<LCD_RS); //RS = 1 for data
	LCD_CPRT &= ~(1<<LCD_RW); //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN); //EN = 1 for H-to-L pulse
	_delay_us(1); //wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN); //EN = 0 for H-to_L pulse
	_delay_us(100); //wait to make enable wide
}

void lcd_init()
{
	LCD_DDDR = 0xFF;
	LCD_CDDR = 0xFF;
	LCD_CPRT &=~(1<<LCD_EN); //LCD_EN = 0
	_delay_us(2000); //wait for init
	lcdCommanda(0x38); //inti. LCD 2 line, 5x7
	lcdCommanda(0x0E); //display on, cursor on
	lcdCommanda(0x01); //clear LCD
	_delay_us(2000); //wait
	lcdCommanda(0x06); //shift cursor right
}

void lcd_gotoxy(unsigned char x, unsigned char y)
{
	unsigned char firstCharAdr[] = {0x80, 0xC0};
	lcdCommanda(firstCharAdr[y-1] + x -1);
	_delay_us(100);
}

void lcd_print(char * str)
{
	unsigned char i = 0;
	while (str[i]!=0)
	{
		lcdData(str[i]);
		i++;
	}
}

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

int main(void)
{
	float temp; // used to get temperate as float
	char* s = "   . F"; // string to be printed
	
	adc_init (); // initialize adc
	sei(); // enable interrupts
	lcd_init(); // initialize lcd
	while(1)
	{
		temp = ADCvalue * (1.1 / 1024) / .01; // adc conversion
		temp = temp * 1.8 + 32; // convert from C to F
		s[1] = temp / 10 + '0'; // tens place
		s[2] = (int)temp % 10 + '0'; // ones place
		s[4] = (int)(temp * 10) % 10 + '0'; // tenths place
		lcd_gotoxy(1,1); // set cursor position to beginning 
		lcd_print("Temp is:"); // print label
		lcd_gotoxy(1,2); // set cursor to middle of lcd
		lcd_print(s); // print string
		_delay_ms(500); // update every half second
	}

	return 0;
}