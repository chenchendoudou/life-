/**
 * @file waitprob1.c
 * @brief waitpid() 思考题 1：父子进程输出流分析
 * @details
 *   父子都会执行 fork 之后的 printf("%d\n", !pid) 和 printf("Bye\n")。
 *   父进程额外 waitpid 子进程并打印子进程 exit status。
 *
 *   关键点：父子都执行 "Hello"、!pid、"Bye"；父进程最后还打印 WEXITSTATUS。
 *   由于父进程 exit(2)，WEXITSTATUS(status) == 2。
 *
 * @note 参考 CS:APP §8.4
 */
#include "csapp.h"

/* $begin waitprob1 */
/**
 * @brief 主函数
 * @return 0（实际不会到达，调用 exit(2)）
 */
int main()
{
    int status;
    pid_t pid;

    printf("Hello\n");
    pid = Fork();
    printf("%d\n", !pid);
    if (pid != 0) {
	if (waitpid(-1, &status, 0) > 0) {
	    if (WIFEXITED(status) != 0)
		printf("%d\n", WEXITSTATUS(status));
	}
    }
    printf("Bye\n");
    exit(2);
}
/* $end waitprob1 */
