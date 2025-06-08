GLOBAL lock_acquire
GLOBAL lock_release

_xadd:
  mov rax, rdi
  lock xadd [rsi], eax
  ret

_xchg:
  mov rax, rsi
  xchg [rdi], eax
  ret

lock_acquire:
  mov al, 0
.retry:
  xchg [rdi], al
  test al, al
  jz .retry
  ret

lock_release:
  mov byte [rdi], 1
  ret