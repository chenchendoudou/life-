/**
 * @file counterprob.c
 * @brief 信号处理思考题：handler 里的 sleep 会让 counter 丢失更新
 * @details
 *   子进程向父进程发 5 次 SIGUSR2，父进程的 handler 每收到一次就 counter++ 然后 sleep(1)。
 *
 *   因为 handler 执行 sleep(1) 时新到的 SIGUSR2 会被默认处理（handler 没阻塞 SIGUSR2，
 *   但 sleep 期间信号不会丢失，会被排队——不过 POSIX signal() 语义里同一信号多次
 *   在 pending 状态只记一次）。
 *
 *   实际：counter 通常小于 5。父进程 Wait(NULL) 等子进程退出后打印 counter。
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin counterprob */
#include "csapp.h"

/** 全局计数器，被 SIGUSR2 handler 自增。*/
int counter = 0;

/**
 * @brief SIGUSR2 handler：counter++ 然后 sleep(1) 模拟"做点工作"
 * @param sig 收到的信号编号
 * @details 经典陷阱：handler 里有慢操作会导致信号合并/丢失
 */
void handler(int sig)
{
    counter++;
    sleep(1); /* Do some work in the handler */
    return;
}

/**
 * @brief 主函数：fork 后子进程连发 5 次 SIGUSR2，父进程回收后打印 counter
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    int i;

    Signal(SIGUSR2, handler);

    if (Fork() == 0) {  /* Child */
	for (i = 0; i < 5; i++) {
	    Kill(getppid(), SIGUSR2);
	    printf("sent SIGUSR2 to parent\n");
	}
	exit(0);
    }

    Wait(NULL);
    printf("counter=%d\n", counter);
    exit(0);
}
/* $end counterprob */
