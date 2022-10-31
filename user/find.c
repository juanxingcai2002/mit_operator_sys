// 找出第一个参数为目录下的，第二个参数的文件
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char*path,char*exper)
{
    char buf[512];//保存的缓冲区
    char*p;
    int fd;
    // 目录和文件的结构
    struct dirent de; // 目录项结构，里面包含一个inode编号和一个文件名 文件名字用name数组，长度为DIRSIZ == 14;
    struct stat st;

    // 要操作文件首先要打开文件，文件描述符是需要我们用open系统调用后才能得到的
    if((fd = open(path,0)) < 0)
    {
       fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    //将打开的文件的信息，写入结构体中
    if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
    // 如果该打开的文件不是目录，说明用法错误，第一参数必须是目录，是查询该目录下的第二个参数为文件的，所有该文件名字的路径
    if(st.type != T_DIR)
    {
        printf("Usage: find: <path> <expression [, exper1, exper2, ...]>\n");
        exit(1);
    }

    // 判断路径的长度是否超过了缓冲区的大小 + 1是为了放尾0，path是路径，dirsiz是准备放的文件名的长度即该path路径下的文件
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
    }
    // 先把路径拷贝
    strcpy(buf,path);
    p = buf + strlen(path);// 此时的buf是当前路径；
    *p++ = '/';//给buf的后面加上/，为了后序的字符串拼接
    // fd所指向的一定是个目录，通过目录结构dirent，读取该目录的每一个目录项目
    while(read(fd,&de,sizeof(de)) == sizeof(de)) // 每次读取一个目录项，读出来的可能是子目录，也有可能是文件
    {
        if(de.inum == 0) // inode编号为0，说明该目录项没文件
        {
            continue;
        }
        memmove(p,de.name,DIRSIZ);// 把当前找到的文件名记录到 p （追加到 buf 后）
        p[DIRSIZ] = 0;// 终止符 0,比如追加的长度是4，但是4的下标其实是没用到的，字符数组的最好一位一定要补0.
        // 通过文件路径得到的文件信息写如到文件结构体st中，如果写入失败（找不到这一项），就读取下一个目录项
        if(stat(buf,&st) < 0)
        {
             printf("ls: cannot stat %s\n", buf);
            continue;
        }
        
        if(st.type == T_DIR && strcmp(p,".") && strcmp(p,".."))
        {
            find(buf,exper);// buf 为文件夹，且不为 . / .. 时才递归 ，去buf的目录下，查找目录项目
        }
        // 说明找到了该文件，打印路径
        else if(strcmp(p,exper) == 0)
        {
             printf("%s\n", buf);
        }        
    }

    close(fd);

}

int main(int argc,char*argv[])
{
    int i;
    if(argc < 3)
    {
        printf("Usage: find: <path> <expression [, exper1, exper2, ...]>\n");
        exit(1);
    }
    for( i = 2;i < argc;i++)
    {
        find(argv[1],argv[i]);// 0为操作的的名字，argv【1】要在哪个目录下，找出所以名字为argv【i】的文件，并打印出它的路径；
    }
    exit(0);
}