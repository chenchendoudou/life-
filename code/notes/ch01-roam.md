# Ch1 · 计算机系统漫游

## 📌 核心概念

- hello.c 的 7 个生命周期阶段：源程序 → 预处理 → 编译 → 汇编 → 链接 → 加载 → 执行
- 硬件组成：总线、主存、I/O 设备、处理器
- 存储层次：寄存器 → L1/L2/L3 → 主存 → 磁盘 → 网络
- 操作系统抽象：进程、虚拟内存、文件
- 网络：客户端-服务器模型，TCP/IP

## 🧠 关键命令

```bash
gcc -E hello.c -o hello.i     # 预处理
gcc -S hello.i -o hello.s     # 编译到汇编
gcc -c hello.s -o hello.o     # 汇编到目标文件
gcc hello.o -o hello          # 链接
./hello                       # 加载并执行
ldd hello                     # 查看动态库依赖
strace ./hello                # 跟踪系统调用
```

## 🐛 易错点

1. `#include <stdio.h>` 的尖括号与引号搜索路径不同
2. 静态库 `.a` 与动态库 `.so` 的链接时机不同
3. `-O0` / `-O2` / `-O3` 的优化等级对生成代码影响巨大

## 💡 心得
