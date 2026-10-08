# Ch10 · 系统级 I/O

## 📌 核心概念

- Unix I/O：一切皆文件（socket、pipe、device、file）
- 文件描述符：0/1/2 = stdin/stdout/stderr
- 打开/关闭：open/close；读写：read/write；定位：lseek
- RIO（Robust I/O）包：rio_readn / rio_writen / rio_readlineb / rio_readnb
  - 解决 short count（不足字节数被读）
- 标准 I/O（fopen/fread/fgets/fprintf）：用户态缓冲
- 何时用哪种：网络/设备 = Unix I/O；格式化文本 = 标准 I/O
- 文件元数据：stat
- 共享文件：同一打开文件表项 / 不同文件表项 / 同一 inode
- I/O 重定向：dup / dup2

## 🧠 速查

```c
int open(const char *pathname, int flags, mode_t mode);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
off_t lseek(int fd, off_t offset, int whence);
int dup2(int oldfd, int newfd);
```

## 🔑 例题归档

### RIO 实现思路（待填）
### dup2 重定向（待填）

## 🐛 易错点

1. 标准 I/O 不适合网络套接字（无 full-duplex）
2. fflush / fseek / fclose 才能真正落盘
3. `O_APPEND` 内核保证原子追加
4. read 返回值可能 < count，要循环

## 💡 心得
