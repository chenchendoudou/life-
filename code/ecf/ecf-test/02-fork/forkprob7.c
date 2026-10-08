/**
 * @file forkprob7.c
 * @brief fork() 思考题 7：fork 后的 counter 是共享还是独立
 * @details
 *   counter 是全局变量。fork 出的子进程把 counter 减 1（自己的副本）后退出；
 *   父进程 Wait 子进程，再把 counter 加 1（也是自己的副本）。
 *
 *   由于 fork 后父子各自有独立地址空间，counter 在两边是不同变量。
 *   输出：counter = 2 （1 + 1，父进程从 1 自增到 2）
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin forkprob7 */
#include "csapp.h"

/** 全局计数器。父子进程各有自己的副本（COW）。*/
int counter = 1;

/**
 * @brief 主函数
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    if (fork() == 0)
    {
        counter--;
        exit(0);
    }
    else
    {
        Wait(NULL);
        printf("counter = %d\n", ++counter);
    }
    exit(0);
}
/* $end forkprob7 */
