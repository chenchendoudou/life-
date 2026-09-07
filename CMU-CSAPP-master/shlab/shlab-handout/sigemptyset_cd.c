#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    sigset_t set;
    sigemptyset(&set);  //集合清零，里面空空如也

    // 判断SIGCHLD是否在集合内
    if(sigismember(&set, SIGCHLD))
        printf("包含SIGCHLD\n");
    else
        printf("不包含SIGCHLD\n");

    return 0;
}
