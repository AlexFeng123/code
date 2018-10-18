	.file	"example1.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl function
	.type	 function,@function
function:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	movl 8(%ebp),%eax
	addl 12(%ebp),%eax
	movl 16(%ebp),%edx
	addl %eax,%edx
	movl %edx,-20(%ebp)
	movl -20(%ebp),%eax
	jmp .L1
	.align 4
.L1:
	leave
	ret
.Lfe1:
	.size	 function,.Lfe1-function
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	pushl $3
	pushl $2
	pushl $1
	call function
	addl $12,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
.L2:
	leave
	ret
.Lfe2:
	.size	 main,.Lfe2-main
	.ident	"GCC: (GNU) 2.7.2.3"
