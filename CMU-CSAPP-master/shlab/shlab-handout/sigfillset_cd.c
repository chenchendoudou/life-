#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    sigset_t mask_all;
    sigfillset(&mask_all);   //全部信号装进去

    if(sigismember(&mask_all, SIGINT))
        printf("集合里面有 SIGINT(Ctrl+C)\n");

    if(sigismember(&mask_all, SIGCHLD))
        printf("集合里面有 SIGCHLD\n");

    return 0;
}
