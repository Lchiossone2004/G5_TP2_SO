
GLOBAL syscall
GLOBAL div0
section .text

syscall:
    int 80h
    ret

div0:
    mov rax, 10
    mov rdx, 10
    mov rcx, 10
    mov rbx, 10
    div rcx
    ret

section .data
section .bss
