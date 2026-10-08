/**
 * @file waitprob0.c
 * @brief waitpid() 思考题 0：父子进程 + waitpid 的输出顺序
 * @details
 *   父进程分支先打印 "b" 然后 waitpid 等待子进程，子进程分支只打印 "a"。
 *   父子最后都打印 "c"。
 *
 *   关键点：waitpid 让父进程必须等子进程退出后才打印 "c"，
 *   所以父进程的 "c" 一定在子进程 "ac" 之后。
 *   典型输出：b a c c 或 a b c c
 *
 * @note 参考 CS:APP §8.4
 */
#include "csapp.h"

/* $begin waitprob0 */
/**
 * @brief 主函数
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    if (Fork() == 0) {
	printf("a");
    }
    else {
	printf("b");
	waitpid(-1, NULL, 0);
    }
    printf("c");
    exit(0);
}
/* $end waitprob0 */
