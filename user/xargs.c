#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc,char*argv[])
{
    // xargs 命令执行的格式应该是：./xargs 命令 + 命令参数（从标准输入中得到），本lab因为是-n 1，因此命令一次执行要使用一个命令参数。
    // 要求从标准输入中一次读取一行，一次读取一个字节，直到遇到换行符。
    char *args[MAXARG]; //定义该命令的命令参数
    int i;                  // i 要单独定义，要标识args命令参数的末尾位置
    for(i = 0;i < argc;i++)
    {
        args[i] = argv[i];
    } 
    // args[0] == xargs,args[1]== 要执行的命令

    char buf[256];// 存放从标准输入中读取的参数；

    for(;;) // 从标准输入中读取命令
    {
        int j = 0;//标识缓冲区的末尾位置
        while((0!=read(0,buf+j,sizeof(char))) && buf[j] != '\n') ++j;// 从标准输入中，每次读取一字节的数据，且没读取到末尾
        // 如果读完了，直接退出while循环，如果读取了，但是是\n 说明本次这行读完了j++ = 1；
        
        if(j == 0) break;//说明没有进入while循环了，说明没有read（）说明读取完了而不只是本行读取完了。
        // 读取完了
        buf[j] = 0;// 字符串要用尾0来标识结束

        // 将标准输入中得到的参数传入命令行参数中
        args[i] = buf;
        args[i+1] = 0;// 标识命令行参数的结束 // 必须写i+1，不能写i++，否则死循环

        // 执行命令
        if(fork() == 0)
        {
            exec(args[1],args+1);// argv【1】是要执行的命令，args+1 即从args【1】开始到尾0，都是算要执行的参数，args【1】是要执行命令的名字，也应该再参数列表中
            printf("exec error\n");
        }
        else
        {
            wait(0);
        }

    }    
     exit(0);

}