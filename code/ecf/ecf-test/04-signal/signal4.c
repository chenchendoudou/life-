/**
 * @file signal4.c
 * @brief 用 csapp.h 的 Signal() 包装（自动带 SA_RESTART）
 * @details
 *   与 signal3 相比，这里用 csapp.h 的 Signal() 替代 signal()，
 *   Signal() 内部用 sigaction 且设置 SA_RESTART，所以 read 被信号
 *   打断后会自动重启，main 里不需要 while 循环。
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin signal4 */
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
    pid_t pid;

    Signal(SIGCHLD, handler2); /* sigaction error-handling wrapper */

    /* Parent creates children */
    for (i = 0; i < 3; i++) {
	pid = Fork();
	if (pid == 0) {
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
/* $end signal4 */
