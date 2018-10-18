#include <asm/unistd.h>

int errno;

_syscall3(int, write, int, fd, char *, data, int, len);

_syscall1(int, exit, int, status);

_start()
{
	    write(0, "Hello world!\n", 13);
	    exit(0);
}
