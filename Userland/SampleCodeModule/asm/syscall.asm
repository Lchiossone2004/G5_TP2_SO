
GLOBAL syscall
GLOBAL div0
section .text

syscall:
    int 80h
    ret

div0:
    lock nop
    mov rax, [num1]     ; Cargamos el dividendo en RAX
    xor rdx, rdx      ; Limpiamos RDX para evitar residuos previos
    mov rbx, [num2]     ; Cargamos el divisor en RBX
    div rbx  
    ret

section .data
    num1 dq 2       ; Dividendo (ejemplo)
    num2 dq 2           ; Divisor (ejemplo)
section .bss
