# Ch12 · 并发编程

## 📌 核心概念

- 并发方式：基于进程 / 基于 I/O 多路复用 / 基于线程
- 线程模型：POSIX pthreads
- 共享变量：线程共享进程的全局/堆/部分栈
- 线程同步：信号量（sem_init/sem_wait/sem_post）、互斥锁（pthread_mutex_*）
- 进度图（progress graph）：判断互斥/死锁/不安全
- 死锁必要条件：互斥 / 占有并等待 / 不可剥夺 / 循环等待
- 可重入性（reentrancy）、线程安全
- 竞争（race）：结果依赖于执行时序
- 并行程序性能：加速比、Amdahl 定律
- Future / 消息传递（高级）

## 🧠 速查

```c
pthread_create(pthread_t *t, const pthread_attr_t *attr,
               void *(*start_routine)(void *), void *arg);
pthread_join(pthread_t t, void **retval);

sem_init(sem_t *sem, int pshared, unsigned int value);
sem_wait(sem_t *sem);   // P
sem_post(sem_t *sem);   // V

pthread_mutex_init(pthread_mutex_t *m, NULL);
pthread_mutex_lock / unlock
```

## 🔑 例题归档

### 共享变量 + 竞态（待填，见 code/conc/race.c）
### 生产者-消费者 + sbuf（待填，见 code/conc/sbuf.c）
### 并行求和（待填，见 code/conc/psum.c）
### badcnt 错误版本 vs norace 正确版本（待填）

## 🐛 易错点

1. 编译器/处理器会**重排**内存访问；用 atomic/mutex 显式约束
2. 信号量/锁用前必须初始化；用完销毁
3. 锁的粒度：太粗并发度低；太细易死锁
4. `volatile` 不能替代同步
5. 死锁的四个条件至少破坏一个

## 💡 心得
