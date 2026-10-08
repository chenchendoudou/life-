/**
 * @file waitprob3.c
 * @brief waitpid() 思考题 3：用裸 fork() 替代 Fork() 的对比
 * @details
 *   与 waitprob0 结构完全相同，唯一的区别是本程序用裸 fork() 而非
 *   csapp.h 的 Fork() 包装。
 *
 *   这个差异在输出上看不出来（行为相同），但展示了两种风格。
 *
 * @note 参考 CS:APP §8.4
 */
#include "csapp.h"

/* $begin waitprob3 */
/**
 * @brief 主函数
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    if (fork() == 0) {
	printf("a");
	exit(0);
    }
    else {
	printf("b");
	waitpid(-1, NULL, 0);
    }
    printf("c");
    exit(0);
}
/* $end waitprob3 */
