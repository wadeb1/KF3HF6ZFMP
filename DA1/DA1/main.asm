;
; DA1.asm
;
; Created: 2/25/2016 3:40:18 PM
; Author : Brandon
;

	LDI		XL, LOW(RAMEND/2)	 
	LDI		XH, HIGH(RAMEND/2)	; X gets RAMEND/2

	MOVW	YL, XL				; Y gets X

	LDI		R16, 25				; loop 25 times

storeLoop:

	ST		Y, YL				; store LOW(RAMEND/2) + n at Y
	INC		YL					; increment memory location and number
	DEC		R16					; decrement counter
	CPI		R16, 0				; check if counter is at zero
	BRNE	storeLoop			; loop 25 times

	LDI		R16, 25				; loop 25 times
	LDI		R18, 0				; used for carry
	LDI		R20, 0
	LDI		R21, 0
	LDI		R22, 0
	LDI		R23, 0				; initialize sum registers

mainLoop:

	MOV		R17, XL				; R17 gets value at RAMEND/2 + n

check7Loop:

	SUBI	R17, 7				; subtract seven from number
	BRCS	false7				; jump if not divide by 7
	CPI		R17, 0				; check for remainder
	BRNE	check7Loop			; jump if continue

	ADD		R20, XL
	ADC		R21, R18			; add current number to sum

false7:

	MOV		R17, XL				; R17 gets value at RAMEND/2 + n

check3Loop:

	SUBI	R17, 3				; subtract three from number
	BRCS	false3				; jump if not divide by 3
	CPI		R17, 0				; check for remainder
	BRNE	check3Loop			; jump if continue

	ADD		R22, XL
	ADC		R23, R18			; add current number to sum

false3:

	INC		XL
	DEC		R16					; decrement counter
	CPI		R16, 0				; check if counter is at zero
	BRNE	mainLoop			; loop 25 times

	CPI		R21, 0				; check if sum is greater than 8 bits
	BRNE	over8bits			; jump to set bit
	CPI		R23, 0				; check if sum is greater than 8 bits
	BRNE	over8bits			; jump to set bit
	RJMP done					; done

over8bits:

	SBR		R18, 8				; set third bit in R18
	MOV		R7, R18				; move R18 to R7

done:
	RJMP done