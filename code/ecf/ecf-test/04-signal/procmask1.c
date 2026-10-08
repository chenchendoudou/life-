/**
 * @file procmask1.c
 * @brief SIGCHLD 回收子进程 — 错误版：没用 sigprocmask，存在竞争
 * @details
 *   父进程注册 SIGCHLD handler 用 waitpid 回收子进程 + deletejob。
 *   关键 bug：Fork() 和 addjob() 之间存在竞争 — 如果 SIGCHLD 在
 *   addjob() 执行前到达，handler 会执行 deletejob(pid)，但此时
 *   job 还没 add 进去，导致 job list 不一致。
 *
 *   procmask2.c 修复了这个 bug。
 *
 * @note 参考 CS:APP §8.5
 */
#include "csapp.h"

/**
 * @brief 初始化 job list（本例为空操作）
 */
void initjobs()
{
}

/**
 * @brief 把子进程加入 job list（本例为空操作）
 * @param pid 子进程 PID
 */
void addjob(int pid)
{
}

/**
 * @brief 从 job list 删除子进程（本例为空操作）
 * @param pid 子进程 PID
 */
void deletejob(int pid)
{
}

/* $begin procmask1 */
/**
 * @brief SIGCHLD handler：循环回收所有已退出的子进程
 * @param sig 收到的信号编号
 * @details 用 waitpid(-1, NULL, 0) 回收所有僵尸子进程
 */
void handler(int sig)
{
    pid_t pid;
    while ((pid = waitpid(-1, NULL, 0)) > 0) /* Reap a zombie child */
        deletejob(pid);                      /* Delete the child from the job list */
    if (errno != ECHILD)
        unix_error("waitpid error");
}

/**
 * @brief 主函数
 * @param argc 参数个数
 * @param argv 参数列表（传给 /bin/date）
 * @return 0（不会到达，调用 exit(0)）
 */
int main(int argc, char **argv)
{
    int pid;

    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1)
    {
        /* Child process */
        if ((pid = Fork()) == 0)
        {
            Execve("/bin/date", argv, NULL);
        }

        /* Parent process */
        addjob(pid); /* Add the child to the job list */
    }
    exit(0);
}
/* $end procmask1 */
