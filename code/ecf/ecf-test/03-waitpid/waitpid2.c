/**
 * @file waitpid2.c
 * @brief waitpid() 用法：按指定顺序回收 N 个子进程
 * @details
 *   与 waitpid1 不同，本程序用 waitpid(pid[i++], &status, 0) 显式指定
 *   要等待的子进程 PID，因此回收顺序与创建顺序一致。
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin waitpid2 */
#include "csapp.h"
#define N 2

/**
 * @brief 主函数：创建 N 个子进程并按创建顺序回收
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    int status, i;
    pid_t pid[N], retpid;

    /* Parent creates N children */
    for (i = 0; i < N; i++)
	if ((pid[i] = Fork()) == 0)  /* Child */          //line:ecf:waitpid2:fork
	    exit(100+i);

    /* Parent reaps N children in order */
    i = 0;
    while ((retpid = waitpid(pid[i++], &status, 0)) > 0) { //line:ecf:waitpid2:waitpid
	if (WIFEXITED(status))
	    printf("child %d terminated normally with exit status=%d\n",
		   retpid, WEXITSTATUS(status));
	else
	    printf("child %d terminated abnormally\n", retpid);
    }

    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)
	unix_error("waitpid error");

    exit(0);
}
/* $end waitpid2 */
