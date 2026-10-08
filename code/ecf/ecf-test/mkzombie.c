/* mkzombie.c - 亲手制造一个僵尸进程，并用 ps 观察它 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        /* 子进程: 退出 -> 变成僵尸 (等父进程 wait) */
        printf("[child ] pid=%d 我现在 exit, 马上变僵尸(Z)\n", getpid());
        exit(0);
    }

    /* 父进程: 故意先不 wait, 睡 5 秒 */
    printf("[parent] pid=%d 故意不 wait, 子进程 pid=%d 将变僵尸(Z), 5 秒后我才 wait\n",
           getpid(), pid);
    sleep(2);
    wait(NULL);   /* 5 秒后才回收 */
    printf("[parent] 已调用 wait, 僵尸被回收掉了\n");
    return 0;
}