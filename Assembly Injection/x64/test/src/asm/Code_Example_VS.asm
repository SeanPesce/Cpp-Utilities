; Made by Sean P


_DATA SEGMENT

PUBLIC JUMPBACK_ADDRESS

JUMPBACK_ADDRESS DQ 1111111111111111h

_DATA ENDS



_CODE SEGMENT

asm_code_example proc
	pop rax

	;  User code would normally go here
	
	push rax
	mov rax, QWORD PTR [JUMPBACK_ADDRESS]
	jmp rax
asm_code_example endp

_CODE ENDS

END
