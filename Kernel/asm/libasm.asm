GLOBAL cpuVendor
GLOBAL getHours
GLOBAL getMins
GLOBAL getSec
GLOBAL getKey
GLOBAL saveCPURegisters
GLOBAL getCPURegisters
GLOBAL outb
GLOBAL inb
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
    mov rbp, rsp

    in al, 60h
    test al, 80h ;si el bit 7 esta activo significa que la tecla es liberada
    jnz .tecla_liberada

    mov ah, 1 ; "flag" de si esta presionada o liberada
    jmp .fin

.tecla_liberada:
    and al, 7Fh  ; limpiar el bit 7 para obtener el scancode real
    mov ah, 0

.fin:
    mov rsp, rbp 
    pop rbp
    ret
outb:
	push rbp
	mov rbp, rsp
	mov rdx, rdi
	mov rax, rsi
	out dx, al
	mov rsp, rbp
	pop rbp
	ret
inb:
	push rbp
	mov rbp, rsp
	mov rdx, rdi
	in al, dx
	mov rsp, rbp
	pop rbp
	ret

section .bss
	regBuffer resq 16