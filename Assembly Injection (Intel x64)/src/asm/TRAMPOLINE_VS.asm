

_CODE SEGMENT

TRAMPOLINE_FUNC proc
	pop rax
	jmp QWORD PTR [TRAMPOLINE_JMP_TO_LBL]
	push rax
	mov rax,1111111111111111h
	jmp rax  
	TRAMPOLINE_JMP_TO_LBL:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
TRAMPOLINE_FUNC endp



_CODE ENDS

END
