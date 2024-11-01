
GLOBAL syscall
GLOBAL invalidOp
section .text

syscall:
    int 80h
    ret

invalidOp:
    mov cs, eax
    ret

section .data
    num1 dq 2       ; Dividendo (ejemplo)
    num2 dq 2           ; Divisor (ejemplo)
section .bss
