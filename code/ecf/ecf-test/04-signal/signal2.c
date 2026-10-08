/**
 * @file signal2.c
 * @brief signal() 改进版：SIGCHLD 循环回收所有子进程
 * @details
 *   与 signal1 相比，handler2 用 while 循环 waitpid 回收所有已退出的子进程，
 *   不会出现"剩下子进程变僵尸"的问题。
 *
 *   与 signal3 的区别：signal3 显式处理 EINTR（慢系统调用被信号打断后手动重启）。
 *   与 signal4 的区别：signal4 用 csapp.h 的 Signal() 包装（带 SA_RESTART）。
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin signal2 */
#include "csapp.h"

/**
 * @brief SIGCHLD handler：循环回收所有已退出的子进程
 * @param sig 收到的信号编号
 */
void handler2(int sig)
{
    pid_t pid;

    while ((pid = waitpid(-1, NULL, 0)) > 0)
	printf("Handler reaped child %d\n", (int)pid);
    if (errno != ECHILD)
	unix_error("waitpid error");
    Sleep(2);
    return;
}

/**
 * @brief 主函数
 * @return 0（不会到达，最后进入死循环）
 */
int main()
{
    int i, n;
    char buf[MAXBUF];

    if (signal(SIGCHLD, handler2) == SIG_ERR)
	unix_error("signal error");

    /* Parent creates children */
    for (i = 0; i < 3; i++) {
	if (Fork() == 0) {
	    printf("Hello from child %d\n", (int)getpid());
	    Sleep(1);
	    exit(0);
	}
    }

    /* Parent waits for terminal input and then processes it */
    if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
	unix_error("read error");

    printf("Parent processing input\n");
    while (1)
	;

    exit(0);
}
/* $end signal2 */
