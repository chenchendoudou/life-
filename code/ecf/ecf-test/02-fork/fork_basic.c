/**
 * @file fork_basic.c
 * @brief fork() 教学示例（带详细注释）
 * @details
 *   演示三个核心点：
 *     1. 一次 fork 产生两个进程（"一次调用，两次返回"）。
 *     2. 父子进程拥有独立的地址空间（COW, copy-on-write）。
 *     3. 父子都会执行 fork 之后的代码。
 *
 *   编译运行：
 *   @code
 *     gcc -O0 -g -I.. -o fork_basic fork_basic.c
 *     ./fork_basic
 *   @endcode
 *   预期：会看到两行输出，父子顺序不定（受调度影响）。
 *
 * @note 参考 CS:APP §8.4
 */
#include "csapp.h"

/** 全局变量 — 父子是否共享？答案：不共享，子进程得到副本（写时复制 COW）。*/
int g_x = 1;

/**
 * @brief 父子进程分别修改并打印 g_x 与局部 x
 * @return 0（实际不会到达，子进程和父进程都调用 exit(0)）
 */
int main(void) {
    pid_t pid;
    int   x = 1;  /* 局部变量，父子也不共享 */

    pid = Fork();   /* 陷阱 → 内核创建子进程 → 父子都从这一行返回。*/

    if (pid == 0) {
        /* 子进程分支。
         * 这里修改 g_x / x 不会影响父进程。 */
        g_x = 100;
        x   = 100;
        printf("child : pid=%d, g_x=%d, x=%d\n", getpid(), g_x, x);
        exit(0);
    }

    /* 父进程分支。
     * 这里看到的 g_x / x 还是 1，因为子进程的修改在自己副本里。 */
    printf("parent: pid=%d, child=%d, g_x=%d, x=%d\n", getpid(), pid, g_x, x);
    exit(0);
}