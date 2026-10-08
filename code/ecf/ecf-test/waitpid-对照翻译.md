# waitpid.txt 对照翻译

> 📖 原文：BSD 系统调用手册 `wait(2)`（wait, wait3, wait4, waitpid）
> 🎯 配套：CS:APP 第 8 章「异常控制流」
> 📐 格式：中英逐段对照

---

## NAME（名称）

| 原文 | 译文 |
|---|---|
| wait, wait3, wait4, waitpid – wait for process termination | wait, wait3, wait4, waitpid —— 等待进程终止 |

## SYNOPSIS（概要 · 函数原型）

```c
#include <sys/wait.h>

pid_t
wait(int *stat_loc);

pid_t
wait3(int *stat_loc, int options, struct rusage *rusage);

pid_t
wait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage);

pid_t
waitpid(pid_t pid, int *stat_loc, int options);
```

## DESCRIPTION（描述）

| 原文 | 译文 |
|---|---|
| The wait() function suspends execution of its calling process until stat_loc information is available for a terminated child process, or a signal is received. | wait() 函数会挂起（暂停）调用进程的执行，直到某个已终止的子进程的 stat_loc 状态信息可用，或者收到一个信号为止。 |
| On return from a successful wait() call, the stat_loc area contains termination information about the process that exited as defined below. | 从一次成功的 wait() 调用返回后，stat_loc 所指向的区域将包含下面定义的、关于已退出进程的终止信息。 |
| The wait4() call provides a more general interface for programs that need to wait for certain child processes, that need resource utilization statistics accumulated by child processes, or that require options. | wait4() 调用为那些需要等待特定子进程、需要子进程累积的资源使用统计、或需要使用选项的程序，提供了更为通用的接口。 |
| The other wait functions are implemented using wait4(). | 其余几个 wait 函数都是用 wait4() 实现的。 |
| The pid parameter specifies the set of child processes for which to wait. | pid 参数指明要等待的那组子进程。 |
| If pid is -1, the call waits for any child process. | 若 pid 为 -1，则等待任意一个子进程。 |
| If pid is 0, the call waits for any child process in the process group of the caller. | 若 pid 为 0，则等待与调用者同属一个进程组的任意一个子进程。 |
| If pid is greater than zero, the call waits for the process with process id pid. | 若 pid 大于 0，则等待进程 ID 恰好等于 pid 的那个进程。 |
| If pid is less than -1, the call waits for any process whose process group id equals the absolute value of pid. | 若 pid 小于 -1，则等待任何一个进程组 ID 等于 pid 绝对值（|pid|）的进程。 |
| The stat_loc parameter is defined below. The options parameter contains the bitwise OR of any of the following options. | stat_loc 参数在下面定义。options 参数是下列选项的按位或（bitwise OR）。 |
| The WNOHANG option is used to indicate that the call should not block if there are no processes that wish to report status. | WNOHANG 选项用于指明：当没有进程需要报告状态时，调用不应阻塞。 |
| If the WUNTRACED option is set, children of the current process that are stopped due to a SIGTTIN, SIGTTOU, SIGTSTP, or SIGSTOP signal also have their status reported. | 若设置了 WUNTRACED 选项，则当前进程那些因 SIGTTIN、SIGTTOU、SIGTSTP 或 SIGSTOP 信号而停止的子进程，也会被报告其状态。 |
| If rusage is non-zero, a summary of the resources used by the terminated process and all its children is returned (this information is currently not available for stopped processes). | 若 rusage 非零，则返回已终止进程及其所有子进程的资源使用情况汇总（该信息目前对已停止的进程不可用）。 |
| When the WNOHANG option is specified and no processes wish to report status, wait4() returns a process id of 0. | 当指定了 WNOHANG 选项且没有进程需要报告状态时，wait4() 返回进程 ID 0。 |
| The waitpid() call is identical to wait4() with an rusage value of zero. | waitpid() 调用与 rusage 取零时的 wait4() 完全相同。 |
| The older wait3() call is the same as wait4() with a pid value of -1. | 较老的 wait3() 调用与 pid 取 -1 时的 wait4() 相同。 |

## 退出状态判断宏（原文 61–93 行）

| 原文 | 译文 |
|---|---|
| The following macros may be used to test the manner of exit of the process. One of the first three macros will evaluate to a non-zero (true) value: | 下列宏可用来检测进程的退出方式。前三个宏中必有一个求值为非零（真）： |
| **WIFEXITED(status)** — True if the process terminated normally by a call to _exit(2) or exit(3). | **WIFEXITED(status)** —— 若进程通过调用 _exit(2) 或 exit(3) 而正常终止，则为真。 |
| **WIFSIGNALED(status)** — True if the process terminated due to receipt of a signal. | **WIFSIGNALED(status)** —— 若进程因收到信号而终止，则为真。 |
| **WIFSTOPPED(status)** — True if the process has not terminated, but has stopped and can be restarted. This macro can be true only if the wait call specified the WUNTRACED option or if the child process is being traced (see ptrace(2)). | **WIFSTOPPED(status)** —— 若进程尚未终止，但已停止且可以重新启动，则为真。只有当 wait 调用指定了 WUNTRACED 选项、或该子进程正被跟踪（参见 ptrace(2)）时，此宏才可能为真。 |
| Depending on the values of those macros, the following macros produce the remaining status information about the child process: | 根据上述宏的取值，下列宏将给出关于子进程的其余状态信息： |
| **WEXITSTATUS(status)** — If WIFEXITED(status) is true, evaluates to the low-order 8 bits of the argument passed to _exit(2) or exit(3) by the child. | **WEXITSTATUS(status)** —— 若 WIFEXITED(status) 为真，则求值为子进程传给 _exit(2) 或 exit(3) 的参数的**低 8 位**（即退出码）。 |
| **WTERMSIG(status)** — If WIFSIGNALED(status) is true, evaluates to the number of the signal that caused the termination of the process. | **WTERMSIG(status)** —— 若 WIFSIGNALED(status) 为真，则求值为导致进程终止的那个信号的编号。 |
| **WCOREDUMP(status)** — If WIFSIGNALED(status) is true, evaluates as true if the termination of the process was accompanied by the creation of a core file containing an image of the process when the signal was received. | **WCOREDUMP(status)** —— 若 WIFSIGNALED(status) 为真，且进程终止时还生成了 core（核心转储）文件（内含收到信号时进程的内存映像），则求值为真。 |
| **WSTOPSIG(status)** — If WIFSTOPPED(status) is true, evaluates to the number of the signal that caused the process to stop. | **WSTOPSIG(status)** —— 若 WIFSTOPPED(status) 为真，则求值为导致进程停止的那个信号的编号。 |

## NOTES（注释）

| 原文 | 译文 |
|---|---|
| See sigaction(2) for a list of termination signals. A status of 0 indicates normal termination. | 关于终止信号的列表，参见 sigaction(2)。退出状态值 0 表示正常终止。 |
| If a parent process terminates without waiting for all of its child processes to terminate, the remaining child processes are assigned the parent process 1 ID (the init process ID). | 若父进程在未等待其所有子进程终止的情况下就终止了，则剩下的子进程会被重新指定父进程 ID 为 1（即 init 进程的 ID）。 |
| If a signal is caught while any of the wait() calls is pending, the call may be interrupted or restarted when the signal-catching routine returns, depending on the options in effect for the signal; see intro(2), System call restart. | 若在任一 wait() 调用挂起期间捕获到信号，则该调用可能在信号处理例程返回时被中断或重新启动，具体取决于该信号生效的选项；参见 intro(2)（系统调用重启）。 |

## RETURN VALUES（返回值）

| 原文 | 译文 |
|---|---|
| If wait() returns due to a stopped or terminated child process, the process ID of the child is returned to the calling process. Otherwise, a value of -1 is returned and errno is set to indicate the error. | 若 wait() 因某个子进程停止或终止而返回，则向调用进程返回该子进程的进程 ID。否则返回 -1，并设置 errno 以指明错误。 |
| If wait3(), wait4(), or waitpid() returns due to a stopped or terminated child process, the process ID of the child is returned to the calling process. If there are no children not previously awaited, -1 is returned with errno set to [ECHILD]. Otherwise, if WNOHANG is specified and there are no stopped or exited children, 0 is returned. If an error is detected or a caught signal aborts the call, a value of -1 is returned and errno is set to indicate the error. | 若 wait3()、wait4() 或 waitpid() 因某个子进程停止或终止而返回，则向调用进程返回该子进程的进程 ID。若不存在尚未等待过的子进程，则返回 -1 并将 errno 设为 [ECHILD]。否则，若指定了 WNOHANG 且没有已停止或已退出的子进程，则返回 0。若检测到错误、或捕获到的信号使调用中止，则返回 -1 并设置 errno 以指明错误。 |

## ERRORS（错误）

| 原文 | 译文 |
|---|---|
| The wait() system call will fail and return immediately if: | 在下列情况下，wait() 系统调用将失败并立即返回： |
| [ECHILD] The calling process has no existing unwaited-for child processes. | [ECHILD] 调用进程没有尚存的、未等待过的子进程。 |
| [EFAULT] The status or rusage argument points to an illegal address (may not be detected before the exit of a child process). | [EFAULT] status 或 rusage 参数指向非法地址（可能要到子进程退出之后才会被检测到）。 |
| [EINVAL] Invalid or undefined flags are passed in the options argument. | [EINVAL] 在 options 参数中传入了无效或未定义的标志。 |
| The wait3() and waitpid() calls will fail and return immediately if: | 在下列情况下，wait3() 和 waitpid() 调用将失败并立即返回： |
| [ECHILD] The process specified by pid does not exist or is not a child of the calling process, or the process group specified by pid does not exist or does not have any member process that is a child of the calling process. | [ECHILD] pid 指定的进程不存在、或不是调用进程的子进程；或者 pid 指定的进程组不存在、或不包含任何属于调用进程子进程的成员进程。 |
| The waitpid() call will fail and return immediately if: | 在下列情况下，waitpid() 调用将失败并立即返回： |
| [EINVAL] The options argument is not valid. | [EINVAL] options 参数无效。 |
| Any of these calls will fail and return immediately if: | 在下列情况下，上述任一调用都将失败并立即返回： |
| [EINTR] The call is interrupted by a caught signal or the signal does not have the SA_RESTART flag set. | [EINTR] 调用被捕获到的信号中断，或者该信号未设置 SA_RESTART 标志。 |

## STANDARDS（标准）

| 原文 | 译文 |
|---|---|
| The wait() and waitpid() functions are defined by POSIX; wait3() and wait4() are not specified by POSIX. The WCOREDUMP() macro and the ability to restart a pending wait() call are extensions to the POSIX interface. | wait() 和 waitpid() 函数由 POSIX 定义；wait3() 和 wait4() 不在 POSIX 规范中。WCOREDUMP() 宏、以及重启一个挂起的 wait() 调用的能力，是对 POSIX 接口的扩展。 |

## LEGACY SYNOPSIS（旧式概要）

```c
#include <sys/types.h>
#include <sys/wait.h>
```

| 原文 | 译文 |
|---|---|
| The include file \<sys/types.h\> is necessary. | 头文件 \<sys/types.h\> 是必需的。 |

## SEE ALSO（另见）

| 原文 | 译文 |
|---|---|
| sigaction(2), exit(3), compat(5) | sigaction(2), exit(3), compat(5) |

## HISTORY（历史）

| 原文 | 译文 |
|---|---|
| A wait() function call appeared in Version 6 AT&T UNIX. | wait() 函数调用最早出现于第 6 版 AT&T UNIX。 |