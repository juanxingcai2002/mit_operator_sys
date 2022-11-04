#include "kernel/types.h"
#include "user/user.h"



void pipeline(int rp) // rp是上一个进程的管道的读端
{
    int p,n; // p：存放读的数字
    if(0 == read(rp,&p,sizeof(int)))// 0 说明从那个管道中已经没有数据可以读了
    {
        close(rp);
        exit(0);
    }


    printf("prime %d\n",p);//每个子进程打印上一个进程传过来的第一个数

    // 打印完之后，过滤一遍数据之后，在传递给它的子进程
    int ppl[2];
    pipe(ppl);

    if(fork() == 0)
    {
        close(ppl[1]);//子进程关闭写端
        pipeline(ppl[0]);//递归创建下一个过滤器，将它的读端口传递给它的子进程
    }
    else
    {
        close(ppl[0]);
        // 从上一个过滤器中把数晒选一遍，然后写给它的子进程
        for(;;)
        {
            if(0 == read(rp,&n,sizeof(int)))
            {
                close(rp);// 关闭读端，是它的父为了和它通信创建的管道的读端
                break; //  读完了就退出
            }
            if(n % p != 0) // 把那些晒不掉的就传过去给下一个过滤器
            {
                write(ppl[1],&n,sizeof(int));
            }
        }
        close(ppl[1]);// break for出来说明读完了。那么关闭写端
        wait((void*)0);
        exit(0);
    }

}

int main(int argc ,char*argv[])
{
    int p[2];
    pipe(p);


    if(fork() == 0)
    {
        close(p[1]);//子进程只读不写,文件描述符也是一种资源，为了节约资源，不用的就应关闭释放它
        pipeline(p[0]);
        exit(0);
    }
    else // 主进程进行的任务：生成2~35的素数，写传递给子进程
    {
        close(p[0]); // 关闭读端
        int i ;
        for(i = 2;i <= 35;i++)
        {
            write(p[1],&i,sizeof(int));
        }
        close(p[1]);//写完数据之和也应该及时关闭写段
        wait((void*)0);
        exit(0);
    }
}