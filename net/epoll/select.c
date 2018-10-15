#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/wait.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX_PROCESS 10
#define SEND_BUFF_SIZE 1024
#define exit_if(r, ...)\
    if(r){  \
        printf(__VA_ARGS__);\
        printf("%s:%d error no:%d error msg %s\n",__FILE__,__LINE__,errno,strerror(errno));\
        exit(1); \
    }

int main()
{
    int fd;
    int con_fd;
    int pid;
    char send_buff[SEND_BUFF_SIZE];
    struct sockaddr_in server_addr;
    int ret = 0;
    fd = socket(PF_INET, SOCK_STREAM,0);
    exit_if(fd < 0, "socket failed");
    server_addr.sin_family  = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(1234);

    ret = bind(fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    exit_if(ret < 0, "bind failed");
    ret = listen(fd,1024);
    exit_if(ret < 0, "listen failed");

    int i;
    for(i = 0; i < MAX_PROCESS; ++i)
    {
        pid = fork();
        if(pid == 0 )
        {
                printf("process %d is accepting !\n", getpid());
                con_fd = accept(fd, (struct sockaddr*)NULL, NULL);
                exit_if(con_fd < 0, "accept failed");
                snprintf(send_buff,sizeof(send_buff), "accept pid is %d", getpid());
                send(con_fd, send_buff, strlen(send_buff)+1,0);
                printf("process %d accept success !\n", getpid());
                close(con_fd);
                exit(0);
        }
    }
    wait(NULL);
    return 0;
}
