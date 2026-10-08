# 04 — signal：信号处理

> CS:APP 8.5 "Signals"
> 信号是**软件层面的中断**：内核把事件投递到进程，进程可以选择处理、忽略或接受默认行为。

---

## 信号三要素

1. **发送**：`kill(pid, sig)`，内核把信号加入进程的位图
3. **接收**：当内核把进程从内核态切回用户态时，检查待处理信号
4. **处理**：
   - 默认行为（terminate / core dump / stop / ignore）
   - 自定义 handler（通过 `signal` / `sigaction` 注册）
   - 忽略（SIGKILL / SIGSTOP 不可忽略/捕获）

---

## 本目录文件

| 文件 | 主题 |
|------|------|
| `signal1.c` | signal 的最简形式 |
| `signal2.c` | sigaction vs signal 的差异 |
| `signal3.c` | 子进程继承父进程的 handler 吗？ |
| `signal4.c` | 阻塞信号 vs 待处理信号 |
| `signalprob0.c` | 思考题：信号与 fork 的协作 |
| `sigint1.c` | SIGINT（Ctrl+C）处理 |
| `kill.c` | `kill()` 显式发送信号 |
| `alarm.c` | `alarm()` 定时发送 SIGALRM |
| `procmask1.c` | sigprocmask 屏蔽信号的简单演示 |
| `procmask2.c` | sigprocmask 阻塞/解除阻塞的复杂演示 |
| `counterprob.c` | 思考题：信号处理中的全局计数器 |
| `restart.c` | SA_RESTART：慢系统调用被信号打断后自动重启 |

---

## 编译运行

```bash
cd /home/xj/chendou/life-/code/ecf/ecf-test
make 04-signal/

# 看一个最简单的 signal handler
./04-signal/signal1
# 在另一个终端用： kill -INT <pid>

# Ctrl+C 演示
./04-signal/sigint1
# 按 Ctrl+C，会触发 handler 而不是结束

# 父子程序：父用 kill() 杀子
./04-signal/kill

# 5 秒后自动 SIGALRM
./04-signal/alarm
```

---

## 核心心法

- **信号是异步的** — handler 可能在任意指令之间被调用
- **同步等待 + 异步信号** 是经典编程陷阱，必须用 `sigsuspend` 或 `sigprocmask` 协调
- **SIGKILL / SIGSTOP 不可捕获也不可忽略** — 这是"最后手段"
- `csapp.h` 的 `Signal()` 包装默认带 `SA_RESTART`，慢系统调用被打断会自动重启

---

## 信号处理执行流（陷阱视角）

```
用户进程执行 read()
   ↓
syscall 指令 → 进入内核
   ↓
内核检查是否有信号待处理
   ↓
有 SIGINT → 内核切回用户态，但不是回到 read() 后
           → 而是跳转到用户预设的 handler
   ↓
handler 执行完 → 再回到原 read() 之后（如果有 SA_RESTART 则从 read 重试）
```

所以**信号处理本身就是一种"陷阱"在内核态与用户态之间的异常跳转**。