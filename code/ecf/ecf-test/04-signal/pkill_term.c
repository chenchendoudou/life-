/* $begin pkill_term */
#include "csapp.h"
#include <time.h> /* for time()  （精简版 csapp.h 没引这个） */

/**
 * @file pkill_term.c
 * @brief 收到 pkill（默认 SIGTERM）时打印信息再退出
 * @details
 *   关于"pkill -9"的说明：
 *   SIGKILL (信号 9) 在 Linux 上是"特权信号"，内核强制保证：
 *     - 进程无法通过 signal()/sigaction() 捕获
 *     - 进程无法通过 sigprocmask() 阻塞
 *     - 进程无法通过 Signal() 忽略
 *   pkill -9 会让内核直接终止进程，连 handler 都不会调用。
 *
 *   实际运维中，pkill 不带 -9 时默认发的是 SIGTERM (信号 15)，
 *   SIGTERM 是可以捕获的。这个示例演示的就是这种情况：
 *     1. 注册 SIGTERM handler
 *     2. handler 里打印清理信息（比如"开始关闭文件/释放资源..."）
 *     3. 然后调用 exit(0) 干净退出
 *
 *   测试方法（开两个终端）：
 *     终端 A：./04-signal/pkill_term
 *     终端 B：pkill pkill_term      # 默认就是 SIGTERM
 *     终端 A 会看到 handler 输出，然后进程退出
 *
 *     终端 B：pkill -9 pkill_term    # SIGKILL 仍会立即杀进程，无输出
 *
 * @note 参考 CS:APP §8.5
 */

/**
 * @brief SIGTERM 处理函数
 * @param sig 收到的信号编号（应为 SIGTERM = 15）
 * @details 打印清理信息（包括 pid 和当前时间），然后 exit(0) 干净退出。
 *          handler 里只调用 async-signal-safe 的函数最稳妥。
 *          printf 在严格意义上不是 async-signal-safe 的，
 *          但在教学示例里这样用最直观；真实工程里应该用 write()。
 */
void handler(int sig) // line:ecf:pkillterm:beginhandler
{                     // line:ecf:pkillterm:endhandler
    printf("[pkill_term] caught signal %d (SIGTERM), cleaning up...\n", sig);

    /* 用 getpid() 和 time() 让输出能区分每次不同的进程实例 */
    // 退出时间显示能看懂的时间格式

    time_t now = time(NULL);
    printf("pid=%d, time=%s", (int)getpid(), ctime(&now));
    fflush(stdout); /* 确保缓冲区内容在 exit 前被刷出 */

    exit(0); // line:ecf:pkillterm:exit
}

/**
 * @brief 主函数：安装 SIGTERM handler 后 pause 等待
 * @return 0（不会到达，handler 内部 exit(0)）
 */
int main()
{
    /* 安装 SIGTERM handler（pkill 不带 -9 时的默认信号）
     * 这里用 stdlib 的 signal() 而非 csapp.h 的 Signal() 包装，
     * 跟同目录的 sigint1.c 保持一致——精简版 csapp.h 没有 typedef
     * 出"正确的" handler_t 类型，用裸 signal() 编译最干净。 */
    if (signal(SIGTERM, handler) == SIG_ERR) // line:ecf:pkillterm:begininstall
        unix_error("signal error");          // line:ecf:pkillterm:endinstall

    printf("[pkill_term] pid=%d, waiting for SIGTERM (try: pkill pkill_term)\n",
           (int)getpid());
    fflush(stdout);

    /* pause() 会让进程休眠，直到任意信号被收到
     * 收到 SIGTERM 时会先进入 handler，handler 里 exit(0) 后进程结束。
     * 不会回到 pause() 之后。 */
    while (1)    // line:ecf:pkillterm:beginpause
        Pause(); // line:ecf:pkillterm:endpause

    /* 不会到达这里 */
    printf("control should never reach here!\n");
    exit(0);
}
/* $end pkill_term */
