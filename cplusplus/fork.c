#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
int main()
{
    for(int i = 0; ; ++i)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            FILE *f =  NULL;
            char cmd[128] = {0}; 
            printf("it's child %d process %d\n",i,  getpid());
            snprintf(cmd, sizeof(cmd) - 1, "pmap -X %d|tail -n 1|awk -F' ' '{print $3}'",  getpid());
            f = popen(cmd, "r");
            if(f == NULL)
            {
                printf("popen failed %s with cmd %s\n", strerror(errno), cmd);
                continue;
            }
            while(fgets(cmd,128, f) != NULL)
            {
                printf(" resp : %s \n", cmd);
            }
            pclose(f);
        }
        else if(pid > 0)
        {
            /*printf("it's parent process %d, %dth child porcess %d\n", i,  getpid(), pid);*/
        }
        else
        {
            printf(" fork process %d failed %s \n", i, strerror(errno));
        }
    }
    return 0;
}
