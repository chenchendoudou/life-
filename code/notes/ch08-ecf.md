# Ch8 · 异常控制流（ECF）

## 📌 核心概念

- 异常类型：中断（异步）/ 陷阱（同步, syscall）/ 故障（可恢复, page fault）/ 终止（不可恢复, 硬件错）
- 进程：私有地址空间 + 上下文切换
- fork / execve / waitpid / exit
- 信号：发送（kill/raise）、接收（默认/忽略/handler）、阻塞
- signal/sigaction 语义差异（System V vs BSD）
- 非本地跳转：setjmp/longjmp
- 进程组、作业控制、终端

## 🧠 速查

```c
pid_t fork(void);
int execve(const char *path, char *const argv[], char *const envp[]);
pid_t waitpid(pid_t pid, int *status, int options);
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
```

## 🔑 例题归档

### fork 题目集合（待填）
### 信号处理（待填）
### shlab tsh（待填）

## 🐛 易错点

1. fork 后父子进程**都**从 fork 返回；返回值区分
2. 信号处理函数中调用异步信号不安全的函数 = UB
3. waitpid 需循环处理 + WNOHANG/WUNTRACED 选项
4. SIGCHLD 默认是 SIG_IGN，子进程会变僵尸

## 🧩 Lab 链接

- `labs/LAB_PROGRESS.md` · shlab (tsh)

## 💡 心得
