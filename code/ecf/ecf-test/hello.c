/* hello.c - 打印自己的 PID 和父进程 PPID，用来证明 bash 是它的父进程 */
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("hello! 我的 pid=%d, 我的父进程 ppid=%d\n", getpid(), getppid());
    return 0;
}