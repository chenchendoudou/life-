/**
 * @file procmask2.c
 * @brief SIGCHLD 回收子进程 — 正确版：用 sigprocmask 消除竞争
 * @details
 *   procmask1 的 bug 修复版。在 fork 之前阻塞 SIGCHLD，addjob 之后再解除阻塞，
 *   这样 SIGCHLD 一定在 addjob 之后才被递达，handler 里 deletejob 就一定安全。
 *
 *   子进程里 fork 后立即解除 SIGCHLD 阻塞（子进程不继承父进程的 job list，
 *   所以不需要保护）。
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

/* $begin procmask2 */
/**
 * @brief SIGCHLD handler：循环回收所有已退出的子进程
 * @param sig 收到的信号编号
 */
void handler(int sig)
{
    pid_t pid;
    while ((pid = waitpid(-1, NULL, 0)) > 0) /* Reap a zombie child */
	deletejob(pid); /* Delete the child from the job list */
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
    sigset_t mask;

    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1) {
	Sigemptyset(&mask);
	Sigaddset(&mask, SIGCHLD);
	Sigprocmask(SIG_BLOCK, &mask, NULL); /* Block SIGCHLD */

	/* Child process */
	if ((pid = Fork()) == 0) {
	    Sigprocmask(SIG_UNBLOCK, &mask, NULL); /* Unblock SIGCHLD */
	    Execve("/bin/date", argv, NULL);
	}

	/* Parent process */
	addjob(pid);  /* Add the child to the job list */
	Sigprocmask(SIG_UNBLOCK, &mask, NULL);  /* Unblock SIGCHLD */
    }
    exit(0);
}
/* $end procmask2 */
