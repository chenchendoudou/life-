/**
 * @file signal1.c
 * @brief signal() 的最简形式：SIGCHLD 回收一个子进程
 * @details
 *   父进程注册 SIGCHLD handler1，每次只 waitpid 一个子进程。
 *   创建 3 个子进程后阻塞在 read 上等待 stdin 输入。
 *
 *   与 signal2 的区别：本版 handler 一次只回收一个子进程（不循环），
 *   剩下的子进程会变僵尸直到下次 SIGCHLD 触发再回收。
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin signal1 */
#include "csapp.h"

/**
 * @brief SIGCHLD handler：回收一个子进程
 * @param sig 收到的信号编号
 * @details 与 signal2 不同：这里只 waitpid 一次，sleep(2) 模拟"做点事"
 */
void handler1(int sig)
{
    pid_t pid;

    if ((pid = waitpid(-1, NULL, 0)) < 0)
	unix_error("waitpid error");
    printf("Handler reaped child %d\n", (int)pid);
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

    if (signal(SIGCHLD, handler1) == SIG_ERR)
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
	unix_error("read");

    printf("Parent processing input\n");
    while (1)
	;

    exit(0);
}
/* $end signal1 */
