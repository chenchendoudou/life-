/**
 * @file shellex.c
 * @brief 一个 100 行左右的简易 shell（综合应用 fork / execve / waitpid）
 * @details
 *   这是 CS:APP §8.4 + §8.5 的综合示例：把前面几个主题里学过的
 *   fork / execve / waitpid / builtin 命令等机制串成一个交互式 shell。
 *
 *   支持的能力：
 *     - 读入一行命令，解析为 argv
 *     - 内建命令 quit（直接 exit）和 单行 '&'（忽略）
 *     - 其它命令走 fork + execve
 *     - 前台命令父进程等待（waitpid），后台命令父进程打印 PID 后继续
 *
 *   @note
 *     为了兼容 CS:APP 教材里的 begin / end 行号锚点
 *     （教材和本仓库 README 都靠它们做 grep 对照），
 *     下方保留全部 $begin ... $end 标记，并在它们之间
 *     用 Doxygen 注释丰富文档。
 *
 *     参考 CS:APP §8.4 + §8.5
 */
/* $begin shellmain */
#include "csapp.h"
#define MAXARGS   128

/* function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

/**
 * @brief 主循环：反复读入命令并求值。
 * @return 0 不会到达（正常通过 exit(0) / Ctrl-D 离开）。
 */
int main()
{
    char cmdline[MAXLINE]; /* Command line */

    while (1) {
	/* Read */
	printf("> ");                   
	Fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);

	/* Evaluate */
	eval(cmdline);
    } 
}
/* $end shellmain */
  
/* $begin eval */
/**
 * @brief 解析 cmdline 并执行对应的内建命令或外部程序。
 * @param cmdline 从 stdin 读到的一整行输入（已去掉行尾换行带来的副作用前）。
 *
 * @details
 *   实现流程：
 *     1. 用 parseline 解析为 argv，并判断是否后台任务
 *     2. 空行直接 return
 *     3. 如果是内建命令（builtin_command），由它直接处理
 *     4. 否则 fork：
 *        - 子进程：execve(argv[0], argv, environ)；失败打印并 exit(0)
 *        - 父进程：前台任务 waitpid 等待；后台任务打印 PID 后继续
 */
void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { 
	if ((pid = Fork()) == 0) {   /* Child runs user job */
	    if (execve(argv[0], argv, environ) < 0) {
		printf("%s: Command not found.\n", argv[0]);
		exit(0);
	    }
	}

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid, &status, 0) < 0)
		unix_error("waitfg: waitpid error");
	}
	else
	    printf("%d %s", pid, cmdline);
    }
    return;
}

/**
 * @brief 判断 argv[0] 是否是 shell 内建命令；如果是则执行它。
 * @param argv 由 parseline 生成的参数数组（argv[0] 是命令名）。
 * @return 1 表示已在内建里处理（quit 已 exit，或者单 '&' 忽略行）；
 *         0 表示不是内建命令，调用方应当继续走 fork + execve。
 *
 * @details
 *   当前实现的内建命令：
 *     - "quit"：调用 exit(0) 退出 shell（不会返回）
 *     - "&"  ：光秃秃的 '&'，直接当空行忽略
 */
int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit")) /* quit command */
	exit(0);  
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	return 1;
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
/**
 * @brief 把 buf 解析为以 NULL 结尾的 argv 数组。
 * @param buf  命令行字符串；本函数会就地修改（替换分隔符为 '\0'）。
 * @param argv 输出参数；解析后 argv[0] 是命令名，结尾是 NULL。
 * @return 1 表示后台任务（行尾有 '&'），0 表示前台任务；
 *         整行为空时也返回 1（这样 eval 可以用 "argv[0]==NULL" 判断空行）。
 *
 * @details
 *   主要步骤：
 *     1. 把末尾 '\n' 替换成空格
 *     2. 跳过开头连续空格
 *     3. 按空格切分 token，写入 argv
 *     4. 如最后一个参数是 '&'，把它去掉并把返回值设为 1（后台）
 */
int parseline(char *buf, char **argv)
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
	       buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */


