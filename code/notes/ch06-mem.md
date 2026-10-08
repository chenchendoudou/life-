# Ch6 · 存储器层次结构

## 📌 核心概念

- 局部性原理：**时间局部性**（最近访问的将来再用）、**空间局部性**（相邻地址的将来也用）
- 存储山（memory mountain）：以 size 和 stride 为轴的吞吐图
- 高速缓存组织：S=2^s 组、E=路数、B=2^b 块大小、m=地址位、t=tag 位、s=set index、b=block offset
- 读/写策略：写直达（write-through）+ 写缓冲、写回（write-back）
- 写分配 vs 非写分配
- LRU、FIFO 替换策略；i-cache / d-cache 分离
- 山谷利用：将工作集塞进 L1

## 🧠 地址划分

```
| t (tag) | s (set index) | b (block offset) |
```

## 🔑 例题归档

### 命中判断（待填）
### 替换与 miss 类型（待填）
### 矩阵转置（待填）

## 🐛 易错点

1. 容量 miss / 冲突 miss / 强制 miss 的区分
2. 步长（stride）= 1 才接近空间局部性最优
3. 同一个矩阵按行/按列遍历性能差几十倍

## 🧩 Lab 链接

- `labs/LAB_PROGRESS.md` · cachelab

## 💡 心得
