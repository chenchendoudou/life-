/**
 * @file forkprob8.c
 * @brief fork() 思考题 8：foo(n) 会产生 2^n 个进程
 * @details
 *   foo(n) 里循环 n 次无条件 Fork()。每次 Fork 把当前进程"裂变"成两个，
 *   所以执行完后会有 2^n 个进程都执行到 printf("hello")。
 *
 *   编译：forkprob8 <n>
 *
 * @note 参考 CS:APP §8.4
 */
#include "csapp.h"

/* $begin forkprob8 */
/**
 * @brief 循环 fork n 次后打印 hello
 * @param n fork 次数
 * @details 产生 2^n 个进程，每个都执行 printf
 */
void foo(int n)
{
    int i;

    for (i = 0; i < n; i++)
	Fork();
    printf("hello\n");
    exit(0);
}
/* $end forkprob8 */

/**
 * @brief 主函数：解析命令行参数 n，调用 foo(n)
 * @param argc 参数个数
 * @param argv 参数列表
 * @return 0（不会到达，调用 exit(0)）
 */
int main(int argc, char **argv)
{
    if  (argc < 2) {
	printf("usage: %s <n>\n", argv[0]);
	exit(0);
    }
    foo(atoi(argv[1]));
    exit(0);
}
