# 05 — setjmp / longjmp：非局部跳转

> CS:APP 8.6 "Nonlocal Jumps"
> 在 C 里实现类似异常的机制，从深层嵌套直接跳出。

---

## 用法

```c
#include <setjmp.h>
int setjmp(jmp_buf env);             // 保存当前环境
void longjmp(jmp_buf env, int val);  // 跳回 setjmp 处
```

- `setjmp(env)` 直接返回 0
- 之后 `longjmp(env, val)` 会让 `setjmp(env)` 再次"返回"，但返回值是 `val`（不能是 0）

典型用途：
- 错误处理（从深层嵌套直接返回最外层）
- 协同程序（coroutines）
- 在信号处理里安全跳出

---

## 本目录文件

| 文件 | 说明 |
|------|------|
| `setjmp.c` | 教学示例：在嵌套函数里 longjmp 跳回 main |

---

## 编译运行

```bash
cd /home/xj/chendou/life-/code/ecf/ecf-test
make 05-setjmp/

./05-setjmp/setjmp
```

---

## ⚠️ 陷阱

- 跳过的栈帧里的自动变量如果是 `volatile` 的还好，否则优化器可能把变量放在寄存器里，跳回来时值"过时"
- 千万不要让 `longjmp` 跳到一个已经返回的函数（栈帧已销毁）
- 信号处理里 `longjmp` 出来很危险，可能让程序处于错误的同步状态