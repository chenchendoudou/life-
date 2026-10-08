# CS:APP 学习计划

> 系统学习《深入理解计算机系统》（CS:APP, 3/E）的逐章路线与节奏建议。

## 学习节奏建议

- **总投入**：CSAPP 一遍 8–12 周比较稳妥（每章 3–5 天 + Lab）
- **每日**：阅读教材 1.5h + 课后习题 0.5h + 写代码 1h
- **每周**：完成一个 Lab 的一个阶段
- **关键**：**动手 > 看书**。每个示例都要自己编译运行一次

## 分章学习清单

### Ch1 计算机系统漫游（0.5 天）
- [ ] 通读章节，理解 hello.c 的生命周期
- [ ] 梳理：源程序 → 预处理 → 编译 → 汇编 → 链接 → 加载 → 执行 → 退出
- [ ] 命令汇总：`gcc -E / -S / -c / objdump / ldd / strace`

### Ch2 信息的表示和处理（3–4 天）
- [ ] 进制、二进制补码、反码、移位
- [ ] IEEE 754 浮点数（规格化/非规格化/NaN/Inf）
- [ ] 整数运算的溢出与环绕
- [ ] 课后习题
- [ ] **Lab: datalab**（位级操作 15 道题，限操作符）
- [ ] 🔑 例题归档：`isTmax(x)` 解答

### Ch3 程序的机器级表示（4–5 天）
- [ ] x86-64 寄存器、寻址方式
- [ ] 数据传送、算术、逻辑、控制（条件/循环/跳转表）
- [ ] 过程调用（栈帧结构）
- [ ] 数据/过程在内存中的布局
- [ ] **Lab: bomblab**（6 个炸弹 + 隐藏关）
- [ ] **Lab: attacklab**（栈溢出 / ROP）

### Ch4 处理器体系结构（4–5 天）
- [ ] Y86-64 指令集
- [ ] SEQ 顺序实现 → PIPE 流水线
- [ ] 数据冒险、控制冒险的解决
- [ ] **Lab: archlab**（PartA 写 SEQ / PartB 写 iaddq 流水线 / PartC 优化）

### Ch5 优化程序性能（2–3 天）
- [ ] CPE、循环展开、并行累积
- [ ] 内存访问模式对性能的影响
- [ ] 理解 `gcc -O2 / -O3` / `-march=native`

### Ch6 存储器层次结构（3–4 天）
- [ ] 局部性原理（时间/空间）
- [ ] 高速缓存组织（直接/组相联/全相联）
- [ ] 写策略、写回 vs 写直达
- [ ] **Lab: cachelab**（PartA 模拟 cache / PartB 矩阵转置优化）

### Ch7 链接（2–3 天）
- [ ] 静态链接、ELF 节、重定位
- [ ] 动态链接、共享库、位置无关代码 PIC
- [ ] 动手：`objdump -d / readelf / nm / ldd`

### Ch8 异常控制流（3–4 天）
- [ ] 进程、fork/exec/waitpid
- [ ] 信号：发送/接收/处理、signal/sigaction
- [ ] 非本地跳转：setjmp/longjmp
- [ ] **Lab: shlab (tshlab)**——实现一个简易 shell

### Ch9 虚拟内存（4–5 天）
- [ ] 地址翻译、TLB、多级页表
- [ ] 内存映射（mmap）、私有 vs 共享
- [ ] 动态内存分配器：隐式空闲链表、显式空闲链表、分离空闲链表
- [ ] **Lab: malloclab**（写一个 malloc 分配器，追求吞吐+利用率）

### Ch10 系统级 I/O（2 天）
- [ ] 文件描述符、open/read/write/close
- [ ] RIO（Robust I/O）包
- [ ] 标准 I/O 与系统 I/O 的对比

### Ch11 网络编程（3–4 天）
- [ ] 客户端-服务器模型、套接字接口
- [ ] Web 服务器、HTTP 协议
- [ ] **Lab: proxylab**（并发代理服务器，PartA 串行 / PartB 并发 / PartC 缓存）

### Ch12 并发编程（3–4 天）
- [ ] 基于进程的并发、基于线程的并发
- [ ] 共享变量、信号量、互斥锁
- [ ] 线程安全、死锁、可重入性
- [ ] 性能与正确性权衡

## 工具链速查

```bash
# 查看文件类型、节信息
file / readelf -h / readelf -S

# 反汇编
objdump -d a.out
objdump -d -M intel a.out        # Intel 语法

# 性能分析
gcc -O2 -pg && ./a.out && gprof a.out
perf stat / perf record

# 内存调试
valgrind --tool=memcheck ./a.out
valgrind --tool=callgrind ./a.out

# 链接分析
ldd a.out
nm a.out | grep T                # 全局符号
```

## 学习方法

1. **不要光看不练**：每章至少编译运行 5 个示例
2. **善用 objdump**：把每段 C 代码反汇编对照看
3. **重视 Lab**：Lab 是真正把概念固化的唯一方式
4. **笔记要图示化**：栈帧、流水线、缓存结构多画图
5. **复盘错题**：课后习题错了的，回到笔记里加一笔

## 与其他材料的协同

- `CMU-CSAPP-master/`：含全部 7 个 Lab 的 handout 与笔记
- `code/`：按章节组织的代码练习
- `ostep-code-master/` / `ostep-homework-master/`：OSTEP 操作系统三件套，与 Ch8/Ch9 互补
- `red_green/`：个人笔记与反思
