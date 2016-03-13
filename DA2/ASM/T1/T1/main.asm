;
; DA2.asm
; Task 1 ASM
; Created: 3/6/2016 11:36:31 AM
; Author : Brandon
;

	LDI		R16, 0x01
	OUT		DDRC, R16		; PortC configured for output
	LDI		R16, 0x00		; will hold PortC signals
	OUT		PORTC, R16		; initialize PortC to 0
	LDI		R18, 0x01		; will be used to toggle PC0

BEGIN:

	RCALL	DELAY			; wait for .25 seconds

	EOR		R16, R18		; toggle signal
	OUT		PORTC, R16		; output

	RJMP	BEGIN			; loop forever

DELAY:

	LDI		R23, 0x85
	STS		TCNT1H, R23
	LDI		R23, 0xEE
	STS		TCNT1L, R23		; set initial count of timer

	LDI		R23, 0x00		
	STS		TCCR1A, R23		; normal mode

	LDI		R23, 0x03		
	STS		TCCR1B, R23     ; start timer with 1024 ps

AGAIN:

	IN		R23, TIFR1		; check for overflow
	SBRS	R23, TOV1		; if overflow, exit loop
	RJMP	AGAIN			; loop till overflow

	LDI		R23, 0x00		
	STS		TCCR1B, R23		; turn off timer

	LDI		R23, 0x01
	OUT		TIFR1, R23		; clear overflow flag
	RET