/**
 * @file signalprob0.c
 * @brief 信号思考题 0：父子进程对 counter 的不同步访问
 * @details
 *   counter 初值 2，main 打印 "2" 后 fork，子进程空转。
 *   父进程 kill(pid, SIGUSR1) 发信号给自己（因为 fork 返回子进程 pid 后，
 *   父进程持有的 pid 变量还是子进程的 pid；但 waitpid 用 -1）。
 *   父进程注册 handler1：counter--, 打印, exit(0)。
 *   waitpid 后父进程 counter++, 打印。
 *
 *   注意：handler1 里的 exit(0) 会让父进程在 waitpid 之前就退出，
 *   实际行为受调度影响。
 *
 * @note 参考 CS:APP §8.5
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

/* $begin signalprob0 */
/** 全局：fork 出来的子进程 PID，由父进程持有并用于 kill/waitpid。*/
pid_t pid;
/** 全局计数器，被 main 和 handler1 修改。*/
int counter = 2;

/**
 * @brief SIGUSR1 handler：counter 自减并打印后 exit(0)
 * @param sig 收到的信号编号
 */
void handler1(int sig) {
    counter = counter - 1;
    printf("%d", counter);
    fflush(stdout);
    exit(0);
}

/**
 * @brief 主函数
 * @return 0（不会到达，调用 exit(0)）
 */
int main() {
    signal(SIGUSR1, handler1);

    printf("%d", counter);
    fflush(stdout);

    if ((pid = fork()) == 0) {
	while(1) {};
    }
    kill(pid, SIGUSR1);
    waitpid(-1, NULL, 0);
    counter = counter + 1;
    printf("%d", counter);
    exit(0);
}
/* $end signalprob0 */
