#include <iostream>
 
extern "C" int func();
//func 的定义以汇编语言书写
// 未处理字符串字面量可以很有用
asm(R"(
.globl func
.type func, @function
func:
.cfi_startproc
movl $7, %eax
ret
.cfi_endproc
)");

int main()
{
    int n = func();
    // 扩展内联汇编
    asm ("leal (%0,%0,4),%0"
            : "=r" (n)
            : "0" (n));
    std::cout << "7*5 = " << n << std::endl; // 冲入是有意的

    // 标准内联汇编
    asm ("movq $60, %rax\n\t" // Linux 上的退出 syscall 号
            "movq $2,  %rdi\n\t" // 此程序返回 2
            "syscall");
}
