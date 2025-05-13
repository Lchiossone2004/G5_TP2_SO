
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq08Handler

GLOBAL _exception0Handler
GLOBAL _exception06Handler
GLOBAL getCPURegisters
GLOBAL saveCPURegisters


EXTERN getStackBase
EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallsManager
EXTERN scheduler
EXTERN getStackBase
SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro saveRegisters 0
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
	mov [rax], qword rbp 
	add rax, 8
    mov [rax], qword rsp
	add rax, 8
	push rdi
	mov rdi, [auxRIP]
	mov [rax], rdi
	pop rdi
	add rax, 8
	mov [rax], qword r8
	add rax, 8
	mov [rax], qword r9 
	add rax, 8
    mov [rax], qword r10
	add rax, 8
	mov [rax], qword r11
	add rax, 8
	mov [rax], qword r12
	add rax, 8
    mov [rax], qword r13
	add rax, 8
	mov [rax], qword r14
	add rax, 8
	mov [rax], qword r15
%endmacro

%macro irqHandlerMaster 1
	pushState
	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro irqHandlerMaster 0
	pushState
	mov rdi, rsp
	call scheduler
	mov rsp,rax

	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


%macro exceptionHandler 1
	push rax
	mov rax, [rsp + 8]
	mov [auxRIP], rax
	pop rax
	saveRegisters
	pushState
	mov rdi, %1
	call exceptionDispatcher
	popState
	call clearRegs
	call getStackBase
	mov [rsp+8*3], rax
	mov rax, userland
	mov [rsp], rax
	iretq
%endmacro

saveCPURegisters:
	saveRegisters
	ret
getCPURegisters:
	mov rax, regBuffer
	ret

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	push rax
	mov rax, [rsp + 8]
	mov [auxRIP], rax
	pop rax
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

_irq08Handler:
	pushState
	call syscallsManager
	popState
	iretq			;no se por que con ret no funciona pero con iretq si

;activa el sti (para poder recibir interrupciones dentro del getChar)


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Wrong Op Code Exception
_exception06Handler:
	exceptionHandler 1

haltcpu:
	cli
	hlt
	ret

clearRegs:
	push rcx
	push rax
	push rdi
	mov rdi, regBuffer
	mov rax, 0
	mov rcx, 0
.ciclo:
	cmp rcx, 17
	je .fin
	mov [rdi], rax
	add rdi, 8
	inc rcx
	jmp .ciclo
.fin:
	pop rdi
	pop rax
	pop rcx
	ret
section .rodata
userland equ 0x400000
SECTION .bss
regBuffer resq 17
auxRIP resq 1