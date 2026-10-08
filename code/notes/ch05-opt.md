# Ch5 · 优化程序性能

## 📌 核心概念

- CPE（Cycles Per Element）：每元素周期数
- 度量性能：wall-clock time、user time、system time
- 优化方法：消除循环的低效率、减少过程调用、消除不必要的内存引用
- 循环展开（unrolling）：减少分支、增加并行
- 提升（promotion）：把不变计算提到循环外
- 并行累积（parallel accumulation）：多个独立累加器最后合并
- 重新结合变换：`(a+b)+c → a+(b+c)` 改变并行度
- 内存访问的局部性：步长、块大小、对齐

## 🧠 速查

```bash
gcc -O2 prog.c -o prog
perf stat ./prog
```

## 🔑 例题归档

### 指针 vs 数组下标（待填）
### 矩阵乘（待填）
### 循环展开 k×k（待填）

## 🐛 易错点

1. **优化仅在 -O1+ 生效**，-O0 编译看到的代码可能完全不是性能特征
2. 不要优化编译器已经做得很好的事
3. 测量！CPE 公式 `T = CPE × n + 启动开销`
4. 内存别名假设（restrict 关键字）影响优化

## 💡 心得
