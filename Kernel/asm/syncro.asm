GLOBAL lock_acquire
GLOBAL lock_release

lock_acquire:
  mov al, 0
.retry:
  pause
  xchg byte [rdi], al
  cmp al, 1
  jne .retry
  ret

lock_release:
  mov byte [rdi], 1
  ret