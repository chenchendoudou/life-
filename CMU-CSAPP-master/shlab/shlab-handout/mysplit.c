/*
 * mysplit.c - Another handy routine for testing your tiny shell
 *
 * usage: mysplit <n>
 * Fork a child that spins for <n> seconds in 1-second chunks.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
/**
 * @brief 主函数        
 * 
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return int 0
 * @details 创建一个子进程，子进程循环 n 秒，父进程等待子进程结束       
 * 
 * */
int main(int argc, char **argv)
{
    int i, secs;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        exit(0);
    }
    secs = atoi(argv[1]);//将命令行参数转换为整数

    if (fork() == 0 )// 创建子进程
    { /* child */
        for (i = 0; i < secs; i++)//循环secs次
            sleep(1);//每次循环1秒
        exit(0);
    }

    /* parent waits for child to terminate */
    wait(NULL);//等待子进程结束

    exit(0);
}
