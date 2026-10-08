/**
 * @file forkprob1.c
 * @brief fork() 思考题 1：循环里 fork 会产生多少进程
 * @details
 *   for 循环执行 2 次 Fork()。每次 Fork 都会让当前进程"裂变"为两个。
 *   进程数：1 -> 2 -> 4，所以一共会有 4 个进程都执行到 printf。
 *
 *   输出 4 行 hello，但 pid 和 i 因父子分裂时机不同而不同。
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin forkprob1 */
#include "csapp.h"

/**
 * @brief 主函数：循环 2 次 fork
 * @return 0（不会到达，每个子进程都 exit(0)）
 */
int main()
{
    int i;
    pid_t pid;

    for (i = 0; i < 2; i++)
    {
        pid = Fork();
    }
    printf("pid=%d,i=%d, hello\n", pid,i);
    exit(0);
}
/* $end forkprob1 */
