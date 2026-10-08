#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

void main()
{
    printf("pid=%d\n", getpid());   
    printf("ppid=%d\n", getppid());
    exit(0);
}