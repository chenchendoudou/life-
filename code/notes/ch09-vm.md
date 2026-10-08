# Ch9 · 虚拟内存

## 📌 核心概念

- 虚拟地址 → 物理地址的翻译：页表 + TLB
- 多级页表（4 级：PGD/PUD/PMD/PTE）
- TLB：MMU 内部的高速缓存；i-TLB / d-TLB
- 缺页（page fault）处理：换入 vs 换出
- 替换策略：LRU 近似（Clock/二次机会）
- 内存映射（mmap）：私有（copy-on-write）vs 共享
- fork 的写时复制（COW）
- execve 的内存段映射
- 动态内存分配器：隐式空闲链表、显式空闲链表、分离空闲链表、伙伴系统
- 内部碎片 vs 外部碎片
- 合并（boundary tags）、分割、放置策略（First/Best/Next Fit）

## 🧠 速查

```c
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);
int mprotect(void *addr, size_t len, int prot);
```

## 🔑 例题归档

### 隐式空闲链表（待填）
### 显式空闲链表（待填）
### 分离空闲链表（待填）
### malloclab 实践（待填）

## 🐛 易错点

1. 分配器返回的指针必须 8/16 字节对齐（双字对齐）
2. 不要把 payload 起始的 prev_size 当作可丢弃的（free 时它属于 footer）
3. 合并要检查前/后是否都是 free
4. realloc 实现：原地扩展 / 分配-拷贝-释放

## 🧩 Lab 链接

- `labs/LAB_PROGRESS.md` · malloclab

## 💡 心得
