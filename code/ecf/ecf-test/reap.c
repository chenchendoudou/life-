/* reap.c - 亲手体验 waitpid: 父进程回收僵尸子进程 */
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        /* 子进程: 直接退出，退出码 42 */
        printf("[child ] pid=%d 即将 exit(42)\n", getpid());
        exit(42);
    }

    /* 父进程: 用 waitpid 回收子进程 (bash 回收 ./fork 用的就是同一套调用) */
    int status = 0;
    pid_t w = waitpid(pid, &status, 0);
    printf("[parent] waitpid 返回 %d, 子进程退出码 WEXITSTATUS=%d\n",
           w, WEXITSTATUS(status));
    return 0;
}