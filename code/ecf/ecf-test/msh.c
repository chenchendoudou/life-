/* msh.c - 一个迷你 shell: 读取命令 -> fork+exec -> 父进程 waitpid 回收子进程
 *
 * 用法:
 *   ./msh
 *   msh> hello            (运行 ./hello, 父进程回收, 打印退出码)
 *   msh> reap             (运行 ./reap, 演示 waitpid)
 *   msh> ls               (任何 PATH 里的命令也能跑)
 *   msh> exit             (退出)
 *
 * 关键点: 下面父进程的 waitpid() 就是 bash 回收 ./hello 时做的事情。
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024

/* 把一行按空格拆成参数, 原地用 '\0' 切断, 返回参数个数 */
static int parse(char *line, char **argv)
{
    int argc = 0;
    char *p = line;
    while (*p) {
        while (*p == ' ' || *p == '\t' || *p == '\n')
            *p++ = '\0';          /* 抹掉分隔符 */
        if (*p == '\0')
            break;
        argv[argc++] = p;         /* 记录一个参数起点 */
        while (*p && *p != ' ' && *p != '\t' && *p != '\n')
            p++;                  /* 走到参数结尾 */
    }
    argv[argc] = NULL;
    return argc;
}

int main(void)
{
    char line[MAXLINE];
    char *argv[64];

    printf("msh 启动, 我的 PID=%d (我就是那个负责回收子进程的父进程)\n", getpid());

    for (;;) {
        printf("msh> ");
        fflush(stdout);

        if (fgets(line, sizeof line, stdin) == NULL) {
            printf("\nbye\n");   /* Ctrl-D 退出 */
            break;
        }

        int argc = parse(line, argv);
        if (argc == 0)
            continue;             /* 空行 */

        /* 内建命令 */
        if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0)
            break;

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            /* 子进程: 用 execvp 执行命令 */
            execvp(argv[0], argv);
            /* execvp 失败才会走到这: 命令里没 '/' 时, 再试 "./命令" */
            if (strchr(argv[0], '/') == NULL) {
                char path[MAXLINE];
                snprintf(path, sizeof path, "./%s", argv[0]);
                execvp(path, argv);
            }
            fprintf(stderr, "msh: 找不到命令: %s\n", argv[0]);
            exit(127);
        }

        /* 父进程: ★ 用 waitpid 回收子进程, 消除僵尸 ★
         * 如果这里不调用 waitpid, 子进程退出后就会变成僵尸(Z)。
         */
        int status = 0;
        pid_t w = waitpid(pid, &status, 0);
        if (w == pid && WIFEXITED(status)) {
            printf("[msh] 已用 waitpid 回收子进程 pid=%d, 退出码=%d\n",
                   pid, WEXITSTATUS(status));
        }
    }

    return 0;
}