/* demo_forkasm.c
 *
 * 证明点：fork() 也是 syscall 指令。
 *
 * 这个程序本身非常平凡（fork + 两个分支打印），但意义在于让你
 * 亲手用 objdump 看到 fork 内部就是 0x05 syscall 指令。
 *
 * 编译：   gcc -O0 -g -I.. -o demo_forkasm demo_forkasm.c
 * 反汇编： objdump -d demo_forkasm | sed -n '/<main>:/,/^$/p'
 */

#include "csapp.h"

int main(void) {
    pid_t pid = Fork();

    if (pid == 0) {
        printf("child:  pid = %d\n", getpid());
    } else {
        printf("parent: child pid = %d, my pid = %d\n", pid, getpid());
    }
    return 0;
}