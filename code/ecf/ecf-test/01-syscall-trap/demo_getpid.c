/* demo_getpid.c
 *
 * 证明点：getpid() 的底层就是 syscall 指令（陷阱）。
 *
 * 编译：   gcc -O0 -g -I.. -o demo_getpid demo_getpid.c
 * 反汇编： objdump -d demo_getpid | sed -n '/<main>:/,/^$/p'
 *
 * 你会看到 main 里调用了 syscall@plt，进入 libc 后最终落到：
 *
 *      mov    %rdi,%rax       ; 系统调用号（getpid = 39 = 0x27）
 *      ...
 *      syscall                ; <-- 0f 05，这就是陷阱指令！
 *      cmp    $0xfffffffffffff001,%rax
 *      jae    <error>
 *      ret
 *
 * 所以 "getpid()" 这一个普通的 C 函数调用，底层就跨越了用户/内核边界。
 */

#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
    /* 直接用 syscall() 包装，显式把系统调用号作为第一参数。 */
    long pid = syscall(SYS_getpid);
    printf("pid = %ld\n", pid);
    return 0;
}