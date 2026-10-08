/**
 * @file forkprob5.c
 * @brief fork() 思考题 5：doit 里只在子进程再 fork
 * @details
 *   doit() 仅在子进程里再 Fork() 一次，所以会产生 3 个进程：
 *     - doit 的父进程（1 个）
 *     - doit 的子进程（1 个）
 *     - doit 的子进程再 fork 出的孙子进程（1 个）
 *
 *   这 3 个进程都执行 printf("hello")；main 自己再打印 1 行。
 *   所以最终输出 4 行 hello。
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin forkprob5 */
#include "csapp.h"

/**
 * @brief 父进程空操作，子进程再 fork 后打印 hello 并退出
 * @details 父进程（doit 视角）什么也不做就 return；子进程再 fork 后两个都打印 hello
 */
void doit()
{
    if (Fork() == 0)
    {
        Fork();
        printf("hello\n");
        exit(0);
    }
    return;
}

/**
 * @brief 主函数
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    doit();
    printf("hello\n");
    exit(0);
}
/* $end forkprob5 */
