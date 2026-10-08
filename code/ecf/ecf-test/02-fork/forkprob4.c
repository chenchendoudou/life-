/**
 * @file forkprob4.c
 * @brief fork() 思考题 4：doit 里的连续两次 fork
 * @details
 *   doit() 里两次无条件 Fork()，会产生 4 个进程都执行到 printf("hello")。
 *   main 自己也再 printf("hello") 一次。
 *
 *   所以最终输出 5 行 hello（4 个 doit 子进程 + 1 个 main）。
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin forkprob4 */
#include "csapp.h"

/**
 * @brief 无条件 fork 两次后打印
 * @details 两次连续 Fork() 产生 4 个进程（2x2 裂变），每个都执行 printf
 */
void doit()
{
    Fork();
    Fork();
    printf("hello\n");
    return;
}

/**
 * @brief 主函数：调用 doit 后再打印一次
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    doit();
    printf("hello\n");
    exit(0);
}
/* $end forkprob4 */
