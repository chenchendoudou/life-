/**
 * @file waitpid1.c
 * @brief waitpid() 用法：以任意顺序回收 N 个子进程
 * @details
 *   父进程创建 N 个子进程（每个用不同 exit code 退出），然后用
 *   waitpid(-1, &status, 0) 循环回收任意一个退出的子进程。
 *   回收顺序由内核调度决定，不保证与创建顺序一致。
 *
 * @note 参考 CS:APP §8.4
 */
/* $begin waitpid1 */
#include "csapp.h"
#define N 2

/**
 * @brief 主函数：创建 N 个子进程并以任意顺序回收
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    int status, i;
    pid_t pid;

    /* Parent creates N children */
    for (i = 0; i < N; i++)                       //line:ecf:waitpid1:for
	if ((pid = Fork()) == 0)  /* child */     //line:ecf:waitpid1:fork
	    exit(100+i);                          //line:ecf:waitpid1:exit

    /* Parent reaps N children in no particular order */
    while ((pid = waitpid(-1, &status, 0)) > 0) { //line:ecf:waitpid1:waitpid
	if (WIFEXITED(status))                    //line:ecf:waitpid1:wifexited
	    printf("child %d terminated normally with exit status=%d\n",
		   pid, WEXITSTATUS(status));     //line:ecf:waitpid1:wexitstatus
	else
	    printf("child %d terminated abnormally\n", pid);
    }

    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)                          //line:ecf:waitpid1:errno
	unix_error("waitpid error");

    exit(0);
}
/* $end waitpid1 */
