#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<linux/kernel.h>
int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        char *ptr;
        int num = strtoul(argv[1], &ptr, 10);
        printf(" argv0 =%s argv1 = %s %d\n", argv[0],argv[1], num);
        for(int i = 0; i < 2; ++i)
        {
            fork();
            if(num == 0)
            {
                printf("%d\n",i);
            }
            else
            {
                printf("%d",i);
            }
        }
    }
    else
    {
        printf("invalid argc %d", argc);
    }
    return 0;
}
