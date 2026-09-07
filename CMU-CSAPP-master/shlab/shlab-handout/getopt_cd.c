#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
//写一个样例讲解getopt函数的使用
//接受h选项 打印帮助信息
//接受v选项 打印版本信息
//接受p选项 打印进程ID
//接受其他选项 打印错误信息

//如果输入./a.out -hv 为什么不会打印版本信息
//因为-h选项会优先于-v选项，所以-h选项会先被解析，然后-v选项会被忽略

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "hvp")) != EOF)
    {
        printf("argc = %d\n", argc);
        printf("opt = %c\n", opt);
        switch (opt)
        {
        case 'h':
            printf("Usage: %s [-h] [-v] [-p]\n", argv[0]);
            //exit(0);
            break;
        case 'v':
            printf("Version 1.0\n");
            //exit(0);
            break;
        case 'p':
            printf("Process ID: %d\n", getpid());
            //exit(0);
            break;
        default:
            printf("Unknown option: %c\n", opt);
            break;
        }
    }
    return 0;
}