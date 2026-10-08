/**
 * @file signal3.c
 * @brief 显式处理 EINTR：慢系统调用被信号打断后手动重启
 * @details
 *   与 signal2 的区别：read 用了 while + errno == EINTR 手动重试。
 *   这在没有 SA_RESTART 时是标准做法。
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin signal3 */
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
int main() {
    int i, n;
    char buf[MAXBUF];
    pid_t pid;

    if (signal(SIGCHLD, handler2) == SIG_ERR)
	unix_error("signal error");

    /* Parent creates children */
    for (i = 0; i < 3; i++) {
	pid = Fork();
	if (pid == 0) {
	    printf("Hello from child %d\n", (int)getpid());
	    Sleep(1);
	    exit(0);
	}
    }

    /* Manually restart the read call if it is interrupted */
    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
	if (errno != EINTR)
	    unix_error("read error");

    printf("Parent processing input\n");
    while (1)
	;

    exit(0);
}
/* $end signal3 */
