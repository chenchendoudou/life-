/**
 * @file forkprob3.c
 * @brief fork() 思考题 3：父子进程 printf 的执行流
 * @details
 *   与 prob0 类似，但用 if ((pid = Fork()) != 0) 区分父子：
 *   - 父进程：只执行 printf1（x=4）和 printf2（x=3）
 *   - 子进程：只执行 printf2（x=2）
 *
 *   注意：父进程的 x 是 3 自增 1 = 4；子进程是 3 自减 1 = 2。
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin forkprob3 */
#include "csapp.h"
/**
 * @brief 主函数
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    int x = 3;
    pid_t pid;
    if ((pid = Fork()) != 0)
        printf("pid=%d, printf1: x=%d\n", pid, ++x);

    printf("pid=%d, printf2: x=%d\n", pid, --x);
    exit(0);
}
/* $end forkprob3 */
