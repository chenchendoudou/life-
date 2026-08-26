#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <poll.h>

/*
 * TCP 客户端: 连接 127.0.0.1:8080, 与服务端互发数据
 * 使用 poll 实现同时监听 stdin 和 socket
 */

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;
    char ip_str[INET_ADDRSTRLEN];

    /* 1. 创建 socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    /* 2. 设置服务端地址 */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    /* 调试: 打印地址信息 */
    inet_ntop(AF_INET, &server_addr.sin_addr, ip_str, sizeof(ip_str));
    printf("[Client] Connecting to %s:%d ...\n", ip_str, ntohs(server_addr.sin_port));

    /* 3. 连接服务端 */
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("[Client] Connected!\n");
    printf("[Client] Type 'exit' to quit.\n\n");

    /* 4. 使用 poll 同时监听 stdin(0) 和 sockfd */
    struct pollfd fds[2];
    fds[0].fd = 0;              // stdin
    fds[0].events = POLLIN;//    监听stdin是否有数据可读
    fds[1].fd = sockfd;         // server socket
    fds[1].events = POLLIN;//   监听sockfd是否有数据可读

    char buf[1024];

    while (1) {
        int ret = poll(fds, 2, -1);  // 阻塞等待
        if (ret < 0) {
            perror("poll");
            break;
        }

        /* stdin 有数据 → 发送给服务端 */
        if (fds[0].revents & POLLIN) {
            memset(buf, 0, sizeof(buf));
            if (fgets(buf, sizeof(buf), stdin) == NULL)
                break;
            buf[strcspn(buf, "\n")] = '\0';

            if (strcmp(buf, "exit") == 0) {
                printf("[Client] Disconnected.\n");
                break;
            }
            write(sockfd, buf, strlen(buf));
            printf("[Client] Sent: %s\n", buf);
        }

        /* 服务端有数据 → 接收并显示 */
        if (fds[1].revents & POLLIN) {
            memset(buf, 0, sizeof(buf));
            int n = read(sockfd, buf, sizeof(buf) - 1);
            if (n <= 0) {
                printf("[Client] Server disconnected.\n");
                break;
            }
            buf[n] = '\0';
            printf("[Client] Received: %s\n", buf);
        }
    }

    close(sockfd);
    return EXIT_SUCCESS;
}