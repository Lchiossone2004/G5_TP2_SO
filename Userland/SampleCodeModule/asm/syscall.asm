
GLOBAL syscall
GLOBAL div0
section .text

syscall:
    int 80h
    ret
section .data

div0:
    mov rax, 30
    mov rdx, 30
    mov rcx, 30
    div rcx
    ret
section .bss
