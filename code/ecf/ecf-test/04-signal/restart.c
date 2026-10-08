/**
 * @file restart.c
 * @brief sigsetjmp / siglongjmp：用 SIGINT 重启主循环
 * @details
 *   sigsetjmp 第一次返回 0，handler 用 siglongjmp 跳回时返回非 0。
 *   主循环每 Sleep(1) 后会被 SIGINT 打断，跳回 sigsetjmp 处重新进入循环。
 *
 *   用 sigsetjmp（而非 setjmp）是为了不屏蔽当前信号屏蔽字 — 这点
 *   跟 signal handler 配合时是关键。
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin restart */
#include "csapp.h"

/** sigsetjmp 保存的上下文，handler 用 siglongjmp 跳回。*/
sigjmp_buf buf;

/**
 * @brief SIGINT handler：siglongjmp 跳回 sigsetjmp 处
 * @param sig 收到的信号编号
 */
void handler(int sig)
{
    siglongjmp(buf, 1);
}

/**
 * @brief 主函数：每秒打印一行 processing，被 SIGINT 打断后从 starting 重启
 * @return 0（不会到达，循环里没有 break 也没有 exit）
 */
int main()
{
    Signal(SIGINT, handler);

    if (!sigsetjmp(buf, 1))
	printf("starting\n");
    else
	printf("restarting\n");

    while(1) {
	Sleep(1);
	printf("processing...\n");
    }
    exit(0);
}
/* $end restart */
