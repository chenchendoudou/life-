# 02 — fork：进程创建

> CS:APP 8.4 "Process Control"
> 核心现象：**"一次调用，两次返回"** — 在同一指令处，父亲和儿子各自返回。

---

## fork 的关键事实

```c
pid_t pid = fork();
```

- `pid` 在**子进程**里 = `0`
- `pid` 在**父进程**里 = 子进程的 PID（正数）
- 出错时 = `-1`
- **父子都从 fork 调用之后的下一条语句继续执行**
- 父子拥有**几乎相同**的地址空间副本（写时复制 COW）
- 子进程获得父进程所有打开的**文件描述符副本**

---

## 本目录文件

| 文件 | 说明 |
|------|------|
| `fork_basic.c` | 教学版，带详细中文注释 |
| `forkprob0.c` | 父子执行顺序：if 分支后还有一行 printf，谁先谁后？ |
| `forkprob1.c` | for 循环里 fork() → 产生多少个进程？ |
| `forkprob2.c` | atexit 注册的函数会随子进程一起执行 |
| `forkprob3.c` | 子进程的 `++x` / `--x` 是否影响父进程？ |
| `forkprob4.c` | `doit()` 里两次 fork 产生多少 "hello\n"？ |
| `forkprob5.c` | if 分支内的 fork + exit → 子子进程是不是孤儿？ |
| `forkprob6.c` | 对比 forkprob5：子子进程没有 exit，靠 return |
| `forkprob7.c` | 全局变量 `counter` 在 fork 后是否共享？ |
| `forkprob8.c` | 命令行参数 n，循环 n 次 fork，能产生 2ⁿ 个进程？ |

---

## 编译运行

```bash
cd /home/xj/chendou/life-/code/ecf/ecf-test
make

# 教学版
./02-fork/fork_basic

# 跑全部 forkprob，输出顺序每次可能不同（调度相关）
for p in 02-fork/forkprob{0..8}; do
    echo "=== $p ==="; ./$p
done

# forkprob8 接受 n
./02-fork/forkprob8 4
```

---

## 核心心法

`fork` 是一个**陷阱**（系统调用），所以：
1. 进入内核 → 创建子进程 → 返回**两次**
2. 子进程**几乎**是父进程的完整副本，但内存是独立的（COW）
3. 因此 `printf("x=%d\n", x)` 在子进程里 `++x` 不会影响父进程的 `x`

---

## 反汇编验证

```bash
objdump -d 02-fork/fork_basic | grep syscall
```

你会发现 `fork` 内部也是 `syscall` 指令（陷阱体现成 SYS_clone/SYS_fork 系统调用）。

---

## 思考题答案线索

- **forkprob0**：父进程走到 `printf2` 时 x=0，子进程走到 `printf1` 时 x=2，父子都执行了 `printf2`（x=0）
- **forkprob1**：1 次 fork → 2 进程；2 次 fork → 4 进程 → 4 个 "hello"
- **forkprob2**：`atexit` 注册的清理函数会随 fork 复制到子进程，每个进程 exit 时都执行
- **forkprob3**：父子各打印一次，子 x=1，父 x=3（基于各自副本）
- **forkprob4**：`doit()` 内两次 fork 产生 4 个进程，各打印一次；外加 main 末尾 1 次 → 共 5 个 "hello"
- **forkprob5**：子进程里又 fork 一次，孙子直接 exit 不回外层；只剩 2 个 "hello"
- **forkprob6**：vs forkprob5，孙子**没有 exit**，return 后外层**再次执行** printf → 4 个 "hello"
- **forkprob7**：父子 counter 互不影响，所以父进程 Wait 后 counter = 2
- **forkprob8**：循环 n 次 fork 产生 2ⁿ 个进程，每个都打印一次