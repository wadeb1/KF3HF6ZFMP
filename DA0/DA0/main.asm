;
; DA0.asm
;
; Created: 2/15/2016 4:15:51 PM
; Author : Brandon
;

		LDI		R16, 59		; load number 1 into R16
		LDI		R17, 59		; load number 2 into R17
		LDI		R18, 59		; load number 3 into R18
		LDI		R19, 59		; load number 4 into R19
		LDI		R20, 59		; load number 5	into R20

		ADD		R16, R17	; R16 = R16 + R17, no chance of unsgined overflow
		ADD		R16, R18	; R16 = R16 + R18, no chance of unsgined overflow
		ADD		R16, R19	; R16 = R16 + R19, no chance of unsgined overflow
		ADD		R16, R20	; R16 = R16 + R20, no chance of unsgined overflow

		BRCS	OF			; check carry bit for unsigned overflow, branch to OF if true

		LDI		R21, 0		; R21 gets 0
		OUT		PORTB, R21	; PORTB.2 gets 0

		JMP		DONE		; jump to end

OF:

		LDI		R21, 2		; R21 gets 2
		OUT		PORTB, R21	; PORTB.2 gets 1

DONE:

		JMP		DONE		; program done
