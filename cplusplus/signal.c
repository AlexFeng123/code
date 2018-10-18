#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<execinfo.h>
#define  BT_MAX_TRACE 100
void dump_bt()
{
    void *buff[BT_MAX_TRACE];
    size_t size;
    char **strings;
    size_t i;


    size = backtrace(buff,BT_MAX_TRACE);
    strings = backtrace_symbols(buff,size);

    printf("stack: \n");

    for(int i = 0; i < size ; ++i)
    {
        printf("%s\n",strings[i]);
    }

    free(strings);
}
void handle_sigchld()
{
    dump_bt();
    printf("handle chld\n");
}
void handle_sig()
{
    dump_bt();
    printf("handle sig\n");
}
int main()
{
    int pid = 0;
    signal(SIGCHLD, handle_sigchld);
    signal(SIGUSR1, handle_sig);
    pid = fork();
    if(pid == 0)
    {
        printf("process %d ", pid);
        sleep(10);
        exit(0);
    }
    if(pid > 0)
    {
        printf("wait pid %d", pid);
        kill(pid,SIGUSR1);
        raise(SIGUSR1);
        wait(&pid);
    }
    return 0;
}
