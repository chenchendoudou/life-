/**
 * @file strerrorcd.c
 * @brief 微试验：循环打印 0..139 的 errno 对应的 strerror 字符串。
 * @details
 *   Linux 上 errno 的合法范围大约是 0..133 / 140（不同版本略有差异），
 *   这个程序简单地循环 0..139，把每个 errno 值对应的描述字符串打出来，
 *   用来直观感受 strerror 是怎么工作的。
 *
 *   注意：
 *     - 0 并不是真正的错误码（strerror 在多数实现里返回 "Success"）
 *     - 真正传递给系统调用失败后的 errno 值才有意义；
 *       这里"逐个打"只是为了看 strerror 的覆盖面
 *
 *   该文件**没有**被 Makefile 收录，仅作临时试验用。
 */
#include <stdio.h>
#include <string.h>

/**
 * @brief 进程入口：循环打印 errno=0..139 的 strerror 字符串。
 * @return 0 表示正常打印完。
 */
int main()
{
    /* Linux 上 errno 最大一般不超过 140，循环打印 */
    for (int i = 0; i < 140; i++)
    {
        printf("errno=%3d | %s\n", i, strerror(i));
    }
    return 0;
}
