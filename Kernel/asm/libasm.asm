GLOBAL cpuVendor
GLOBAL getHours
GLOBAL getMins
GLOBAL getSec
GLOBAL getKey
GLOBAL getCPURegisters
section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getHours:
	push rbp
	mov rbp, rsp

	mov al, 0x04
	out 70h, al 
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

getMins:
	push rbp
	mov rbp, rsp

	mov al, 0x02
	out 70h, al 
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

getSec:
	push rbp
	mov rbp, rsp

	mov al, 0x00
	out 70h, al 
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

getKey:
	push rbp
	mov rbp,rsp

 	in al, 60h
	test al, 80h
	jnz .nada
	jmp .fin
.nada
	mov al, 0
.fin
	mov rsp, rbp 
	pop rbp
	ret

getCPURegisters:
    mov [regBuffer], qword rax
	mov rax, regBuffer
	add rax, 8
    mov [rax], qword rbx
	add rax,8
    mov [rax], qword rcx
	add rax, 8
    mov [rax], qword rdx
	add rax, 8
	mov [rax], qword rsi
	add rax, 8
    mov [rax], qword rdi
	add rax, 8
	mov [rax], qword rsp
	add rax, 8
    mov [rax], qword rbp
	mov rax, regBuffer
	ret

section .bss
	regBuffer resq 10