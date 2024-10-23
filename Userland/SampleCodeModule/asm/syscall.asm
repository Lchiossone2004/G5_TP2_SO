
GLOBAL syscall
section .text

syscall:
    push rdi
    push rsi
    push rdx
    push rcx
    int 80h
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    ret
section .data
section .bss
