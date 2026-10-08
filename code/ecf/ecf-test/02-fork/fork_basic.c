/* fork_basic.c — fork() 教学示例，带详细注释。
 *
 * 这个文件演示三个核心点：
 *   1. 一次 fork 产生两个进程（"一次调用，两次返回"）。
 *   2. 父子进程拥有独立的地址空间（COW）。
 *   3. 父子都会执行 fork 之后的代码。
 *
 * 编译：gcc -O0 -g -I.. -o fork_basic fork_basic.c
 * 运行：./fork_basic
 * 预期：会看到两行输出，父子顺序不定（受调度影响）。
 */

#include "csapp.h"

/* 全局变量 — 父子是否共享？
 * 答案：不共享。子进程得到的是副本，写时复制（COW）。
 */
int g_x = 1;

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