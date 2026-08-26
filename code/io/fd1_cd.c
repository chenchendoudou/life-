#include <fcntl.h>  

int main(void) 
{
    int fd1,fd2;
    fd1 = open("fd1_cd.c",O_RDONLY);
    //close(fd1);

    fd2 = open("fd1_cd.c",O_RDONLY);
    printf("fd1=%d,fd2=%d\n",fd1,fd2);

    close(fd2);
    return 0;
}