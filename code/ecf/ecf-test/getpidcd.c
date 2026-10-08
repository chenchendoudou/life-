/**
 * @file getpidcd.c
 * @brief 微试验：打印当前进程 PID 和父进程 PPID。
 * @details
 *   对应 ECF 章节中"每个进程都有一个唯一 PID" 的最小演示。
 *   与同目录 01-syscall-trap/demo_getpid.c 配合：
 *   - demo_getpid.c 直接用 syscall() 包装调用 SYS_getpid
 *   - getpidcd.c     直接用 glibc 的 getpid()/getppid() 包装
 *
 *   该文件**没有**被 Makefile 收录，仅作临时试验用。
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief 进程入口：打印本进程 PID 和其父进程 PPID，然后 exit(0)。
 * @return 不会返回（exit 终止进程）。
 */
void main()
{
    printf("pid=%d\n", getpid());
    printf("ppid=%d\n", getppid());
    exit(0);
}