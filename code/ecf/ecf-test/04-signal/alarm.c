/**
 * @file alarm.c
 * @brief alarm() 定时信号示例：5 次 BEEP 后 BOOM
 * @details
 *   用 Signal(SIGALRM, handler) 注册 SIGALRM 处理函数，handler 每收到
 *   一次 SIGALRM 就 BEEP 一次并重新 alarm(1)，连续 5 次后打印 BOOM 并 exit。
 *
 *   信号链：内核 -> alarm 到期 -> 投递 SIGALRM -> handler
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin alarm */
#include "csapp.h"

/**
 * @brief SIGALRM 处理函数
 * @param sig 收到的信号编号（应为 SIGALRM）
 * @details 用 static 计数器实现"5 次 BEEP 后 BOOM"逻辑
 */
void handler(int sig)
{
    static int beeps = 0;

    printf("BEEP\n");
    if (++beeps < 5)
        Alarm(1); /* Next SIGALRM will be delivered in 1 second */
    else
    {
        printf("BOOM!\n");
        exit(0);
    }
}

/**
 * @brief 主函数：注册 SIGALRM handler 后进入死循环
 * @return 0（不会到达，handler 内部 exit(0)）
 */
int main()
{
    Signal(SIGALRM, handler); /* install SIGALRM handler */
    Alarm(1);                 /* Next SIGALRM will be delivered in 1s */

    while (1)
    {
        ; /* Signal handler returns control here each time */
    }
    exit(0);
}
/* $end alarm */
