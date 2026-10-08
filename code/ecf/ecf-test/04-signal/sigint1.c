/**
 * @file sigint1.c
 * @brief SIGINT（Ctrl+C）处理：捕获后打印信息再退出
 * @details
 *   signal(SIGINT, handler) 注册 SIGINT 处理函数。
 *   主程序 pause() 等待任意信号，handler 打印 "Caught SIGINT" 后 exit(0)。
 *
 *   测试：在终端运行后按 Ctrl+C，进程会打印信息并退出。
 *
 * @note 参考 CS:APP §8.5
 */
/* $begin sigint1 */
#include "csapp.h"

/**
 * @brief SIGINT handler
 * @param sig 收到的信号编号（SIGINT = 2）
 * @details 打印 "Caught SIGINT" 后 exit(0) 干净退出
 */
void handler(int sig) /* SIGINT handler */   //line:ecf:sigint1:beginhandler
{
    printf("Caught SIGINT\n");               //line:ecf:sigint1:printhandler
    exit(0);                                 //line:ecf:sigint1:exithandler
}                                            //line:ecf:sigint1:endhandler

/**
 * @brief 主函数：注册 SIGINT handler 后 pause 等待
 * @return 0（不会到达，handler 内部 exit(0)）
 */
int main()
{
    /* Install the SIGINT handler */
    if (signal(SIGINT, handler) == SIG_ERR)  //line:ecf:sigint1:begininstall
	unix_error("signal error");          //line:ecf:sigint1:endinstall

    pause(); /* Wait for the receipt of a signal */  //line:ecf:sigint1:pause

    exit(0);
}
/* $end sigint1 */
