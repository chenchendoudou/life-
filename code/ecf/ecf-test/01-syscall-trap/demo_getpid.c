/**
 * @file demo_getpid.c
 * @brief 证明 getpid() 的底层是 syscall 指令
 * @details
 *   本程序直接用 glibc 的 syscall() 包装函数，以 SYS_getpid 作为系统调用号
 *   调用一次系统调用。这跟普通的 getpid() 走的是同一条路。
 *
 *   用 objdump 反汇编可看到：
 *     - 编译器生成的 main 调用的是 libc 里的 getpid 或 __getpid
 *     - libc 内部最终落到 `mov $0x27, %rax; syscall`
 *     - "0x27" 就是 SYS_getpid 的系统调用号
 *
 *   测试方法：
 *   @code
 *     gcc -O0 -g -I.. -o demo_getpid demo_getpid.c
 *     objdump -d demo_getpid | sed -n '/<main>:/,/^$/p'
 *   @endcode
 *
 * @note 参考 CS:APP §8.1-8.3
 */
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @brief 直接用 syscall() 包装调用 getpid
 * @return 0 正常退出
 */
int main(void) {
    /* 直接用 syscall() 包装，显式把系统调用号作为第一参数。 */
    long pid = syscall(SYS_getpid);
    printf("pid = %ld\n", pid);
    return 0;
}