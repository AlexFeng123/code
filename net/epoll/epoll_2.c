#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/wait.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<arpa/inet.h>

#define MAX_PROCESS 5
#define SEND_BUFF_SIZE 1024
#define MAX_EPOLL_EVENTS 100
#define EPOLL_EVENTS_MASK1  (EPOLLIN|EOPLLET)
#define EPOLL_EVENTS_MASK2  (EPOLLIN)
#define EPOLL_EVENTS_MASK3  (EPOLLIN|EOPLLET|EPOLLONESHOT)


#define exit_if(r, ...)\
    if(r){  \
        printf(__VA_ARGS__);\
        printf("pid=%d|%s:%d error no:%d error msg %s\n",getpid(),__FILE__,__LINE__,errno,strerror(errno));\
        exit(1); \
    }

#define debug(fmt, ...)\
    do{ \
      printf("pid=%d|%s:%d|"fmt,getpid(),__FILE__,__LINE__, ##__VA_ARGS__);\
    }while(0) 


static int sock_init(int port)
{
    struct sockaddr_in server_addr;
    int ret = 0;
    int  fd = socket(PF_INET, SOCK_STREAM,0);
    exit_if(fd < 0, "socket failed");
    server_addr.sin_family  = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    ret = bind(fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    exit_if(ret < 0, "bind 0.0.0.0:%d failed",port);
    return fd;
}

static int set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL,0);
    exit_if(flags < 0, "fcntl failed");
    int ret = fcntl(fd,F_SETFL, flags|O_NONBLOCK);
    exit_if(ret < 0, "fcnlt failed");
    return ret;
}

void update_events(int efd, int fd, int events, int op)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;
    debug("%s fd %d events read %d write %d\n", (op == EPOLL_CTL_MOD?"mod":"add"), fd, ev.events & EPOLLIN, ev.events & EPOLLOUT);
    int ret = epoll_ctl(efd,op,fd,&ev);
    exit_if(ret < 0, "epoll_ctl failed");
}

void handle_accept(int efd, int fd)
{
    struct sockaddr_in remote_addr;
    socklen_t  addr_len = sizeof(remote_addr);
    int infd = accept(fd,(struct sockaddr*)&remote_addr,&addr_len);
    exit_if(infd < 0, "accept failed" );
    struct sockaddr_in peer;
    int ret = getpeername(infd, (struct sockaddr*)&peer,&addr_len);
    exit_if(ret < 0, "getpeername failed");
    debug("accept connnection from %s\n", inet_ntoa(remote_addr.sin_addr));
    set_nonblock(infd);
    update_events(efd,infd,EPOLL_EVENTS_MASK2,EPOLL_CTL_ADD);
}
void loop_once(int efd, int sfd, int waitms)
{
    while(1)
    {
        struct epoll_event active_evts[100];
        int num = epoll_wait(efd, active_evts, MAX_EPOLL_EVENTS, waitms);
        for( int i = 0; i < num; ++i)
        {
            int fd  = active_evts[i].data.fd;
            int events = active_evts[i].events;
            if(fd == -1)
            {
                debug("socket was closed in this iteration");
                continue; 
            }
            if((events & EPOLLERR) | (events & EPOLLHUP))
            {
                debug("epoll wait error");
                close(fd);
                continue;
            }
            else if( fd == sfd)
            {
                handle_accept(efd,fd);   
            }
            else if(events & EPOLLIN)
            {
            }
            else if(events & EPOLLOUT)
            {
            }
        }
    }
}
int main()
{
    int fd;
    int con_fd;
    int pid;
    char send_buff[SEND_BUFF_SIZE];
    int ret = 0;
    
    fd = sock_init(1234);
    exit_if(fd < 0, "sock init failed");
    ret = listen(fd,1024);
    exit_if(ret < 0, "listen failed");
    set_nonblock(fd);

    int efd = epoll_create(1);
    exit_if(efd < 0, "epoll create failed");
    update_events(efd,fd,EPOLL_EVENTS_MASK2 ,EPOLL_CTL_ADD);
    int i;
    for(i = 0; i < MAX_PROCESS; ++i)
    {
        pid = fork();
        if(pid == 0 )
        {
            loop_once(efd,fd,-1);
        }
    }
    wait(NULL);
    close(fd);
    return 0;
}
