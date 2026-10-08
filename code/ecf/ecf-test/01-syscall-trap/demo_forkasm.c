/**
 * @file demo_forkasm.c
 * @brief 证明 fork() 内部也是 syscall 指令
 * @details
 *   这个程序本身很平凡（fork + 两个分支打印），但意义在于让你亲手用
 *   objdump 看到 fork 内部就是 `0x05` 的 syscall 指令。
 *
 *   编译并反汇编：
 *   @code
 *     gcc -O0 -g -I.. -o demo_forkasm demo_forkasm.c
 *     objdump -d demo_forkasm | sed -n '/<main>:/,/^$/p'
 *   @endcode
 *
 * @note 参考 CS:APP §8.1-8.3
 */
#include "csapp.h"

/**
 * @brief fork 后父子进程分别打印自己的 pid
 * @return 0 正常退出
 */
int main(void) {
    pid_t pid = Fork();

    if (pid == 0) {
        printf("child:  pid = %d\n", getpid());
    } else {
        printf("parent: child pid = %d, my pid = %d\n", pid, getpid());
    }
    return 0;
}