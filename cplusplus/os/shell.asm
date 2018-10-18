.global _start
_start:
    # /bin/sh
    movabs $0x68732f6e69622f,%rbx
    push %rbx
    mov $0x3b, %al
    lea (%rsp),%rdi
    xor %rsi,%rsi
    xor %rdx, %rdx

    syscall

