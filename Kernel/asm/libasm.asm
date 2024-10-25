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
    mov [rdi], r15
    mov [rdi+8], r14
    mov [rdi+16], r13
    mov [rdi+24], r12
    mov [rdi+32], r11
    mov [rdi+40], r10
    mov [rdi+48], r9
    mov [rdi+56], r8
    mov [rdi+64], rsi
    mov [rdi+72], rdi
    mov [rdi+80], rbp
    mov [rdi+88], rdx
    mov [rdi+96], rcx
    mov [rdi+104], rbx
    mov [rdi+112], rax

	ret
