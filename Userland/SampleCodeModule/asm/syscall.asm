
GLOBAL syscall
GLOBAL div0
section .text

syscall:
    int 80h
    ret
section .data

section .bss
