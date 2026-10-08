# CSAPP Lab 进度跟踪

> 跟踪 8 个 Lab 的完成度、得分、笔记位置。基线数据来源：`CMU-CSAPP-master/README.md` 时间线。

## 总览

| Lab | 章节 | 难度 | 状态 | 得分 | 笔记 |
|-----|------|------|------|------|------|
| datalab | Ch2 | ⭐ | ✅ 完成 | 100/100 | `notes/ch02-bits.md` |
| bomblab | Ch3 | ⭐⭐ | ✅ 完成 | 100/100 | `notes/ch03-asm.md` |
| attacklab | Ch3 | ⭐⭐⭐ | ⬜ 未开始 | — | `notes/ch03-asm.md` |
| archlab | Ch4 | ⭐⭐⭐ | 🚧 PartI+II 完成 | ~40/100 | `notes/ch04-arch.md` |
| cachelab | Ch6 | ⭐⭐ | ✅ 完成 | 100/100 | `notes/ch06-mem.md` |
| shlab (tsh) | Ch8 | ⭐⭐ | ✅ 完成 | 100/100 | `notes/ch08-ecf.md` |
| malloclab | Ch9 | ⭐⭐⭐⭐ | 🚧 缺 realloc | 53/100 | `notes/ch09-vm.md` |
| proxylab | Ch11 | ⭐⭐⭐ | ✅ 完成 | 100/100 | `notes/ch11-netp.md` |

## 各 Lab 详细记录

### datalab（Ch2 · 位级操作）
- 入口：`CMU-CSAPP-master/datalab/`
- 关键约束：仅允许 `~ & ^ | + << >>` 与 `!`，操作数 ≤ 5
- 难点：`isTmax / isLessOrEqual / trueFiveThreeEight / floatScale2 / floatFloat2Int`
- 🔑 例题：`isTmax(x)` — 见 `notes/ch02-bits.md`
- 心得：位运算的本质是**用算术识别位模式**

### bomblab（Ch3 · 反汇编拆炸弹）
- 入口：`CMU-CSAPP-master/bomblab/`
- 工具：gdb、objdump、strings、phase_*
- 6 个 phase + 1 个隐藏关
- 关键技能：栈帧解读、跳转表、switch 反汇编模式

### attacklab（Ch3 · 代码注入 + ROP）
- 入口：`CMU-CSAPP-master/attacklab/`
- Phase 1–3：缓冲区溢出注入代码
- Phase 4–5：ROP（ret2gadget）
- 前置：理解 rdi/rsp/rsi 寄存器、ret 指令如何衔接

### archlab（Ch4 · Y86-64 模拟器）
- 入口：`CMU-CSAPP-master/archlab/`
- PartA：用 HCL 写 SEQ/Y86-64 流水线寄存器与冒险处理
- PartB：写 iaddq 指令实现
- PartC：写 ncopy 函数并通过 ncopy.ys 优化版本在 pipeline 上跑分

### cachelab（Ch6 · Cache 模拟 + 矩阵优化）
- 入口：`CMU-CSAPP-master/cachelab/`
- PartA：写一个 cache 模拟器（csim.c）
- PartB：在 32×32 / 64×64 / 61×67 矩阵上实现转置并控制 miss 数
- 关键技巧：blocking（分块）、8×8、4×4 子块、变量映射到局部变量

### shlab (tshlab)（Ch8 · 简易 shell）
- 入口：`CMU-CSAPP-master/shlab/`
- 任务：实现 eval/builtin_cmd、sigchld_handler、sigint_handler、sigtstp_handler
- 关键点：waitpid 的 WNOHANG/WUNTRACED、jobs 状态机

### malloclab（Ch9 · 分配器）
- 入口：`CMU-CSAPP-master/malloclab/`
- 目标：写 malloc/free/realloc，追求 **util × throughput** 最高
- 主流方案：Segregated Free List + LIFO + Boundary Tags
- 当前进度：基础功能实现，realloc 缺

### proxylab（Ch11 · HTTP 代理）
- 入口：`CMU-CSAPP-master/proxylab/`
- PartI：单线程串行代理
- PartII：多线程/多进程并发代理
- PartIII：缓存（LRU 替换）
- 关键系统调用：socket/bind/listen/accept/connect/read/write/select/epoll

## 复盘清单

每完成一个 Lab，在笔记里记录：
- [ ] 题目目的与关键约束
- [ ] 关键设计决策与权衡
- [ ] 性能/正确性数据
- [ ] 踩过的坑
- [ ] 可重用的代码片段 / 模式
