# Ch3 · 程序的机器级表示

## 📌 核心概念

- x86-64 寄存器组：通用寄存器（%rax/%rbx/%rcx/%rdx/%rsi/%rdi/%rsp/%rbp/%r8–%r15）、条件码寄存器
- 寻址模式：立即数、寄存器、内存（基址+偏移+索引+比例）
- 数据传送指令：mov、movz、movs
- 算术与逻辑：add/sub/inc/dec/imul/neg/and/or/xor/not/shl/sar/sal
- 条件码：CF/ZF/SF/OF
- 控制流：cmp/test + jX（je/jne/jl/jle/jg/jge/js/jns）、jmp
- 过程调用：call/ret、栈帧、保存的寄存器
- 数据布局：数组/struct/union 的内存对齐

## 🧠 速查表

| 模式 | 例 | 含义 |
|------|----|------|
| 立即数 | `$0x10` | 常量 |
| 寄存器 | `%rax` | 64-bit |
| 间接 | `(%rax)` | `[rax]` |
| 偏移 | `8(%rax)` | `[rax+8]` |
| 比例 | `(%rax,%rcx,4)` | `[rax+4*rcx]` |

## 🔑 例题归档

### 栈帧示例（待填）
### switch 跳转表（待填）
### 结构体对齐（待填）

## 🐛 易错点

1. `%rsp` 必须 16 字节对齐（在 call 前）
2. 算术右移 vs 逻辑右移（sar vs shr）
3. 字节序：小端（little-endian）x86
4. 编译器可能会把局部变量存在寄存器而非内存

## 🧩 Lab 链接

- `labs/LAB_PROGRESS.md` · bomblab / attacklab

## 💡 心得
