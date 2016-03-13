;
; DA2.asm
; Task 3 ASM
; Created: 3/6/2016 11:36:31 AM
; Author : Brandon
;

.org 0x00
	
	RJMP INIT				

.org 0x20

	RJMP TCNT1_overflow		; interrupt
	
INIT:

	LDI		R16, 0xFF
	OUT		DDRB, R16		; PortB configured for output
	LDI		R16, 0x61
	OUT		DDRC, R16		; PortC configured for output
	LDI		R16, 0x00		; will hold PortC signals
	OUT		PORTB, R16		; initialize PortB to 0
	OUT		PORTC, R16		; initizlize PortC to 0
	LDI		R17, 0x00		; holds current count
	LDI		R18, 0x01		; will be used to toggle PC0
	LDI		R19, 0x20		; will be used to toggle PC5
	LDI		R20, 0x40		; will be used to toggle PC6
	LDI		R21, 0x00		; holds modulo 5
	LDI		R22, 0x00		; holds modulo 10

BEGIN:

	LDI		R23, 0x85
	STS		TCNT1H, R23
	LDI		R23, 0xEE
	STS		TCNT1L, R23		; set initial count of timer

	LDI		R23, 0x00		
	STS		TCCR1A, R23		; normal mode

	LDI		R23, 0x03		
	STS		TCCR1B, R23     ; start timer with 1024 ps
	
	LDI		R23, 0x01
	STS		TIMSK1, R23		; enable timer interrupt
	SEI						; enable interrupts

AGAIN:

	RJMP	AGAIN			; loop forever
	
TCNT1_overflow:

	LDI		R23, 0x01
	OUT		TIFR1, R23
	
	EOR		R16, R18		; toggle signal
	OUT		PORTC, R16		; output

	SBRS	R16, 0		
	RJMP	SKIP_INC		; only increment on rising edge

	INC		R17				; increment count
	OUT		PORTB, R17		; output

	INC		R21				; increment modulo 5
	CPI		R21, 5
	BRNE	SKIP_INC		; only toggle on 5th rising edge

	CLR		R21				; clear modulo 5
	EOR		R16, R19		; toggle on 5th rising edge
	OUT		PORTC, R16		; output

	INC		R22				; increment modulo 10
	CPI		R22, 2
	BRNE	SKIP_INC		; only toggle on 10th rising edge

	CLR		R22				; clear modulo 10
	EOR		R16, R20		; toggle on 10th rising edge
	OUT		PORTC, R16		; output

SKIP_INC:

	LDI		R23, 0x85
	STS		TCNT1H, R23
	LDI		R23, 0xEE
	STS		TCNT1L, R23		; set initial count of timer

	RETI