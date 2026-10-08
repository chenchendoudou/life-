/* $begin fork */
#include "csapp.h"

int main() 
{
    pid_t pid;
    int x = 1;

    pid = Fork(); //line:ecf:forkreturn
    if (pid == 0) {  /* Child */
	printf("child : x=%d\n", ++x); //line:ecf:childprint
    //查看x的地址
    printf("child: x=%p\n", &x);
    //查看x的物理地址
    printf("child: x=%p\n", &x);
	exit(0);
    }

    /* Parent */
    printf("parent: x=%d\n", --x); //line:ecf:parentprint
    //查看x的地址
    printf("parent: x=%p\n", &x);
    exit(0);
}
/* $end fork */

