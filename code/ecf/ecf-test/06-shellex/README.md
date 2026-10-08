# 06 — shellex：简易 shell（综合实战）

> CS:APP 第 8 章末尾的综合示例：用 fork + execve + signal 实现一个能跑命令的 shell。

---

## 这个 shell 支持什么

`shellex.c` 是一个约 100 行的简化 shell，提供：

- 打印提示符 `> `
- 读取一行输入
- 用空格分割成 argv
- `fork` 子进程 + `execve` 执行
- 父进程 `waitpid` 回收
- 响应 SIGINT（Ctrl+C）杀掉**前台**子进程，而不是 shell 本身

它**不**支持：
- 管道 `|`、重定向 `>`、后台作业 `&`
- 内建命令（cd, exit）
- 环境变量展开

要这些就得去看 bash / zsh 那种几千行的大项目了。

---

## 编译运行

```bash
cd /home/xj/chendou/life-/code/ecf/ecf-test
make 06-shellex/

./06-shellex/shellex

# 在提示符下输入：
> /bin/ls
> /bin/echo hello world
# Ctrl+C 不会杀 shell，只会杀掉正在跑的前台子进程
```

---

## 它用到了本章所有概念

| 概念 | 在 shellex.c 哪里出现 |
|------|----------------------|
| 异常控制流（fork + execve） | `main` 里的 eval 循环 |
| 同步等待（waitpid） | `waitfg()` 函数 |
| 信号（SIGINT + SIGCHLD） | `sigint_handler`、`sigchld_handler` |
| 信号屏蔽（sigprocmask） | 防止父/子竞争 |
| 陷阱（系统调用） | fork / execve / waitpid 全部是陷阱 |

读懂这个程序 ≈ 把整个第 8 章贯通。

---

## 推荐阅读顺序

1. 先看 `eval()`：shell 主循环
2. 再看 `builtin_command()`：区分内建 vs 外部
3. 然后看 `waitfg()`：父怎么阻塞等子
4. 最后看两个 handler：信号让 Ctrl+C 不杀 shell

注意 handler 里调用 `Waitpid` 时用 `WNOHANG | WUNTRACED`，是因为 handler 可能在子进程已经子退出之后才被调度，**绝不能阻塞**。