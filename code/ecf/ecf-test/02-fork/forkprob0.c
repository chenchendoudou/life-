/**
 * @file forkprob0.c
 * @brief fork() 思考题 0：验证 fork 两次返回的值与父子进程的变量
 * @details
 *   一次 fork 调用产生两个执行流（父子进程），它们都从 fork() 调用处
 *   继续往下执行。子进程得到 pid==0；父进程得到子进程的 pid。
 *   本题验证父子进程对各自变量副本的修改互不影响。
 *
 *   预期输出（顺序不一定）：
 *     pid=0,   printf1: x=2   <-- 子进程
 *     pid=xxxx,printf2: x=1   <-- 父进程（x 是 1，因为是原值；注意：printf2 子进程也会执行）
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin forkprob0 */
#include "csapp.h"

/**
 * @brief fork 父子进程都会执行 main 后半段
 * @return 0（实际不会到达，调用 exit(0)）
 */
int main()
{
    int x = 1;
    pid_t pid;
    pid = Fork();
    if (pid == 0)
        printf("pid=%d, printf1: x=%d\n", pid, ++x);
    printf("pid=%d, printf2: x=%d\n", pid, --x);
    exit(0);
}
/* $end forkprob0 */
