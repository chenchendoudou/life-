# Ch11 · 网络编程

## 📌 核心概念

- 客户端-服务器模型
- 互联网：TCP/IP 协议栈
- IP 地址：IPv4 / IPv6
- 套接字接口：socket / bind / listen / accept / connect / send / recv
- 套接字地址结构：sockaddr_in、sockaddr_in6、sockaddr（通用）
- 字节序：htonl / htons / ntohl / ntohs
- 并发策略：进程 / 线程 / I/O 多路复用（select / poll / epoll）
- HTTP/1.0 简单协议：GET / 200 OK / Content-Type / Content-Length
- Web 服务器：Tiny / 代理

## 🧠 速查

```c
int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

## 🔑 例题归档

### Tiny HTTP 服务器（待填）
### Echo Server 三个版本（待填，见 code/conc/）
  - `echoserverp.c` — 基于进程
  - `echoservert.c` — 基于线程
  - `echoservert_pre.c` — 预线程化
  - `echoservers.c` — 顺序
### proxylab 实践（待填）

## 🐛 易错点

1. `bind` 失败常因端口占用 + SO_REUSEADDR
2. accept 返回的 connfd 是已连接 socket（不同 socket）
3. read 0 字节 = 对端关闭连接
4. epoll 水平触发 LT vs 边沿触发 ET
5. HTTP 响应头与 body 之间必须有 \r\n\r\n

## 🧩 Lab 链接

- `labs/LAB_PROGRESS.md` · proxylab

## 💡 心得
