
GLOBAL syscall
GLOBAL invalidOp
section .text

syscall:

    push rbp
    mov rbp, rsp

    lea rbx, [rbp+16]  

    int 80h
    
    mov rsp, rbp
    pop rbp

    ret

invalidOp:
    mov cs, eax
    ret

section .data
    num1 dq 2       ; Dividendo (ejemplo)
    num2 dq 2           ; Divisor (ejemplo)
section .bss
