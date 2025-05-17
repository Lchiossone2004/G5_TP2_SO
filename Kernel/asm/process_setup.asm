

EXTERN entry_point_wrapper

GLOBAL setup_process_stack


; | Argumento                      | Registro |
; | ------------------------------ | -------- |
; | 1º argumento (`fn`)            | `rdi`    |
; | 2º argumento (`argc`)          | `rsi`    |
; | 3º argumento (`argv`)          | `rdx`    |
; | 4º argumento (`new_stack_top`) | `rcx`    |

setup_process_stack:
    ; Guardamos el RSP original
    mov r11, rsp
    
    ; Cambiamos al nuevo stack
    mov rsp, rcx
    mov rbp, rsp

    push 0x00
    push 0x8
    push 0x202
    push entry_point_wrapper

    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax

    mov rax, rsp
    
    ; Restauramos el RSP original
    mov rsp, r11
    
    ret