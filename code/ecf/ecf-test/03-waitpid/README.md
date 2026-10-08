# 03 — waitpid：进程回收

> CS:APP 8.4 "Process Control" — 父进程回收已终止的子进程，避免僵尸。

---

## 为什么要 waitpid

`fork` 之后，子进程终止时不会**自动消失**，而是变成**僵尸进程（zombie）**
保留在内核的进程表里，直到父进程调用 `wait`/`waitpid` 读取它的退出状态。

- 不回收 → 进程表占满 → 之后无法再创建新进程
- 父进程已死但子进程还在 → 子进程被 init (PID=1) 收养，由 init 清理

---

## wait / waitpid 用法速记

```c
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *statusp, int options);
```

- `pid > 0`  →  等待指定子进程
- `pid == -1`→  等待**任一**子进程（最常用）
- `pid == 0` →  等待与当前进程同组的任一子进程
- `pid < -1` →  等待指定进程组

常用 options：
- `0`          → 默认（阻塞）
- `WNOHANG`    → 没子进程退出就立刻返回 0
- `WUNTRACED`  → 也报告停止的子进程
- `WCONTINUED` → 也报告继续运行的子进程

status 编码：
- `WIFEXITED(status)`  → 正常退出，用 `WEXITSTATUS(status)` 取退出码
- `WIFSIGNALED(status)`→ 被信号终止，用 `WTERMSIG(status)` 取信号
- `WIFSTOPPED(status)` → 停止，用 `WSTOPSIG(status)` 取信号
- `WIFCONTINUED(status)`→ 继续

---

## 本目录文件

| 文件 | 说明 |
|------|------|
| `waitpid1.c` | 教学版：父 fork 后 waitpid 回收 |
| `waitpid2.c` | waitpid 配合 WNOHANG 演示 |
| `waitprob0.c` | 思考题：父子谁先终止？ |
| `waitprob1.c` | 思考题：exit 码能否传到父进程？ |
| `waitprob3.c` | 思考题：status 的解码 |

---

## 编译运行

```bash
cd /home/xj/chendou/life-/code/ecf/ecf-test
make 03-waitpid/

./03-waitpid/waitpid1
./03-waitpid/waitpid2
```

---

## 关键洞察

- `waitpid` 本身是个**系统调用**（陷阱），会阻塞直到有子进程终止或被停止
- 如果没有子进程且 `pid == -1`，会失败并设 `errno = ECHILD`
- `csapp.h` 里包装好的 `Waitpid` 把 ECHILD 当成"成功"——已经没有子进程了，回收完毕