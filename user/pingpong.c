#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define BUFSIZE 1024


int main()
{
    char buff[BUFSIZE];// 存放从管道中获得的消息 
    int fd1[2]; // pipe1 用于父进程发消息，子进程接受消息
    int fd2[2];// pipe1  用于子进程发消息，父进程接受消息
    pipe(fd1);
    pipe(fd2);

    int pid = fork();
    if(pid < 0)
    {
        exit(1);
    }
    else if(pid == 0) // 子进程 ：接受父进程的消息，打印自己的pid，向父进程中写信息
    {
        close(fd1[1]);
        read(fd1[0],buff,4);
        printf("%d: received %s\n",getpid(),buff);
        close(fd1[0]);

        close(fd2[0]);
        write(fd2[1],"pong",4);
        close(fd2[1]);
        exit(0);
    }
    else
    {
        close(fd1[0]);
        write(fd1[1],"ping",4);
        close(fd1[1]);

        close(fd2[1]);
        read(fd2[0],buff,sizeof(buff));
        printf("%d: received %s\n",getpid(),buff);
        close(fd2[0]);
        exit(0);
    }

}