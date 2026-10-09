/* 
 * mystop.c - Another handy routine for testing your tiny shell
 * 
 * usage: mystop <n>
 * Sleeps for <n> seconds and sends SIGTSTP to itself.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char **argv) 
{
    int i, secs;
    pid_t pid; 

    if (argc != 2) {
	fprintf(stderr, "Usage: %s <n>\n", argv[0]);
	exit(0);
    }
    secs = atoi(argv[1]);//将命令行参数转换为整数

    for (i=0; i < secs; i++)//循环secs次
       sleep(1);//每次循环1秒
	
    pid = getpid(); 

    if (kill(-pid, SIGTSTP) < 0)//发送SIGTSTP信号给进程组
       fprintf(stderr, "kill (tstp) error");

    exit(0);

}
