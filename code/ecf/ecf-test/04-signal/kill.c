/**
 * @file kill.c
 * @brief kill() 显式发送信号：父进程发 SIGKILL 强杀子进程
 * @details
 *   子进程 Pause() 等待任意信号；父进程用 Kill(pid, SIGKILL) 强杀子进程。
 *   因为 SIGKILL 不可捕获，子进程直接被内核终止，不会回到 Pause 之后，
 *   所以"control should never reach here!"这行不会输出。
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin kill */
#include "csapp.h"

/**
 * @brief 主函数：fork 后子进程 pause 等待，父进程发 SIGKILL 强杀子进程
 * @return 0（实际不会到达，父进程 exit(0)）
 */
int main()
{
    pid_t pid;

    /* Child sleeps until SIGKILL signal received, then dies */
    if ((pid = Fork()) == 0) {
	Pause();  /* Wait for a signal to arrive */
	printf("control should never reach here!\n");
	exit(0);
    }

    /* Parent sends a SIGKILL signal to a child */
    Kill(pid, SIGKILL);
    exit(0);
}
/* $end kill */
