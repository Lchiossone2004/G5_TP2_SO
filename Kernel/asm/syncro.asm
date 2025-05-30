GLOBAL lock_acquire
GLOBAL lock_release

lock_acquire:
    mov al, 1
.try:
    xchg [rdi], al
    cmp al, 1
    jne .try
    ret

lock_release:
    mov byte [rdi], 1
    ret
