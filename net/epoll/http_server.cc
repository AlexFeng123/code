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
#include<string>
#include<map>
using namespace std;
#define MAX_PROCESS 5
#define SEND_BUFF_SIZE 1024
#define MAX_EPOLL_EVENTS 100
#define EPOLL_EVENTS_MASK1  (EPOLLIN|EOPLLET)
#define EPOLL_EVENTS_MASK2  (EPOLLIN)
#define EPOLL_EVENTS_MASK3  (EPOLLIN|EOPLLET|EPOLLONESHOT)

int debug_enable = 0;

#define exit_if(r, ...)\
    if(r){  \
        printf(__VA_ARGS__);\
        printf("pid=%d|%s:%d error no:%d error msg %s\n",getpid(),__FILE__,__LINE__,errno,strerror(errno));\
        exit(1); \
    }

#define debug(fmt, ...)\
    if(debug_enable){ \
    do{ \
      printf("pid=%d|%s:%d|"fmt,getpid(),__FILE__,__LINE__, ##__VA_ARGS__);\
    }while(0); \
    }

struct con
{
    string readed;
    size_t written;
    bool write_enable;
    con(): written(0), write_enable(false) {}
};
map<int, con> cons;
string http_res;

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

void send_res(int efd, int fd)
{
   con &con = cons[fd];
   size_t left = http_res.length()  - con.written;
   int w_len = 0;
   while((w_len = ::write(fd, http_res.data() + con.written, left)) > 0)
   {
       con.written += w_len;
       left += w_len;
       debug("write %d bytes left: %lu\n",w_len, left);
   }
   if(left == 0)
   {
       if(con.write_enable)
       {
           update_events(efd,fd,EPOLL_EVENTS_MASK2,EPOLL_CTL_MOD);
           con.write_enable = false;
       }
       cons.erase(fd);
       return; 
   }
   if(w_len < 0 && 
     (errno == EAGAIN || errno == EWOULDBLOCK))
   {
      if(!con.write_enable) 
      {
          update_events(efd, fd, EPOLLIN|EPOLLOUT, EPOLL_CTL_MOD);
          con.write_enable = true;
      }
      return ;
   
   }
   else if(w_len <= 0)
   {
      debug("write errno for %d: %d %s",fd , errno, strerror(errno));
      close(fd);
      cons.erase(fd);
   }
}

void handle_read(int efd, int fd)
{
    char buf[4096];
    int n = 0; 
    while((n = ::read(fd,  buf, sizeof(buf))) > 0 )
    {
        debug("read %d bytes \n", n);
        string &readed = cons[fd].readed;
        readed.append(buf, n);
        if(readed.length() > 4)
        {
            if(readed.substr(readed.length() - 2, 2) == "\n\n" ||
               readed.substr(readed.length() - 4, 4) == "\r\n\r\n")
            {
                // read http end, send response
                send_res(efd,fd);
            }
        }
    }
    if( n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
    {
        debug("read %d  error %d %s\n",fd,errno,strerror(errno));
        return;
    }
    else if( n < 0)
    {
        debug("read %d  error %d %s\n",fd,errno,strerror(errno));
    }
    close(fd);
    cons.erase(fd);
}

void handle_write(int efd, int fd)
{
    send_res(efd, fd);
}

static bool set_socket(int fd, int option, bool val)
{
   int enable = (val? 1: 0);
   int err = setsockopt(fd, SOL_SOCKET, option, &enable, sizeof(enable));
   return !err;
}

static int sock_init(int port)
{
    struct sockaddr_in server_addr;
    int ret = 0;
    int  fd = socket(PF_INET, SOCK_STREAM,0);
    exit_if(fd < 0, "socket failed");
    server_addr.sin_family  = AF_INET;
    server_addr.sin_addr.s_addr = htonl(2887189910);
    server_addr.sin_port = htons(port);
    ret = set_socket(fd, SO_REUSEPORT,false);
    exit_if(ret < 0, "setsockopt %s:%d failed",inet_ntoa(server_addr.sin_addr), port);
    ret = bind(fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    exit_if(ret < 0, "bind %s:%d failed",inet_ntoa(server_addr.sin_addr), port);
    return fd;
}

void loop_once(int efd, int sfd, int waitms)
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
        if(events & ( EPOLLERR |  EPOLLHUP))
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
            handle_read(efd,fd);
        }
        else if(events & EPOLLOUT)
        {
            handle_write(efd, fd);
        }
        else
        {
            exit_if(1, "unknow event");
        }
    }
}
int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        debug_enable = 1;
    }
    int fd;
    int ret = 0;
    
    signal(SIGPIPE, SIG_IGN);

    http_res = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 1048576\r\n\r\n123456";
    for(int i = 0; i < 1048570; ++i)
    {
        http_res +="\0";
    }
    fd = sock_init(80);
    exit_if(fd < 0, "sock init failed");
    ret = listen(fd,1024);
    exit_if(ret < 0, "listen failed");
    set_nonblock(fd);

    int efd = epoll_create(1);
    exit_if(efd < 0, "epoll create failed");
    update_events(efd,fd,EPOLL_EVENTS_MASK2 ,EPOLL_CTL_ADD);
    for(;;)
    {
        loop_once(efd,fd,5000);
    }
    close(fd);
    return 0;
}
