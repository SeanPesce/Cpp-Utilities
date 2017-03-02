; Made by Sean P


_DATA SEGMENT

PUBLIC JUMPBACK_ADDRESS

JUMPBACK_ADDRESS DQ 1111111111111111h

_DATA ENDS



_CODE SEGMENT

asmCodeExample proc
	pop rax

	;  User code would normally go here
	
	push rax
	mov rax, QWORD PTR [JUMPBACK_ADDRESS]
	jmp rax
asmCodeExample endp

_CODE ENDS

END
