#include <stdio.h>

// 全局环境变量表，extern声明
extern char **environ;

int main(int argc, char *argv[])
{
    int i;
    char **p;

    printf("Command‑line arguments:\n");
    for(i = 0; argv[i] != NULL; i++){
        printf("    argv[%d]: %s\n", i, argv[i]);
    }

    printf("\nEnvironment variables:\n");
    for(p = environ, i = 0; *p != NULL; p++){
        printf("envp[%d]    %s\n",i++, *p);
    }

    return 0;
}
