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
 * TCP 服务端: 监听 127.0.0.1:8080, 与客户端互发数据
 * 使用 poll 实现同时监听 stdin 和 socket
 */

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    /* 1. 创建 socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    /* 2. 设置地址复用（避免 TIME_WAIT 问题） */
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* 3. 绑定地址 */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    /* 4. 开始监听 */
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }
    printf("[Server] Listening on 127.0.0.1:8080 ...\n");

    /* 5. 接受客户端连接 */
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("[Server] Client connected: %s:%d\n", client_ip, ntohs(client_addr.sin_port));
    printf("[Server] Type 'exit' to quit.\n\n");

    /* 6. 使用 poll 同时监听 stdin(0) 和 client_fd */
    struct pollfd fds[2];
    fds[0].fd = 0;              // stdin
    fds[0].events = POLLIN;
    fds[1].fd = client_fd;      // client socket
    fds[1].events = POLLIN;

    char buf[1024];

    while (1) {
        int ret = poll(fds, 2, -1);  // 阻塞等待
        if (ret < 0) {
            perror("poll");
            break;
        }

        /* stdin 有数据 → 发送给客户端 */
        if (fds[0].revents & POLLIN) {
            memset(buf, 0, sizeof(buf));
            if (fgets(buf, sizeof(buf), stdin) == NULL)
                break;
            buf[strcspn(buf, "\n")] = '\0';

            if (strcmp(buf, "exit") == 0) {
                printf("[Server] Disconnected.\n");
                break;
            }
            write(client_fd, buf, strlen(buf));
            printf("[Server] Sent: %s\n", buf);
        }

        /* 客户端有数据 → 接收并显示 */
        if (fds[1].revents & POLLIN) {
            memset(buf, 0, sizeof(buf));
            int n = read(client_fd, buf, sizeof(buf) - 1);
            if (n <= 0) {
                printf("[Server] Client disconnected.\n");
                break;
            }
            buf[n] = '\0';
            printf("[Server] Received: %s\n", buf);
        }
    }

    close(client_fd);
    close(server_fd);
    return EXIT_SUCCESS;
}