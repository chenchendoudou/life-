# 01 — 陷阱（Trap）与系统调用（System Call）

> **核心回答**：陷阱是**机制**，系统调用是**目的**。
> 系统调用通过陷阱实现，但陷阱本身是个更广义的术语。

---

## 概念梳理

CS:APP 把异常（exception）分为 4 类：

| 类别 | 触发原因 | 同步性 | 行为 |
|------|----------|--------|------|
| 中断（Interrupt） | 外部 I/O 设备 | 异步 | 下一条指令 |
| **陷阱（Trap）** | **故意的**同步事件 | 同步 | **下一条指令** |
| 故障（Fault） | 可恢复错误 | 同步 | 可能重试当前指令 |
| 中止（Abort） | 致命错误 | 同步 | 不返回 |

**系统调用 = 应用主动使用陷阱进入内核**。

- **陷阱**：怎么"过去"的（机制）
- **系统调用**：过去"干什么"的（目的）

---

## 本目录的实证

| 文件 | 证明 |
|------|------|
| `demo_getpid.c` | `getpid()` 在汇编里就是 `syscall` 指令 |
| `demo_forkasm.c` | `fork()` 同样落到 `syscall` |

反汇编命令（手动复现）：
```bash
objdump -d demo_getpid | sed -n '/<main>:/,/^$/p'
objdump -d /lib/x86_64-linux-gnu/libc.so.6 | sed -n '/<syscall>:/,/^$/p'
```

你应该能看到：
```asm
mov    $0x27,%edi        ; 系统调用号 = SYS_getpid (39)
call   syscall@plt
...
syscall                  ; 0f 05 ←←← 这就是陷阱指令！
cmp    $0xfffffffffffff001,%rax
jae    ...               ; 检查错误
ret
```

---

## 关键洞察

- `syscall`（x86-64）/ `int $0x80`（x86-32）/ `svc`（ARM） 都是**陷阱指令**
- CPU 执行时从**用户模式**（Ring 3）切到**内核模式**（Ring 0）
- 内核根据 `rax` 中的系统调用号查表找到对应服务函数
- 完成后切回用户态，**返回 `syscall` 之后的下一条指令**

注意：**不是所有陷阱都是系统调用**：
- 缺页（page fault）→ 故障 → 不是系统调用
- 除以 0 → 故障 → 不是系统调用
- 键盘 `Ctrl+C` → 中断（不是 CPU 执行trap）→ 触发 SIGINT 信号

---

## 编译运行

```bash
cd /home/xj/chendou/life-/code/ecf/ecf-test
make 01-syscall-trap/demo_getpid
./01-syscall-trap/demo_getpid

# 看 syscall 指令在哪里
objdump -d 01-syscall-trap/demo_getpid | sed -n '/<main>:/,/^$/p'

# 跟踪所有发生的系统调用
strace ./01-syscall-trap/demo_getpid
```

---

## 思考题

- 把 `demo_getpid.c` 中 `$0x27`（SYS_getpid）改成 `$0x1`（SYS_write），看是否真的调用了 write
- 用 `strace ./demo_getpid` 观察实际的 `getpid()` 系统调用