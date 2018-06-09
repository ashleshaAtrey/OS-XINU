
.section .text
.global zfunction

zfunction:
	push %ebp 
	mov  %esp, %ebp
	mov  8(%ebp), %eax
	and  $0xFFC01FFF, %eax
	shl  $0x4, %eax
	pop  %ebp
	ret
