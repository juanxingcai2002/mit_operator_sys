#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int args,char*argv[])
{
    if(args != 2)
    {
        printf("Usage args error\n");
        exit(1);
    }
    int tmp = atoi(argv[1]);
    sleep(tmp);
    exit(0);
}