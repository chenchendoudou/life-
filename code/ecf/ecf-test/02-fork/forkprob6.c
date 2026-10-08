/**
 * @file forkprob6.c
 * @brief fork() 思考题 6：doit 里子进程 fork 后没 exit
 * @details
 *   与 prob5 类似，但子进程在再 fork 后**没有调用 exit(0)**，而是 return。
 *   这导致子进程（退出 doit 后）会回到 main 继续执行 main 里的 printf("hello")。
 *
 *   最终 hello 总数会比 prob5 多，原因是子进程 return 后还会执行 main 的 printf。
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin forkprob6 */
#include "csapp.h"

/**
 * @brief 父进程空操作，子进程再 fork 后只 return（不 exit）
 * @details 子进程 return 后会回到 main 继续执行 main 的 printf
 */
void doit()
{
    if (Fork() == 0)
    {
        Fork();
        printf("hello\n");
        return;
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
/* $end forkprob6 */
