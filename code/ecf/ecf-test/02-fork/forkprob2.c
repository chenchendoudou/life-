/**
 * @file forkprob2.c
 * @brief fork() 思考题 2：atexit 在 fork 后的执行情况
 * @details
 *   - 第一个 fork：子进程通过 atexit 注册了 end()
 *   - 第二个 fork：所有进程里第一个字符的子进程打印 "0"
 *   - 父进程打印 "1"
 *   - 退出时 atexit 注册的 end() 会执行，输出 "2"
 *
 *   关键点：atexit 注册的函数在进程 exit 时执行，每个进程（包括 fork
 *   出来的子进程）退出时都会执行自己注册过的 end()。
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin forkprob2 */
#include "csapp.h"

/**
 * @brief atexit 注册的退出处理函数
 * @details 进程 exit 时由 atexit 机制调用，输出 "2"
 */
void end(void)
{
    printf("2");
}

/**
 * @brief 主函数：两次 fork + atexit + 打印
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    if (Fork() == 0)
        atexit(end); // 子进程注册exit函数
    if (Fork() == 0)
        printf("0\n");
    else
        printf("1\n");
    exit(0);
}
/* $end forkprob2 */
